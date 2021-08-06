class weak_fensying:
	def __init__(self, cycles, hb_edges, mo_edges, rf_edges, fr_edges, rf1_edges):
		self.candidate_cycles = self.check_for_weak_compositions(cycles, hb_edges, mo_edges, rf_edges, fr_edges, rf1_edges)

	def check_for_weak_compositions(self, cycles, hb_edges, mo_edges, rf_edges, fr_edges, rf1_edges):
		candidate_cycles = []
		for cycle in cycles:
			edges_in_cycle = []
			
			# first separate out all the edges in the cycle
			for i in range(len(cycle)):
				if i == len(cycle)-1:
					e0 = cycle[i]
					e1 = cycle[0]
					edges_in_cycle.append((e0, e1))
					break
				e0 = cycle[i]
				e1 = cycle[i+1]
				edges_in_cycle.append((e0, e1))

			# fix
			# if edge in fr_edges:
			# 	continue
			
			size = len(edges_in_cycle)

			# check for composition 1- all hb
			check1 = 0
			edge_generator = self.circular_list(edges_in_cycle, 0)
			for i in range(size):
				edge = next(edge_generator)
				if edge not in hb_edges:
					check1 = 0
					break
				else:
					check1 += 1
			if check1 == size:
				candidate_cycles.append(cycle)
				continue
			
			# check for composition 2- rf -> hb
			flag = 0
			for i in range(size):
				check2 = 0
				edge_generator = self.circular_list(edges_in_cycle, i)
				rf_over = 0
				for j in range(size):
					edge = next(edge_generator)
					if rf_over == 0 and edge in rf_edges:
						check2 += 1
					elif edge in hb_edges:
						rf_over = 1
						check2 += 1
				if check2 == size:
					candidate_cycles.append(cycle)
					flag = 1
					break
			if flag == 1:
				continue
			
			# check for composition 3- mo -> hb
			flag = 0
			for i in range(size):
				check3 = 0
				edge_generator = self.circular_list(edges_in_cycle, i)
				mo_over = 0
				for j in range(size):
					edge = next(edge_generator)
					if mo_over == 0 and edge in mo_edges:
						check3 += 1
					elif edge in hb_edges:
						mo_over = 1
						check3 += 1
				if check3 == size:
					candidate_cycles.append(cycle)
					flag = 1
					break
			if flag == 1:
				continue

			# check for composition 4- mo -> rf -> hb
			flag = 0
			for i in range(size):
				check4 = 0
				edge_generator = self.circular_list(edges_in_cycle, i)
				mo_over = 0
				rf_over = 0
				for j in range(size):
					edge = next(edge_generator)
					if mo_over == 0 and edge in mo_edges:
						check4 += 1
					elif rf_over == 0 and edge in rf_edges:
						mo_over = 1
						check4 += 1
					elif edge in hb_edges:
						rf_over = 1
						check4 += 1
				if check4 == size:
					candidate_cycles.append(cycle)
					flag = 1
					break
			if flag == 1:
				continue

			# check for composition 5- mo -> hb -> rf-1
			flag = 0
			for i in range(size):
				check5 = 0
				edge_generator = self.circular_list(edges_in_cycle, i)
				mo_over = 0
				hb_over = 0
				for j in range(size):
					edge = next(edge_generator)
					if mo_over == 0 and edge in mo_edges:
						check5 += 1
					elif hb_over == 0 and edge in hb_edges:
						mo_over = 1
						check5 += 1
					elif edge in rf1_edges:
						hb_over = 1
						check5 += 1
				if check5 == size:
					candidate_cycles.append(cycle)
					flag = 1
					break
			if flag == 1:
				continue

			# check for composition 6- mo -> rf -> hb -> rf-1
			flag = 0
			for i in range(size):
				check6 = 0
				edge_generator = self.circular_list(edges_in_cycle, i)
				mo_over = 0
				rf_over = 0
				hb_over = 0
				for j in range(size):
					edge = next(edge_generator)
					if mo_over == 0 and edge in mo_edges:
						check6 += 1
					elif rf_over == 0 and edge in rf_edges:
						mo_over = 1
						check6 += 1
					elif hb_over == 0 and edge in hb_edges:
						rf_over = 1
						check6 += 1
					elif edge in rf1_edges:
						hb_over = 1
						check6 += 1
				if check6 == size:
					candidate_cycles.append(cycle)
					flag = 1
					break
			if flag == 1:
				continue
		
		return candidate_cycles

	def circular_list(self, cycle, index):
		cycle_size = len(cycle)
		while True:
			for i in range(len(cycle)):
				connection = cycle[(i + index) % cycle_size]
				yield connection
	
	def get(self):
		return self.candidate_cycles