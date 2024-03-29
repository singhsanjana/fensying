# --------------------------------------------------------
# Gets an output form the CDS Model Checker and translates
# it into a required format.
# --------------------------------------------------------

import os
import subprocess
import shlex
import time
import signal

from .create_list import create_list
from constants import file_info as fi
from constants import output_colours as oc

class translate_cds:
	def __init__(self, test_filepath, input_ext, current_iteration):

		self.traces_raw = []											# list of all traces raw
		self.traces = []												# list of processed traces
		self.cnt_buggy_execs = 0										# number of buggy executions for this run
		
		self.error_string = None										# handle error in CDS Checker
		self.mc_time   = 0
		self.make_time = 0
		
		self.buggy_trace_no = []										# no of buggy traces (for mo file name)
		self.has_data_race_bug = False									# data race in input program (cannot process)
		self.has_initialization_bug = False								# missing initiaization in input program (cannot process)
		self.bug_report = ''											# details of race/missing initialization (if any)
		self.cds_output = ''

		if current_iteration > 1:
			self.make_fixed_file(test_filepath, input_ext)

	def run_cds(self, test_filepath_from_cds, traces_batch_size, cds_flags, timeout_value):
		cds_cmd = './run.sh '+ test_filepath_from_cds	# cmd to run cds checker
		if traces_batch_size:
			cds_cmd += ' -c ' + str(traces_batch_size)
		if cds_flags['y']:
			cds_cmd += ' -y'
		if cds_flags['m']:
			cds_cmd += ' -m ' + str(cds_flags['m'])
		cds_cmd = shlex.split(cds_cmd)

		cds_start = time.time()
		try:
			p = subprocess.check_output(cds_cmd,
										cwd = fi.CDS_FOLDER_PATH,
										stderr=subprocess.STDOUT,
										timeout=timeout_value)		# get std output from CDS Checker
		except subprocess.TimeoutExpired:
			self.error_string = "\nModel Checking time exceeded 15 minutes."
		except subprocess.CalledProcessError as exc:
			self.error_string = "\n"
			outputstr = exc.output.decode('utf-8', errors='ignore')
			if 'Error:' in outputstr:
				outputstr = outputstr[outputstr.find('Error:') : ]
			elif 'Out of ' in outputstr:
				outputstr = outputstr[outputstr.find('Out of ') : ]		
			elif 'Segmentation fault' in outputstr:	
				outputstr = outputstr[outputstr.find('Segmentation fault') : ]	
			elif 'For debugging' in outputstr:
				outputstr = outputstr[outputstr.find('For debugging') : ]
			if (outputstr):
				print(oc.FAIL, oc.BOLD, '\nCalledProcessError while model checking.', oc.ENDC)
				print(outputstr)
				print(oc.FAIL, oc.BOLD, '\nPlease resolve the error for fence synthesis to proceed.', oc.ENDC)
			else:
				os.killpg(os.getpgid(p.pid), signal.SIGTERM)
		except Exception as e:
			self.error_string = "\nError while model checking.\nPlease resolve the error for fence synthesis to proceed."
			print('error',e)
		else:
			cds_end = time.time()
			self.mc_time = cds_end - cds_start
			self.cds_output = p.decode('utf-8', errors='ignore')	# convert to string

	def translate(self, current_iteration):
			self.obtain_traces(self.cds_output.split('\n'))
			self.cnt_buggy_execs = int(self.cnt_buggy_execs)

			if self.error_string:
				return

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
			
			if self.cnt_buggy_execs != 0:
				self.create_structure()
			
	def get(self):
		return self.traces, self.mc_time, self.make_time, self.cnt_buggy_execs, self.error_string, self.buggy_trace_no

	def make_cmd(self, filename, obj_filepath, input_ext):
		test_name  = filename[:filename.rfind('.')] # filename without extension
		path_depth = '../' * ( obj_filepath.count('/')-1 ) 
		cmd = 'g++ '
		if input_ext == 'c':
			cmd = 'gcc '
		cmd += '-MMD -MF ./.' + test_name + '.o.d -o ' + test_name + '.o ' + test_name + '.' + input_ext + ' '
		cmd += '-Wall -g -O3 '
		cmd += '-I' + path_depth + 'model-checker/cds-checker '
		cmd += '-I' + path_depth + 'model-checker/cds-checker/include -fpermissive '
		cmd += '-L' + path_depth + 'model-checker/cds-checker -lmodel'
		return cmd

	def make_fixed_file(self, obj_filepath, input_ext):
		filename = obj_filepath.split('/')[-1]
		make_path = obj_filepath[: -1*len(filename) ]
		change_dir = 'cd ' + make_path
		make = change_dir + ' && ' + self.make_cmd(filename, obj_filepath, input_ext)

		make_time_start = time.time()
		os.system(make + "> /dev/null 2>&1")				# make/compile into object file for CDS Checker
		self.make_time = time.time() - make_time_start
		print('time of model checker MAKE = ', round(self.make_time, 2))

	# to differentiate and obtain each trace from the std output in the terminal
	def obtain_traces(self, lines):
		# assert violations are reported by CDSchecker when NA race is detected
		# ie output would either have buggy data-race traces or buggy assert-violating traces
		cnt_lines = len(lines)

		assert_violation_bug = False
		bug_details = []

		model_checking_completed = False

		i = 0
		while i < cnt_lines:
			if 'Model-checking complete' in lines[i]:
				model_checking_completed = True

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

		if not model_checking_completed and not self.bug_report:
			self.error_string = "\nModel Checking time exceeded 15 minutes."
			
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

	# Helper functions
	def print_traces(self):
		i=1
		for trace in self.traces:
			print('Trace', i)
			for event in trace:
				print(*event, sep='\t')
			i=i+1
	
		
	
