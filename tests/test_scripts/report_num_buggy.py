from cProfile import run
import os
import subprocess
import sys
import glob
from math import inf

from importlib_metadata import csv

res_dir = 'tests/test_scripts/result'

# N = 3       # no. of runs per tests
# T = [1, inf]  # values to be passed to -t flags. should be in increasing order
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
    for i in range(len(lines)):
        if '#buggy_traces:' in lines[i]:
            return int(lines[i][lines[i].find('#buggy_traces:')+len('#buggy_traces:') : ])

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
    
    process = subprocess.Popen(process_command, stdout=subprocess.PIPE, bufsize=1, universal_newlines=True)
    lines = process.stdout.readlines()
    
    num_buggy = read_result(lines)
    return num_buggy


def run_all_config(filename):
    os.chdir(cwd + '/src')
    csv_row = ''
    
    # config1: no fence and depth bounding
    test_cols = execute_test(filename)
    csv_row += str(test_cols)
    os.chdir(cwd)
    csv_row += '\n'
    return csv_row


def write_csv_header(csv_file_name):
    csv_header = 'Test Name, #buggy \n'

    csv_file = open(os.path.join(res_dir, csv_file_name), 'w')
    csv_file.write(csv_header)
    csv_file.close()
    return

def run_single_test(dir_path, file):
    print('dir_path:', dir_path, 'file:', file)
    print('Testing ' + dir_path + '/' + file[:-2])
    filepath = '../' + os.path.join(dir_path, file)

    input_file = glob.glob(dir_path + '/' + file[:-2]+'.c*')[0]
    input_ext  = input_file[ input_file.rfind('.')+1 : ]

    f = filepath + '$' + input_ext

    csv_row = file + ',' + run_all_config(str(f))
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
