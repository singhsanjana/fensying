import os
import sys

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
    l += 1
    addtext = '__FILE__, __LINE__'
    intext = line[ l : l+(line[l:].find(')')) ]
    if intext.strip() != '':
        addtext += ', '
    return line[:l] + addtext + line[l:]

def convert_load(line):
    if 'atomic_load_explicit' in line:
        return convert_action(line, 'atomic_load_explicit')
    return convert_action(line, '.load')

def convert_store(line):
    if 'atomic_store_explicit' in line:
        return convert_action(line, 'atomic_store_explicit')
    return convert_action(line, '.store')

def convert_fetch_add(line):
    if 'atomic_fetch_add_explicit' in line:
        return convert_action(line, 'atomic_fetch_add_explicit')
    return convert_action(line, '.fetch_add')

def convert_fetch_sub(line):
    if 'atomic_fetch_sub_explicit' in line:
        return convert_action(line, 'atomic_fetch_sub_explicit')
    return convert_action(line, '.fetch_sub')

def convert_fetch_or(line):
    if 'atomic_fetch_or_explicit' in line:
        return convert_action(line, 'atomic_fetch_or_explicit')
    return convert_action(line, '.fetch_or')

def convert_fetch_and(line):
    if 'atomic_fetch_and_explicit' in line:
        return convert_action(line, 'atomic_fetch_and_explicit')
    return convert_action(line, '.fetch_and')

def convert_compare_exchange(line):
    if 'atomic_compare_exchange_strong_explicit' in line:
        return convert_action(line, 'atomic_compare_exchange_strong_explicit')
    elif 'atomic_compare_exchange_strong' in line:
        return convert_action(line, 'atomic_compare_exchange_strong')
    elif '.compare_exchange_weak' in line:
        return convert_action(line, '.compare_exchange_weak')
    return convert_action(line, '.compare_exchange')

def convert_exchange(line):
    if 'atomic_exchange_explicit' in line:
        return convert_action(line, 'atomic_exchange_explicit')
    return convert_action(line, '.exchange')

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

    if l != 0 and line[l-1] != ' ' and line[l-1] != '=' and line[l-1] != '(':
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

    if l != 0 and line[l-1] != ' ' and line[l-1] != '=' and line[l-1] != '(':
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

def convert_mutex(line, pattern):
    newpattern = 'lock()'
    if pattern == 'pthread_mutex_unlock':
        newpattern = 'unlock()'
    l = line.find(pattern)
    newline = line[:l]

    l += len(pattern)
    l += line[l:].find('(') + 1
    l += line[l:].find(')') + 1

    obj = line[ line[l:].find('(')+1: ]
    obj = obj[ :obj.find(')') ]
    obj.replace(' ','')
    if '&' in obj:
        obj = obj.split('&')[1]
        newline += obj + '.' + newpattern
        newline += line[l:]
    else:
        newline += obj + '->' + newpattern
        newline += line[l:]
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

