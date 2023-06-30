# --------------------------------------------------------
# Inputs a c++ file and outputs fences and fence positions
# with respect to C++11 version compiler.
#
# main.py runs the entire program.
# Usage:
#   python3 main.py -f [input file]
# --------------------------------------------------------

import argparse
import time
import sys
import os

import publish_result as res
from constants import output_colours as oc
from constants import timeouts as TO
from processing import Processing
from z3run import z3run
from allocate_fence_orders import allocate_fence_orders
from insert import insert
from model_checking_output.model_checking_output import model_checking_output
from model_checking_output.translators.cds_checker.delete_file import delete_generated_file

parser = argparse.ArgumentParser()

# input file
parser.add_argument('file', type=str,
					help="File path of object file.")

# bounds
parser.add_argument("--traces", "-t", type=int, required=False, dest="batch_size",
					help="Batch buggy traces (non-optimal).")
parser.add_argument("--max-iter", "-m", type=int, required=False, dest="max_iter",
					help="Max number of batches. Used with -t flag.")
parser.add_argument("--fence-bound", "-f", type=int, required=False, dest="max_fence",
					help="Max number of fences in cycles.")
parser.add_argument("--max-depth", "-d", type=int, required=False, dest="max_depth",
					help="Max depth when lookign for cycles.")
parser.add_argument("--max-cycles", "-c", type=int, required=False, dest="max_cycles",
					help="Max number of cycles.")
parser.add_argument("--num_fences", "-n", type=int, required=False, dest="num_fences",
					help="Max number of candidate fences tobbe placed.")

# print options
parser.add_argument("--synthesis-summary", "-s", required=False, action='store_true', dest="print_synthesis_summary",
					help="Print the file wise summary of fences synthesized and strengthened.")

# parallelization
parser.add_argument("--parallel", "-p", required=False, action='store_true', dest="parallel",
					help="Use multiprocessing for cycle detection in coherece conditions.")

# CDSchecker flags
parser.add_argument("--yield", "-y", required=False, action='store_true', dest="cds_y_flag",
					help="Pass yield flag to model checker.")
parser.add_argument("--liveness", "-l", type=int, required=False, dest="cds_m_flag",
					help="Pass liveness flag to model checker.")

args = parser.parse_args()
filepath   = args.file									# gets the input file name
batch_size = args.batch_size							# gets the input number of traces to be checked
max_iter   = args.max_iter								# gets the input maximum number of iterations
max_fences = args.max_fence								# max number of fences to be passed to cycle detection
max_depth  = args.max_depth	
max_cycles = args.max_cycles							# max search depth for cycle finding DFS
num_fences = args.num_fences
if(num_fences==None):
	num_fences = 100
print_synthesis_summary = args.print_synthesis_summary  # print summarya of synthesis if set
parallel   = args.parallel								# use multi-processing to parallel detect cycles of separate sccs
cds_y_flag = args.cds_y_flag							# pass -y flag to cds (when input program uses yield())
cds_m_flag = args.cds_m_flag							# pass -m N flag (with value N) to cds

batching    = (batch_size is not None)				# true if batch_size is set
iter_bound  = (max_iter is not None)				# true if max_iter is set
fence_bound = (max_fences is not None)				# true if max_fences is set
depth_bound = (max_depth is not None)				# true if max_depth is 
cycle_bound = (max_cycles is not None)				# true if max_cycles is set

cds_flags = {'y':cds_y_flag, 'm':cds_m_flag}
flags = {'batching':batching, 'iter_bound':iter_bound, 'fence_bound':fence_bound, 'depth_bound':depth_bound, 'parallel':parallel,'cycle_bound':cycle_bound}
bounds = {'batch_size':batch_size, 'max_iter':max_iter, 'max_fences':max_fences, 'max_depth':max_depth,'max_cycles':max_cycles,'num_fences':num_fences}

