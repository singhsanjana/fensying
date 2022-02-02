from pathlib import Path
from os import fwalk
from constants import file_info as fi
from constants import f_tags as ft
from constants import *

def file_write(filepath, lines):
	file = open(filepath, 'w')
	for line in lines:
		file.write(line)
	file.close()

def fix_includes(lines, include_fixes, modified_files):
	modified_includes = False

	for (i, filename, line_new) in include_fixes:
		if filename in modified_files:
			lines[i] = line_new
			modified_includes = True
	
	return lines, modified_includes

def list_includes(lines, path):
	included_files = []
	include_fixes = []
	modified_includes = False

	for i in range(len(lines)):
		# if '#include' in lines[i] and '"' in lines[i] and not '_fixed' in lines[i]:
		if '#include' in lines[i] and '"' in lines[i]:
			line = lines[i][lines[i].find('"')+1:]
			filename = line[:line.find('"')]   # extracted filename from include
			
			filepath = Path(path + filename)
			if filepath.is_file():
				included_files.append(filename)
				if not '_fixed' in lines[i]:
					filename_new = filename[:filename.rfind('.')] + '_fixed' + filename[filename.rfind('.'):]
					line_new = '#include "' + filename_new + '"\n'
					row = (i, filename, line_new)
					include_fixes.append(row)				
	
	return included_files, include_fixes

def add_fences_to_file(filename, lines, fence_tags_by_file):		
	count_strengthened_fences = 0
	is_modified = False

	for f, order_req in fence_tags_by_file[filename]:
		already_has_fence, is_strengthened, fen_index, order = get_fence_to_insert(f, order_req, lines)

		# insert the fence
		if not already_has_fence:
			# Synthesize fence
			lines[fen_index] += fi.FENCE_INSTRUCTIONS[order]
			is_modified = True
		elif is_strengthened:
			# Strengthen order of existing fence
			count_strengthened_fences += 1
			lines[fen_index] = fi.FENCE_INSTRUCTIONS[order]
			is_modified = True

	return lines, count_strengthened_fences, is_modified

def recursive_insert(filename, path, fence_tags_by_file, modified_files, done_files):
	fence_files = fence_tags_by_file.keys()
	
	filepath = path + filename
	fileext  = filename[ filename.rfind('.'): ]

	with open(filepath) as f:
		lines = f.readlines()

	included_files, include_fixes = list_includes(lines, path)
	count_strengthened_fences = 0

	# first fix included files
	for file in included_files:
		if file in done_files: # included files already updated
			continue

		c,m,d = recursive_insert(file, path, fence_tags_by_file, modified_files, done_files)
		count_strengthened_fences += c
		modified_files = m
		done_files = d

	# fix current file
	lines, this_modified = fix_includes(lines, include_fixes, modified_files)

	if filename in fence_files:
		lines, this_count_strengthened_fences, added_fence = add_fences_to_file(filename, lines, fence_tags_by_file)
		this_modified = this_modified or added_fence
		count_strengthened_fences += this_count_strengthened_fences

	filepath_new = filepath
	
	if this_modified:
		if not fi.OUTPUT_FILE_APPEND_STRING in filename:
			extension_length = -1 * len(fileext)
			filepath_new = filepath[:extension_length] + fi.OUTPUT_FILE_APPEND_STRING + fileext

		file_write(filepath_new, lines)
		modified_files.append(filename)
	done_files.append(filename)

	return count_strengthened_fences, modified_files, done_files

def insert(fence_tags_by_file, filename, input_ext): # filename has path and extn .o
	path = filename[ : -1*len(filename.split('/')[-1])]
	input_filename = filename[ -1*len(filename.split('/')[-1]) : ]
	input_filename = input_filename[ : input_filename.rfind('.')+1 ] + input_ext
	
	count_strengthened_fences, modified_files, d = recursive_insert(input_filename, path, fence_tags_by_file, [], [])

	filename_new = filename
	if input_filename in modified_files:
		if not fi.OUTPUT_FILE_APPEND_STRING in filename:
			extension_length = -1 * len('.o')
			filename_new = filename[:extension_length] + fi.OUTPUT_FILE_APPEND_STRING + '.o'

	return filename_new, count_strengthened_fences, modified_files

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
	is_strengthened = False

	if has_prgm_fence:
		order_req = get_updated_fence_order(order_req, prgm_fence_ord)
		if order_req != prgm_fence_ord:
			is_strengthened = True

	return has_prgm_fence, is_strengthened, index_to_insert, order_req

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