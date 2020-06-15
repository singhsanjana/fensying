# --------------------------------------------------------
# Processes the std:out data from the command line output
# and curates lists of:
#   1. Order of events including pseudo fences
#   2. Happens-before relationships (HB)
#   3. Modification Order relationships (MO)
#   4. Sequenced-before relationships (SB)
#   5. Total order relationships (TO)
#
# Then proceeds on to find out cycles from the TO graph
# and finally insert the required fences in the input
# program.
# --------------------------------------------------------

from hb import hb
from graph import Graph
from mo import mo
from to import to
from to_transitive import to_transitive
from cycle import Cycles
from insert import insert
from z3translate import z3translate
from z3convert import z3convert

import sys

class Processing:
	def __init__(self,traces):
		self.z3vars = []												# list of all z3 constants
		self.disjunctions = []											# list of disjunctions for the z3 function

		trace_no = 0

		for trace in traces[:2]:											# run for each trace
			self.fence_thread = []										# list of fences separated by threads
			self.fence_sc_event_thread = []								# list of all fences and events separated by threads
			self.sc_sb_edges = []										# list of sb edge pairs between fences as well as sc events
			self.cycles = []                                            # list of all cycles between the fences and events
			self.loc_info = {}                                          # information regarding the required fence locations

			trace_no += 1
			print("trace=",trace_no)

			hb_graph = hb(trace)
			mat,size = hb_graph.get()

			get_mo = mo(trace,mat,size)
			mo_edges = get_mo.get()

			order=self.fence(trace)

			to(order,mo_edges,self.sc_sb_edges)

			cycles = Cycles()

			to_transitive()

			unique_fences = list(sorted(set(x for l in cycles for x in l)))
			unique_fences = [uf for uf in unique_fences if 'F' in uf]
			# print("unique_fences=",unique_fences)

			if len(unique_fences)>0:
				for fence in unique_fences:
					for i in range(len(order)):
						if fence == order[i]:
							o = order[i-1]
							fence_name = order[i]
							var_name = 'l'+str(o['line'])
							self.loc_info[fence_name] = var_name
				
				print("loc_nfo=",self.loc_info)

				get_translation = z3translate(cycles,self.loc_info)
				consts, translation = get_translation.get()

				for con in consts:
					if con not in self.z3vars:
						self.z3vars.append(con)
				self.disjunctions.append(translation)

			else:
				print("No TO cycles can be formed for trace",trace_no,"\nHence this behaviour cannot be stopped using SC fences")
				sys.exit()

		z3convert(self.z3vars,self.disjunctions)


	def fence(self,trace):

		# find out the number of threads in the program
		threads = int(trace[-1][1])

		order = [] # IDEA: any var with _thread at the end means that it is separated by thread number

		for j in range(1,threads+1):
			fence_no = 0
			fences_in_thread = []
			fences_events_in_thread = []
			fences_events_in_thread_min = []									# a minimal version of the above in order to find sb's

			for i in range(len(trace)):
				if int(trace[i][1])==j:
					fence_no+=1
					fence_name = 'F'+str(j)+'n'+str(fence_no)
					order.append(fence_name)
					fences_in_thread.append(fence_name)							# fence order in a thread
					fences_events_in_thread.append(fence_name)					# fence added to fences+all events order in a thread
					fences_events_in_thread_min.append(fence_name)				# fence added to fences+sc events order in a thread

					event = {'no': trace[i][0],									# trace[i][0] is the event number
							'thread': j,
							'mo': trace[i][3],
							'type': trace[i][2]
					}
					if trace[i][2]=='read' or trace[i][2]=="rmw":
						event['rf'] = trace[i][6]								# trace[i][7] gives Read-from (Rf)
						event['loc'] = trace[i][4]
						event['line'] = trace[i][8]								# line number in the original source code
					elif trace[i][2]=='write':
						event['loc'] = trace[i][4]
						event['line'] = trace[i][8]

					if event['mo'] == 'seq_cst':
						fences_events_in_thread_min.append(trace[i][0])			# event added to fences+sc events order in a thread

					order.append(event)
					fences_events_in_thread.append(event)

			fence_no+=1
			fence_name = 'F'+str(j)+'n'+str(fence_no)

			# IDEA: so many extra vars so that- use only where needed cuz loops increase complexity
			fences_events_in_thread.append(fence_name)
			order.append(fence_name)
			fences_in_thread.append(fence_name)
			fences_events_in_thread_min.append(fence_name)

			self.fence_thread.append(fences_in_thread)
			self.fence_sc_event_thread.append(fences_events_in_thread_min)

		# now find all sb relations between fences and events
		self.sb()

		return order

	def sb(self):
		to_sets = {}

		for i in self.fence_sc_event_thread:
			for j in range(len(i)):
				for k in range(j+1,len(i)):
					sb_relation = (i[j],i[k])
					if not sb_relation in self.sc_sb_edges:
						self.sc_sb_edges.append(sb_relation)
						fence_list = []

						if ('F' in i[j]):
							fence_list.append(i[j])
						if ('F' in i[k]):
							fence_list.append(i[k])
						if fence_list:
							fence_list.sort()
							to_sets[sb_relation] = [fence_list]
		
		to_sets_store = open("store/to_sets_store", 'w')
		to_sets_store.write(str(to_sets))
		to_sets_store.close()

		self.sc_sb_edges.sort(key = lambda x: x[0])

	def get(self):
		return self.loc_info
