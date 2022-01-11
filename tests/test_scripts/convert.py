import os

def convert_action(line, pattern):
    has_line = False
    has_file = False

    if '__LINE__' in line:
        has_line = True
    if '__FILE__' in line:
        has_file = True

    if has_file and has_line:
        return line
    if has_line:
        l = line.find(pattern) + len(pattern)
        while line[l] != '(':
            l += 1
        return line[:l+1] + '__FILE__, ' + line[l+1:]
    if has_file:
        l = line.find('__FILE__') + len('__FILE__,')
        return line[:l+1] + ' __LINE__,' + line[l+1:]

    l = line.find(pattern) + len(pattern)
    while line[l] != '(':
        l += 1
    return line[:l+1] + '__FILE__, __LINE__, ' + line[l+1:]

def convert_load(line):
    return convert_action(line, 'atomic_load_explicit')

def convert_store(line):
    return convert_action(line, 'atomic_store_explicit') 

def convert_fetch_add(line):
    return convert_action(line, 'atomic_fetch_add_explicit') 

def convert_fetch_sub(line):
    return convert_action(line, 'atomic_fetch_sub_explicit') 

def convert_fence(line):
    return convert_action(line, 'atomic_thread_fence') 

def convert_main(line):
    start = line.find('main')
    end   = start + len('main')
    return line[:start] + 'user_main' + line[end:]

def convert_thread_declaration(line):
    start = line.find('pthread_t')
    end   = start + len('pthread_t')
    return line[:start] + 'thrd_t' + line[end:]

def convert_thread_create(line):
    l = line.find('pthread_create')
    newline = line[:l]

    if l != 0 and line[l-1] != ' ' and line[l-1] != '=':
        return line

    line = line[l + len('pthread_create'):]
    
    temp = ''
    if '&' in line:
        temp = line.split('&')[1]
        create = 'thrd_create(&'
    else:
        l = 0
        while line[l] != '(':
            l += 1
        temp = line[l+1:]
        create = 'thrd_create('
    
    tid  = temp.split(',')[0]
    fn   = temp.split(',')[2]

    create += tid + ', (thrd_start_t)&' + fn + ', NULL)'

    lb = 0
    lr = 0
    l  = 0
    for c in line:
        if lb > 0 and lb == lr:
            break
    
        if c == '(':
            lb += 1
        elif c == ')':
            lr += 1

        l += 1
    
    newline = newline + create + line[l:]
    return newline

def convert_thread_join(line):
    l = line.find('pthread_join')
    newline = line[:l]

    if l != 0 and line[l-1] != ' ' and line[l-1] != '=':
        return line

    line = line[l + len('pthread_join'):]
    temp = line.split('(')[1]
    tid  = temp.split(',')[0]

    join = 'thrd_join(' + tid + ')'

    lb = 0
    lr = 0
    l  = 0
    for c in line:
        if lb > 0 and lb == lr:
            break

        if c == '(':
            lb += 1
        elif c == ')':
            lr += 1

        l += 1
    
    newline = newline + join + line[l:]
    return newline

def negate_assert(line):
    if 'if' in line:
        l = line.find('if')
        newline = line[:l] + 'if'

        l += len('if')
        while line[l] != '(':
            newline += line[l]
            l += 1

        line = line[l+1:]
        newline += '('

        lb = 1
        rb = 0
        l  = 0
        for c in line:
            if lb == rb:
                break

            if c == '(':
                lb += 1
            elif c == ')':
                rb += 1

            l += 1

        condition = line[:l-1]
        condition.replace(' ', '')
        if condition == '0':
            condition = '1'
        elif condition == '1':
            condition = '0'
        else:
            condition = ' !(' + condition + ') '


        line = newline + condition + ')' + line[l:]
    
def convert_assert(line):
    # line = negate_assert(line)
    start = line.find('assert') 
    end   = start + len('assert')
    return (line[:start] + 'MODEL_ASSERT' + line[end:])

base_dir = 'tests'

filecount = 0
fileModcount = 0

for root, subdirs, files in os.walk(base_dir):
    for filename in files:
        if not (filename[-2:] == '.c' or filename[-3:] == '.cc' or filename[-4:] == '.cpp'):
            continue

        print('converting ' + filename)
        mv_cmd = 'mv ' + str(os.path.join(root, filename)) + ' '
        
        if filename[-2:] == ".c":
            filename = filename[:-1] + 'cc'
            filecount += 1
            mv_cmd += str(os.path.join(root, filename))
            os.system(mv_cmd)
        
        if filename[-4:] == '.cpp':
            filename = filename[:-3] + 'cc'
            filecount += 1
            mv_cmd += str(os.path.join(root, filename))
            os.system(mv_cmd)

        f = open(os.path.join(root, filename), 'r')
        lines = f.readlines()
        
        newlines = ''

        for line in lines:
            line_without_spaces = line.replace(' ', '')

            if '#include <pthread.h>' in line:
                line = '#include <threads.h>'
            elif '#include <assert.h>' in line:
                line = '#include "librace.h" \n#include "model-assert.h"\n'
            elif 'atomic_load_explicit' in line:
                line = convert_load(line)
            elif 'atomic_store_explicit' in line:
                line = convert_store(line)
            elif 'atomic_fetch_add_explicit' in line:
                line = convert_fetch_add(line)
            elif 'atomic_fetch_sub_explicit' in line:
                line = convert_fetch_sub(line)
            elif 'atomic_thread_fence' in line:
                line = convert_fence(line)
            # elif 'atomic_int' in line:
            #     line = convert_declaration(line)
            # elif 'main(' in line_without_spaces:
            #     line = convert_main(line)
            elif 'pthread_t' in line:
                line = convert_thread_declaration(line)
            elif 'pthread_create' in line:
                line = convert_thread_create(line)
            elif 'pthread_join' in line:
                line = convert_thread_join(line)
            # not required for Tracer benchmarks.
            # elif 'assert' in line:
            #     line = convert_assert(line) # NOTE THIS NEGATES ASSERT CONDITIONS TOO

            newlines += line

        f.close()

        f = open(os.path.join(root, filename), 'w')
        f.write(newlines)
        f.close()
        
        print('converted ' + filename)
        fileModcount += 1

print('\n-------------------------------------------\n')
print (str(filecount) + ' filenames chanaged')
print (str(fileModcount) + ' files modified')
