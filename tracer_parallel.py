from cProfile import run
import os
import subprocess
import sys
import glob
from math import inf
import threading
from importlib_metadata import csv
import time
import re

res_dir = 'tests/test_scripts/result'

N = 1       # no. of runs per tests
Th = 4      # no. of threads
T = [1]  # values to be passed to -t flags. should be in increasing order
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
    fence_type = ''
    
    result_generated = False
    status = 'OK'

    for i in range(len(lines)):
        if 'ABORT' in lines[i]:
            status = 'ABORT'

        if 'Model Checking time exceeded' in lines[i]:
            return 'MCTO', synthesized, strengthened, time_ceg, time_z3, time_fensying, fence_type

        if 'Tool time exceeded' in lines[i]:
            return 'FTO', synthesized, strengthened, time_ceg, time_z3, time_fensying, fence_type

        if 'No buggy traces. Nothing to do.' in lines[i]:
            return 'NOBUG', synthesized, strengthened, time_ceg, time_z3, time_fensying, fence_type

        if 'RESULT SUMMARY' in lines[i]:
            result_generated = True
            break

    if not result_generated:
        return 'FAIL', synthesized, strengthened, time_ceg, time_z3, time_fensying, fence_type

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
    for i in range(len(lines)):
        if 'SYNTHESIS SUMMARY' in lines[i]:
            break
    for j in range(i,len(lines)):
        words = lines[j].split('|')
        if(len(words)>3 and j!=i+2):
            fence_type+=words[3]+' '
    print(fence_type)
    return status, synthesized, strengthened, time_ceg, time_z3, time_fensying,fence_type

def execute_test(filepath, t=inf, d=inf, f=inf):
    csv_row = ''
    status = ''
    test_cols = ''
    correct_res = 0
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
    process_command.append('-d')
    process_command.append('10')
    process_command.append('-s')
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
        
        status, synthesized, strengthened, time_ceg, time_z3, time_fensying,fence_type = read_result(lines)
        print(fence_type)
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
        _time_ceg = avg(_time_ceg)
        _time_z3  = avg(_time_z3)
        _time_fensying = avg(_time_fensying)
        _time_total = add(_time_ceg, add(_time_z3, _time_fensying))
        if _aborted:
            _synthesized = 'Cannot fix.'
            status = 'OK'
            test_cols = str(_synthesized) + ',' + str(_strengthened) + ',' +_time_ceg + ',' + _time_z3 + ',' + _time_fensying + ',' + _time_total + ','
            # return 'OK', str(_synthesized) + ',' + str(_strengthened) + ',' +_time_ceg + ',' + _time_z3 + ',' + _time_fensying + ',' + _time_total + ','
        else:
            status = 'OK'
            correct_res = 1
            test_cols = str(_synthesized/N) + ',' + str(_strengthened/N) + ',' + _time_ceg + ',' + _time_z3 + ',' + _time_fensying + ',' + _time_total + ','
        # return 'OK', str(_synthesized/N) + ',' + str(_strengthened/N) + ',' + _time_ceg + ',' + _time_z3 + ',' + _time_fensying + ',' + _time_total + ','
    else:
        test_cols = 'Fensying TO (15m)' + ',,,,,,'
        status = 'TO'
    # return 'TO', 'Fensying TO (15m)' + ',,,,,,'
    csv_row += test_cols
    csv_row+=fence_type
    return csv_row,correct_res

def thread_func(filename, results, index,lock,correct_thread):
    out_row,out_res = execute_test(filename, t=1)
    if(out_res==1):
        with lock:
            if correct_thread[0]==-1:
                print(index)
                correct_thread[0] = index
    results[index] = out_row
    

def run_all_config(filename):
    os.chdir(cwd + '/src')
    csv_row = ''
    t_succ = True

    results = [None]*4
    correct_thread = [-1]
    lock = threading.Lock()
    # lock.acquire()
    threads = []
    for i in range(4):
        thread = threading.Thread(target=thread_func,args=(filename,results,i,lock,correct_thread))
        threads.append(thread)
        thread.start()
    
    all_threads_finished = False
    while correct_thread[0] == -1 and not all_threads_finished:
        all_threads_finished = True
        for thread in threads:
            if thread.ident != threading.current_thread().ident and thread.is_alive():
                all_threads_finished = False
                break
        else:
            break
        time.sleep(1)

    for thread in threads:
        if thread.ident != threading.current_thread().ident:
            thread.join(timeout=0)
    
    for thread in threads:
        thread.join()



    # while correct_thread[0] == -1:
    #     pass


    # for thread in threads:
    #     if thread.ident != threading.current_thread().ident:
    #         thread.join(timeout=0)

    # for thread in threads:
    #     thread.join()

    # lock.release()
    # for t=1 and inf
    # for t_conf in T: 
        # config1: no fence and depth bounding
    # csv_row = execute_test(filename, t=t_conf)
        # csv_row += test_cols
        # if status == 'STOP':
            # if 'STOP' for t=1, won't work for t=inf
            # break
            
        # if not t_succ:
        #     # config2: -f 3
        #     status, test_cols = execute_test(filename, t=t_conf, f=3)
        #     csv_row += test_cols
            
        #     # config3: -d 10
        #     status, test_cols = execute_test(filename, t=t_conf, d=10)
        #     csv_row += test_cols
        # else: 
        #     csv_row += ',,,,,,' * 2
                
    os.chdir(cwd)
    # csv_row += '\n'
    print(results)
    csv_row = []
    if correct_thread[0]==-1:
        csv_row.append(results[0])
    else:
        csv_row.append(results[correct_thread[0]]) 
    # print(results)
    # for  i in range(len(results)):
    #     if results[i][0] == 'STOP':
    #         results[i] = results[i][1]
    print(csv_row)
    return csv_row    
        

def write_csv_header(csv_file_name):
    csv_header = 'Test Name,'
    for t in range(1):
        # for no fence and depth bounding
        csv_header_t = '#synthesized,#strengthened,Time-CEG,Time-Z3,Time-fensying,Time-total,Fence-Type,'
        csv_header_t = csv_header_t.replace(',', 
                        '(t=1' + '),')
        csv_header += (csv_header_t)
        
    csv_header = csv_header[:-1] + '\n'

    csv_file = open(os.path.join(res_dir, csv_file_name), 'w')
    csv_file.write(csv_header)
    csv_file.close()
    return

def run_single_test(dir_path, file):
    filepath = '../' + os.path.join(dir_path, file)

    input_file = glob.glob(dir_path + '/' + file[:-2]+'.c*')[0]
    input_ext  = input_file[ input_file.rfind('.')+1 : ]

    f = filepath + '$' + input_ext

    results = run_all_config(str(f))
    csv_row = file + ','
    for i in range(len(results)):
        csv_row+=results[i]
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
    print ('ERROR: No command line argument')
