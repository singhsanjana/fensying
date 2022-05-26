import subprocess
import shlex
import time

from constants import file_info as fi
from model_checking_output.translators.genmc.format_input import format_input

class translate_genmc:
	def __init__(self, input_filepath):
		self.has_buggy_traces = True
		self.execution_error  = False
		self.total_time = 0
		self.genmc_output = ''
		
		fi = format_input(input_filepath)

	def run_genmc(self, test_filepath, timeout_value):
		test_path = fi.TEST_FOLDER_PATH_FROM_GENMC + test_filepath
		genmc_cmd = './src/genmc '+ test_path	# cmd to run genmc
		genmc_cmd = shlex.split(genmc_cmd)
		
		genmc_start = time.time()
		try:
			p = subprocess.check_output(genmc_cmd,
										cwd = fi.GENMC_FOLDER_PATH,
										stderr=subprocess.STDOUT,
										timeout=timeout_value)		# get std output from GenMC
		except subprocess.TimeoutExpired:
			self.error_string = "\nModel Checking (GenMC) time exceeded 15 minutes."
		except Exception as e:
			self.error_string = "\nError while model checking (GenMC).\nPlease resolve the error for fence synthesis to proceed."
			print('error',e)
			self.execution_error = True
		else:
			genmc_end = time.time()
			self.total_time = (genmc_end - genmc_start)
			self.genmc_output = p.decode('utf-8', errors='ignore')			# convert to string

	def translate_genmc(self):
		for line in self.genmc_output:
			if 'No errors were detected.' in line:
				self.has_buggy_traces = False
				return
			if 'Error detected: Safety violation!' in line:
				return  # self.has_buggy_traces is True by default

	def get(self):
		return self.has_buggy_traces, (not self.execution_error), self.total_time