def convert_files(root, files):
    filecount = 0
    fileModcount = 0

    for filename in files:
        if not (filename[-2:] == '.c' or filename[-3:] == '.cc' or filename[-4:] == '.cpp' or filename[-4:] == '.hpp' or filename[-2:] == '.h'):
            continue
        if filename[:filename.rfind('.')] in ignore:
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
        lkmm_test = False # for gemnc tests
        has_mutex = False
        has_librace_include = False
        has_model_assert_include = False
        has_mutex_include = False

        for line in lines:
            # if line.count('#include') > 1:
            #     pos = line.rfind('#include')
            #     line = line[:pos] + '\n' + line[pos:]
            # elif 'librace.h' in line:
            #     has_librace_include = True
            # elif 'model-assert.h' in line:
            #     has_model_assert_include = True
            # elif '<mutex>' in line:
            #     has_mutex_include = True
            # elif 'lkmm.h' in line:
            #     lkmm_test = True
            #     break
            # elif 'stdatomic.h' in line:
            #     line = line.replace('../','')
            # elif '#include <pthread.h>' in line:
            #     line = '#include <threads.h>\n'
            # elif '#include <assert.h>' in line:
            #     line = ''
            # elif 'model-MODEL_ASSERT.h' in line:
            #     line = line.replace('model-MODEL_ASSERT.h', 'model-assert.h')
            # elif 'MODEL_ASSERT.h' in line:
            #     line = line.replace('MODEL_ASSERT.h', 'model-assert.h')
            # elif '#include' in line and '.c' in line and not '.cc"' in line:
            #     start = line.rfind('.')
            #     end   = line.rfind('"')
            #     line = line[:start] + '.cc' + line[end:]
            # elif 'atomic_load_explicit' in line or '.load' in line:
            if 'atomic_load_explicit' in line or '.load' in line:
                line = convert_load(line)
            elif 'atomic_store_explicit' in line or '.store' in line:
                line = convert_store(line)
            elif 'atomic_fetch_add_explicit' in line or '.fetch_add' in line:
                line = convert_fetch_add(line)
            elif 'atomic_fetch_sub_explicit' in line or '.fetch_sub' in line:
                line = convert_fetch_sub(line)
            elif 'atomic_fetch_or_explicit' in line or '.fetch_or' in line:
                line = convert_fetch_or(line)
            elif 'atomic_fetch_and_explicit' in line or '.fetch_and' in line:
                line = convert_fetch_and(line)
            elif 'compare_exchange' in line:
                line =  convert_compare_exchange(line)
            elif 'atomic_exchange_explicit' in line or '.exchange' in line:
                line =  convert_exchange(line)
            elif 'atomic_thread_fence' in line:
                line = convert_fence(line)
            # elif 'atomic_int' in line:
            #     line = convert_declaration(line)
            # elif 'main(' in line.strip() and not 'user_main' in line:
            #     line = convert_main(line)
            # elif 'atomic_bool' in line:
            #     line = line.replace('atomic_bool', 'bool')
            # elif 'pthread_t' in line:
            #     line = convert_thread_declaration(line)
            # elif 'pthread_create' in line:
            #     line = convert_thread_create(line)
            # elif 'pthread_join' in line:
            #     line = convert_thread_join(line)
            # elif 'pthread_mutex_t' in line:
            #     line = line.replace('pthread_mutex_t', 'std::mutex')
            #     has_mutex = True
            # elif 'pthread_mutex_lock' in line:
            #     line = convert_mutex(line, 'pthread_mutex_lock')
            # elif 'pthread_mutex_unlock' in line:
            #     line = convert_mutex(line, 'pthread_mutex_unlock')
            # elif 'return NULL' in line:
            #     line = line.replace('return NULL', '')
            # # not required for Tracer benchmarks.
            # elif 'assert' in line and not '#include' in line:
            #     line = convert_assert(line) # NOTE THIS NEGATES ASSERT CONDITIONS TOO
            # elif 'abort()' in line:
            #     line = line.replace('abort()', 'MODEL_ASSERT(0)')
            # elif '__VERIFIER_assume' in line:
            #     line = line.replace('__VERIFIER_assume', 'assume')
            # elif '__VERIFIER_MODEL_ASSERT' in line:
            #     line = line.replace('__VERIFIER_MODEL_ASSERT', 'MODEL_ASSERT')
            # elif '__VERIFIER_assert' in line:
            #     line = line.replace('__VERIFIER_assert', 'MODEL_ASSERT')

            newlines += line

        f.close()

        # if lkmm_test:
        #     os.system('rm ' + os.path.join(root, filename))
        #     continue

        f = open(os.path.join(root, filename), 'w')
        # if not has_librace_include:
        #     f.write('#include "librace.h" \n')
        # if not has_model_assert_include:
        #     f.write('#include "model-assert.h"\n')
        # if has_mutex and not has_mutex_include:
        #     f.write('#include <mutex>\n')
        f.write(newlines)
        f.close()
        
        print('converted ' + filename)
        fileModcount += 1

    return filecount, fileModcount

base_dir = 'tests/benchmarks/CDSbench'
# base_dir = 'tests/benchmarks/genmc_tests'
ignore = [] #['apr_1', 'apr_2']

filecount = 0
fileModcount = 0

if len(sys.argv) == 1: # no file passed
    for root, subdirs, files in os.walk(base_dir):
        fc, fmc = convert_files(root, files)
        filecount += fc
        fileModcount += fmc
else:
    filecount, fileModcount = convert_files('', [sys.argv[1]])

print('\n-------------------------------------------\n')
print (str(filecount) + ' filenames chanaged')
print (str(fileModcount) + ' files modified')
