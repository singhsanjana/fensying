# --------------------------------------------------------
# Gets an output form the CDS Model Checker and translates
# it into a required format.
# --------------------------------------------------------

import os
import subprocess
import shlex
import time
import sys
import signal
from operator import itemgetter

from .create_list import create_list
from constants import file_info as fi
from constants import time_handler
from .delete_file import delete_copied_file

class translate_cds:
	def __init__(self, filename, no_traces):

		self.traces_raw = []											# list of all traces raw
		self.traces = []												# list of processed traces
		self.no_buggy_execs = 0											# number of buggy executions for this run
		self.error_string = None										# handle error in CDS Checker
		self.cds_time = 0
		self.buggy_trace_no = []										# no of buggy traces, required for mo file name

		copy = "cp " + filename + " " + fi.CDS_TEST_FOLDER_PATH
		make = "cd "+ fi.CDS_FOLDER_PATH + " && make"

		input_file = filename.split('/')
		input_file = input_file[-1]

		input_file = "test/"+input_file[:-2]+'o'
		cds_cmd = './run.sh '+input_file								# cmd to run cds checker
		if no_traces:
			cds_cmd += " -c "+str(no_traces)
		cds_cmd = shlex.split(cds_cmd)

		os.system(copy)													# copy input file to cds checker directory
		cds_start = time.time()
		os.system(make)													# make/compile into object file for CDS Checker

		signal.signal(signal.SIGALRM, time_handler)
		signal.alarm(1800)												# set timer for 30 minutes for CDSChecker
		try:
			p = subprocess.check_output(cds_cmd,
										cwd = fi.CDS_FOLDER_PATH,
										stderr=subprocess.PIPE)			# get std output from CDS Checker
			cds_end = time.time()
			p = p.decode('utf-8')										# convert to string

			self.cds_time = cds_end-cds_start
			self.obtain_traces(p)
		except RuntimeError:
			self.error_string = "\nModel Checking time exceeded 30 minutes."
		except:
			self.error_string = "\nError while model checking.\nPlease check and resolve the error."
		else:
			signal.alarm(1800)											# set timer for 30 minutes for the rest of the tool
			self.no_buggy_execs = int(self.no_buggy_execs)
			# print("\n\nBuggy executions:\t",self.no_buggy_execs)

			if self.no_buggy_execs != 0:
				self.create_structure()
				# self.print_traces()
		finally:
			delete_copied_file(filename)
			#return


	def get(self):
		# print('traces:', self.traces)
		# print('cds_time: ', self.cds_time)
		# print('no_buggy_execs: ', self.no_buggy_execs)
		# print('error_string: ', self.error_string)
		# print('buggy_trace_no', self.buggy_trace_no)
		return self.traces, self.cds_time, self.no_buggy_execs, self.error_string, self.buggy_trace_no

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
	
		
	
