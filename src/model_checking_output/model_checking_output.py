import math
import signal

from model_checking_output.translators.cds_checker.cds_checker import translate_cds
from constants import time_handler
from constants import file_info as fi
from constants import timeouts as TO

def model_checking_output(obj_filepath, input_ext, traces_batch_size, total_iter, cds_flags, mc_total_time, tool_total_time):
	cds_test_file = fi.TEST_FOLDER_PATH_FROM_CDS + '/' + obj_filepath[3:]   # "../obj_filepath"[3:] = "obj_filepath"
	timeout_value = TO.mc - mc_total_time   # TO value for model checker - time used by model checker in prev iterations
	cds = translate_cds(obj_filepath, input_ext, total_iter)
	cds.run_cds(cds_test_file, traces_batch_size, cds_flags, timeout_value) # execute cds subprocess and get output
	
	# restart tool timer
	tool_timeout_value = math.ceil(TO.tool - tool_total_time)
	signal.signal(signal.SIGALRM, time_handler)
	signal.alarm(tool_timeout_value)			# timeout for the rest of the tool

	cds.translate(total_iter) # translate cds output to get structure buggy traces or execution errors
	traces, mc_time, mc_make_time, cnt_buggy_execs, mc_error_string, buggy_trace_no = cds.get()

	return traces, mc_time, mc_make_time, cnt_buggy_execs, mc_error_string, buggy_trace_no