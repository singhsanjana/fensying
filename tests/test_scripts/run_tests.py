from ast import Pass
from cProfile import run
import os
import subprocess
import sys
import glob
from math import inf

res_dir = 'tests/test_scripts/result'
test_dirs = [
    'tests/benchmarks/VBMCbench/configs/burns',
    'tests/benchmarks/VBMCbench/configs/bakery',
    'tests/benchmarks/VBMCbench/configs/dekker'
]

N = 5       # no. of runs per tests
T = [1, inf]  # values to be passed to -t flags. should be in increasing order
Parallel = False

def add(val1, val2):
    if val1 == '':
        return val2
    if val2 == '':
        return val1
    return str(float(val1) + float(val2))

def avg(val, num=N):
    if val == '':
        return '0'
    return str( round(float(val) / num, 2) )

def read_result(lines):
    synthesized   = ''
    strengthened  = ''
    time_ceg      = ''
    time_z3       = ''
    time_fensying = ''
    iterations    = ''
    
    result_generated = False
    status = 'OK'

    for i in range(len(lines)):
        if 'ABORT' in lines[i]:
            status = 'ABORT'
        if 'Model Checking time exceeded' in lines[i]:
            return 'MCTO', synthesized, strengthened, iterations, time_ceg, time_z3, time_fensying
        if 'Tool time exceeded' in lines[i]:
            return 'FTO', synthesized, strengthened, iterations, time_ceg, time_z3, time_fensying
        if 'No buggy traces. Nothing to do.' in lines[i]:
            return 'NOBUG', synthesized, strengthened, iterations, time_ceg, time_z3, time_fensying
        if 'RESULT SUMMARY' in lines[i]:
            result_generated = True
            break

    if not result_generated:
        return 'FAIL', synthesized, strengthened, iterations, time_ceg, time_z3, time_fensying

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
        if 'BTG:' in words and 'Avg.' not in words :
            time_ceg = words[2]
            continue
        if 'Z3:' in words and 'Avg.' not in words :
            time_z3 = words[2]
            continue
        if 'Fensying:' in words and 'Avg.' not in words :
            time_fensying = words[2]
            continue   
        if 'Total' in words and 'iterations:' in words:
            iterations = int(words[2])
            continue

    return status, synthesized, strengthened, iterations, time_ceg, time_z3, time_fensying


def compute_total_times(_time_ceg, time_ceg, _time_z3, time_z3, _time_fensying, time_fensying):
    _time_ceg = add(_time_ceg, time_ceg)
    _time_z3  = add(_time_z3, time_z3)
    _time_fensying = add(_time_fensying, time_fensying)
    return _time_ceg, _time_z3, _time_fensying

def compute_total_fences(total_synthesized, min_synthesized, max_synthesized, synthesized, total_strengthened, min_strengthened, max_strengthened,  strengthened, total_iterations, min_iterations, max_iterations, iterations):
    # take sum of synthesized and strengthened for all itr
    if synthesized == '' or strengthened == '':
        print('got empty string for #fence')
        exit(0)
    else:
        total_synthesized = total_synthesized + int(synthesized)
        min_synthesized = min(min_synthesized, int(synthesized))
        max_synthesized = max(max_synthesized, int(synthesized))
        total_strengthened = total_strengthened + int(strengthened)
        min_strengthened = min(min_strengthened, int(strengthened))
        max_strengthened = max(max_strengthened, int(strengthened))
        if iterations != '':
            total_iterations = total_iterations + int(iterations)
            min_iterations = min(min_iterations, int(iterations))
            max_iterations = max(max_iterations, int(iterations))
    return total_synthesized, min_synthesized, max_synthesized, total_strengthened, max_strengthened, max_strengthened, total_iterations, min_iterations, max_iterations, 

def update_status(_aborted, _result_generated, _status, status):
    """Compute the new values of status, abort and result generated based on older runs and current run"""
    aborted = _aborted or _status == 'ABORT'
    result_generated = _result_generated or status == 'OK' or status == 'ABORT'
    # if program has already run once then status is older one
    if _status == 'OK' and (status == 'MCTO' or status == 'FTO'):
        status = 'OK'
    return aborted, result_generated, status

