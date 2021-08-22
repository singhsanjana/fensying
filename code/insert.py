from operator import itemgetter
from constants import file_info as fi
from constants import f_tags as ft
from constants import *

def insert(fences, fence_tags, filename, fences_present_locs):
	with open(filename) as f:
		lines = f.readlines()
	
	if fi.OUTPUT_FILE_APPEND_STRING in filename:
		filename_new = filename
	else:
		extension_length = -1*len(fi.FILE_EXTENSION)
		filename_new = filename[:extension_length]+fi.OUTPUT_FILE_APPEND_STRING

	output_file = open(filename_new,'w')

	for f in fences:
		if f not in fences_present_locs:
			pos = int(f[1:])
			order = fence_tags[f]
			lines[pos-1] += fi.FENCE_INSTRUCTIONS[order]

	for w in lines:
		output_file.writelines(w)

	return filename_new
