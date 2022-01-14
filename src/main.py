# --------------------------------------------------------
# Inputs a c++ file and outputs fences and fence positions
# with respect to C++11 version compiler.
#
# main.py runs the entire program.
# Usage:
#   python3 main.py -f [input file]
# --------------------------------------------------------

import argparse
import os
import subprocess
import shlex
import time
import sys

import publish_result as res
from constants import output_colours as oc
from constants import file_info as fi
from processing import Processing
from z3run import z3run
from allocate_fence_orders import allocate_fence_orders
from insert import insert
from model_checking_output.model_checking_output import model_checking_output
from model_checking_output.translators.cds_checker.delete_file import delete_generated_file

parser = argparse.ArgumentParser()
parser.add_argument("--file", "-f", type=str, required=True, 
					help="File path of object file.")
parser.add_argument("--traces", "-t", type=int, required=False, dest="no_traces",
					help="Batch buggy traces (non-optimal).")
parser.add_argument("--max-iter", "-m", type=int, required=False, dest="max_iter",
					help="Max number of batches. Used with -t flag.")
parser.add_argument("--synthesis-summary", "-s", required=False, action='store_true', dest="print_synthesis_summary",
					help="Print the file wise summary of fences synthesized and strengthened.")
parser.add_argument("--yield", "-y", required=False, action='store_true', dest="cds_y_flag",
					help="Pass yield flag to model checker.")

args = parser.parse_args()
filename = args.file											# gets the input file name
no_traces = args.no_traces										# gets the input number of traces to be checked
max_iter = args.max_iter										# gets the input maximum number of iterations
print_synthesis_summary = args.print_synthesis_summary          # print summarya of synthesis if set
cds_y_flag = args.cds_y_flag


if not os.path.exists(filename):
	print(oc.BOLD + oc.FAIL + "\nInput file not found.\n" + oc.ENDC)
	sys.exit(0)
if max_iter is not None and no_traces is None:
	print(oc.BOLD + oc.FAIL + "\nPlease specify the batch size of traces (-t).\n" + oc.ENDC)
	sys.exit(0)
elif no_traces == 0 or max_iter == 0:
	print(oc.BOLD + oc.FAIL + "\nFlag values cannot be 0. (-t or -m)\n" + oc.ENDC)
	sys.exit(0)

mc_total = 0
mc_make_total = 0
z3_total = 0
fences_added = 0
fences_modified = 0
fence_tags_final = {}
total_iter = 0
error_string = ""
synthesis_summary = ""

def fn_main(filename):
	global mc_total
	global mc_make_total
	global pre_calc_total
	global z3_total
	global fences_added
	global fences_modified
	global fence_tags_final
	global total_iter
	global error_string

	z3_time = 0
	pre_calc_total = 0

	if max_iter and total_iter == max_iter:
		return
	
	total_iter += 1
	if no_traces:
		print(oc.HEADER + oc.BOLD + "\n\n=============== ITERATION",total_iter,"===============" + oc.ENDC)

	traces, mc_time, mc_make_time, no_buggy_execs, mc_error_string, buggy_trace_no = model_checking_output(filename, no_traces, total_iter, cds_y_flag)
	# print('after model_checking_output, buggy_trace_no:', buggy_trace_no)

	if mc_error_string is not None:
		print(oc.BOLD + oc.FAIL + mc_error_string + oc.ENDC)
		sys.exit(0)

	elif no_buggy_execs: # has buggy traces
		get_p = Processing(traces, buggy_trace_no)
		z3vars, disjunctions, error_string, pre_calc_total, cycles_tags_by_trace = get_p.get()				# runs and returns locations
		
		if error_string:
			print(oc.WARNING + error_string + oc.ENDC)
			delete_generated_file(filename)

		else:
			req_fences, z3_time = z3run(z3vars, disjunctions)	# get output from z3 & get required locations
			# print('mi-model', req_fences)
			fence_tags = allocate_fence_orders(req_fences, cycles_tags_by_trace)
			# print('solution', fence_tags)
			(new_filename, count_modified_fences) = insert(fence_tags, filename) # insert fences into the source file at the required locations

			fences_added += len(req_fences)
			fences_modified += count_modified_fences
			fence_tags_final.update(fence_tags)

	mc_total += mc_time
	mc_make_total += mc_make_time
	z3_total += z3_time
	if no_traces:
		if no_buggy_execs and not error_string:
			res.iteration_result_summary((mc_time+pre_calc_total), z3_time, len(req_fences), count_modified_fences)

	if no_traces and no_buggy_execs and not error_string:
		fn_main(new_filename)

	return

try:
	start = time.time()
	fn_main(filename)
	end = time.time()
except RuntimeError:
	print(oc.BOLD + oc.FAIL + "\nTool time exceeded 15 minutes.\n" + oc.ENDC)
	sys.exit(0)

res.final_result_summary((end-start-mc_make_total), (mc_total+pre_calc_total), z3_total, fences_added, fences_modified, no_traces, total_iter, print_synthesis_summary, fence_tags_final)