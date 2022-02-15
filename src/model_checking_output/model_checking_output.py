from model_checking_output.translators.cds_checker.cds_checker import translate_cds
from model_checking_output.pre_calculator.pre_calculations import pre_calculations

def model_checking_output(filename, traces_batch_size, total_iter, cds_y_flag):
	cds = translate_cds(filename, traces_batch_size, total_iter, cds_y_flag)								# translates CDS Checker output & returns a structure containing the traces
	traces, mc_time, mc_make_time, cnt_buggy_execs, mc_error_string, buggy_trace_no = cds.get()

	return traces, mc_time, mc_make_time, cnt_buggy_execs, mc_error_string, buggy_trace_no