input_ext      = filepath.split('$')[1]
obj_filepath   = filepath.split('$')[0]

if not os.path.exists(obj_filepath):
	print(oc.BOLD + oc.FAIL + "\nInput file not found.\n" + oc.ENDC)
	sys.exit(0)
if iter_bound and not batching:
	print(oc.BOLD + oc.FAIL + "\nPlease specify the batch size of traces (-t).\n" + oc.ENDC)
	sys.exit(0)
elif batch_size == 0 or max_iter == 0:
	print(oc.BOLD + oc.FAIL + "\nFlag values cannot be 0. (-t or -m)\n" + oc.ENDC)
	sys.exit(0)

# processing times
mc_total      = 0
mc_make_total = 0
z3_total      = 0
tool_total    = 0

fences_added = 0
fences_modified = 0
fence_tags_final = {}
total_iter = 0
error_string = ""
synthesis_summary = ""
modified_files = []

def fn_main(obj_filepath, tool_timeout_value=TO.tool):
	global mc_total
	global mc_make_total
	global pre_calc_total
	global z3_total
	global tool_total
	global fences_added
	global fences_modified
	global fence_tags_final
	global modified_files
	global total_iter
	global error_string
	global input_ext

	iteration_time_start = time.time()

	z3_time = 0
	pre_calc_total = 0

	if iter_bound and total_iter == max_iter:
		return
	
	total_iter += 1
	if batching:
		print(oc.HEADER + oc.BOLD + "\n\n=============== ITERATION",total_iter,"===============" + oc.ENDC)
	
	traces, mc_time, mc_make_time, cnt_buggy_execs, mc_error_string, buggy_trace_no = model_checking_output(obj_filepath, input_ext, batch_size, total_iter, cds_flags, mc_total, tool_total)
	# print(len(traces))
	if mc_error_string is not None:
		print(oc.BOLD + oc.FAIL + mc_error_string + oc.ENDC)
		mc_total = TO.mc
		return

	elif cnt_buggy_execs: # has buggy traces
		get_p = Processing(traces, buggy_trace_no, flags, bounds)
		z3vars, disjunctions, error_string, pre_calc_total, cycles_tags_by_trace = get_p.get()				# runs and returns locations
		
		if error_string:
			print(oc.WARNING + error_string + oc.ENDC)
			delete_generated_file(obj_filepath)

		else:
			req_fences, z3_time = z3run(z3vars, disjunctions)	# get output from z3 & get required locations
			fence_tags = allocate_fence_orders(req_fences, cycles_tags_by_trace)
			(new_obj_filepath, count_modified_fences, iter_modified_files) = insert(fence_tags, obj_filepath, input_ext) # insert fences into the source file at the required locations

			fences_added += len(req_fences)
			fences_modified += count_modified_fences
			fence_tags_final.update(fence_tags)
			modified_files = modified_files + list(set(iter_modified_files) - set(modified_files)) # set of modified files

	iteration_time_end = time.time()
	tool_time = (iteration_time_end-iteration_time_start) - mc_make_time - mc_time - z3_time

	mc_time 	  += pre_calc_total
	mc_total      += mc_time
	mc_make_total += mc_make_time
	z3_total      += z3_time
	tool_total    += tool_time
	
	if batching:
		if cnt_buggy_execs and not error_string:
			res.iteration_result_summary(mc_time, z3_time, tool_time, len(req_fences), count_modified_fences)

	if batching and cnt_buggy_execs and not error_string:
		fn_main(new_obj_filepath)

	return

try:
	fn_main(obj_filepath)
except RuntimeError:
	print(oc.BOLD + oc.FAIL + "\nTool time exceeded 15 minutes.\n" + oc.ENDC)
	sys.exit(0)

res.final_result_summary(tool_total, (mc_total+pre_calc_total), z3_total, fences_added, fences_modified, batch_size, total_iter, print_synthesis_summary, fence_tags_final, modified_files, error_string)