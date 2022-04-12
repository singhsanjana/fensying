# --------------------------------------------------------
# Gets an output form the CDS Model Checker and translates
# it into a required format.
# --------------------------------------------------------

import os
import subprocess
import shlex
import time
import signal
import math

from .create_list import create_list
from constants import file_info as fi
from constants import time_handler
from constants import output_colours as oc
from constants import timeouts as TO

class translate_cds:
	def __init__(self, filepath, traces_batch_size, current_iteration, cds_flags, cds_total_time, tool_total_time):

		self.traces_raw = []											# list of all traces raw
		self.traces = []												# list of processed traces
		self.cnt_buggy_execs = 0										# number of buggy executions for this run
		
		self.error_string = None										# handle error in CDS Checker
		self.cds_time = 0
		self.make_time = 0
		
		self.buggy_trace_no = []										# no of buggy traces (for mo file name)
		self.has_data_race_bug = False									# data race in input program (cannot process)
		self.has_initialization_bug = False								# missing initiaization in input program (cannot process)
		self.bug_report = ''											# details of race/missing initialization (if any)

		test_file = fi.TEST_FOLDER_PATH_FROM_CDS + '/' + filepath[3:]   # "../filepath"[3:] = "filepath"
		
		cds_cmd = './run.sh '+ test_file	# cmd to run cds checker
		if traces_batch_size:
			cds_cmd += ' -c ' + str(traces_batch_size)
		if cds_flags['y']:
			cds_cmd += ' -y'
		if cds_flags['m']:
			cds_cmd += ' -m ' + str(cds_flags['m'])
		cds_cmd = shlex.split(cds_cmd)

		if current_iteration > 1:
			filename = filepath.split('/')[-1]
			make_path = filepath[: -1*len(filename) ]
			change_dir = 'cd ' + make_path
			make = change_dir + ' && ' + self.make_cmd(filename, filepath)

			make_time_start = time.time()
			os.system(make + "> /dev/null 2>&1")				# make/compile into object file for CDS Checker
			self.make_time = time.time() - make_time_start
			print('time of model checker MAKE = ', round(self.make_time, 2))

		cds_start = time.time()
		timeout_value = TO.cds - cds_total_time         # TO value for CDS - time used by CDS in prev iterations
		try:
			p = subprocess.check_output(cds_cmd,
										cwd = fi.CDS_FOLDER_PATH,
										stderr=subprocess.STDOUT,
										timeout=timeout_value)		# get std output from CDS Checker
		except subprocess.TimeoutExpired:
			self.error_string = "\nModel Checking time exceeded 15 minutes."
		except subprocess.CalledProcessError as exc:
			self.error_string = "\n"
			print(oc.FAIL, oc.BOLD, '\nCalledProcessError while model checking.', oc.ENDC)
			outputstr = exc.output.decode('utf-8', errors='ignore')
			if 'Error:' in outputstr:
				outputstr = outputstr[outputstr.find('Error:') : ]
			elif 'Out of ' in outputstr:
				outputstr = outputstr[outputstr.find('Out of ') : ]		
			elif 'Segmentation fault' in outputstr:	
				outputstr = outputstr[outputstr.find('Segmentation fault') : ]	
			elif 'For debugging' in outputstr:
				outputstr = outputstr[outputstr.find('For debugging') : ]		
			print(outputstr)
			print(oc.FAIL, oc.BOLD, '\nPlease resolve the error for fence synthesis to proceed.', oc.ENDC)
		except Exception as e:
			self.error_string = "\nError while model checking.\nPlease resolve the error for fence synthesis to proceed."
			print('error',e)
		else:
			cds_end = time.time()
			tool_timeout_value = math.ceil(TO.tool - tool_total_time)
			signal.signal(signal.SIGALRM, time_handler)
			signal.alarm(tool_timeout_value)			# timeout for the rest of the tool

			p = p.decode('utf-8', errors='ignore')			# convert to string
			self.cds_time = cds_end - cds_start
			self.obtain_traces(p.split('\n'))
			self.cnt_buggy_execs = int(self.cnt_buggy_execs)

			if self.has_data_race_bug:
				print(oc.FAIL, oc.BOLD, 'Data race detected. Program behavior undefined.', oc.ENDC)
				print(self.bug_report)
				print(oc.FAIL, oc.BOLD, 'Remove race to proceed.', oc.ENDC)
				exit(0)

			if self.has_initialization_bug:
				print(oc.FAIL, oc.BOLD, 'Unitialized atomic detected.', oc.ENDC)
				print(self.bug_report)
				print(oc.WARNING, oc.BOLD, 'All atomic accesses must be initialized.', oc.ENDC)
				exit(0)

			if self.cnt_buggy_execs == 0:
				if current_iteration == 1: 
					print(oc.OKGREEN, oc.BOLD, 'No buggy traces. Nothing to do.', oc.ENDC)
					exit(0)
				else:
					print('No more buggy traces.')
			# print("\n\nBuggy executions:\t",self.cnt_buggy_execs)

			if self.cnt_buggy_execs != 0:
				self.create_structure()
				# self.print_traces()

	def make_cmd(self, filename, filepath):
		test_name  = filename[:filename.rfind('.')] # filename without extension
		path_depth = '../' * ( filepath.count('/')-1 ) 
		cmd  = 'g++ -MMD -MF ./.' + test_name + '.o.d -o ' + test_name + '.o ' + test_name + '.cc '
		cmd += '-Wall -g -O3 '
		cmd += '-I' + path_depth + 'model-checker/cds-checker '
		cmd += '-I' + path_depth + 'model-checker/cds-checker/include -fpermissive '
		cmd += '-L' + path_depth + 'model-checker/cds-checker -lmodel'
		return cmd

	def get(self):
		# print('traces:', self.traces)
		# print('cds_time: ', self.cds_time)
		# print('cnt_buggy_execs: ', self.cnt_buggy_execs)
		# print('error_string: ', self.error_string)
		# print('buggy_trace_no', self.buggy_trace_no)
		return self.traces, self.cds_time, self.make_time, self.cnt_buggy_execs, self.error_string, self.buggy_trace_no

	# to differentiate and obtain each trace from the std output in the terminal
	def obtain_traces(self, lines):
		# assert violations are reported by CDSchecker when NA race is detected
		# ie output would either have buggy data-race traces or buggy assert-violating traces
		cnt_lines = len(lines)

		assert_violation_bug = False
		bug_details = []

		i = 0
		while i < cnt_lines:
			if not 'Bug report:' in lines[i]:
				i += 1 # skip till next buggy trace data starts
				continue
			i += 1 # next line after line with 'Bug report:'

			while not 'Execution trace' in lines[i]:
				if self.has_data_race_bug or self.has_initialization_bug:
					bug_details.append(lines[i])

				if '[BUG]' in lines[i]:
					if 'hit assertion' in lines[i]:
						assert_violation_bug = True
					elif 'Data race detected' in lines[i]:
						self.has_data_race_bug = True
						bug_details.append(lines[i])
					elif 'read from uninitialized atomic' in lines[i]:
						self.has_initialization_bug = True
						bug_details.append(lines[i])
				i += 1

			if not assert_violation_bug:
				self.bug_report = '\n'.join(bug_details)
				break 				# bugs other than assert violations not handled

			trace_id_index = lines[i].find('Execution trace ') # execution id
			if trace_id_index != -1:
				self.buggy_trace_no.append(int(lines[i][trace_id_index+len('Execution trace '):lines[i].find(':')]))
			i += 4 # skipping trace header to reach trace events

			trace_list = [] # list of trace events
			while not 'HASH' in lines[i]: # 'HASH' marks the end of trace events
				trace_list.append(lines[i].split()) # collect trace events as a list
				i += 1

			# end of trace
			self.traces_raw.append(trace_list) # list of list of trace events
			i += 1

		self.cnt_buggy_execs = len(self.traces_raw)	
			
	# to convert each trace into a structure
	def create_structure(self):

		# map the variable names from the source code to the memory address used by the variable as shown in the traces
		for trace in self.traces_raw:
			execution = []
			for instr in trace:
				line = create_list(instr)
				execution.append(line)
			execution.sort(key = lambda x:x[1])							# sorts the list of instructions by the thread number
			# IDEA: using key function is faster since it is called exactly once for each input record

			self.traces.append(execution)

		# for i in self.traces[S_NO]:
		# 	print(i)

	# Helper functions
	def print_traces(self):
		i=1
		for trace in self.traces:
			print('Trace', i)
			for event in trace:
				# print_event(event)
				print(*event, sep='\t')
			i=i+1
	
		
	
