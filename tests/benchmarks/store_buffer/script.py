import sys
import re
import os

def process_file(file_path):
    with open(file_path, 'r') as file:
        content = file.read()

    # Remove pre-existing includes
    content = re.sub(r'#include\s*<.*?>', '', content)
    content = re.sub(r'#include\s*\".*?\"', '', content)

    # Add necessary headers
    header_includes = [
        '#include <stdio.h>',
        '#include <stdlib.h>',
        '#include <pthread.h>',
        '#include <stdatomic.h>',
        '#include <stdbool.h>',
        '#include <assert.h>'
    ]
    content = '\n'.join(header_includes) + '\n\n' + content

    # Modify thread function signatures
    content = re.sub(r'(?<!\w)void\s+(\w+)\s*\((.*?)\)\s*\{', r'void* \1(\2) {', content)

    # Add return NULL at the end of void* functions
    # content = re.sub(r'(?<!\w)void\*\s+(\w+)\s*\((.*?)\)\s*\{([\s\S]*?)(?<![\s\S])return\s+;', r'void* \1(\2) {\3\n    return NULL;', content)
    content = re.sub(r'(?<!\w)void\*\s+(\w+)\s*\((.*?)\)\s*\{([\s\S]*?)(?<!\})\s*\}', r'void* \1(\2) {\3\n    return NULL;}', content)


    # Remove __FILE__ and __LINE__ macros
    content = re.sub(r'(__FILE__,|__LINE__,)', '', content)
    # Remove extra commas
    # content = re.sub(r'(?<=\w),\s*(?=[^\)\(]*\))', '', content)

    # Replace MODEL_ASSERT with assert
    content = re.sub(r'MODEL_ASSERT', 'assert', content)
    content = re.sub(r'thrd_t', 'pthread_t', content)

    # Change thread creation functions
    content = re.sub(r'thrd_create\s*\((.*?),\s*(.*?),\s*(.*?)\)', r'pthread_create(\1, NULL, \2, \3)', content)

    # content = re.sub(r'thrd_create', 'pthread_create', content)
    # content = re.sub(r'thrd_join', 'pthread_join', content)
    content = re.sub(r'thrd_join\s*\((.*?)\)', r'pthread_join(\1, NULL)', content)

    # Change user_main to main
    content = re.sub(r'user_main', 'main', content)

    # Change void functions to void* and add return NULL statements

    return content

# Usage: python modify_c_file.py input_file.cpp output_file.cpp
if __name__ == '__main__':
    if len(sys.argv) < 2:
        print('Usage: python script.py <input_file.cpp>')
        sys.exit(1)

    input_file = sys.argv[1]
    with open(input_file,'r') as file:
        test_list = file.read().replace("'","").strip().split(' ')
    for test in test_list:
        file_name,ext = os.path.splitext(os.path.basename(test))
        output_file = './tests/litmus/tracer_litmus_mod_genmc/modified_' + file_name
        modified_content = process_file(test)

        with open(output_file, 'w') as write_file:
            write_file.write(modified_content)

        print(f'Modified file saved as {output_file}')
