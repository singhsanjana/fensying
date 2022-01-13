from operator import itemgetter
from os import fwalk
from constants import file_info as fi
from constants import f_tags as ft
from constants import *

def fix_includes(lines, modified_files):
	for i in range(len(lines)):
		if '#include' in lines[i] and '"' in lines[i] and not '_fixed' in lines[i]:
			line = lines[i][lines[i].find('"')+1:]
			filename = line[:line.find('"')]
			if filename in modified_files:
				filename = filename[:filename.rfind('.')] + '_fixed' + filename[filename.rfind('.'):]
				lines[i] = '#include "' + filename + '"\n'
	return lines

def insert(fence_tags_by_file, input_filename):
	path = input_filename[:-1*len(input_filename.split('/')[-1])]
	
	for filename in fence_tags_by_file:
		filepath = filename
		if not filename.startswith(path):
			filepath = path + filename
		with open(filepath) as f:
			lines = f.readlines()

		lines = fix_includes(lines, fence_tags_by_file.keys())
		
		if fi.OUTPUT_FILE_APPEND_STRING in filename:
			filepath_new = filepath
		else:
			extension_length = -1*len(fi.FILE_EXTENSION)
			filepath_new = filepath[:extension_length]+fi.OUTPUT_FILE_APPEND_STRING

		if filepath[:-3] == input_filename[:-2]: # input file (.o) is object of current file (.cc)
			input_filename = filepath_new[:-3] + '.o'

		output_file = open(filepath_new,'w')
		
		count_modified_fences = 0
		for f, order_req in fence_tags_by_file[filename]:
			already_has_fence, fen_index, order = get_fence_to_insert(f, order_req, lines)

			# insert the fence
			if not already_has_fence:
				# Synthesize fence
				lines[fen_index] += fi.FENCE_INSTRUCTIONS[order]
			else:
				# Strengthen order of existing fence
				count_modified_fences += 1
				lines[fen_index] = fi.FENCE_INSTRUCTIONS[order]

		for w in lines:
			output_file.writelines(w)

	return (input_filename, count_modified_fences)

def get_fence_to_insert(f, order_req, lines):
	line_no = int(f[f.rfind('_')+1:])
	if 'before' in f:
		prgm_fen_index = line_no - 2
		index_to_insert = prgm_fen_index
	elif 'after' in f: 
		prgm_fen_index = line_no
		index_to_insert = line_no - 1
	elif 'at' in f:
		prgm_fen_index = line_no -1
		index_to_insert = prgm_fen_index	
	has_prgm_fence, prgm_fence_ord = get_prgm_fence_at_line_no(lines[prgm_fen_index])
	if has_prgm_fence:
		order_req = get_updated_fence_order(order_req, prgm_fence_ord)
	return has_prgm_fence, index_to_insert, order_req

def get_updated_fence_order(order_req, prgm_fence_ord):
	if order_req == prgm_fence_ord:
		return prgm_fence_ord
	elif order_req == ft.sc or prgm_fence_ord == ft.sc:
		return ft.sc
	elif order_req == ft.ar or prgm_fence_ord == ft.ar:
		return ft.ar
	elif (order_req == ft.a and prgm_fence_ord == ft.r) or \
		(order_req == ft.r and prgm_fence_ord == ft.a):
		return ft.ar
	else:
		print("ERROR: fence order cannot be relaxed")

def get_prgm_fence_at_line_no(line):
	has_fence = False
	ord = RLX
	if 'atomic_thread_fence' in line:
		has_fence = True
		ord = get_prgm_fence_ord(line)
	return has_fence, ord

def get_prgm_fence_ord(line):
	if 'seq_cst' in line:
		return ft.sc
	elif 'acquire' in line:
		return ft.a
	elif 'release' in line:
		return ft.r
	elif 'acq_rel' in line:
		return ft.ar
	else:
		print('ERROR: line', line, 'is not a fence instruction')


	

# TO FIX:
