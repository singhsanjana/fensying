import os
import subprocess
import shlex
import argparse
import time
import signal

from os import listdir
from os.path import isfile, join

def clean(str):
    str = str.replace('\n', '')
    str = str.replace('\t', '')
    str = str.replace(' ' , '')
    return str

def run_file(dir_name, filename, t=0):
    f = dir_name + "/" + filename

    process_command = ['python3', 'main.py', '-f', f]
    if t > 0:
        process_command.append('-t')
        process_command.append(str(t))

    process = subprocess.Popen(process_command, stdout=subprocess.PIPE, bufsize=1, universal_newlines=True)
    lines = process.stdout.readlines()
    
    aborted   = False
    timed_out = False
    time_out_msg = ''

    for i in range(len(lines)):
        if 'ABORT' in lines[i]:
            aborted = True

        if 'Model Checking time exceeded' in lines[i]:
            time_limit = lines[i][len('Model Checking time exceeded '):]
            time_limit = time_limit.split('minutes')[0]
            time_limit = time_limit.replace(' ', '')

            timed_out = True
            time_out_msg = 'CDS TO (' + time_limit + ')'

        if 'Tool time exceeded' in lines[i]:
            time_limit = lines[i][len('Tool time exceeded '):]
            time_limit = time_limit.split('minutes')[0]
            time_limit = time_limit.replace(' ', '')

            timed_out = True
            time_out_msg = 'Fensying TO (' + time_limit + ')'

        if 'RESULT SUMMARY' in lines[i]:
            break

    synthesized   = ''
    strengthened  = ''
    time_ceg      = ''
    time_z3       = ''
    time_fensying = ''
    time_total    = ''
    attributes_collected = 0
    total_attributes     = 6
    if aborted:
        total_attributes = 3
    elif timed_out:
        total_attributes = 0

    for j in range(i,len(lines)):
        line = clean(lines[j])
        val = ''
        if ':' in line:
            val = line.split(':')[1]

        if 'Totalfencessynthesized' in line:
            synthesized = val[:-3]
            attributes_collected+=1
            continue

        if 'Totalfencesstrengthened' in line:
            strengthened = val[:-3]
            attributes_collected+=1
            continue

        if 'Time-CEG' in line:
            time_ceg = val
            attributes_collected+=1
            continue

        if 'Time-Z3' in line:
            time_z3 = val
            attributes_collected+=1
            continue

        if 'Time-Fensying' in line:
            time_fensying = val
            attributes_collected+=1
            continue

        if 'Time-Total' in line:
            time_total = val
            attributes_collected+=1
            continue

    if timed_out:
        csv_out = time_out_msg + ',,,,,,'
    elif aborted:
        csv_out = 'ABORT,,' + time_ceg + ',,' + time_fensying + ',' + time_total + ','
    else:
        csv_out = synthesized + ',' + strengthened + ',' + time_ceg + ',' + time_z3 + ',' + time_fensying + ',' + time_total + ','

    output = ''
    if attributes_collected != total_attributes:
        output = 'ATRIBUTES MISMATCH collected=' + attributes_collected + ' total=' + total_attributes + '\n'
    output = output.join(map(str, lines)) # list to string
    return (csv_out, output)




directories = [
    "benchmarks/cds_examples", "benchmarks/genmc_examples", "benchmarks/misc",
    "benchmarks/rcmc_examples", "benchmarks/tracer_examples",
    "benchmarks/watts_examples",
    "litmus", "litmus/weak_fensying"
]
# directories = ["litmus", "litmus/weak_fensying"]
t_flags = [0, 1, 2]

csv_header = 'Directory,Test Name,'
for t in t_flags:
    csv_header_t = '#synthesized,#strengthened,Time-CEG,Time-Z3,Time-fensying,Time-total,'
    if t > 0:
        csv_header_t = csv_header_t.replace(',', '(t=' + str(t) + '),')
    csv_header += (csv_header_t)
csv_header = csv_header[:-1] + '\n'

csv_rows = ''
output_dump = ''

for dir_name in directories:
    print ('\n-------- Entering ', dir_name, ' -----------')
    
    files = [f for f in listdir(dir_name) if isfile(join(dir_name, f))]
    for filename in files:
        if filename[-3:] != '.cc':
            continue
        if '_fixed' in filename:
            continue

        print ('Running ', filename)

        csv_row = dir_name + ',' + filename + ','
        output_dump += '\n=================== ' + filename + ' =================\n'
        for t_flag in t_flags:
            (csv, dump) = run_file(dir_name, filename, t_flag)
            csv_row += csv
            output_dump += dump

        csv_rows += csv_row[:-1] + '\n'
    
    print ('--------- Leaving ', dir_name, ' -----------')


if not os.path.exists('result'):
    os.system('mkdir result')

csv_file = open('result/result.csv', 'w')
csv_file.write(csv_header)
csv_file.write(csv_rows)
csv_file.close()

dump_file = open('result/result_dump', 'w')
dump_file.write(output_dump)
dump_file.close()