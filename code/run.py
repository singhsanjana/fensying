import subprocess
import os
import sys

sys.path.append('test_scripts')

#### TO CHANGE #######
from list_tracer_litmus1 import tracer_litmus as files
output_filename = '/result_' + 'litmus_tracer1' + '.csv'
dir_name = 'litmus/tracer_litmus_mod/'
#####################

def add(val1, val2):
    if val1 == '':
        return val2
    if val2 == '':
        return val1
    return str(float(val1) + float(val2))

def avg(val):
    if val == '':
        return '0'
    return str(float(val) / 3)

def clean(str):
    str = str.replace('\n', '')
    str = str.replace('\t', '')
    str = str.replace(' ' , '')
    return str

def run_file(filename, t=0):
    global dir_name
    f = dir_name + "/" + filename

    process_command = ['python3', 'main.py', '-f', f]
    if t > 0:
        process_command.append('-t')
        process_command.append(str(t))

    _synthesized   = ''
    _strengthened  = ''
    _time_ceg      = ''
    _time_z3       = ''
    _time_fensying = ''
    _time_total    = ''

    _aborted = False
    _result_generated = False

    for run_id in range(3): # take avg of 3 runs
        process = subprocess.Popen(process_command, stdout=subprocess.PIPE, bufsize=1, universal_newlines=True)
        lines = process.stdout.readlines()
        
        aborted   = False
        result_generated = False
        
        for i in range(len(lines)):
            if 'ABORT' in lines[i]:
                aborted = True

            if 'Model Checking time exceeded' in lines[i]:
                time_limit = lines[i][len('Model Checking time exceeded '):]
                time_limit = time_limit.split('minutes')[0]
                time_limit = time_limit.replace(' ', '')

                csv_out = 'CDS TO (' + time_limit + ')' + ',,,,,,'
                return (csv_out, 'TO')

            if 'Tool time exceeded' in lines[i]:
                time_limit = lines[i][len('Tool time exceeded '):]
                time_limit = time_limit.split('minutes')[0]
                time_limit = time_limit.replace(' ', '')

                csv_out = 'Fensying TO (' + time_limit + ')' + ',,,,,,'
                return (csv_out, 'TO')

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
        
        if not (result_generated or aborted):
            csv_out = 'run not completed' + ',,,,,,'
            return (csv_out, 'FAIL')

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
                if aborted:
                    total_attributes = 4
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

        if run_id == 0:
            if float(time_fensying) == 0.0:
                csv_out = 'NO-BUGGY-TRACES' + ',,,,,,'
                return (csv_out, 'NOBUG')

            _synthesized = synthesized
            _strengthened = strengthened
            _time_ceg = time_ceg
            _time_z3 = time_z3
            _time_fensying = time_fensying
            _time_total = time_total
            _aborted = aborted
            _result_generated = result_generated

        if run_id > 0:
            _time_ceg = add(_time_ceg, time_ceg)
            _time_fensying = add(_time_fensying, time_fensying)
            _time_total = add(_time_total, time_total)

            if (_synthesized != synthesized) or (_strengthened != strengthened) or (_aborted != aborted):
                csv_out = 'result-mismatch-accross-runs' + ',,,,,,'
                return (csv_out, 'NONDET')

            if not aborted: # run completed
                _time_z3 = add(_time_z3, time_z3)


    _time_ceg = avg(_time_ceg)
    _time_fensying = avg(_time_fensying)
    _time_total = avg(_time_total)
    if _aborted:
        csv_out = 'ABORT,,' + _time_ceg + ',,' + _time_fensying + ',' + _time_total + ','
    else:
        _time_z3 = avg(_time_z3)
        csv_out = _synthesized + ',' + _strengthened + ',' + _time_ceg + ',' + _time_z3 + ',' + _time_fensying + ',' + _time_total + ','

    if attributes_collected != total_attributes:
        print('ATRIBUTES MISMATCH collected=' + str(attributes_collected) + ' total=' + str(total_attributes) + '\n')
    return (csv_out, 'OK')


t_flags = [0, 1, 2]
result_dir = 'result'

csv_header = 'Test Name,'
for t in t_flags:
    csv_header_t = '#synthesized,#strengthened,Time-CEG,Time-Z3,Time-fensying,Time-total,'
    if t > 0:
        csv_header_t = csv_header_t.replace(',', '(t=' + str(t) + '),')
    csv_header += (csv_header_t)
csv_header = csv_header[:-1] + '\n'

if not os.path.exists(result_dir):
    os.system('mkdir ' + result_dir)

csv_file = open(result_dir + output_filename, 'w')
csv_file.write(csv_header)

for filename in files:
    if filename[-3:] != '.cc':
        continue
    if '_fixed' in filename:
        continue

    print ('Running ', filename)

    csv_row = filename + ','
    output_dump = '\n=================== ' + filename + ' =================\n'
    for t_flag in t_flags:
        (csv, status) = run_file(filename, t_flag)
        csv_row += csv
        if status != 'OK':
            csv_row += '\n'
            break

    csv_row = csv_row[:-1] + '\n'

    csv_file.write(csv_row)
    
csv_file.close()