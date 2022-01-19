import sys

from os import listdir
from os.path import isfile, join


n = len(sys.argv)
if n != 2:
    print('USAGE: gen_list_from_dir.py <directory>')
    exit()

dir_name = sys.argv[1]

# dir_name = '../litmus/tracer_litmus_mod/'
files = [join(dir_name, f) for f in listdir(dir_name) if isfile(join(dir_name, f))]
print(files[0]) 

batch_size = 1000

file_list = ""
batch_number = 1

def flush():
    global file_list
    global batch_number

    file = open('list_tracer_litmus' + str(batch_number), 'w')
    # file.write('tracer_litmus = [')
    # for i in range(len(file_list)-1):
    file.write(file_list)
    # file.write("'" + file_list[-1])
    file.close()

    file_list = ""
    batch_number = batch_number + 1


done_file_count  = 0
total_file_count = len(files)

for file in files:
    if file[-3:] == '.cc':
        file_list +=  "'" + file + "' "
        done_file_count = done_file_count + 1
        if done_file_count % batch_size == 0 and total_file_count - done_file_count > batch_size:
            flush()
    else:
        print('ignoring ' + file)

flush()
print('recorded ' + str(done_file_count) + ' file in ' + str(batch_number-1) + ' batches')