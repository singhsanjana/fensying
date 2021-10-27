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

from constants import output_colours as oc
from processing import Processing
from z3run import z3run
from allocate_fence_orders import allocate_fence_orders
from insert import insert
from model_checking_output.model_checking_output import model_checking_output
from model_checking_output.translators.cds_checker.delete_file import delete_generated_file

parser = argparse.ArgumentParser()
parser.add_argument("--file", "-f", type=str, required=True, 
					help="Enter file path and name")
parser.add_argument("--traces", "-t", type=int, required=False, dest="no_traces",
					help="For a faster but less optimized output, enter an int value with this flag to check only this number of traces at once")
parser.add_argument("--max-iter", "-m", type=int, required=False, dest="max_iter",
					help="After entering traces, you can enter a maximum number of iterations as well to avoid going in an infinite loop for programs. You can only enter this after entering the number of traces")

args = parser.parse_args()
filename = args.file											# gets the input file name
no_traces = args.no_traces										# gets the input number of traces to be checked
max_iter = args.max_iter										# gets the input maximum number of iterations

if not os.path.exists(filename):
	print(oc.BOLD + oc.FAIL + "\nFile does not exist at provided location.\nPlease check the input file path.\n" + oc.ENDC)
	sys.exit(0)
if max_iter is not None and no_traces is None:
	print(oc.BOLD + oc.FAIL + "\nPlease specify number of traces to be checked as well using flag -t.\n" + oc.ENDC)
	sys.exit(0)
elif no_traces == 0 or max_iter == 0:
	print(oc.BOLD + oc.FAIL + "\nFlag values cannot be 0.\n" + oc.ENDC)
	sys.exit(0)

mc_total = 0
z3_total = 0
fences_added = 0
fences_modified = 0
fence_tags_final = []
total_iter = 0
error_string = ""

def fn_main(filename):
	global mc_total
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

	traces, mc_time, no_buggy_execs, mc_error_string, buggy_trace_no = model_checking_output(filename, no_traces)
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
			fence_tags_final += list(fence_tags.values())

	mc_total += mc_time
	z3_total += z3_time
	if no_traces:
		print("Time- CDS Checker:\t",round(mc_time, 2))
		if no_buggy_execs and not error_string:
			print("Time- Z3:\t\t",round(z3_time, 2))
			print("Fences synthesized: \t",len(req_fences))
			print("Fences strengthened:\t",count_modified_fences)

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

print(oc.OKBLUE + oc.BOLD + "\n\n================= RESULT SUMMARY =================" + oc.ENDC)
if not error_string:
	# TODO: how to compute order of added fences if some existing fence is modified
	print(oc.OKGREEN, oc.BOLD, "Total fences synthesized:  \t", fences_added, oc.ENDC)
	print(oc.OKGREEN, oc.BOLD, "Total fences strengthened: \t", fences_modified, oc.ENDC)

print("Time- CDS Checker:\t",round(mc_total, 2))
print("Time- Cycle computation:",round(pre_calc_total, 2))
if z3_total > 0:
	print("Time- Z3:\t\t",round(z3_total, 2))
print("\nTime- Total:\t\t",round(end-start, 2))
print("Time- Fensying:\t\t",round(end-start-mc_total-pre_calc_total-z3_total, 2))
if no_traces:
	print("\nTotal iterations:\t",total_iter)
	print("Time- avg per iter:\t",round((end-start)/total_iter, 2))

fenced_filename = filename[:-3] + '_fenced.cc'
print(oc.OKBLUE, oc.BOLD, "\n\nFixed program at:", fenced_filename, "\n", oc.ENDC)