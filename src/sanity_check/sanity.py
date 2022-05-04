import sys, os, shlex, subprocess

filepath = sys.argv[1]
path = filepath[ :filepath.rfind('/') ]
filename = filepath[ filepath.rfind('/')+1: ]
filename_ext = filename[ filename.rfind('.') : ]
fixed_filename = filename[:filename.rfind('.')] + '_fixed' + filename_ext
fixed_filepath = path + '/' + fixed_filename
test_location = 'sanity_test'
test_name = 'test'
test_path = test_location + '/' + test_name + '.cc'

def make_cmd():
		path_depth = '../' * 3 
		cmd  = 'g++ -MMD -MF ./.' + test_name + '.o.d -o ' + test_name + '.o ' + test_name + '.cc '
		cmd += '-Wall -g -O3 '
		cmd += '-I' + path_depth + 'model-checker/cds-checker '
		cmd += '-I' + path_depth + 'model-checker/cds-checker/include -fpermissive '
		cmd += '-L' + path_depth + 'model-checker/cds-checker -lmodel'
		return cmd

def get_order(line):
    start = line.find('memory_order_') + len('memory_order_')
    end   = line[start:].find(')') + start
    return line[ start:end ]

def weaker_orders(order):
    if order == 'release' or order == 'acquire':
        return []
    if order == 'acq_rel':
        return ['acquire', 'release']
    return ['acquire', 'release', 'acq_rel']

def replace_order(line, new_order):
    start = line.find('memory_order_') + len('memory_order_')
    end   = line[start:].find(')') + start
    return line[:start] + new_order + line[end:]

def error_message(errorcode, filename, line_no, program_order='', weaker_order=''):
    emsg  = '\t Test Name:' + filename + '\n'
    emsg += '\t line#' + str(line_no )+ '\n'

    if errorcode > 0:
        emsg  = '\033[93m' + 'Verification could not complete.' + '\033[0m \n' + emsg
    elif errorcode == -3:
        emsg  = '\033[91m' + 'Correctness fail (\'fixed program\' is buggy)' + '\033[0m \n' + emsg
    elif errorcode == -1:
        emsg  = '\033[91m' + 'Optimality fail (Fence too strong)' + '\033[0m \n' + emsg
        emsg += 'program order:', program_order, 'correct order:', weaker_order
    else:
        emsg  = '\033[91m' + 'Optimality fail (Redundant fence)' + '\033[0m \n' + emsg
        
    return emsg

def verify(program):
    program = ''.join(program)
    f = open(test_path, 'w')
    f.write(program)
    f.close()

    os.system('cd ' + test_location + ' && ' + make_cmd() )

    cds_test_file = '../../src/sanity_check/' + test_location + '/' + test_name + '.o'
    cds_cmd = './run.sh ' + cds_test_file + ' -c 1'
    cds_cmd = shlex.split(cds_cmd)

    try:
        p = subprocess.check_output(cds_cmd,
                                    cwd = "../../model-checker/cds-checker",
                                    stderr=subprocess.STDOUT,
                                    timeout=900)
    except subprocess.TimeoutExpired:
        print('\033[93m' + '\nModel Checking time exceeded 15 minutes.' + '\033[0m')
    except subprocess.CalledProcessError as exc:
            outputstr = exc.output.decode('utf-8', errors='ignore')
            if 'Error:' in outputstr:
                outputstr = outputstr[outputstr.find('Error:') : ]
            elif 'Out of ' in outputstr:
                outputstr = outputstr[outputstr.find('Out of ') : ]		
            elif 'Segmentation fault' in outputstr:	
                outputstr = outputstr[outputstr.find('Segmentation fault') : ]	
            elif 'For debugging' in outputstr:
                outputstr = outputstr[outputstr.find('For debugging') : ]
            if (outputstr):
                print('\033[93m' + '\nCalledProcessError while model checking.' + '\033[0m')
                print(outputstr)
                print('\033[93m' + '\nPlease resolve the error for fence synthesis to proceed.' + '\033[0m')
    except Exception as e:
        print('\033[93m' + '\nError while model checking.\nPlease resolve the error for fence synthesis to proceed.' + '\033[0m')
        print('error',e)

    output = p.decode('utf-8', errors='ignore')
    lines  = output.split('\n')

    i = 0
    while i < len(lines):
        if 'Model-checking complete' in lines[i]:
            break
        i += 1

    if i >= len(lines):
        return 'NC' # 'not complete': verification could not be done
                    # 'Model-checking complete' not found in output

    for j in range(i, len(lines)):
        if 'Number of buggy executions' in lines[j]:
            if int(lines[j].split(':')[1]) == 0:
                return 'VF' # verification failed
            return 'V'      # verified correct

    return 'NC' # 'not complete': verification could not be done 
                # 'Number of buggy executions' not found in output

def test_optimality(line, line_no, fixed_program, new_program):
    memory_order = get_order(line)

    print('Testing fence:', line[:-1])

    for weaker_order in weaker_orders(memory_order):
        print('weakening to', weaker_order)

        new_line = replace_order(line, weaker_order)
        final_program  = new_program[:]
        final_program += new_line
        for j in range(line_no+1, len(fixed_program)):
            final_program += fixed_program[j]
        verify_res = verify(final_program) # test for a weaker fence
            
        if verify_res == 'NC':
            return (1, memory_order, weaker_order)
        if verify_res == 'VF':
            return (-1, memory_order, weaker_order)

        print('OK')

    print('removing fence...')
    final_program = new_program[:]
    for j in range(line_no+1, len(fixed_program)):
        final_program += fixed_program[j]
    verify_res = verify(final_program) # test without current fence

    if verify_res == 'NC':
        return (2, '', '')
    if verify_res == 'VF':
        return (-2, '', '')

    print('OK\n')
    return (0, '', '')

def issynthesized(iline):
    return ('atomic_thread_fence' not in iline)

def isstrengthened(fline, iline):
    if 'atomic_thread_fence' not in iline:
        return False

    fmemory_order = get_order(fline)
    imemory_order = get_order(iline)

    return not (fmemory_order == imemory_order)

def soundness_check_ok(fixed_program):
    print('checking soundness...')
    fixed_program = ''.join(fixed_program)
    verify_res = verify(fixed_program)
    
    if verify_res == 'NC': # verification could not complete
        print( error_message(1, fixed_filename, 0, '', '') )
        return False
    if verify_res == 'V': # verification failed: 'fixed_program' has bugs
        print( error_message(-3, fixed_filename, 0, '', '') )
        return False
    return True

# main

failed = False

ifile_content = open(filepath, 'r')
ffile_content = open(fixed_filepath, 'r')

input_program = ifile_content.readlines()
fixed_program = ffile_content.readlines()
new_program = ''

if soundness_check_ok(fixed_program):
    # fixed_program is bug free
    print('OK\n')

    k = 0
    for i in range(len(fixed_program)):
        iline = input_program[k]
        fline = fixed_program[i]
        k += 1

        if 'atomic_thread_fence' in fline:
            synthesized  = issynthesized(iline)
            strengthened = isstrengthened(fline, iline)
            if synthesized or strengthened: # fence synthesized or strengthened
                (error_code, program_order, weaker_order) = test_optimality(fline, i, fixed_program, new_program)
                if error_code:
                    print( error_message(error_code, fixed_filename, i, program_order, weaker_order) )
                    failed = True
                    break
                    
                if synthesized:
                    k -= 1 # input program remains on the same line
            
        new_program += fixed_program[i] 

    if not failed:
        print('\033[92m' + 'Verified sound and optimal fence synthesis.' + '\033[0m')