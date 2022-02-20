from cProfile import run
import os
import subprocess
import sys
import glob
import math

from importlib_metadata import csv

res_dir = 'tests/test_scripts/result'
test_dirs = [
    'tests/litmus/weak_fensying'
]

N = 3       # no. of runs per tests
T = [1, math.inf]  # values to be passed to -t flags
Parallel = False

def add(val1, val2):
    if val1 == '':
        return val2
    if val2 == '':
        return val1
    return str(float(val1) + float(val2))

def avg(val):
    if val == '':
        return '0'
    return str( round(float(val) / 3, 2) )

def read_result(lines):
    synthesized   = ''
    strengthened  = ''
    time_ceg      = ''
    time_z3       = ''
    time_fensying = ''
    
    result_generated = False
    status = 'OK'

    for i in range(len(lines)):
        if 'ABORT' in lines[i]:
            status = 'ABORT'

        if 'Model Checking time exceeded' in lines[i]:
            return 'MCTO', synthesized, strengthened, time_ceg, time_z3, time_fensying

        if 'Tool time exceeded' in lines[i]:
            return 'FTO', synthesized, strengthened, time_ceg, time_z3, time_fensying

        if 'No buggy traces. Nothing to do.' in lines[i]:
            return 'NOBUG', synthesized, strengthened, time_ceg, time_z3, time_fensying

        if 'RESULT SUMMARY' in lines[i]:
            result_generated = True
            break

    if not result_generated:
        return 'FAIL', synthesized, strengthened, time_ceg, time_z3, time_fensying

    for j in range(i,len(lines)): # remaining lines after line containing 'RESULT SUMMARY'
        words = lines[j].split()
        if len(words) == 0: # empty line
            continue

        if 'synthesized:' in words:
            synthesized = words[5]
            continue

        if 'strengthened:' in words:
            strengthened = words[5]
            continue

        if 'CEG:' in words:
            time_ceg = words[2]
            continue

        if 'Z3:' in words:
            time_z3 = words[2]
            continue

        if 'Fensying:' in words:
            time_fensying = words[2]
            continue

    return status, synthesized, strengthened, time_ceg, time_z3, time_fensying

def execute_test(filepath, t=math.inf, d=math.inf, f=math.inf):
    process_command = ['python3', 'main.py', filepath]
    if t != math.inf:
        process_command.append('-t')
        process_command.append(str(t))
    if d != math.inf:
        process_command.append('-d')
        process_command.append(str(d))
    if f != math.inf:
        process_command.append('-f')
        process_command.append(str(f))
    if Parallel:
        process_command.append('-p')
    print('executing: ', process_command)
    
    _synthesized   = 0
    _strengthened  = 0
    _time_ceg      = ''
    _time_z3       = ''
    _time_fensying = ''
    _time_total    = ''

    _aborted = False            # could bot fix test (execution successful)
    _result_generated = False   # execution success

    for n in range(N): # avg of N runs
        process = subprocess.Popen(process_command, stdout=subprocess.PIPE, bufsize=1, universal_newlines=True)
        lines = process.stdout.readlines()
        
        status, synthesized, strengthened, time_ceg, time_z3, time_fensying = read_result(lines)
        
        if status == 'NOBUG':
            return 'STOP', 'No buggy traces.' + ',,,,,,'
        if status == 'MCTO':
            return 'STOP', 'CDS TO (15m)' + ',,,,,,'
        if status == 'FAIL':
            return 'STOP', 'Failed to run.' + ',,,,,,'
        if status == 'FTO':
            continue

        # status = OK or ABORT
        _aborted = _aborted or status == 'ABORT'
        _result_generated = _result_generated or status == 'OK' or status == 'ABORT'

        # take avg of synthesized and strengthened for all itr
        if synthesized != '':
            _synthesized = _synthesized + int(synthesized)
        if strengthened != '':
            _strengthened = _strengthened + int(strengthened)

        _time_ceg = add(_time_ceg, time_ceg)
        _time_z3  = add(_time_z3, time_z3)
        _time_fensying = add(_time_fensying, time_fensying)

    if _result_generated:
        if _aborted:
            _synthesized = 'Cannot fix.'
        _time_ceg = avg(_time_ceg)
        _time_z3  = avg(_time_z3)
        _time_fensying = avg(_time_fensying)
        _time_total = add(_time_ceg, add(_time_z3, _time_fensying))
        return 'OK', str(_synthesized/N) + ',' + str(_strengthened/N) + ',' + _time_ceg + ',' + _time_z3 + ',' + _time_fensying + ',' + _time_total + ','
    return 'TO', 'Fensying TO (15m)' + ',,,,,,'

