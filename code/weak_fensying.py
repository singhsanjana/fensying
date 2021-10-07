from cycle import Cycles
import time

class weak_fensying:
	def coherence_compositions(self):
		rules = [ 
			[self.hb_edges],
			[self.mo_edges, self.hb_edges],
			[self.rf_edges, self.hb_edges],
			[self.mo_edges, self.rf_edges, self.hb_edges],
			[self.mo_edges, self.hb_edges, self.rfinv_edges],
			[self.mo_edges, self.rf_edges, self.hb_edges, self.rfinv_edges]
		]

		return rules


	def __init__(self, hb_edges, mo_edges, rf_edges, rfinv_edges):
		self.mo_hb_rf_rfinv_cycles = []
		self.hb_edges    = hb_edges
		self.mo_edges    = mo_edges
		self.rf_edges    = rf_edges
		self.rfinv_edges = rfinv_edges

		self.coherence_rules = self.coherence_compositions()

		self.weak_cycles = []

		has_cycles_in_union = self.compute_cycles_in_union()
		if has_cycles_in_union:
			self.detect_cycles_in_coherence_conditions()
			self.weak_cycles = [list(item) for item in set(tuple(row) for row in self.weak_cycles)]
			# [snj]:  TODO remove duplicates of kind [1,2] [2,1]
			
	def has_weak_cycles(self):
		return (len(self.weak_cycles) > 0)

	def compute_cycles_in_union(self):
		rf_permutations = []
		# given w --rf--> r a cycle can either contain w --rf--> r or r --rf_inv --> w
		# thus, from rf_edges, pick 1 edge at a time and compute cycles with its inv
		# considering all other rf, hb and mo edges
		for i in range(len(self.rf_edges)):
			temp_list = self.rf_edges[:] # to pass by value and not by reference/object-reference
			del temp_list[i] # remove current rf edge
			temp_list.append(self.rfinv_edges[i]) # append corresponding rfinv edge
			# append list to list of permutations of rf where each entry is a list with inv of 1 rf edge
			rf_permutations.append(temp_list)
		
		for p in rf_permutations:
			self.mo_hb_rf_rfinv_cycles += Cycles(self.hb_edges + self.mo_edges + p)
		
		# remove duplicate cycles
		self.mo_hb_rf_rfinv_cycles = [list(item) for item in set(tuple(row) for row in self.mo_hb_rf_rfinv_cycles)]
		# print("mo_hb_rf_rfinv_cycles=",self.mo_hb_rf_rfinv_cycles)

		if len(self.mo_hb_rf_rfinv_cycles) == 0:
			return False
		return True

	def detect_cycles_in_coherence_conditions(self):
		for cycle in self.mo_hb_rf_rfinv_cycles:
			current_relation = [0]    * len(self.coherence_rules)
			active_rules     = [True] * len(self.coherence_rules)
			active_count     = len(self.coherence_rules)
			count_of_relations_in_cycle = [0] * len(self.coherence_rules)

			first_edge = ( cycle[0], cycle[1] )
			for n in range(len(self.coherence_rules)):
				for i in range(len(self.coherence_rules[n])):
					if first_edge in self.coherence_rules[n][i]:
						current_relation[n] = i
						break
				if i == len(self.coherence_rules[n]):
					active_rules[n] = False
					active_count = active_count - 1
					

			for i in range(1, len(cycle)):
				if active_count == 0:
					break

				next_edge = ( cycle[i], cycle[ (i+1) % len(cycle) ] ) 

				for n in range(len(self.coherence_rules)):
					if not active_rules[n]:
						continue
					
					if next_edge in self.coherence_rules[n][current_relation[n]]:
						continue

					count_of_relations_in_cycle[n] = count_of_relations_in_cycle[n] + 1
					current_relation[n] = (current_relation[n] + 1) % len(self.coherence_rules[n])
					if next_edge in self.coherence_rules[n][current_relation[n]]:
						continue

					active_rules[n] = False
					active_count = active_count - 1
				
			if active_count == 0:
				continue # no rule is active, not a candidate cycle

			for n in range(len(self.coherence_rules)):
				if not active_rules[n]:
					continue

				count = count_of_relations_in_cycle[n]
				rule_size = len(self.coherence_rules[n])
				if count == rule_size - 1  or count == rule_size:
					self.weak_cycles.append(cycle)
					break	
	
	def get(self):
		return self.weak_cycles