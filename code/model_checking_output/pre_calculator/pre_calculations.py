from model_checking_output.pre_calculator.hb import hb
from model_checking_output.pre_calculator.mo import mo
from model_checking_output.pre_calculator.graph import Graph
from constants import *

def pre_calculations(trace, buggy_trace_no):
	so_edges = []
	mat_size = max(map(lambda x: x[0], trace))			# number of instructions in the execution trace
	mat_size += 1
	initial_mat = Graph(mat_size)

	for t in trace:
		print(t)

	hb_calc = hb(trace, so_edges, initial_mat, mat_size)
	hb_edges, so_edges = hb_calc.get()
	# print("hb_edges==",hb_edges)

	mo_calc = mo(trace, so_edges, buggy_trace_no)
	mo_edges, so_edges = mo_calc.get()
	# print("mo_edges==",mo_edges)

	return hb_edges, mo_edges, so_edges

def preprocessing(trace):
	writes = []
	reads = []
	for instr in trace:
		if instr[TYPE] == WRITE or instr[TYPE] == RMW or instr[TYPE] == INIT:
			writes.append(instr)
		elif instr[TYPE] == READ or instr[TYPE] == RMW:
			reads.append(instr)
	
	return writes, reads