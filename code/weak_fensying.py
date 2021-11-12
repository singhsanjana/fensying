from all_paths import all_path
import time

class weak_fensying:
	def __init__(self, hb_edges, mo_edges, rf_edges, rfinv_edges):
		self.mo_hb_rf_rfinv_cycles = []
		self.hb_edges    = hb_edges
		self.mo_edges    = mo_edges
		self.rf_edges    = rf_edges
		self.rfinv_edges = rfinv_edges
		
		self.hb_paths    = {}
		self.rf_hb_paths = {}
		self.mo_hb_paths = {}

		self.weak_cycles = []

		self.all_hb_paths()
		print('hb_paths', self.hb_paths)

		self.weak_cycles += self.hb_cycles()
		self.weak_cycles += self.rf_hb_cycles()
		self.weak_cycles += self.mo_rf_hb_cycles()
		self.weak_cycles += self.mo_hb_cycles()
		self.weak_cycles += self.mo_hb_rfinv_cycles()
		self.weak_cycles += self.mo_rf_hb_rfinv_cycles()

	def all_hb_paths(self):
		for (e1,e1_) in self.hb_edges:
			self.hb_paths[e1] = {}
			for (e2_,e2) in self.hb_edges:
				if e1 == e2:
					continue
				
				paths = all_path(self.hb_edges, e1, e2)
				if len(paths) > 0: # has an hb path from e1 to e2
					self.hb_paths[e1][e2] = paths

	def hb_cycles(self):
		cycles = []

		for e1 in self.hb_paths:              # for paths from e1
			for e2 in self.hb_paths[e1]:      # exists path from e1 to e2
				if not e2 in self.hb_paths:   # no paths from e2
					continue
				
				if e1 in self.hb_paths[e2]:   # exists path from e2 to e1
					for path_e1_e2 in self.hb_paths[e1][e2]:
						for path_e2_e1 in self.hb_paths[e2][e1]:
							# join path from e1 to e2 with path from e2 to e1, remove 1 copy of repeating e1 and e2
							cycle = path_e1_e2[:-1] + path_e2_e1[:-1]
							cycles += cycle
		return cycles

	def rf_hb_cycles(self):
		cycles = []

		for (e1,e2) in self.rf_edges:
			if not e2 in self.hb_paths:
				continue

			# exist hb path(s) from e2 ==> exist rf.hb path(s) from e1
			self.rf_hb_paths[e1] = {}

			for e3 in self.hb_paths[e2]:
				# exists rf.hb path from e1 to e3 (via e2 ie. e1 --rf--> e2 --hb--> e3) 
				e1_paths = [ ([e1] + p) for p in self.hb_paths[e2][e3] ]
				self.rf_hb_paths[e1][e3] = e1_paths

				if e3 == e1: # exists rf.hb path from e1 to e1 (via e2 ie. e1 --rf--> e2 --hb--> e1)
					cycles += self.hb_paths[e2][e1] 				

		return cycles

	def mo_rf_hb_cycles(self):
		cycles = []

		for (e1,e2) in self.mo_edges:
			if not e2 in self.rf_hb_paths: # no rf.hb path from e2 ==> no mo.rf.hb path from e1 via e2
				continue

			if e1 in self.rf_hb_paths[e2]:
				# exists mo.rf.hb path from e1 to e1 (via e2 ie. e1 --mo--> e2 --rf.hb--> e1)
				cycles += self.rf_hb_paths[e2][e1]

		return cycles

	def mo_hb_cycles(self):
		cycles = []

		for (e1,e2) in self.mo_edges:
			if not e2 in self.hb_paths:
				continue

			# exist hb path(s) from e2 ==> exist mo.hb path(s) from e1
			self.mo_hb_paths[e1] = {}

			for e3 in self.hb_paths[e2]:
				# exists mo.hb path from e1 to e3 (via e2 ie. e1 --mo--> e2 --hb--> e3) 
				e1_paths = [ ([e1] + p) for p in self.hb_paths[e2][e3] ]
				self.mo_hb_paths[e1][e3] = e1_paths

				if e3 == e1: # exists mo.hb path from e1 to e1 (via e2 ie. e1 --mo--> e2 --hb--> e1)
					cycles += self.hb_paths[e2][e3]
					
		return cycles

	def mo_hb_rfinv_cycles(self):
		cycles = []

		for (e1,e2) in self.rfinv_edges:
			if not e2 in self.mo_hb_paths: # no mo.hb path from e2 ==> no mo.hb.rfinv cycle ends on e2
				continue

			if not e1 in self.mo_hb_paths[e2]: # no mo.hb path from e2 to e1 ==> no mo.hb.rfinv cycle on e2 via e1
				continue

			# exists mo.hb.rfinv path from e2 to e2 (via e1 ie. e2 --mo.hb--> e1 --rfinv--> e2)
			cycles += self.mo_hb_paths[e2][e1]

		return cycles

	def mo_rf_hb_rfinv_cycles(self):
		cycles = []

		for (e1,e2) in self.mo_edges:
			if not e2 in self.rf_hb_paths: # no rf.hb path from e2 ==> no mo.rf.hb path from e1 via e2
				continue

			# exists mo.rf.hb path from e1

			for (e3,e4) in self.rfinv_edges:
				if not e3 in self.rf_hb_paths[e2] or not e4 == e1:
					continue

				# exists mo.rf.hb.rfinv path from e1 to e1 (via e2,e3 ie. e1 --mo--> e2 --rf.hb--> e3 --rfinv--> e1)
				e1_cycles = [ ([e1] + p) for p in self.rf_hb_paths[e2][e3] ]
				cycles += e1_cycles
	
	def get(self):
		return self.weak_cycles