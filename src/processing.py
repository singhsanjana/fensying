from model_checking_output.pre_calculator.pre_calculations import pre_calculations
from preprocessing import preprocessing
from edges_computation import edges_computation
from cycle import cycles
from publish_result import print_trace
from weak_fensying import weak_fensying
from compute_cycles_tags import compute_relaxed_tags, compute_strong_tags
from z3translate import z3translate
from constants import *

# import sys
import time

# IDEA: any var with _thread at the end means that it is an array of arrays separated by thread number
class Processing:
	def __init__(self, traces, buggy_trace_no, flags, bounds):
		self.z3vars  = []										# list of all z3 variables ie fences
		self.formula = []										# list of disjunctions for the z3 formula
		self.error_string = ''
		self.pre_calc_total = 0									# time taken for calculation of initial values - HB, MO, SB
	
		self.cycles_tags_by_trace = []

		trace_no = 0

		for trace in traces:									# run for each trace
			self.all_events_by_thread = []						# list of all events separated by threads
			self.fences_by_thread     = []						# list of fences in each thread
			candidate_cycles_tags     = []							# list of all cycles in this trace with fence tags
			
			trace_no += 1
			
			pre_calc_start = time.time()
			# [snj]: hb edges does not include sb, sb would be computed after inserting fences
			hb_edges, mo_edges, rf_edges, rfinv_edges, so_edges = pre_calculations(trace, buggy_trace_no[trace_no-1])
			
			pre_calc_end = time.time()
			self.pre_calc_total += (pre_calc_end-pre_calc_start)

			# ADD FENCES
			order=self.fence(trace)
			
			# transitive SB calc, put into hb edges
			sb_edges, so_edges_from_sb = self.sb()
			hb_edges += sb_edges
			so_edges += so_edges_from_sb
			
			# pre-process and obtain separately reads, writes with neighbouring fences
			reads, writes = preprocessing(order)
			
			# CALC EDGES
			calc_edges = edges_computation(reads, writes, self.all_events_by_thread, self.fences_by_thread, mo_edges, so_edges)
			swdob_edges, fr_edges, so_edges = calc_edges.get()
			hb_edges = hb_edges + swdob_edges
			
			# WEAK FENSYING
			wf = weak_fensying(flags, bounds, hb_edges, mo_edges, rf_edges, fr_edges)
			if wf.has_weak_cycles():
				weak_cycles = wf.get()
				candidate_cycles_tags = compute_relaxed_tags(weak_cycles, swdob_edges)
				
			# STRONG FENSYING
			strong_cycles = cycles(flags, bounds, so_edges)
			candidate_cycles_tags += compute_strong_tags(strong_cycles, sb_edges)
			
			if (len(candidate_cycles_tags) > 0):
				self.cycles_tags_by_trace.append(candidate_cycles_tags)

				cycles_of_candidate_fences = []
				for c in candidate_cycles_tags:
					cycles_of_candidate_fences.append([f for f in c.keys() if ('_at_' not in f)])
				
				get_translation = z3translate(cycles_of_candidate_fences)
				formula_variables, formula = get_translation.get()

				self.z3vars = list(set(self.z3vars + formula_variables)) # add to list of unique fences
				self.formula.append(formula) # add to disjuctions

			else:
				self.error_string = '\nABORT: C11 fences cannot stop the buggy trace\n'
				print_trace(trace)
				return

	def fence(self, trace):
		def ord(mo):
			mo_short = {'release':f_tags.r, 'acquire':f_tags.a, 'acq_rel':f_tags.ar, 'seq_cst':f_tags.sc}
			return mo_short[mo]

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
				fence_name  = 'F(' + ord(trace[i][MO]) +')_at_'
				fence_name += str(trace[i][S_NO]) + '-' + str(trace[i][LINE_NO]) + '@' + trace[i][FILENAME]
				order.append(fence_name)
				events_in_thread.append(fence_name)
				fences_in_thread.append(fence_name) 
				continue

			if trace[i][LINE_NO] != 'NA': # is a read or a write or an rmw event
				fence_name  = 'F_before_'
				fence_name += str(trace[i][S_NO]) + '-' + str(trace[i][LINE_NO]) + '@' + trace[i][FILENAME]
				order.append(fence_name)
				events_in_thread.append(fence_name)
				fences_in_thread.append(fence_name)
			
				# add event itself
				order.append(trace[i])
				events_in_thread.append(trace[i])

				# add fence after the read/write event
				fence_name  = 'F_after_'
				fence_name += str(trace[i][S_NO]) + '-' + str(trace[i][LINE_NO]) + '@' + trace[i][FILENAME]
				order.append(fence_name)
				events_in_thread.append(fence_name)
				fences_in_thread.append(fence_name)
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
		so_edges = []
		for thread_events in self.all_events_by_thread: # events of one thread at a time
			last_sc_event = None
			last_sc_fence = None

			for i in range(1, len(thread_events)): # 1 event at a time, 1st event skipped because it has no sb before
				is_fence = False
				is_sc = False

				# event = label (if fence) or S_NO (otherwise)
				if type(thread_events[i]) is str: # event is a fence
					event = thread_events[i]
					is_fence = True
					is_sc = True
				else:
					event = thread_events[i][S_NO]
					if thread_events[i][MO] == SEQ_CST:
						is_sc = True

				# if seq_cst ordered add so edge
				if is_sc:
					if last_sc_event != None: # some sc event has been found before this event
						so_edges.append((last_sc_event, event))
					if last_sc_fence != None:
						so_edges.append((last_sc_fence, event))

					if is_fence:
						last_sc_fence = event
					else:
						last_sc_event = event

				# add sb edge with each event that occurs before in thread
				for j in range(i):
					if type(thread_events[j]) is str:
						sb_edges.append((thread_events[j], event))
					else:
						sb_edges.append((thread_events[j][S_NO], event))

		return sb_edges, so_edges

	def get(self):
		return self.z3vars, self.formula, self.error_string, self.pre_calc_total, self.cycles_tags_by_trace