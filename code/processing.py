# --------------------------------------------------------
# Processes the std:out data from the command line output
# and curates lists of:
#   1. Order of events including pseudo fences
#   2. Happens-before relationships (HB)
#   3. Modification Order relationships (MO)
#   4. Sequenced-before relationships (SB)
#   5. Total order relationships (TO)
#
# Then proceeds on to find out cycles from the TO graph.
# --------------------------------------------------------

from model_checking_output.pre_calculator.pre_calculations import pre_calculations
from preprocessing import preprocessing
from edges_computation import edges_computation
# from to import to
from cycle import Cycles
from weak_fensying import weak_fensying
from z3translate import z3translate
from constants import *

# import sys
import time

# IDEA: any var with _thread at the end means that it is an array of arrays separated by thread number
class Processing:
	def __init__(self, traces):
		self.z3vars = []												# list of all z3 constants
		self.disjunctions = []									# list of disjunctions for the z3 function
		self.fences_present = []								# list of fences converted to their respective variable names
		self.fences_present_locs = []
		self.error_string = ''
		self.pre_calc_total = 0									# time taken for calculation of initial values - HB, MO, SB
		self.all_relaxed_cycles = []
		self.all_strong_cycles = []

		trace_no = 0
		# print("traces=",traces)

		for trace in traces:										# run for each trace
			self.all_events_thread = []						# list of all events separated by threads
			self.all_sc_events = []								# list of all sc events in program after inserting candidate fences
			self.fences_thread = []								# list of fences in each thread
			self.fences_in_trace = []							# list of fences already present in the program

			# lists of edges
			# hb_edges = []													# list of all SB+SW+DOB edge tuples
			# mo_edges = []
			# rf_edges = []
			# fr_edges = []
			self.sc_edges = []										# list of hb+mo+rf+fr edges with only sc events
			self.sw_edges = []										# list of edges between thread create-start, finish-join statements
			# rf1_edges = []

			# cycles = []                        		# list of all cycles between the fences and events
			loc_info = {}                         # information regarding the required fence locations

			trace_no += 1
			# print("---------Trace",trace_no,"---------")

			pre_calc_start = time.time()
			pre_calculations(trace)
			# HB
			# hb_graph = hb(trace)
			# hb_matrix, size, so_edges = hb_graph.get()

			# MO
			# get_mo = mo(trace, hb_matrix, size)
			# mo_edges, sc_edges = get_mo.get()
			# print("mo =",mo_edges)
			pre_calc_end = time.time()
			self.pre_calc_total += (pre_calc_end-pre_calc_start)

			# ADD FENCES
			order=self.fence(trace)
			# print("order =",order)
			# print("fences_present =", fences_in_trace)
			# print("fences_thread =", fences_thread)
			# print("all_events_thread", self.all_events_thread)

			# transitive SB calc, put into hb edges
			# hb_edges = []
			hb_edges = self.sb()
			hb_edges += self.sw_edges
			# print(hb_edges)

			# pre-process and obtain separately reads, writes with neighbouring fences
			reads, writes = preprocessing(order)

			# CALC EDGES
			calc_edges = edges_computation(reads, writes, self.fences_thread, hb_edges, self.sc_edges)
			hb_edges, mo_edges, rf_edges, fr_edges, rf1_edges, self.sc_edges = calc_edges.get()
			# print("hb = ", hb_edges)
			# print("mo = ", mo_edges)
			# print("rf = ", rf_edges)
			# print("fr = ", fr_edges)
			# print("rf1 = ", rf1_edges)
			# print("sc = ", self.sc_edges)
			
			# CYCLES
			relaxed_edges = hb_edges + mo_edges + rf_edges + fr_edges + rf1_edges
			relaxed_cycles = Cycles(relaxed_edges)

			if len(relaxed_cycles) == 0:
				self.error_string = "\nNo TO cycles can be formed for trace "+str(trace_no)+"\nHence this behaviour cannot be stopped using SC fences\n"
				return
			
			# WEAK FENSYING
			relaxed_cycles = [list(item) for item in set(tuple(row) for row in relaxed_cycles)] # removing duplicate values
			check1 = weak_fensying(relaxed_cycles, hb_edges, mo_edges, rf_edges, fr_edges, rf1_edges)
			relaxed_cycles = check1.get()
			self.all_relaxed_cycles.append(relaxed_cycles)
			# print("relaxed_cycles =",relaxed_cycles)

			# STRONG FENSYING
			strong_cycles = Cycles(self.sc_edges)
			# print("strong_cycles =",strong_cycles)
			self.all_strong_cycles.append(strong_cycles)

			cycles = relaxed_cycles+strong_cycles
			cycles_with_only_fences = []
			for i in range(len(cycles)):
				cycles_with_only_fences.append([c for c in cycles[i] if type(c) is str])
			cycles_with_only_fences = [list(item) for item in set(tuple(sorted(row)) for row in cycles_with_only_fences)] # removing duplicate values
			unique_fences = list(sorted(set(x for l in cycles_with_only_fences for x in l)))
			# print("unique_fences=",unique_fences)
			# print("cycles_with_only_fences =",cycles_with_only_fences)

			if len(unique_fences)>0:
				for fence in unique_fences:
					i = order.index(fence)
					fence_name = order[i]
					var_name = 'l'+str(order[i-1][LINE_NO])
					loc_info[fence_name] = var_name
					
					# check for the fences already present in input prgm and replace them with these variables
					if (fence in self.fences_in_trace) and (var_name not in self.fences_present):
						self.fences_present.append(var_name)
						self.fences_present_locs.append(order[i-1][LINE_NO])
			
				# print("fences loc_info =",loc_info)

				get_translation = z3translate(cycles_with_only_fences, loc_info)
				consts, translation = get_translation.get()

				for con in consts:
					if con not in self.z3vars:
						self.z3vars.append(con)
				self.disjunctions.append(translation)

			else:
				self.error_string = "\nNo TO cycles can be formed for trace "+str(trace_no)+"\nHence this behaviour cannot be stopped using SC fences\n"
				return

	def fence(self, trace):
		order = ['F1n1']								
		all_events = ['F1n1']
		self.all_sc_events = ['F1n1']
		fences_in_thread = ['F1n1']

		current_thread = 1				# for fence naming
		fence_no = 2

		for i in range(len(trace)):
			if trace[i][T_NO] != current_thread:
				self.all_events_thread.append(all_events)
				self.fences_thread.append(fences_in_thread)

				all_events = []
				fences_in_thread = []
				current_thread += 1
				fence_no = 1
				fence_name = 'F'+str(current_thread)+'n'+str(fence_no)
				fence_no += 1
				order.append(fence_name)
				all_events.append(fence_name)
				self.all_sc_events.append(fence_name)
				fences_in_thread.append(fence_name)

			if not trace[i][TYPE] == FENCE:
				order.append(trace[i])
				all_events.append(trace[i])
				if trace[i][MO] == SEQ_CST:
					self.all_sc_events.append(trace[i][S_NO])
			else:
				self.fences_in_trace.append(fence_name)
				if i == (len(trace)-1):
					self.all_all_events_thread.append(all_events)
					self.fences_thread.append(fences_in_thread)
				continue

			if trace[i][LINE_NO] != 'NA':
				fence_name = 'l'+str(trace[i][LINE_NO])
			else:
				fence_name = 'F'+str(current_thread)+'n'+str(fence_no)
			fence_no += 1

			order.append(fence_name)
			all_events.append(fence_name)
			fences_in_thread.append(fence_name)
			
			if i == (len(trace)-1):
				self.all_events_thread.append(all_events)
				self.fences_thread.append(fences_in_thread)

			# create sw's between thread create statements
			if trace[i][TYPE] == CREATE:
				v1 = trace[i][S_NO]
				v2 = v1+1
				self.sw_edges.append((v1,v2))
			# create sw's between thread finish and join statements
			if trace[i][TYPE] == JOIN:
				t = trace[i][VALUE]
				for j in range(i,len(trace)):
					if trace[j][TYPE] == FINISH and trace[j][T_NO] == t:
						v1 = trace[j][S_NO]
						v2 = trace[i][S_NO]
						self.sw_edges.append((v1,v2))
		return order

	def sb(self):
		sb_edges = []
		for i in self.all_events_thread:
			for j in range(len(i)-1):
				e0 = i[j] if type(i[j]) is str else i[j][S_NO]
				e1 = i[j+1] if type(i[j+1]) is str else i[j+1][S_NO]
				sb_edges.append((e0, e1))
				if type(i[j]) is list and type(i[j+1]) is list:
					if i[j][MO]  == SEQ_CST and i[j+1][MO] == SEQ_CST:
						self.sc_edges.append((e0,e1))
		# print("sb_edges",sb_edges)
		return sb_edges

	def get(self):
		return self.fences_present, self.fences_present_locs, self.z3vars, self.disjunctions, self.error_string, self.pre_calc_total