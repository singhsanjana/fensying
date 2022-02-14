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
from constants import time_handler
from constants import output_colours as oc

class translate_cds:
	def __init__(self, filepath, traces_batch_size, current_iteration, cds_y_flag):

		self.traces_raw = []											# list of all traces raw
		self.traces = []												# list of processed traces
		self.no_buggy_execs = 0											# number of buggy executions for this run
		self.error_string = None										# handle error in CDS Checker
		self.cds_time = 0
		self.make_time = 0
		self.buggy_trace_no = []										# no of buggy traces, required for mo file name

		test_file = fi.TEST_FOLDER_PATH_FROM_CDS + '/' + filepath[3:] # "../filepath"[3:] = "filepath"
		
		cds_cmd = './run.sh '+ test_file	# cmd to run cds checker
		if traces_batch_size:
			cds_cmd += ' -c ' + str(traces_batch_size)
		if cds_y_flag:
			cds_cmd += ' -y'
		cds_cmd = shlex.split(cds_cmd)

		if current_iteration > 1:
			filename = filepath.split('/')[-1]
			make_path = filepath[: -1*len(filename) ]
			change_dir = 'cd ' + make_path
			make = change_dir + ' && ' + 'make'

			print('making:', make)
			make_time_start = time.time()
			os.system(make + "> /dev/null 2>&1")												# make/compile into object file for CDS Checker
			self.make_time = time.time() - make_time_start
			print('time of model checker MAKE = ', round(self.make_time, 2))

		cds_start = time.time()
		try:
			p = subprocess.check_output(cds_cmd,
										cwd = fi.CDS_FOLDER_PATH,
										stderr=subprocess.STDOUT,
										timeout=900)		# get std output from CDS Checker
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
			print(e)
		else:
			cds_end = time.time()
			signal.signal(signal.SIGALRM, time_handler)
			signal.alarm(900)								# set timer for 15 minutes for the rest of the tool
			p = p.decode('utf-8', errors='ignore')			# convert to string
			self.cds_time = cds_end - cds_start
			self.obtain_traces(p)
			self.no_buggy_execs = int(self.no_buggy_execs)
			if self.no_buggy_execs == 0:
				if current_iteration == 1: 
					print(oc.OKGREEN, oc.BOLD, 'No buggy traces. Nothing to do.', oc.ENDC)
					exit(0)
				else:
					print('No more buggy traces.')
			# print("\n\nBuggy executions:\t",self.no_buggy_execs)

			if self.no_buggy_execs != 0:
				self.create_structure()
				# self.print_traces()
		# finally:
			#return


	def get(self):
		# print('traces:', self.traces)
		# print('cds_time: ', self.cds_time)
		# print('no_buggy_execs: ', self.no_buggy_execs)
		# print('error_string: ', self.error_string)
		# print('buggy_trace_no', self.buggy_trace_no)
		return self.traces, self.cds_time, self.make_time, self.no_buggy_execs, self.error_string, self.buggy_trace_no

	# to differentiate and obtain each trace from the std output in the terminal
	def obtain_traces(self,p):
		f=0                                                         	# flag for finding execution trace
		for line in p.split('\n'):
			tmp_index = line.find('Execution trace ')					# find execution number for buggy traces
			if tmp_index != -1:											# this line gives us number of this trace
				# print(line[tmp_index+len('Execution trace '):line.find(':')])
				self.buggy_trace_no.append(int(line[tmp_index+len('Execution trace '):line.find(':')]))

			if f==2:
				if "HASH" in line:                                  	# indicates end of one execution trace
					f=0
					self.traces_raw.append(trace_list)
				else:
					trace_list.append(line.split())                 	# collect data from execution trace and convert to list structure

			if f==1:
				f=2

            # the start of a trace/assertion violation case
			if 'Rf' in line:
				trace_list = []
				f=1

			# print number of buggy executions
			if "Number of buggy executions" in line:
				self.no_buggy_execs = line[28:len(line)]
			
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
	
		
	