def run_all_config(filename):
    os.chdir(cwd + '/src')
    csv_row = ''
    f3_succ = True
    f5_succ = True
    d7_succ = True
    d10_succ = True
    f3d10_succ = True
    t_succ = True

    # for t=1 and 0
    for t_conf in T: 
        # config1: -f 3
        if f3_succ:
            status, test_cols = execute_test(filename, t=t_conf, f=3)
            csv_row += test_cols
            if status == 'STOP':
                f3_succ = False
                f5_succ = False

        # config2: -f 5
        # run only if worked with -t 1 -f 3
        if f3_succ and f5_succ:
            status, test_cols = execute_test(filename, t=t_conf, f=5)
            csv_row += test_cols
            if status == 'STOP':
                f5_succ = False

        # config3: -d 7
        if d7_succ:
            status, test_cols = execute_test(filename, t=t_conf, d=7)
            csv_row += test_cols
            if status == 'STOP':
                d7_succ = False
                d10_succ = False
        
        # config4: -d 10
        if d7_succ and d10_succ:
            status, test_cols = execute_test(filename, t=t_conf, d=10)
            csv_row += test_cols
            if status == 'STOP':
                d10_succ = False
        
        # config5: -f 3 -d 10
        if f3d10_succ:
            status, test_cols = execute_test(filename, t=t_conf, f=3, d=10)
            csv_row += test_cols
            if status == 'STOP':
                f3d10_succ = False
        
        # config6: no fence and depth bounding
        if f5_succ and d10_succ and f3d10_succ and t_succ:
            status, test_cols = execute_test(filename, t=t_conf)
            csv_row += test_cols
            if status == 'STOP':
                t_succ = False
        
    os.chdir(cwd)
    csv_row += '\n'
    return csv_row
        

def write_csv_header(csv_file_name):
    csv_header = 'Test Name,'
    for t in T:
        for f_val in [3,5]:
            csv_header_t = '#synthesized,#strengthened,Time-CEG,Time-Z3,Time-fensying,Time-total,'
            csv_header_t = csv_header_t.replace(',', 
                            '(t=' + str(t) + '&f=' + str(f_val) + '),')
            csv_header += (csv_header_t)
        for d_val in [7,10]:
            csv_header_t = '#synthesized,#strengthened,Time-CEG,Time-Z3,Time-fensying,Time-total,'
            csv_header_t = csv_header_t.replace(',', 
                            '(t=' + str(t) + '&d=' + str(d_val) + '),')
            csv_header += (csv_header_t)
        # for -f 3 -d 10
        csv_header_t = '#synthesized,#strengthened,Time-CEG,Time-Z3,Time-fensying,Time-total,'
        csv_header_t = csv_header_t.replace(',', 
                        '(t=' + str(t) + '&f=3&d=10),')
        csv_header += (csv_header_t)
        # for no fence and depth bounding
        csv_header_t = '#synthesized,#strengthened,Time-CEG,Time-Z3,Time-fensying,Time-total,'
        csv_header_t = csv_header_t.replace(',', 
                        '(t=' + str(t) + '),')
        csv_header += (csv_header_t)
    csv_header = csv_header[:-1] + '\n'

    csv_file = open(os.path.join(res_dir, csv_file_name), 'w')
    csv_file.write(csv_header)
    csv_file.close()
    return

def run_single_test(dir_path, file):
    print('Testing ' + dir_path + '/' + file[:-2])
    filepath = '../' + os.path.join(dir_path, file)

    input_file = glob.glob(dir_path + '/' + file[:-2]+'.c*')[0]
    input_ext  = input_file[ input_file.rfind('.')+1 : ]

    f = filepath + '$' + input_ext

    csv_row = file + ',' + run_all_config(f)
    # for t in [0] + T:
    #     os.chdir(cwd + '/src')
    #     status, test_cols = execute_test(f, t)
    #     os.chdir(cwd)

    #     csv_row += test_cols
    #     if status == 'STOP':
    #         csv_row += '\n'
    #         break
    return csv_row

def test_dir(dir_path, csv_file_name='', create_new_csv=True):
    if create_new_csv:
        csv_file_name = 'result-' + str(dir_path).replace('/','_') + '.csv'
        write_csv_header(csv_file_name)
    for file in os.listdir(dir_path):
        if os.path.isdir(os.path.join(dir_path, file)):
            test_dir(os.path.join(dir_path, file), csv_file_name, False)
        if file[-2:] != '.o':
            continue 
        if '_fixed' in file:
            continue
              
        if csv_file_name == '':
            print('Something went wrong with result file name')
            exit(0)
        csv_file = open(os.path.join(res_dir, csv_file_name), 'a')
        full_path = str(dir_path).replace('/','_')
        test_name_for_csv = full_path[len(csv_file_name[len('result-'):-4]):]
        csv_row = run_single_test(dir_path, file)
        csv_row = test_name_for_csv + "/" + csv_row
        csv_row = csv_row[:-1] + '\n'
        csv_file.write(csv_row)
        csv_file.close()
    return 



def run_tests_from_file (filename):
    with open(filename, 'r') as file:
        test_list = file.read().replace("'", "").strip().split(' ')
    # print('test dir:', test_list)
    # print('cwd: ', cwd)

    csv_file = write_csv_header('result-' + os.path.splitext(os.path.basename(filename))[0] + '.csv')
    for test in test_list:
        filename, ext = os.path.splitext(os.path.basename(test))
        if ext != '.o':
            print('\033[0;36mModifying filename:', filename+ext, '-->', filename+'.o\033[0m')
            ext = '.o'
        csv_row = run_single_test(os.path.dirname(test), filename+ext)
        csv_row = csv_row[:-1] + '\n'
        csv_file.write(csv_row)
    
    csv_file.close()


cwd = os.getcwd()
    
if not os.path.exists(res_dir):
    os.system('mkdir ' + res_dir)

# if a filename is passed on command line, 
# read list of tests to run from file
# else run from testdir
if len(sys.argv) == 2:
    run_tests_from_file(sys.argv[1])
    exit(0)
else:
    print ('No command line argument. Running testdirs')
    for dir in test_dirs:
        print('\nENTERING ' + dir)
        test_dir(dir)
        print('LEAVING ' + dir)
