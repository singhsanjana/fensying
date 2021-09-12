from model_checking_output.pre_calculator.hb import hb
from model_checking_output.pre_calculator.mo import mo
from model_checking_output.pre_calculator.graph import Graph
from constants import *

def pre_calculations(trace, buggy_trace_no):
	so_edges = []

	hb_calc = hb(trace, so_edges)
	hb_edges, rf_edges, rfinv_edges, so_edges = hb_calc.get()
	
	mo_calc = mo(trace, so_edges, buggy_trace_no)
	mo_edges, so_edges = mo_calc.get()

	return hb_edges, mo_edges, rf_edges, rfinv_edges, so_edges

def preprocessing(trace):
	writes = []
	reads = []
	for instr in trace:
		if instr[TYPE] == WRITE or instr[TYPE] == RMW or instr[TYPE] == INIT:
			writes.append(instr)
		elif instr[TYPE] == READ or instr[TYPE] == RMW:
			reads.append(instr)
	
	return writes, reads