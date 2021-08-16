from constants import *
from mo import mo

class edges_computation:
	def __init__(self, reads, writes, fences_thread, hb_edges, mo_edges, so_edges):
		self.reads = reads
		self.writes = writes
		self.fences_thread = fences_thread

		self.matched_reads = []					# global variable used in below recursive function to avoid nested lists
		
		# lists of edges
		self.hb_edges = hb_edges
		self.mo_edges = mo_edges
		self.so_edges = so_edges
		self.rf_edges = []
		self.fr_edges = []
		self.rf1_edges = []

		self.compute_all_edges()
	
	def compute_all_edges(self):
		self.hbrf()
		self.mofr()
	
	def get(self):
		self.hb_edges = list(set(self.hb_edges))
		self.rf_edges = list(set(self.rf_edges))
		self.fr_edges = list(set(self.fr_edges))
		self.rf1_edges = list(set(self.rf1_edges))
		self.so_edges = list(set(self.so_edges))
		return self.hb_edges, self.rf_edges, self.fr_edges, self.rf1_edges, self.so_edges

	def hbrf(self):
		# rf related rules
		for wr2_index in range(len(self.reads)):
			wr2 = self.reads[wr2_index]

			if (type(wr2) is list):
				wr1_index = next(i for i,v in enumerate(self.writes)
						if (type(v) is list) and (v[S_NO] == wr2[RF]))
				wr1 = self.writes[wr1_index]

				wr1_thread = wr1[T_NO] -1
				wr2_thread = wr2[T_NO] -1

				f1 = self.writes[wr1_index-1]
				f1_index = self.fences_thread[wr1_thread].index(f1)
				f2 = self.reads[wr2_index+1]
				f2_index = self.fences_thread[wr2_thread].index(f2)

				# wr1 --rf--> wr2
				self.rf_edges.append((wr1[S_NO], wr2[S_NO]))
				self.rf1_edges.append((wr2[S_NO], wr1[S_NO]))
				if wr1[MO] == SEQ_CST:
					if wr2[MO] == SEQ_CST:
						self.so_edges.append((wr1[S_NO], wr2[S_NO]))

				add_ef_edges = True
				for f1_in_sb_index in range(0, f1_index+1):

					if wr2[MO] in read_models:
						edge_FE = (self.fences_thread[wr1_thread][f1_in_sb_index], wr2[S_NO])
						# rule swFE
						self.hb_edges.append(edge_FE)
						if wr2[MO] == SEQ_CST:
							self.so_edges.append(edge_FE)

					for f2_in_sb_index in range(f2_index, len(self.fences_thread[wr2_thread])):
						if add_ef_edges:
							if wr1[MO] in write_models:
								edge_EF = (wr1[S_NO], self.fences_thread[wr2_thread][f2_in_sb_index])
								# rule swEF
								self.hb_edges.append(edge_EF)
								if wr1[MO] == SEQ_CST:
									self.so_edges.append(edge_EF)

						edge_FF = (self.fences_thread[wr1_thread][f1_in_sb_index], self.fences_thread[wr2_thread][f2_in_sb_index])
						# rule swFF for w --rf--> r
						self.hb_edges.append(edge_FF)
						self.so_edges.append(edge_FF)

					add_ef_edges = False # add only in the first run

		# release sequence	related rules
		for i in range(1, len(self.writes), 3):
			write = self.writes[i]
			if type(write) is list:
				if write[MO] in write_models:
					self.matched_reads = []
					self.next_release_sequence(i+3, write)
					self.matched_reads = [self.matched_reads[i] for i in range(len(self.matched_reads)) if i == 0 or self.matched_reads[i] != self.matched_reads[i-1]]					# remove duplicate values

					# make the relations for write (release sequence head) --hb--> read (RF of end of release sequence write)
					f1 = self.writes[self.writes.index(write) - 1]
					write_rel_thread = write[T_NO] - 1
					f1_index = self.fences_thread[write_rel_thread].index(f1)
					for read in self.matched_reads:
						f2 = self.reads[self.reads.index(read) + 1]
						r_thread = read[T_NO] -1
						f2_index = self.fences_thread[r_thread].index(f2)
						
						add_fe_edges = True
						for f1_in_sb_index in range(0, f1_index+1):
							if read[MO] == SEQ_CST:
								edge_FE = (self.fences_thread[write_rel_thread][f1_in_sb_index], read[S_NO])
								self.so_edges.append(edge_FE)

							for f2_in_sb_index in range(f2_index, len(self.fences_thread[r_thread])):
								if add_fe_edges:
									edge_EF = (write[S_NO], self.fences_thread[r_thread][f2_in_sb_index])
									# rule dobEF
									self.hb_edges.append(edge_EF)
									if write[MO] == SEQ_CST:
										self.so_edges.append(edge_EF)

								edge_FF = (self.fences_thread[write_rel_thread][f1_in_sb_index], self.fences_thread[r_thread][f2_in_sb_index])
								self.so_edges.append(edge_FF)

							add_fe_edges = False # add only in the first run

	def next_release_sequence(self, i, write_rel):
		if i >= len(self.writes):
			return

		write_curr = self.writes[i]
		if type(write_curr) is list:
			# sb-release-sequence: write_rel --sb--> write --rf--> read => write_rel --dob--> read
			for j in range(i, len(self.writes), 3):
				write = self.writes[j]
				if type(write) is list:
					if write[T_NO] == write_rel[T_NO] and write[ADDR] == write_rel[ADDR]:
						for k in range(len(self.reads)):
							read = self.reads[k]
							if (type(read) is list) and (read[RF] == write[S_NO]):
								self.matched_reads.append(read)
						self.next_release_sequence(j+3, write_rel)
					else:
						break
			
			# mo-release-sequence: write_rel --mo--> write --rf--> read => write_rel --dob--> read
			for mo in self.mo_edges:
				if mo[0] == write_curr[S_NO]:
					j = next(k for k,v in enumerate(self.writes) if (type(v) is list) and (v[S_NO] == mo[1]))
					write = self.writes[j]
					# release sequence condition for ithb
					if (write[T_NO] == write_curr[T_NO]) or (write[MO] in write_models):
						for k in self.reads:
							read = self.reads[k]
							if (type(read) is list) and (read[RF] == write[S_NO]):
								self.matched_reads.append(read)
						self.next_release_sequence(j+3, write_rel)

		return

	def mofr(self):
		# mo related rules
		for mo in self.mo_edges:
			w1_index = next(i for i,v in enumerate(self.writes) if type(v) is list and v[S_NO] == mo[0])
			w2_index = next(i for i,v in enumerate(self.writes) if type(v) is list and v[S_NO] == mo[1])

			w1 = self.writes[w1_index]
			w2 = self.writes[w2_index]

			f1 = self.writes[w1_index - 1]
			w1_thread = w1[T_NO] - 1
			f1_index = self.fences_thread[w1_thread].index(f1)

			f2 = self.writes[w2_index + 1]
			w2_thread = w2[T_NO] - 1
			f2_index = self.fences_thread[w2_thread].index(f2)

			add_ef_edges = True
			for f1_in_sb_index in range(0, f1_index+1):
				if w2[MO] == SEQ_CST:
					edge_FE = (self.fences_thread[w1_thread][f1_in_sb_index], w2[S_NO])
					self.so_edges.append(edge_FE)
				
				for f2_in_sb_index in range(f2_index, len(self.fences_thread[w2_thread])):
					if add_ef_edges:
						if w1[MO] == SEQ_CST:
							edge_EF = (w1[S_NO], self.fences_thread[w2_thread][f2_in_sb_index])
							self.so_edges.append(edge_EF)
					
					edge_FF = (self.fences_thread[w1_thread][f1_in_sb_index], self.fences_thread[w2_thread][f2_in_sb_index])
					self.so_edges.append(edge_FF)
			
			for read_index in range(len(self.reads)):
				read = self.reads[read_index]
				if type(read) is list and read[RF] == w1[S_NO]:
					self.fr_edges.append((read[S_NO], w2[S_NO]))

					f1 = self.reads[read_index - 1]
					read_thread = read[T_NO] - 1
					f1_index = self.fences_thread[read_thread].index(f1)

					f2 = self.writes[w2_index + 1]
					f2_index = self.fences_thread[w2_thread].index(f2)

					add_ef_edges = True
					for f1_in_sb_index in range(0, f1_index+1):
						if w2[MO] == SEQ_CST:
							edge_FE = (self.fences_thread[read_thread][f1_in_sb_index], w2[S_NO])
							self.so_edges.append(edge_FE)

							for f2_in_sb_index in range(f2_index, len(self.fences_thread[w2_thread])):
								if add_ef_edges:
									if read[MO] == SEQ_CST:
										edge_EF = (read[S_NO], self.fences_thread[w2_thread][f2_in_sb_index])
										self.so_edges.append(edge_EF)
								
								edge_FF = (self.fences_thread[read_thread][f1_in_sb_index], self.fences_thread[w2_thread][f2_in_sb_index])
								self.so_edges.append(edge_FF)
							
							add_ef_edges = False # add only in the first run