def execute_test(filepath, t=inf, d=inf, f=inf):
    process_command = ['python3', 'main.py', filepath]
    if t != inf:
        process_command.append('-t')
        process_command.append(str(t))
    if d != inf:
        process_command.append('-d')
        process_command.append(str(d))
    if f != inf:
        process_command.append('-f')
        process_command.append(str(f))
    if Parallel:
        process_command.append('-p')
    print('executing: ', process_command)
    
    total_synthesized = 0
    min_synthesized = inf
    max_synthesized = 0
    total_strengthened  = 0
    min_strengthened  = inf
    max_strengthened  = 0
    total_iterations = 0
    min_iterations = inf
    max_iterations = 0
    _time_ceg      = ''
    _time_z3       = ''
    _time_fensying = ''
    _time_total    = ''
    
    _status = 'MCTO'
    _aborted = False            # could bot fix test (execution successful)
    _result_generated = False   # execution success

    # n=0
    num_completed_runs = 0
    for n in range(N): # avg of N runs
        process = subprocess.Popen(process_command, stdout=subprocess.PIPE, bufsize=1, universal_newlines=True)
        lines = process.stdout.readlines()
        
        status, synthesized, strengthened, iterations, time_ceg, time_z3, time_fensying = read_result(lines)
        print('result of itr:', n, end=' ')
        print(status, synthesized, strengthened, iterations, time_ceg, time_z3, time_fensying)
        
        if status == 'NOBUG':
            return 'STOP', 'No buggy traces.' + ',,,,,,,,,,,,,,'
        elif status == 'FAIL':
            return 'STOP', 'Failed to run.' + ',,,,,,,,,,,,,,'
        elif status == 'MCTO':
            if t == inf:
                # print('MCTO. returning')
                return 'OK', str(total_synthesized) + ',' \
                    + str(min_synthesized) + ',' \
                    + str(max_synthesized) + ',' \
                    + str(total_strengthened) + ',' \
                    + str(min_strengthened) + ',' \
                    + str(max_strengthened) + ',' \
                    + str(total_iterations) + ',' \
                    + str(min_iterations) + ',' \
                    + str(max_iterations) + ',' \
                    +_time_ceg + ',' \
                    + _time_z3 + ',' \
                    + _time_fensying + ',' \
                    + _time_total + ',' \
                    + str(num_completed_runs) + ','
            else:
                _time_ceg, _time_z3, _time_fensying = compute_total_times(
                    _time_ceg, 900, 
                    _time_z3, time_z3, 
                    _time_fensying, time_fensying)
                _aborted, _result_generated, _status = update_status(
                    _aborted, _result_generated, 
                    _status, status)
            # return 'STOP', 'CDS TO (15m)' + ',,,,,,'
        elif status == 'FTO':
            # print('in FTO. num_completed_runs', num_completed_runs)
            if t == inf:
                return 'STOP', 'Fensying TO (15m)' + ',,,,,,,,,,,,,,'
            else:
                # print('in FTO. Totals so far:')
                _time_ceg, _time_z3, _time_fensying = compute_total_times(
                    _time_ceg, time_ceg, 
                    _time_z3, time_z3, 
                    _time_fensying, 900)
                _aborted, _result_generated, _status = update_status(
                    _aborted, _result_generated, 
                    _status, status)
        else:
            _time_ceg, _time_z3, _time_fensying = compute_total_times(
                _time_ceg, time_ceg, 
                _time_z3, time_z3, 
                _time_fensying, time_fensying)
            total_synthesized, min_synthesized, max_synthesized, total_strengthened, min_strengthened, max_strengthened,total_iterations, min_iterations, max_iterations = compute_total_fences(
                total_synthesized, min_synthesized, max_synthesized,synthesized, 
                total_strengthened, min_strengthened, max_strengthened, strengthened,
                total_iterations, min_iterations, max_iterations, iterations)
            _aborted, _result_generated, _status = update_status(
                _aborted, _result_generated, 
                _status, status)
            num_completed_runs  += 1

    if _result_generated:
        _time_ceg = avg(_time_ceg)
        _time_z3  = avg(_time_z3)
        _time_fensying = avg(_time_fensying)
        _time_total = add(_time_ceg, add(_time_z3, _time_fensying))
        total_synthesized = avg(total_synthesized, num_completed_runs)
        total_strengthened = avg(total_strengthened, num_completed_runs)
        total_iterations = avg(total_iterations, num_completed_runs)
        if _aborted:
            total_synthesized = 'Cannot fix.'
            return 'OK', str(total_synthesized) + ',' \
                + str(min_synthesized) + ',' \
                + str(max_synthesized) + ',' \
                + str(total_strengthened) + ',' \
                + str(min_strengthened) + ',' \
                + str(max_strengthened) + ',' \
                + str(total_iterations) + ',' \
                + str(min_iterations) + ',' \
                + str(max_iterations) + ',' \
                +_time_ceg + ',' \
                + _time_z3 + ',' \
                + _time_fensying + ',' \
                + _time_total + ',' \
                + str(num_completed_runs) + ','
        if _status == 'MCTO':
            return 'OK', str(total_synthesized) + ',' \
                + str(min_synthesized) + ',' \
                + str(max_synthesized) + ',' \
                + str(total_strengthened) + ',' \
                + str(min_strengthened) + ',' \
                + str(max_strengthened) + ',' \
                + str(total_iterations) + ',' \
                + str(min_iterations) + ',' \
                + str(max_iterations) + ',' \
                +_time_ceg + ',' \
                + _time_z3 + ',' \
                + _time_fensying + ',' \
                + _time_total + ',' \
                + str(num_completed_runs) + ','
        if _status == 'FTO':
            return 'OK', 'Fensying TO (15m)' + ',,,,,,,,,,,,,,'
        
        return 'OK', str(total_synthesized) + ',' \
            + str(min_synthesized) + ',' \
            + str(max_synthesized) + ',' \
            + str(total_strengthened) + ',' \
            + str(min_strengthened) + ',' \
            + str(max_strengthened) + ',' \
            + str(total_iterations) + ',' \
            + str(min_iterations) + ',' \
            + str(max_iterations) + ',' \
            +_time_ceg + ',' \
            + _time_z3 + ',' \
            + _time_fensying + ',' \
            + _time_total + ',' \
            + str(num_completed_runs) + ','
    if _status == 'MCTO':
        return 'OK', 'CDS TO (15m)' + ',,,,,,,,,,,,,,'
    if _status == 'FTO':
        return 'OK', 'Fensying TO (15m)' + ',,,,,,,,,,,,,,'
    else:
        return 'TO', 'Fensying TO (15m)' + ',,,,,,,,,,,,,,'

