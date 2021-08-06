from hb_calculator.hb import hb
from mo_calculator.mo import mo
from graph import Graph
from constants import *

def pre_calculations(trace):
	writes, reads = preprocessing(trace)
	initial_mo_edges = []
	initial_hb_edges = []
	calc_all = True
	flag = 0

	mat_size = max(map(lambda x: x[0], trace))			# number of instructions in the execution trace
	mat_size += 1
	initial_mat = Graph(mat_size)

	while flag != 1:
		hb_calc = hb(trace, initial_mo_edges, initial_hb_edges, initial_mat, mat_size, writes, reads, calc_all)
		hb_matrix, hb_edges = hb_calc.get()

		mo_calc = mo(trace, hb_matrix, mat_size)
		mo_edges = mo_calc.get()

		if (initial_mo_edges == mo_edges) and (initial_hb_edges == hb_edges):
			flag = 1
		else:
			initial_hb_edges = hb_edges
			initial_mo_edges = mo_edges
			initial_mat = hb_matrix
			calc_all = False

def preprocessing(trace):
	writes = []
	reads = []
	for instr in trace:
		if instr[TYPE] == WRITE or instr[TYPE] == RMW or instr[TYPE] == INIT:
			writes.append(instr)
		elif instr[TYPE] == READ or instr[TYPE] == RMW:
			reads.append(instr)
	
	return writes, reads