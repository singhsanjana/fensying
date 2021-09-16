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
		self.z3vars = []												# list of all z3 constants
		self.disjunctions = []									# list of disjunctions for the z3 function
		self.fences_present = []								# list of fences converted to their respective variable names
		self.fences_present_locs = []
		self.error_string = ''
		self.pre_calc_total = 0									# time taken for calculation of initial values - HB, MO, SB
		self.all_cycles_by_trace = []
		self.cycles_tags_by_trace = []

		trace_no = 0
		# print("traces=",traces)

		for trace in traces:										# run for each trace
			self.all_events_thread = []						# list of all events separated by threads
			# self.all_sc_events = []								# list of all sc events in program after inserting candidate fences
			self.fences_thread = []								# list of fences in each thread
			self.fences_in_trace = []							# list of fences already present in the program

			cycles = []                        		# list of all cycles in this trace
			cycles_tags = []
			loc_info = {}                         # information regarding the required fence locations

			trace_no += 1
			print("---------Trace",trace_no,"---------")

			pre_calc_start = time.time()
			# [snj]: hb edges does not include sb, sb would be computed after inserting fences
			hb_edges, mo_edges, rf_edges, rfinv_edges, self.so_edges = pre_calculations(trace, buggy_trace_no[trace_no-1])
			# hb_print = hb_edges
			# hb_print.sort(key = lambda x:x[1])
			# hb_print.sort(key = lambda x:x[0])
			# print("hb-pre-fences=" + str(hb_print))

			pre_calc_end = time.time()
			self.pre_calc_total += (pre_calc_end-pre_calc_start)

			# ADD FENCES
			order=self.fence(trace)
			print("order =",order)
			# print("fences_present =", self.fences_in_trace)
			# print("fences_thread =", self.fences_thread)
			# print("all_events_thread", self.all_events_thread)

			# transitive SB calc, put into hb edges
			hb_edges += self.sb()
			# print("hb-post-fences=" + str(hb_edges))
			# print("so-of-sb-post-fences=" + str(self.so_edges))

			# pre-process and obtain separately reads, writes with neighbouring fences
			reads, writes = preprocessing(order)

			# CALC EDGES
			calc_edges = edges_computation(reads, writes, self.fences_thread, mo_edges, self.so_edges)
			swdob_edges, fr_edges, self.so_edges = calc_edges.get()
			hb_edges = hb_edges + swdob_edges
			print("swdob = ", swdob_edges)
			print("hb = ", hb_edges)
			print("mo = ", mo_edges)
			print("rf = ", rf_edges)
			print("fr = ", fr_edges)
			print("rf1 = ", rfinv_edges)
			print("so = ", self.so_edges)
			# TODO: reduce even more by only including fences with 'l' in the name and no 'F' since only those get added anyway
			
			# CYCLES
			check_edges = hb_edges + mo_edges + rf_edges + fr_edges # TODO: confirm this check. exclused rf1
			check_cycles = Cycles(check_edges)

			if len(check_cycles) == 0:
				self.error_string = "\nNo TO cycles can be formed for trace "+str(trace_no)+"\nHence this behaviour cannot be stopped using SC fences\n"
				return
			
			# WEAK FENSYING
			check1 = weak_fensying(hb_edges, mo_edges, rf_edges, rfinv_edges)
			cycles_exist = check1.compute_cycles()
			if cycles_exist:
				relaxed_cycles = check1.check_for_weak_compositions()
				cycles += relaxed_cycles
				cycles_tags += compute_relaxed_tags(relaxed_cycles, swdob_edges)
				# print("relaxed_cycles =",relaxed_cycles)
				# print("self.all_cycles =",self.all_cycles)
				# print("self.cycles_tags =",self.cycles_tags)

			# STRONG FENSYING
			strong_cycles = Cycles(self.so_edges)
			# print("strong_cycles =",strong_cycles)
			cycles += strong_cycles
			cycles_tags += compute_strong_tags(strong_cycles)

			self.all_cycles_by_trace.append(cycles)
			self.cycles_tags_by_trace.append(cycles_tags)

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
		# self.all_sc_events = ['F1n1']
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
				fences_in_thread.append(fence_name)

			if not trace[i][TYPE] == FENCE:
				order.append(trace[i])
				all_events.append(trace[i])
				# if trace[i][MO] == SEQ_CST:
				# 	self.all_sc_events.append(trace[i][S_NO])
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

		return order

	# computes transitive sb
	# computes non-transitive so between events of a thread
	def sb(self):
		sb_edges = []
		for thread_events in self.all_events_thread: # events of one thread at a time
			# first event in a thread is always fence and fences are considered sc
			last_sc_event = thread_events[0] 

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
					self.so_edges.append((last_sc_event, event))
					last_sc_event = event

				# add sb edge with each event that occurs before in thread
				for j in range(i):
					sb_edges.append((thread_events[j][S_NO], event))

		return sb_edges


		# for thread_events in self.all_events_thread:
		# 	for i in range(len(thread_events)):
		# 		event = thread_events[i]
		# 		k = 0
		# 		if type(event) is str:
		# 			e0 = event
		# 			k += 1
		# 		else:
		# 			e0 = event[S_NO]
		# 			if event[MO] == SEQ_CST:
		# 				k += 1
		# 		for j in range(i+1, len(thread_events)):
		# 			k = 1 if k == 2 else k
		# 			sb_event = thread_events[j]
		# 			if type(sb_event) is str:
		# 				e1 = sb_event
		# 				k += 1
		# 			else:
		# 				e1 = sb_event[S_NO]
		# 				if sb_event[MO] == SEQ_CST:
		# 					k += 1
		# 			e1 = sb_event if type(sb_event) is str else sb_event[S_NO]
		# 			sb_edges.append((e0, e1))
		# 			if k == 2:
		# 				self.so_edges.append((e0, e1))
		# # print("sb_edges",sb_edges)
		# # print("so edges", self.so_edges) # TODO: so edges not getting computed properly here
		# return sb_edges

	def get(self):
		return self.fences_present, self.fences_present_locs, self.z3vars, self.disjunctions, self.error_string, self.pre_calc_total, self.all_cycles_by_trace, self.cycles_tags_by_trace