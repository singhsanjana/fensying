from model_checking_output.pre_calculator.hb import hb
from model_checking_output.pre_calculator.mo import mo
from model_checking_output.pre_calculator.graph import Graph
from constants import *

def pre_calculations(trace, buggy_trace_no):
	writes, reads = preprocessing(trace)
	initial_mo_edges = []
	initial_hb_edges = []
	so_edges = []
	calc_all = True
	flag = 0

	mat_size = max(map(lambda x: x[0], trace))			# number of instructions in the execution trace
	mat_size += 1
	initial_mat = Graph(mat_size)

	while flag != 1:
		hb_calc = hb(trace, initial_mo_edges, initial_hb_edges, so_edges, initial_mat, mat_size, writes, reads, calc_all)
		hb_matrix, hb_edges, so_edges = hb_calc.get()
		# print("hb_edges==",hb_edges)

		# print('computing mo for buggy_trace_no:', buggy_trace_no)
		mo_calc = mo(trace, hb_matrix, mat_size, so_edges, writes, reads, buggy_trace_no)
		mo_edges, so_edges = mo_calc.get()
		# print("mo_edges==",mo_edges)

		if (set(initial_mo_edges) == set(mo_edges)) and (set(initial_hb_edges) == set(hb_edges)):
			flag = 1
		else:
			initial_hb_edges = hb_edges
			initial_mo_edges = mo_edges
			initial_mat = hb_matrix
			calc_all = False
	
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