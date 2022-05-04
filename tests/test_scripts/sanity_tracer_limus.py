#!/usr/bin/python
import os
import subprocess
import sys
import glob



res_dir = 'tests/test_scripts/result'


def process_result(output):
    for line in output:
        if 'Verified sound and optimal fence synthesis' in line:
            return 'OK'
        elif 'Redundant fence' in line:
            return 'Redundant fence'
        elif 'Fence too strong' in line:
            return 'Stronger Fence'
        elif 'Correctness fail' in line:
            return 'Correctness fail'
    return 'Do not know'

def sanity_check(pathname, filename):
    print('checking', pathname, filename)
    filepath = '../../' + os.path.join(pathname, filename)
    # print(filepath)
    process_command = ['python3', 'sanity.py', filepath]
    # print(process_command)
    process = subprocess.Popen(process_command, cwd = './src/sanity_check', stdout=subprocess.PIPE, bufsize=1, universal_newlines=True)
    lines = process.stdout.readlines()
        
    status = process_result(lines)
    # print(status)
        
    return filename + ',' + status

def write_csv_header(csv_file_name):
    csv_header = 'Test Name, Sanity \n'
    csv_file = open(os.path.join(res_dir, csv_file_name), 'w')
    csv_file.write(csv_header)
    csv_file.close()
    return

def run_tests_from_file (filename):
    with open(filename, 'r') as file:
        test_list = file.read().replace("'", "").strip().split(' ')
    # print('test dir:', test_list)
    # print('cwd: ', cwd)
    csv_file_name = 'sanity-' + os.path.splitext(os.path.basename(filename))[0] + '.csv'

    write_csv_header(csv_file_name)
    for test in test_list:
        filename, ext = os.path.splitext(os.path.basename(test))
        if ext != '.cc':

            print('ext should be .cc. Found:', filename, ext)
        csv_row = sanity_check(os.path.dirname(test), filename+ext)
        csv_row = csv_row + '\n'
        csv_file = open(os.path.join(res_dir, csv_file_name), 'a')
        
        csv_file.write(csv_row)
        csv_file.close()



cwd = os.getcwd()
if not os.path.exists(res_dir):
    os.system('mkdir ' + res_dir)

if len(sys.argv) == 2:
    run_tests_from_file(sys.argv[1])
    exit(0)
else:
    print ('ERROR: No command line argument')
