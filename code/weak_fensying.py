from cycle import Cycles
import time

class weak_fensying:
	def __init__(self, hb_edges, mo_edges, rf_edges, rf1_edges):
		self.relaxed_cycles = []
		self.hb_edges = hb_edges
		self.mo_edges = mo_edges
		self.rf_edges = rf_edges
		self.rf1_edges = rf1_edges

	def compute_cycles(self):
		rf_permutations = []
		for i in range(len(self.rf_edges)):
			temp_list = self.rf_edges[:]							# to pass by value and not by reference/object-reference
			del temp_list[i]
			temp_list.append(self.rf1_edges[i])
			rf_permutations.append(temp_list)
		
		for p in rf_permutations:
			self.relaxed_cycles += Cycles(self.hb_edges + self.mo_edges + p)
		
		self.relaxed_cycles = [list(item) for item in set(tuple(row) for row in self.relaxed_cycles)]
		# print("relaxed_cycles=",self.relaxed_cycles)

		if len(self.relaxed_cycles) == 0:
			return False
		return True

	def check_for_weak_compositions(self):
		candidate_cycles = []
		print("no of rlx cycles to check=", len(self.relaxed_cycles))
		st = time.time()
		for cycle in self.relaxed_cycles:
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

			size = len(edges_in_cycle)

			# check for composition 1- all hb
			check1 = 0
			edge_generator = self.circular_list(edges_in_cycle, 0)
			for i in range(size):
				edge = next(edge_generator)
				if edge not in self.hb_edges:
					check1 = 0
					break
				else:
					check1 += 1
			if check1 == size:
				candidate_cycles.append(cycle)
				# print(cycle,"from all hb")
				continue
			
			# check for composition 2- rf -> hb
			flag = 0
			for i in range(size):
				check2 = 0
				edge_generator = self.circular_list(edges_in_cycle, i)
				rf_over = 0
				for j in range(size):
					edge = next(edge_generator)
					if rf_over == 0 and edge in self.rf_edges:
						check2 += 1
					elif edge in self.hb_edges:
						rf_over = 1
						check2 += 1
				if check2 == size:
					candidate_cycles.append(cycle)
					# print(cycle,"from all 2- rf -> hb")
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
					if mo_over == 0 and edge in self.mo_edges:
						check3 += 1
					elif edge in self.hb_edges:
						mo_over = 1
						check3 += 1
				if check3 == size:
					candidate_cycles.append(cycle)
					# print(cycle,"from all 3- mo -> hb")
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
				rf_over = 1
				for j in range(size):
					edge = next(edge_generator)
					if mo_over == 0 and edge in self.mo_edges:
						check4 += 1
						rf_over = 0
					elif rf_over == 0 and edge in self.rf_edges:
						mo_over = 1
						check4 += 1
					elif edge in self.hb_edges:
						rf_over = 1
						check4 += 1
				if check4 == size:
					candidate_cycles.append(cycle)
					# print(cycle,"from all 4- mo -> rf -> hb")
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
				hb_over = 1
				for j in range(size):
					edge = next(edge_generator)
					if mo_over == 0 and edge in self.mo_edges:
						check5 += 1
						hb_over = 0
					elif hb_over == 0 and edge in self.hb_edges:
						mo_over = 1
						check5 += 1
					elif edge in self.rf1_edges:
						hb_over = 1
						check5 += 1
				if check5 == size:
					candidate_cycles.append(cycle)
					# print(cycle,"from all 5- mo -> hb -> rf-1")
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
				rf_over = 1
				hb_over = 1
				for j in range(size):
					edge = next(edge_generator)
					if mo_over == 0 and edge in self.mo_edges:
						check6 += 1
						rf_over = 0
					elif rf_over == 0 and edge in self.rf_edges:
						mo_over = 1
						check6 += 1
						hb_over = 0
					elif hb_over == 0 and edge in self.hb_edges:
						rf_over = 1
						check6 += 1
					elif edge in self.rf1_edges:
						hb_over = 1
						check6 += 1
				if check6 == size:
					candidate_cycles.append(cycle)
					# print(cycle,"from all  6- mo -> rf -> hb -> rf-1")
					flag = 1
					break
			if flag == 1:
				continue
		
		en = time.time()
		print("time taken to check=",round(en-st, 2))
		return candidate_cycles

	def circular_list(self, cycle, index):
		cycle_size = len(cycle)
		while True:
			for i in range(len(cycle)):
				connection = cycle[(i + index) % cycle_size]
				yield connection
	
	def get(self):
		return self.candidate_cycles