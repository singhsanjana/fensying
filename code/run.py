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
    result_generated = False
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
            result_generated = True
            break

    synthesized   = ''
    strengthened  = ''
    time_ceg      = ''
    time_z3       = ''
    time_fensying = ''
    time_total    = ''
    attributes_collected = 0
    total_attributes     = 0
    
    if result_generated:
        total_attributes = 6
    
    if aborted:
        total_attributes = 3
    
    if timed_out:
        total_attributes = 0

    if not (result_generated or aborted or timed_out):
        csv_out = 'run not completed' + ',,,,,,'
        output = ''
        output = output.join(map(str, lines)) # list to string
        return (csv_out, output)

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
        output = 'ATRIBUTES MISMATCH collected=' + str(attributes_collected) + ' total=' + str(total_attributes) + '\n'
    output = output.join(map(str, lines)) # list to string
    return (csv_out, output)



rmw_tests = {
    'benchmarks/cds_examples' : ['crew_3.cc', 'fib_bench_false-unreach-call.cc', 'fib_bench_false-unreach-call.cc', 'read_write_lock_unreach_13.cc', 'read_write_lock_unreach_13.cc', 'read_write_lock_unreach_12.cc', 'read_write_lock_unreach_12.cc', 'crew_2.cc', 'crew_2.cc', 'read_write_lock_3.cc', 'read_write_lock_3.cc', 'lamport_true_unreach.cc', 'lamport_true_unreach.cc', 'lamport_true_unreach.cc', 'lamport_true_unreach.cc', 'crew_1.cc', 'crew_1.cc', 'fib_bench_true-unreach-call.cc', 'fib_bench_true-unreach-call.cc', 'read_write_lock_2.cc', 'read_write_lock_2.cc', 'fib_bench_true-longest-unreach-call.cc', 'fib_bench_true-longest-unreach-call.cc', 'fib_mod_false-unreach-call.cc', 'fib_mod_false-unreach-call.cc', 'fib_mod_true-unreach-call.cc', 'fib_mod_true-unreach-call.cc', 'dekker_false-unreach-call.cc', 'dekker_false-unreach-call.cc', 'dekker_false-unreach-call.cc', 'dekker_false-unreach-call.cc', 'read_write_lock_unreach_11.cc', 'read_write_lock_unreach_11.cc', 'mot_eg_4.cc', 'mot_eg_4.cc', 'mot_eg_2.cc', 'mot_eg_2.cc', 'mot_eg_3.cc', 'mot_eg_3.cc'],
    'benchmarks/genmc_examples' : ['hwqueue-ra0.cc', 'hwqueue-ra0.cc', 'hwqueue-ra0.cc', 'hwqueue-ra0.cc', 'inc+inc+rr+w+rr0.cc', 'inc+inc+rr+w+rr0.cc'],
    'benchmarks/misc' : [],
    'benchmarks/rcmc_examples' : ['indexer0.cc'],
    'benchmarks/tracer_examples' : ['MOREDETOUR0685.cc', 'MOREDETOUR0685.cc', 'MOREDETOUR0687.cc', 'MOREDETOUR0687.cc', 'W+RWC+po+lwsync+po.cc', 'R0098.cc', 'MOREDETOUR0406.cc', 'MOREDETOUR0406.cc', 'DETOUR0895.cc', 'DETOUR0895.cc', 'WRW+2W+lwsync+po.cc', '3.2W+lwsync+lwsync+po.cc', '3.2W+lwsync+lwsync+po.cc', 'Z6.5+lwsync+rfi-data+rfi-addr.cc', 'SB0059.cc', 'DETOUR0958.cc', 'DETOUR0958.cc', 'R0119.cc', 'm5dl.cc', 'MOREDETOUR0874.cc', 'MOREDETOUR0874.cc', 'DETOUR0928.cc', 'MOREDETOUR0869.cc', 'MOREDETOUR0869.cc', 'Z6.4+po+lwsync+po.cc', 'R+po+isync.cc', 'ppc-cookbook6.1.partbarrier.cc', 'MOREDETOUR0398.cc', 'IRIW+isync+po.cc', 'Z6.0+po+addr+lwsync.cc', '2+2W0053.cc', 'Z6.4+po+po+sync.cc', 'R0102.cc', 'WRR+2W+addr+lwsync.cc'],
    'benchmarks/watts_examples' : [],
    'litmus' : [],
    'litmus/weak_fensying' : []
}

directories = [
    "benchmarks/cds_examples", 
    "benchmarks/genmc_examples", 
    "benchmarks/misc",
    "benchmarks/rcmc_examples", 
    "benchmarks/tracer_examples",
    "benchmarks/watts_examples",
    "litmus", 
    "litmus/weak_fensying"
]
t_flags = [0, 1, 2]
result_dir = 'result'

csv_header = 'Directory,Test Name,'
for t in t_flags:
    csv_header_t = '#synthesized,#strengthened,Time-CEG,Time-Z3,Time-fensying,Time-total,'
    if t > 0:
        csv_header_t = csv_header_t.replace(',', '(t=' + str(t) + '),')
    csv_header += (csv_header_t)
csv_header = csv_header[:-1] + '\n'

if not os.path.exists(result_dir):
    os.system('mkdir ' + result_dir)

csv_file = open(result_dir + '/result.csv', 'w')
csv_file.write(csv_header)

dump_file = open(result_dir + '/result_dump', 'w')

for dir_name in directories:
    print ('\n-------- Entering ', dir_name, ' -----------')
    
    files = [f for f in listdir(dir_name) if isfile(join(dir_name, f))]
    for filename in files:
        if filename[-3:] != '.cc':
            continue
        if '_fixed' in filename:
            continue
        if filename in rmw_tests[dir_name]:
            continue

        print ('Running ', filename)

        csv_row = dir_name + ',' + filename + ','
        output_dump = '\n=================== ' + filename + ' =================\n'
        for t_flag in t_flags:
            (csv, dump) = run_file(dir_name, filename, t_flag)
            csv_row += csv
            output_dump += dump

        csv_row = csv_row[:-1] + '\n'

        csv_file.write(csv_row)
        dump_file.write(output_dump)
    
    print ('--------- Leaving ', dir_name, ' -----------')

csv_file.close()
dump_file.close()
