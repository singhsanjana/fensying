#!/usr/bin/python3
import os
import sys

def convert_load(line):
    l = line.find('atomic_load_explicit') + len('atomic_load_explicit')
    while line[l] != '(':
        l += 1

    return line[:l+1] + '__FILE__, __LINE__, ' + line[l+1:]

def convert_store(line):
    l = line.find('atomic_store_explicit') + len('atomic_store_explicit')
    while line[l] != '(':
        l += 1

    return line[:l+1] + '__FILE__, __LINE__, ' + line[l+1:]

def convert_fetch_add(line):
    l = line.find('atomic_fetch_add_explicit') + len('atomic_fetch_add_explicit')
    while line[l] != '(':
        l += 1

    return line[:l+1] + '__FILE__, __LINE__, ' + line[l+1:]

def convert_fetch_sub(line):
    l = line.find('atomic_fetch_sub_explicit') + len('atomic_fetch_sub_explicit')
    while line[l] != '(':
        l += 1

    return line[:l+1] + '__FILE__, __LINE__, ' + line[l+1:]

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

    line = line[l + len('pthread_create'):]
    temp = line.split('&')[1]
    tid  = temp.split(',')[0]
    fn   = temp.split(',')[2]

    create = 'thrd_create(&' + tid + ', (thrd_start_t)&' + fn + ', NULL)'

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

def convert_assert(line):
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
    
    start = line.find('assert') 
    end   = start + len('assert')
    return (line[:start] + 'MODEL_ASSERT' + line[end:])


n = len(sys.argv)
if n != 4:
    print('USAGE: convert.py <input filename> -o <output filename>')
    exit()
isOutputFileName = False
for i in range(1, n):
    if sys.argv[i] == '-o':
        isOutputFileName = True
    elif isOutputFileName:
        outFile = sys.argv[i]
        isOutputFileName = False
    else:
        inFile = sys.argv[i]

#print('infile:', inFile, 'outfile:', outFile)

filename = inFile
#print('converting ' + filename)

f = open(filename, 'r')
#print('opened inFile')
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
    # elif 'atomic_int' in line:
    #     line = convert_declaration(line)

    # not required for Tracer benchmarks. 
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

if not os.path.exists(os.path.dirname(outFile)):
    try:
        os.makedirs(os.path.dirname(outFile))
    except OSError as exc: # Guard against race condition
        if exc.errno != errno.EEXIST:
            raise

f = open(outFile, 'w+')
#print('opening outFile')
f.write(newlines)
f.close()
#print('done')

