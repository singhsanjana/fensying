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
from compute_cycles_tags import compute_relaxed_tags, compute_strong_tags
from z3translate import z3translate
from constants import *

# import sys
import time

# IDEA: any var with _thread at the end means that it is an array of arrays separated by thread number
class Processing:
	def __init__(self, traces, buggy_trace_no):
		self.z3vars  = []										# list of all z3 variables ie fences
		self.formula = []										# list of disjunctions for the z3 formula
		self.error_string = ''
		self.pre_calc_total = 0									# time taken for calculation of initial values - HB, MO, SB
	
		self.cycles_tags_by_trace = []

		trace_no = 0
		# print("traces=",traces)

		for trace in traces:									# run for each trace
			self.all_events_by_thread = []						# list of all events separated by threads
			self.fences_by_thread     = []						# list of fences in each thread
			
			candidate_cycles      = []                        	# list of all cycles in this trace
			candidate_cycles_tags = []
			
			trace_no += 1
			# print("---------Trace",trace_no,"---------")
			# print(trace)

			pre_calc_start = time.time()
			# [snj]: hb edges does not include sb, sb would be computed after inserting fences
			hb_edges, mo_edges, rf_edges, rfinv_edges, self.so_edges, rs_edges = pre_calculations(trace, buggy_trace_no[trace_no-1])
			# hb_print = hb_edges
			# hb_print.sort(key = lambda x:x[1])
			# hb_print.sort(key = lambda x:x[0])
			# print("hb-pre-fences=" + str(hb_print))
			# print ('rf:', rf_edges)
			# print ('rfinv:', rfinv_edges)
			# print ('mo:', mo_edges)

			pre_calc_end = time.time()
			self.pre_calc_total += (pre_calc_end-pre_calc_start)

			# ADD FENCES
			order=self.fence(trace)
			# print("order =",order)
			# print("fences_thread =", self.fences_by_thread)
			# print("all_events_thread", self.all_events_by_thread)

			# transitive SB calc, put into hb edges
			hb_edges += self.sb()
			# print("hb-post-fences=" + str(hb_edges))
			# print("so-of-sb-post-fences=" + str(self.so_edges))

			# pre-process and obtain separately reads, writes with neighbouring fences
			reads, writes = preprocessing(order)
			# print ('reads:', reads)
			# print ('writes:', writes)

			# CALC EDGES
			calc_edges = edges_computation(reads, writes, self.all_events_by_thread, self.fences_by_thread, mo_edges, self.so_edges)
			swdob_edges, fr_edges, self.so_edges = calc_edges.get()
			hb_edges = hb_edges + swdob_edges
			# print('done edge computation')
			# print("swdob = ", swdob_edges)
			# print("hb = ", hb_edges)
			# print("mo = ", mo_edges)
			# print("rf = ", rf_edges)
			# print("fr = ", fr_edges)
			# print("rfinv = ", rfinv_edges)
			# print("so = ", self.so_edges)
			
			# WEAK FENSYING
			wf = weak_fensying(hb_edges, mo_edges, rf_edges, rfinv_edges)
			# print('done weak fensying')
			if wf.has_weak_cycles():
				candidate_cycles = wf.get()
				candidate_cycles_tags = compute_relaxed_tags(candidate_cycles, swdob_edges)
			# print ('done weak fence tagging')
				
			# STRONG FENSYING
			strong_cycles = Cycles(self.so_edges)
			candidate_cycles += strong_cycles
			# print('done strong fensying')
			candidate_cycles_tags += compute_strong_tags(strong_cycles)
			# print ('done strong fence tagging')

			# print('candidate cycles=', candidate_cycles)
			if (len(candidate_cycles) > 0):
				self.cycles_tags_by_trace.append(candidate_cycles_tags)

				cycles_of_candidate_fences = []
				for c in candidate_cycles:
					cycles_of_candidate_fences.append([e for e in c if (type(e) is str) and ('_at' not in e)])

				get_translation = z3translate(cycles_of_candidate_fences)
				formula_variables, formula = get_translation.get()

				self.z3vars = list(set(self.z3vars + formula_variables)) # add to list of unique fences
				self.formula.append(formula) # add to disjuctions

			else: # len(candidate_cycles) = 0
				self.error_string = '\nABORT: buggy trace #' + str(trace_no) + ' cannot be stopped by C11 fences\n'
				return

	def fence(self, trace):
		order = []                # trace with fences
		events_in_thread = []     # list events of a thread
		fences_in_thread = []     # list of fences of a thread
		current_thread = 1
		
		for i in range(len(trace)):
			if trace[i][T_NO] != current_thread: # done with events of a thread
				self.all_events_by_thread.append(events_in_thread)
				self.fences_by_thread.append(fences_in_thread)

				events_in_thread = []
				fences_in_thread = []
				current_thread = current_thread + 1

			if trace[i][TYPE] == FENCE:
				# note: no candidate-fences before and after program fences  
				fence_name = 'F_at_' + str(trace[i][LINE_NO])
				order.append(fence_name)
				events_in_thread.append(fence_name)
				fences_in_thread.append(fence_name) 
				continue

			if trace[i][LINE_NO] != 'NA': # is a read or a write event
				order.append('F_before_' + str(trace[i][LINE_NO]))
				events_in_thread.append('F_before_' + str(trace[i][LINE_NO]))
				fences_in_thread.append('F_before_' + str(trace[i][LINE_NO]))
				
				# add event itself
				order.append(trace[i])
				events_in_thread.append(trace[i])

				# add fence after the read/write event
				order.append('F_after_' + str(trace[i][LINE_NO]))
				events_in_thread.append('F_after_' + str(trace[i][LINE_NO]))
				fences_in_thread.append('F_after_' + str(trace[i][LINE_NO]))

				continue

			# not a fence or read or write
			order.append(trace[i])
			events_in_thread.append(trace[i])
			
		# add last thread lists
		self.all_events_by_thread.append(events_in_thread)
		self.fences_by_thread.append(fences_in_thread)
		
		return order

	# computes transitive sb
	# computes non-transitive so between events of a thread
	def sb(self):
		sb_edges = []
		for thread_events in self.all_events_by_thread: # events of one thread at a time
			last_sc_event = [] 

			for i in range(1, len(thread_events)): # 1 event at a time, 1st event skipped because it has no sb before
				is_sc = False

				# event = label (if fence) or S_NO (otherwise)
				if type(thread_events[i]) is str: # event is a fence
					event = thread_events[i]
					is_sc = True
				else:
					event = thread_events[i][S_NO]
					if thread_events[i][MO] == SEQ_CST:
						is_sc = True

				# if seq_cst ordered add so edge
				if is_sc:
					if last_sc_event != []: # some sc event has been found before this event
						self.so_edges.append((last_sc_event, event))
					last_sc_event = event

				# add sb edge with each event that occurs before in thread
				for j in range(i):
					if type(thread_events[j]) is str:
						sb_edges.append((thread_events[j], event))
					else:
						sb_edges.append((thread_events[j][S_NO], event))

		return sb_edges

	def get(self):
		return self.z3vars, self.formula, self.error_string, self.pre_calc_total, self.cycles_tags_by_trace