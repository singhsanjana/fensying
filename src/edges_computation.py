from os import write
from constants import *

class edges_computation:
	def __init__(self, reads, writes, all_events_by_thread, fences_thread, mo_edges, so_edges):
		self.reads = reads
		self.writes = writes

		self.matched_reads = []					# global variable used in below recursive function to avoid nested lists
		
		# lists of edges
		self.swdob_edges = []
		self.mo_edges    = mo_edges
		self.so_edges    = so_edges
		self.fr_edges    = []

		self.compute_all_edges(all_events_by_thread, fences_thread)
	
	def compute_all_edges(self, all_events_by_thread, fences_thread):
		self.sw_fences(all_events_by_thread, fences_thread)
		self.so_fences_from_mofr(fences_thread)
	
	def get(self):
		self.swdob_edges = list(set(self.swdob_edges))
		self.so_edges    = list(set(self.so_edges))
		self.fr_edges    = list(set(self.fr_edges))
		return self.swdob_edges, self.fr_edges, self.so_edges

	def sw_fences(self, all_events_by_thread, fences_thread):
		for wr2_index in range(len(self.reads)): # self.reads includes reads + the fence above and below the reads
			wr2 = self.reads[wr2_index] # read at indes wr2_index

			if (type(wr2) is list): # is a read event, not a fence
				wr1_index = next(i for i,v in enumerate(self.writes)
						if (type(v) is list) and (v[S_NO] == wr2[RF])) # index of write that wr2 reads from
				wr1 = self.writes[wr1_index]  # write at index wr1_index

				wr1_thread = wr1[T_NO] -1
				wr2_thread = wr2[T_NO] -1
				
				if wr1_thread == wr2_thread:
					continue

				if wr1[TYPE] == INIT: 
					# INIT write's MO = relaxed, hence no sw with INIT
					# also, INIT write has no fence before and after
					continue

				# sw edges
				f1 = self.writes[wr1_index-1] # fence before the write wr1
				f1_index = fences_thread[wr1_thread].index(f1)
				f2 = self.reads[wr2_index+1]  # fence after the read wr2
				f2_index = fences_thread[wr2_thread].index(f2)

				add_ef_edges = True
				for f1_in_sb_index in range(0, f1_index+1): # fences in thread of f1, po before f1 (including f1)

					if wr2[MO] in strong_read_models: 
						# fence-event sw added when a strong read reads-from a write with a fence above it
						edge_FE = (fences_thread[wr1_thread][f1_in_sb_index], wr2[S_NO]) 
						# rule swFE
						self.swdob_edges.append(edge_FE)
						if wr2[MO] == SEQ_CST:
							self.so_edges.append(edge_FE)

					for f2_in_sb_index in range(f2_index, len(fences_thread[wr2_thread])):
						if add_ef_edges: # true only once to avoid redundant computation
							if wr1[MO] in strong_write_models: 
								# event-fence sw added when read with a fence below it reads from a strong write 
								edge_EF = (wr1[S_NO], fences_thread[wr2_thread][f2_in_sb_index])
								# rule swEF
								self.swdob_edges.append(edge_EF)
								if wr1[MO] == SEQ_CST:
									self.so_edges.append(edge_EF)

						# fence-fence sw when a read with fence below it reads from a write with a fence above it
						edge_FF = (fences_thread[wr1_thread][f1_in_sb_index], fences_thread[wr2_thread][f2_in_sb_index])
						# rule swFF
						self.swdob_edges.append(edge_FF)
						self.so_edges.append(edge_FF)

					add_ef_edges = False # add only in the first run

				# dob edges
				if wr1[MO] in strong_write_models or wr2[MO] in strong_read_models:
					continue

				wr1_index_in_thread = all_events_by_thread[wr1[T_NO]-1].index(wr1)
				for i in range(wr1_index_in_thread, 0, -1):
					wr = all_events_by_thread[wr1[T_NO]-1][i]
					if wr[TYPE] == WRITE and wr[MO] != RLX and wr[ADDR] == wr1[ADDR]:
						for f2_in_sb_index in range(f2_index, len(fences_thread[wr2_thread])):
							edge_EEF = (wr[S_NO], fences_thread[wr2_thread][f2_in_sb_index])
							# rule dobEF
							self.swdob_edges.append(edge_EEF)
							if wr[MO] == SEQ_CST:
								self.so_edges.append(edge_EEF)

						break




	def so_fences_from_mofr(self, fences_thread):
		# mo related rules
		for mo in self.mo_edges:
			w1_index = next(i for i,v in enumerate(self.writes) if type(v) is list and v[S_NO] == mo[0])
			w2_index = next(i for i,v in enumerate(self.writes) if type(v) is list and v[S_NO] == mo[1])

			# w1 --mo--> w2
			w1 = self.writes[w1_index] 
			w2 = self.writes[w2_index]

			w1_thread = w1[T_NO] - 1
			w2_thread = w2[T_NO] - 1

			
			# so from fr
			for read_index in range(len(self.reads)):
				read = self.reads[read_index]
				if type(read) is list and read[RF] == w1[S_NO]:
					# read --rf_inv--> w1 --mo--> w2 ==> read --fr--> w2
					if read[T_NO] - 1 == w2_thread:		
						continue
					if read[S_NO] != w2[S_NO]: # S_NO could be same in case of type(w2) = rmw
						self.fr_edges.append((read[S_NO], w2[S_NO]))
					if read[MO] == SEQ_CST and w2[MO] == SEQ_CST:
						edge_EE = (read[S_NO], w2[S_NO])
						self.so_edges.append(edge_EE)

					read_thread = read[T_NO] - 1
					f1 = self.reads[read_index - 1] # fence before read
					f1_index = fences_thread[read_thread].index(f1)

					f2 = self.writes[w2_index + 1] # fence after write
					f2_index = fences_thread[w2_thread].index(f2)

					add_ef_edges = True
					for f1_in_sb_index in range(0, f1_index+1):
						if w2[MO] == SEQ_CST:
							edge_FE = (fences_thread[read_thread][f1_in_sb_index], w2[S_NO])
							self.so_edges.append(edge_FE)

						for f2_in_sb_index in range(f2_index, len(fences_thread[w2_thread])):
							if add_ef_edges:
								if read[MO] == SEQ_CST:
									edge_EF = (read[S_NO], fences_thread[w2_thread][f2_in_sb_index])
									self.so_edges.append(edge_EF)
							
							edge_FF = (fences_thread[read_thread][f1_in_sb_index], fences_thread[w2_thread][f2_in_sb_index])
							self.so_edges.append(edge_FF)
							
							add_ef_edges = False # add only in the first run

			if w1[TYPE] == INIT:
				# no fences before INIT hence no so from mo involving INIT
				continue

			if w1_thread == w2_thread:
				continue
			
			f1 = self.writes[w1_index - 1] # fence before w1
			f1_index = fences_thread[w1_thread].index(f1)
			f2 = self.writes[w2_index + 1] # fence after w2
			f2_index = fences_thread[w2_thread].index(f2)

			# so from mo
			add_ef_edges = True
			for f1_in_sb_index in range(0, f1_index+1): # fences po before f1 (including f1)
				if w2[MO] == SEQ_CST:
					edge_FE = (fences_thread[w1_thread][f1_in_sb_index], w2[S_NO])
					self.so_edges.append(edge_FE)
				
				for f2_in_sb_index in range(f2_index, len(fences_thread[w2_thread])):
					if add_ef_edges:
						if w1[MO] == SEQ_CST:
							edge_EF = (w1[S_NO], fences_thread[w2_thread][f2_in_sb_index])
							self.so_edges.append(edge_EF)
					
					edge_FF = (fences_thread[w1_thread][f1_in_sb_index], fences_thread[w2_thread][f2_in_sb_index])
					self.so_edges.append(edge_FF)
					