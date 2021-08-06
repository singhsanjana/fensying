from constants import *
from mo import mo

class edges_computation:
	def __init__(self, reads, writes, fences_thread, hb_edges, sc_edges):
		self.reads = reads
		self.writes = writes
		self.fences_thread = fences_thread
		
		# lists of edges
		self.hb_edges = hb_edges
		self.mo_edges = []
		self.rf_edges = []
		self.fr_edges = []
		self.rf1_edges = []
		self.sc_edges = sc_edges 			# list of hb+mo+rf+fr edges with only sc events

		self.compute_all_edges()
	
	def compute_all_edges(self):
		self.hbrf()
		compute_mo = mo(self.writes, self.reads, self.hb_edges, self.sc_edges)
		self.mo_edges, self.sc_edges = compute_mo.get()
		self.fr()
	
	def get(self):
		self.hb_edges = list(set(self.hb_edges))
		self.mo_edges = list(set(self.mo_edges))
		self.rf_edges = list(set(self.rf_edges))
		self.fr_edges = list(set(self.fr_edges))
		self.rf1_edges = list(set(self.rf1_edges))
		self.sc_edges = list(set(self.sc_edges))
		return self.hb_edges, self.mo_edges, self.rf_edges, self.fr_edges, self.rf1_edges, self.sc_edges

	def hbrf(self):
		# rf
		for wr2_index in range(len(self.reads)):
			wr2 = self.reads[wr2_index]

			if (type(wr2) is list):
				wr1_index = next(i for i,v in enumerate(self.writes)
						if (type(v) is list) and (v[S_NO] == wr2[RF]))
				wr1 = self.writes[wr1_index]

				self.rf_edges.append((wr1[S_NO], wr2[S_NO]))
				self.rf1_edges.append((wr2[S_NO], wr1[S_NO]))
				if wr1[MO] == SEQ_CST and wr2[MO] == SEQ_CST:
					self.sc_edges.append((wr1[S_NO], wr2[S_NO]))

				wr1_thread = wr1[T_NO] -1
				wr2_thread = wr2[T_NO] -1

				f1 = self.writes[wr1_index-1]
				f1_index = self.fences_thread[wr1_thread].index(f1)
				f2 = self.reads[wr2_index+1]
				f2_index = self.fences_thread[wr2_thread].index(f2)
				for f1_in_sb_index in range(0, f1_index+1):
					for f2_in_sb_index in range(f2_index, len(self.fences_thread[wr2_thread])):
						# rule soFF for w --rf--> r
						# self.so_edges.append((self.fences_thread[wr1_thread][f1_in_sb_index], self.fences_thread[wr2_thread][f2_in_sb_index]))
						# rule swdobFF for w --rf--> r
						self.hb_edges.append((self.fences_thread[wr1_thread][f1_in_sb_index], self.fences_thread[wr2_thread][f2_in_sb_index]))
						self.sc_edges.append((self.fences_thread[wr1_thread][f1_in_sb_index], self.fences_thread[wr2_thread][f2_in_sb_index]))

				if wr1[MO] in write_models:
					f = self.reads[wr2_index+1]
					f_index = self.fences_thread[wr2_thread].index(f)

					for f_in_sb_index in range(f_index, len(self.fences_thread[wr2_thread])):
						edge = (wr1[S_NO], self.fences_thread[wr2_thread][f_in_sb_index])
						# rule swEF
						self.hb_edges.append(edge)
							
						if wr1[MO] == SEQ_CST:
						# 	# rule soEF for w --rf--> r
						# 	self.so_edges.append(edge)
							self.sc_edges.append(edge)

				if wr2[MO] in read_models:
					f = self.writes[wr1_index-1]
					f_index = self.fences_thread[wr1_thread].index(f)

					for f_in_sb_index in range(0, f_index+1):
						edge = (self.fences_thread[wr1_thread][f_in_sb_index], wr2[S_NO])
						# rule swdobFE
						self.hb_edges.append(edge)

						if wr2[MO] == SEQ_CST:
						# 	# rule soFE for w --rf--> r
						# 	self.so_edges.append(edge)
							self.sc_edges.append(edge)
				
				if wr1[MO] in write_models and wr2[MO] in read_models:
					# rule sw
					self.hb_edges.append((wr1[S_NO], wr2[S_NO]))
	
					if wr1[MO] == SEQ_CST and wr2[MO] == SEQ_CST:
					# 	# rule sorf
					# 	self.so_edges.append((wr1[S_NO], wr2[S_NO]))
						self.sc_edges.append((wr1[S_NO], wr2[S_NO]))

				for wr3_index in range(wr1_index):
					wr3 = self.writes[wr3_index]
					if (type(wr3) is list) and (wr3[T_NO] == wr1[T_NO]) and (wr3[MO] in write_models):
						f = self.reads[wr2_index+1]
						f_index = self.fences_thread[wr2_thread].index(f)

						for f_in_sb_index in range(f_index, len(self.fences_thread[wr2_thread])):
							# rule dobEF
							self.hb_edges.append((wr3[S_NO], self.fences_thread[wr2_thread][f_in_sb_index]))
							if wr3[MO] == SEQ_CST:
								self.sc_edges.append((wr3[S_NO], self.fences_thread[wr2_thread][f_in_sb_index]))

						if wr2[MO] in read_models:
							# rule dob
							self.hb_edges.append((wr3[S_NO], wr2[S_NO]))
							if wr3[MO] == SEQ_CST and wr2[MO] == SEQ_CST:
								self.sc_edges.append((wr3[S_NO], wr2[S_NO]))
				
				# 	wr3_thread = wr3[T_NO] -1

				# 	f1 = self.reads[wr2_index-1]
				# 	f1_index = self.fences_thread[wr2_thread].index(f1)
				# 	f2 = self.writes[wr3_index+1]
				# 	f2_index = self.fences_thread[wr3_thread].index(f2)
				# 	for f1_in_sb_index in range(0, f1_index+1):
				# 		for f2_in_sb_index in range(f2_index, len(self.fences_thread[wr3_thread])):
				# 			# rule soFF for r --fr--> w
				# 			self.so_edges.append((self.fences_thread[wr2_thread][f1_in_sb_index], self.fences_thread[wr3_thread][f2_in_sb_index]))
				# 			# rule wsFF for r --fr--> w
				# 			self.hb_edges.append((self.fences_thread[wr2_thread][f1_in_sb_index], self.fences_thread[wr3_thread][f2_in_sb_index]))

				# 	if wr2[MO] == SEQ_CST:
				# 		f = self.writes[wr3_index+1]
				# 		f_index = self.fences_thread[wr3_thread].index(f)

				# 		for f_in_sb_index in range(f_index, len(self.fences_thread[wr3_thread])):
				# 			# rule soEF for r --fr--> w
				# 			self.so_edges.append((wr2[S_NO], self.fences_thread[wr3_thread][f_in_sb_index]))
					
				# 	if wr3[MO] == SEQ_CST:
				# 		f = self.reads[wr2_index-1]
				# 		f_index = self.fences_thread[wr2_thread].index(f)

				# 		for f_in_sb_index in range(0, f_index+1):
				# 			# rule soFE for r --fr--> w
				# 			self.so_edges.append((self.fences_thread[wr2_thread][f_in_sb_index], wr3[S_NO]))
					
				# 	if wr2[MO] == SEQ_CST and wr3[MO] == SEQ_CST:
				# 		# rule sofr
				# 		self.so_edges.append((wr2[S_NO], wr3[S_NO]))
					
				# 	wr4_SNOs = []
				# 	wr5_SNOs = []
				# 	for wr5_index in range(wr3_index+1, len(self.writes)):
				# 		wr5 = self.writes[wr5_index]
				# 		if (type(wr5) is list) and (wr5[T_NO] == wr3[T_NO]) and (wr5[MO] in write_models):
				# 			wr5_SNOs.append(wr5[S_NO])
				# 			f = self.reads[wr2_index-1]
				# 			f_index = self.fences_thread[wr2_thread].index(f)
				# 			for f_in_sb_index in range(0, f_index+1):
				# 				# rule wsFE
				# 				self.hb_edges.append((self.fences_thread[wr2_thread][f_in_sb_index], wr5[S_NO]))

				# 	for wr4_index in range(wr2_index):
				# 		wr4 = self.reads[wr4_index]
				# 		if (type(wr4) is list) and (wr4[T_NO] == wr2[T_NO]) and (wr4[MO] in read_models):
				# 			wr4_SNOs.append(wr4[S_NO])
				# 			f = self.writes[wr3_index+1]
				# 			f_index = self.fences_thread[wr3_thread].index(f)
				# 			for f_in_sb_index in range(f_index, len(self.fences_thread[wr3_thread])):
				# 				# rule wsEF
				# 				self.hb_edges.append((wr4[S_NO], self.fences_thread[wr3_thread][f_in_sb_index]))
					
				# 	for wr4_SNO in wr4_SNOs:
				# 		for wr5_SNO in wr5_SNOs:
				# 			# rule ws
				# 			self.hb_edges.append((wr4_SNO, wr5_SNO))
		
	def fr(self):
		for wr2_index in range(len(self.reads)):
			wr2 = self.reads[wr2_index]

			if (type(wr2) is list):
				wr1_index = next(i for i,v in enumerate(self.writes)
						if (type(v) is list) and (v[S_NO] == wr2[RF]))
				wr1 = self.writes[wr1_index]

				wr3_SNOs = []
				try:
					wr3_generator = (v[1] for i,v in enumerate(self.mo_edges)
						if v[0] == wr1[S_NO])
					for wr3_value in wr3_generator:
						wr3_SNOs.append(wr3_value)
						self.fr_edges.append((wr2[S_NO], wr3_value))
				except: continue

				if wr2[MO] == SEQ_CST:
					for wr3_SNO in wr3_SNOs:
						wr3 = next(v for i,v in enumerate(self.writes)
							if (type(v) is list) and (v[S_NO] == wr3_SNO))
						if wr3[MO] == SEQ_CST:
							self.sc_edges.append((wr2[S_NO], wr3[S_NO]))

	# unused
	def mo(self):
		# mo
		for mo_edge in self.mo_edges:
			try: wr1_index = next(i for i,v in enumerate(self.writes) 
					if type(v) is list and v[S_NO] == mo_edge[0])
			except: continue
			wr1 = self.writes[wr1_index]

			wr2_index = next(i for i,v in enumerate(self.writes)
					if (type(v) is list) and (v[S_NO] == mo_edge[1]))
			wr2 = self.writes[wr2_index]

			wr1_thread = wr1[T_NO] -1
			wr2_thread = wr2[T_NO] -1

			f1 = self.writes[wr1_index-1]
			f1_index = self.fences_thread[wr1_thread].index(f1)
			f2 = self.writes[wr2_index+1]
			f2_index = self.fences_thread[wr2_thread].index(f2)
			for f1_in_sb_index in range(0, f1_index+1):
				for f2_in_sb_index in range(f2_index, len(self.fences_thread[wr2_thread])):
					# rule soFF for w --mo--> w
					self.so_edges.append((self.fences_thread[wr1_thread][f1_in_sb_index], self.fences_thread[wr2_thread][f2_in_sb_index]))

			if wr1[MO] == SEQ_CST:
				f = self.writes[wr2_index+1]
				f_index = self.fences_thread[wr2_thread].index(f)

				for f_in_sb_index in range(f_index, len(self.fences_thread[wr2_thread])):
					# rule soEF for w1 --mo--> w2
					self.so_edges.append((wr1[S_NO], self.fences_thread[wr2_thread][f_in_sb_index]))

			if wr2[MO] == SEQ_CST:
				f = self.writes[wr1_index-1]
				f_index = self.fences_thread[wr1_thread].index(f)

				for f_in_sb_index in range(0, f_index+1):
					# rule soFE for w1 --mo--> w2
					self.so_edges.append((self.fences_thread[wr1_thread][f_in_sb_index], wr2[S_NO]))