def run_all_config(filename):
    os.chdir(cwd + '/src')
    csv_row = ''
    t_succ = True

    # for t=1 and inf
    for i in range(len(T)): 
        # config1: no fence and depth bounding
        t_conf = T[i]
        status, test_cols = execute_test(filename, t=t_conf)
        csv_row += test_cols
        print(test_cols)
        if status == 'STOP':
            # if 'STOP' for t=1, won't test for t=inf
            break
            
        # if not t_succ:
        #     # config2: -f 3
        #     status, test_cols = execute_test(filename, t=t_conf, f=3)
        #     csv_row += test_cols
            
        #     # config3: -d 10
        #     status, test_cols = execute_test(filename, t=t_conf, d=10)
        #     csv_row += test_cols
        # else: 
        #     csv_row += ',,,,,,' * 2
    for j in range(len(T)-i-1):
        csv_row += ',,,,,,,,,,,,,,'
    os.chdir(cwd)
    csv_row += '\n'
    return csv_row
        

def write_csv_header(csv_file_name):
    csv_header = 'Test Name,'
    csv_header = 'Test Name,'
    for t in T:
        # for no fence and depth bounding
        csv_header_t = 'avg_synthesized,min_synthesized, max_synthesized,avg_strengthened,min_strengthened,max_strengthened,tot_itr,min_itr, max_itr,Time-BTG,Time-Z3,Time-fensying,Time-total,#succ_run,'
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
    csv_file_name = 'result-' + os.path.splitext(os.path.basename(filename))[0] + '.csv'

    write_csv_header(csv_file_name)
    for test in test_list:
        filename, ext = os.path.splitext(os.path.basename(test))
        if ext != '.o':
            print('\033[0;36mModifying filename:', filename+ext, '-->', filename+'.o\033[0m')
            ext = '.o'
        csv_row = run_single_test(os.path.dirname(test), filename+ext)
        csv_row = csv_row[:-1] + '\n'
        csv_file = open(os.path.join(res_dir, csv_file_name), 'a')
        
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
