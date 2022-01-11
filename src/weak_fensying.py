from all_paths import compute_paths
from cycle import compute_cycles, make_graph

class weak_fensying:
	def __init__(self, hb_edges, mo_edges, rf_edges, rfinv_edges):
		self.mo_hb_rf_rfinv_cycles = []
		self.hb_edges    = hb_edges
		self.mo_edges    = mo_edges
		self.rf_edges    = rf_edges
		self.rfinv_edges = rfinv_edges

		self.hb_graph = make_graph(self.hb_edges)
		
		self.has_hb_path = {}
		self.hb_paths    = {}
		
		self.weak_cycles = []

		self.weak_cycles.extend(self.hb_cycles())
		self.weak_cycles.extend(self.rf_hb_cycles())
		self.weak_cycles.extend(self.mo_rf_hb_cycles())
		self.weak_cycles.extend(self.mo_hb_cycles())
		self.weak_cycles.extend(self.mo_hb_rfinv_cycles())
		self.weak_cycles.extend(self.mo_rf_hb_rfinv_cycles())

	def seen(self, e1, e2):
		if not e1 in self.has_hb_path:
			return False
		if not e2 in self.has_hb_path[e1]:
			return False
		return True

	def record(self, e1, e2, paths):
		if not e1 in self.has_hb_path: # record true if has paths ie (len(paths) > 0)
			self.has_hb_path[e1] = { e2 : (len(paths) > 0) }
		else:
			self.has_hb_path[e1][e2] = (len(paths) > 0)

		if not self.has_hb_path[e1][e2]:
			return 

		if not e1 in self.hb_paths:
			self.hb_paths[e1] = { e2 : paths }
		else:
			self.hb_paths[e1][e2] = paths

	def has_path(self, e1, e2):
		return self.has_hb_path[e1][e2]

	def get_hb_path(self, e1, e2):
		if self.seen(e1, e2):
			if self.has_hb_path[e1][e2]: # paths from e1 to e2 previously computed
				return self.hb_paths[e1][e2]
			else:
				return []

		paths = compute_paths(self.hb_graph, e1, e2) # paths from e1 to e2 to be computed
		self.record(e1, e2, paths)
		return paths
	
	def hb_cycles(self):
		return compute_cycles(self.hb_graph)

	def rf_hb_cycles(self):
		cycles = []
		hb_source_nodes = set([e for (e,e_) in self.hb_edges])

		for (e1,e2) in self.rf_edges:
			if not e2 in hb_source_nodes: # no hb path from e2
				continue

			paths = self.get_hb_path(e2, e1)
			if len(paths) > 0: # found e1 --rf--> e2 --hb--> e1
				cycles.extend(self.hb_paths[e2][e1])

		return cycles

	def mo_rf_hb_cycles(self):
		cycles = []

		hb_source_nodes = set([e for (e,e_) in self.hb_edges])
		rf_source_nodes = set([e for (e,e_) in self.rf_edges])

		for (e1,e2) in self.mo_edges:
			if not e2 in rf_source_nodes: # no rf.hb path from e2 ==> no mo.rf.hb path from e1 via e2
				continue

			for e3 in [ e_ for (e,e_) in self.rf_edges if e==e2 ]:
				if e3 not in hb_source_nodes: # no hb path from e3 ==> no mo.rf.hb path from e1.e2 via e3
					continue

				paths = self.get_hb_path(e3, e1)
				if len(paths) > 0: # found e1 --mo--> e2 --rf--> e3 --hb--> e1
					paths = [ ([e2]+path) for path in paths ] # add e2 to paths from e3 to e1
					cycles.extend(paths)

		return cycles

	def mo_hb_cycles(self):
		cycles = []
		hb_source_nodes = set([e for (e,e_) in self.hb_edges])

		for (e1,e2) in self.mo_edges:
			if not e2 in hb_source_nodes: # no hb path from e2
				continue

			paths = self.get_hb_path(e2, e1)
			if len(paths) > 0: # found e1 --mo--> e2 --hb--> e1
				cycles.extend(self.hb_paths[e2][e1])

		return cycles

	def mo_hb_rfinv_cycles(self):
		cycles = []
		hb_source_nodes = set([e for (e,e_) in self.hb_edges])

		for (e1,e2) in self.mo_edges:
			if not e2 in hb_source_nodes: # no hb path from e2
					continue

			for e3 in [e for (e,e_) in self.rfinv_edges if e_==e1]:
				paths = self.get_hb_path(e2, e3)
				if len(paths) > 0: # found e1 --mo--> e2 --hb--> e3 --rfinv--> e1
					paths = [ ([e1]+path) for path in paths ] # add e1 to paths from e2 to e3
					cycles.extend(paths)

		return cycles

	def mo_rf_hb_rfinv_cycles(self):
		cycles = []
		cycles = []

		hb_source_nodes = set([e for (e,e_) in self.hb_edges])
		
		for (e1,e2) in self.mo_edges:
			for e3 in [e_ for (e,e_) in self.rf_edges if e==e2]:
				if not e3 in hb_source_nodes: # no hb path from e3
						continue

				for e4 in [e for (e,e_) in self.rfinv_edges if e_==e1]: # cycle only when rfinv edge ends in e1
					paths = self.get_hb_path(e3, e4)
					if len(paths) > 0: # found e1 --mo--> e2 --rf--> e3 --hb--> e4 --rfinv--> e1
						paths = [ ([e1, e2]+path) for path in paths ] # add e1.e2 to paths from e3 to e4
						cycles.extend(paths)

		return cycles

	def has_weak_cycles(self):
		if len(self.weak_cycles) > 0:
			return True
		return False
	
	def get(self):
		return self.weak_cycles