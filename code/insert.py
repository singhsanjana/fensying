from operator import itemgetter
from os import fwalk
from constants import file_info as fi
from constants import f_tags as ft
from constants import *

def insert(req_fences, fence_tags, filename):
	with open(filename) as f:
		lines = f.readlines()
	
	if fi.OUTPUT_FILE_APPEND_STRING in filename:
		filename_new = filename
	else:
		extension_length = -1*len(fi.FILE_EXTENSION)
		filename_new = filename[:extension_length]+fi.OUTPUT_FILE_APPEND_STRING

	output_file = open(filename_new,'w')
	print ('opened file ', filename_new)

	for f in req_fences:
		order_req = fence_tags[f]
		fen_index, order = get_fence_to_insert(f, order_req, lines)

		# insert the fence
		lines[fen_index] += fi.FENCE_INSTRUCTIONS[order]
		# lines[fen_index] += fi.FENCE_INSTRUCTIONS[order]

	for w in lines:
		output_file.writelines(w)

	return filename_new

def get_prgm_fence_near_line_no(lines, index):
	print('event at index', lines[index])
	print('event at index+1', lines[index+1])
	has_fence = False
	ord = RLX
	is_before = False
	is_after = False
	if 'atomic_thread_fence' in lines[index]:
		has_fence = True
		ord = get_prgm_fence_ord(lines[index])
		is_before = True
	if 'atomic_thread_fence' in lines[index+1]:
		has_fence = True
		ord = get_prgm_fence_ord(lines[index+1])
		is_after = True
	return has_fence, ord, is_before, is_after

def get_prgm_fence_ord(line):
	if 'seq_cst' in line:
		return SEQ_CST
	elif 'acquire' in line:
		return ACQ
	elif 'release' in line:
		return REL
	elif 'acq_rel' in line:
		return ACQ_REL
	else:
		print('ERROR: line', line, 'is not a fence instruction')

def get_fence_to_insert(f, order_req, lines):
	print('for ', f)
	line_no = int(f[f.rfind('_')+1:])
	if 'before' in f:
		fen_index = line_no - 2
	elif 'after' in f:
		fen_index = line_no -1
	else: 
		print('ERROR: Something went wrong. Fence', f, 'is neither "before" nor "after" some event. ')
	has_prgm_fence, prgm_fence_ord, is_prgm_fence_before, is_prgm_fence_after = get_prgm_fence_near_line_no(lines, fen_index)
	if not has_prgm_fence:
		return fen_index, order_req
	else:
		pass
	

# TO FIX:
# 1. fence_at can be in the solution. Need to remove this statement
# 2. check fences only before or only after the event and continue checking until an read/write is found
# 3. fence_at: 
# 	if not without -t, need to check only at line no and insert before or after it as per the order,
# 	if with -t, line no will be in the edited program after inserting fence in last itr. So still need to check only at line no
