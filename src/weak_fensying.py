from cycle import cycles
import time

class weak_fensying:
	def __init__(self, flags, bounds, hb_edges, mo_edges, rf_edges, fr_edges):
		self.flags  = flags
		self.bounds = bounds

		self.mo_hb_rf_rfinv_cycles = []
		self.hb_edges = hb_edges
		self.mo_edges = mo_edges
		self.rf_edges = rf_edges
		self.fr_edges = fr_edges
		
		self.hb_source_nodes = set([e for (e,e_) in self.hb_edges])
		self.weak_cycles = []

		self.weak_cycles.extend(self.hb_cycles())
		self.weak_cycles.extend(self.rf_hb_cycles())
		self.weak_cycles.extend(self.mo_rf_hb_cycles())
		self.weak_cycles.extend(self.mo_hb_cycles())
		self.weak_cycles.extend(self.mo_hb_rfinv_cycles())
		self.weak_cycles.extend(self.mo_rf_hb_rfinv_cycles())

	def hb_cycles(self):
		return cycles(self.flags, self.bounds, self.hb_edges)

	def rf_hb_cycles(self):	
		rf_hb_cycles_final = [] # rf_hb_cycles \ hb_cycles \rf;hb;rf;hb... cycles

		for (e1,e2) in self.rf_edges:
			if not e2 in self.hb_source_nodes: # no hb path from e2
				continue
		
			rf_hb_edges = self.hb_edges[:] + [ (e1,e2) ]
			rf_hb_cycles = cycles(self.flags, self.bounds, rf_hb_edges)
			
			for cycle in rf_hb_cycles:
				has_rf_edge = False          # not all hb cycle
				has_single_rf_edge = True    # not rf;hb;rf;hb... cycle

				for i in range(len(cycle)):
					e1 = cycle[i]
					e2 = cycle[(i+1)%len(cycle)]
					if (e1,e2) in self.rf_edges:
						if has_rf_edge:     # another rf edge seen previously
							has_single_rf_edge = False
							break
						else: # no rf edge seen yet
							has_rf_edge = True

				if has_rf_edge and has_single_rf_edge: # exactly 1 rf edge in cycle
					rf_hb_cycles_final.append(cycle)

		return rf_hb_cycles_final

	def mo_rf_hb_cycles(self):
		rf_source_nodes = set([e for (e,e_) in self.rf_edges])
		mo_rf_hb_cycles_final = [] # mo_rf_hb_cycles \ hb_cycles \ rf U mo U hb... cycles etc.

		for (e1,e2) in self.mo_edges:
			if not e2 in rf_source_nodes: # no rf.hb path from e2 ==> no mo.rf.hb path from e1 via e2
				continue

			for e3 in [ e_ for (e,e_) in self.rf_edges if e==e2 ]: # for e3 where e2 --rf--> e3
				if e3 not in self.hb_source_nodes: # no hb path from e3 ==> no mo.rf.hb path from e1.e2 via e3
					continue
				
				mo_rf_hb_edges = self.hb_edges[:] + [ (e1,e2), (e2,e3) ]
				mo_rf_hb_cycles = cycles(self.flags, self.bounds, mo_rf_hb_edges)
		
				for cycle in mo_rf_hb_cycles:
					has_rf_edge = False          # not mo U hb cycle
					has_single_rf_edge = True    # not rf;hbUmo;rf;hb... cycle
					has_mo_edge = False          # not rf U hb cycle
					has_single_mo_edge = True    # not mo;rfUhb;mo;hb... cycle
					mo_rf_composition_preserved = True # cycle has mo;rf not mo;hb;rf

					for i in range(len(cycle)):
						e1 = cycle[i]
						e2 = cycle[(i+1)%len(cycle)]
						if (e1,e2) in self.mo_edges:
							e3 = cycle[(i+2)%len(cycle)]
							if not (e2,e3) in self.rf_edges: # mo edge not followed by rf edge
								mo_rf_composition_preserved = False
								break
							if has_mo_edge:
								has_single_mo_edge = False
								break
							else:
								has_mo_edge = True

						elif (e1,e2) in self.rf_edges:
							e0 = cycle[(i-1)%len(cycle)]
							if not (e0,e1) in self.mo_edges: # rf edge not preceeded by mo edge
								mo_rf_composition_preserved = False
								break
							if has_rf_edge:
								has_single_rf_edge = False
								break
							else:
								has_rf_edge = True

					if mo_rf_composition_preserved:
						if has_mo_edge and has_single_mo_edge and has_rf_edge and has_single_rf_edge:
							mo_rf_hb_cycles_final.append(cycle)

		return mo_rf_hb_cycles_final

	def mo_hb_cycles(self):
		mo_hb_cycles_final = [] # mo_cycles \ hb_cycles \ mp;hb;mo;hb... cycles

		for (e1,e2) in self.mo_edges:
			if not e2 in self.hb_source_nodes: # no hb path from e2
				continue
			
			mo_hb_edges = self.hb_edges[:] + [ (e1,e2) ]
			mo_hb_cycles = cycles(self.flags, self.bounds, mo_hb_edges)		

			for cycle in mo_hb_cycles:
				has_mo_edge = False # not hb cycle
				has_single_mo_edge = True # not mo;hb;mo;hb... cycle
				for i in range(len(cycle)):
					e1 = cycle[i]
					e2 = cycle[(i+1)%len(cycle)]
					if (e1,e2) in self.mo_edges:
						if has_mo_edge:
							has_single_mo_edge = False
							break
						else:
							has_mo_edge = True

				if has_mo_edge and has_single_mo_edge:
					mo_hb_cycles_final.append(cycle)

		return mo_hb_cycles_final
	
	def mo_hb_rfinv_cycles(self):
		# rfinv; mo = fr => mo_hb_rfinv_cycles = fr_hb_cycles
		mo_hb_rfinv_cycles_final = [] # mo_hb_rfinv_cycles \ hb_cycles \ rfinv U mo U hb... cycles etc.

		for (e1,e2) in self.fr_edges:
			if not e2 in self.hb_source_nodes: # no hb path from e2
					continue

			mo_hb_rfinv_edges  = self.hb_edges[:] + [ (e1,e2) ] # added rfinv;mo edge
			mo_hb_rfinv_cycles = cycles(self.flags, self.bounds, mo_hb_rfinv_edges)

			for cycle in mo_hb_rfinv_cycles:
				has_fr_edge = False            # not hb cycle
				has_single_fr_edge = True      # not fr; hb; fr; hb... cycle
				
				for i in range(len(cycle)):
					e1 = cycle[i]
					e2 = cycle[(i+1)%len(cycle)]
					if (e1,e2) in self.fr_edges:
						if has_fr_edge:
							has_single_fr_edge = False
							break
						else: 
							has_fr_edge = True

				if has_fr_edge and has_single_fr_edge:
					mo_hb_rfinv_cycles_final.append(cycle)	

		return mo_hb_rfinv_cycles_final

	def mo_rf_hb_rfinv_cycles(self):
		# rfinv; mo = fr => mo_rf_hb_rfinv_cycles = fr_rf_hb_cycles
		mo_rf_hb_rfinv_cycles_final = [] # mo_rf_hb_rfinv_cycles \ cycles that don't have a single rfinv;mo;rf edge
		
		for (e1,e2) in self.fr_edges:
			for e3 in [e_ for (e,e_) in self.rf_edges if e==e2]: # e3 for e2 --rf--> e3
				if not e3 in self.hb_source_nodes: # no hb path from e3
						continue
				
				mo_rf_hb_rfinv_edges  = self.hb_edges[:] + [ (e1,e2), (e2,e3) ]
				mo_rf_hb_rfinv_cycles = cycles(self.flags, self.bounds, mo_rf_hb_rfinv_edges)
				
				for cycle in mo_rf_hb_rfinv_cycles:
					has_fr_edge = False
					has_single_fr_edge = True
					has_rf_edge = False
					has_single_rf_edge = True
					rfinv_mo_rf_composition_conserved = True

					for i in range(len(cycle)):
						e1 = cycle[i]
						e2 = cycle[(i+1)%len(cycle)]

						if (e1,e2) in self.fr_edges:
							e3 = cycle[(i+2)%len(cycle)]
							if not (e2,e3) in self.rf_edges:
								rfinv_mo_rf_composition_conserved = False
								break
							if has_fr_edge:
								has_single_fr_edge = False
								break
							else:
								has_fr_edge = True

						elif (e1,e2) in self.rf_edges:
							e0 = cycle[(i-1)%len(cycle)]
							if not (e0,e1) in self.fr_edges:
								rfinv_mo_rf_composition_conserved = False
								break
							if has_rf_edge:
								has_single_rf_edge = False
								break
							else:
								has_rf_edge = True

					if rfinv_mo_rf_composition_conserved:
						if has_fr_edge and has_single_fr_edge and has_rf_edge and has_single_rf_edge:
							mo_rf_hb_rfinv_cycles_final.append(cycle)
								
		return mo_rf_hb_rfinv_cycles_final


	def has_weak_cycles(self):
		if len(self.weak_cycles) > 0:
			return True
		return False
	
	def get(self):
		return self.weak_cycles