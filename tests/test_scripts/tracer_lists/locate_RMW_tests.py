import os

base_dir = 'C-tests'
files = [f for f in os.listdir(base_dir) if os.path.isfile(os.path.join(base_dir, f))]

rmw_files = '['
filecount = 0

for filename in files:
    file = open(base_dir + '/' + filename, 'r')
    lines = file.readlines()

    for line in lines:
        if 'fetch_' in line or 'compare_' in line:
            rmw_files = rmw_files + "'" + filename + "',\n"
            break

    file.close()
    filecount += 1

rmw_files += "'']"
print (rmw_files)
print (str(filecount) + ' files checked')