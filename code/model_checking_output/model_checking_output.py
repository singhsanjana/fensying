from model_checking_output.translators.cds_checker.cds_checker import translate_cds
from model_checking_output.pre_calculator.pre_calculations import pre_calculations

def model_checking_output(filename, no_traces):
	cds = translate_cds(filename, no_traces)								# translates CDS Checker output & returns a structure containing the traces
	traces, mc_time, no_buggy_execs, mc_error_string = cds.get()

	return traces, mc_time, no_buggy_execs, mc_error_string