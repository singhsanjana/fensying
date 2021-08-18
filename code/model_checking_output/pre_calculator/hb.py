# --------------------------------------------------------

# --------------------------------------------------------

from model_checking_output.pre_calculator.graph import Graph
from constants import *

class hb:

	def __init__(self, trace, mo_edges, initial_hb_edges, so_edges, hb_graph, mat_size, writes, reads, calc_all):
		# print(trace)
		self.hb_edges = initial_hb_edges									# list of all sb+sw+dob edges between instructions
		self.mo_edges = mo_edges
		self.so_edges = so_edges
		self.writes = writes
		self.reads = reads

		self.mat = hb_graph
		self.size = mat_size

		self.matched_reads = []													# global variable used in below recursive function to avoid nested lists

		if calc_all:
			self.sb(trace)
			self.sw(trace)
		self.dob(trace)

		self.hb_edges = list(set(self.hb_edges))					# remove duplicates
		self.complete_matrix()

	def get(self):
		return self.mat, self.hb_edges, self.so_edges

	# adds all transitive relations
	def complete_matrix(self):

		temp = Graph(self.size)
		temp.adjMatrix = self.mat.adjMatrix
		flag = 0

		# print("t=",temp.adjMatrix)

		while flag!=2:
			for i in range(self.size):
				v1 = i
				for j in range(self.size):
					if(self.mat.containsEdge(v1,j)):
						for k in range(self.size):
							if(self.mat.containsEdge(j,k)):
								self.mat.addEdge(v1,k)

			if(temp.adjMatrix == self.mat.adjMatrix):
				flag += 1

	def sb(self, trace):
		# getting a list of sb as tuples of two
		for i in range(len(trace)-1):
			if trace[i][T_NO] == trace[i+1][T_NO]:
				# rule sw
				self.hb_edges.append((trace[i][S_NO],trace[i+1][S_NO]))
				self.mat.addEdge(trace[i][S_NO],trace[i+1][S_NO])
				# no need to add so_edges since sb will be recomputed after fence addition anyway

	def sw(self,trace):
		for i in range(len(trace)):
			# create sw's between thread create statements
			if trace[i][TYPE] == CREATE:
				v1 = trace[i][S_NO]
				v2 = v1+1
				# rule sw
				self.hb_edges.append((v1, v2))
				self.mat.addEdge(v1, v2)
				self.so_edges.append((v1, v2))

			# create sw's between thread finish and join statements
			if trace[i][TYPE] == JOIN:
				t = trace[i][VALUE]
				for j in range(i,len(trace)):
					if trace[j][TYPE] == FINISH and trace[j][T_NO] == t:
						v1 = trace[j][S_NO]
						v2 = trace[i][S_NO]
						# rule sw
						self.hb_edges.append((v1, v2))
						self.mat.addEdge(v1, v2)
						self.so_edges.append((v1, v2))

			# create sw's between read/rmw and write/rmw statements - based on rf
			if trace[i][TYPE] == READ or trace[i][TYPE] == RMW:
				rf = trace[i][RF]
				try: j = next(i for i,v in enumerate(trace)
								if type(v) is list and v[S_NO] == rf)
				except: continue
				if trace[j][MO] in write_models and trace[i][MO] in read_models:
					# rule sw
					self.hb_edges.append((trace[j][S_NO],trace[i][S_NO]))
					self.mat.addEdge(trace[j][S_NO],trace[i][S_NO])
					# no need to check and add so_edges here since rf_edges will be checked after fence addition anyway

	def dob(self, trace):
		for i in range(len(self.writes)-1):
			write = self.writes[i]
			if write[MO] in write_models:
				self.matched_reads = []
				self.next_release_sequence(i+1, write)
				self.matched_reads = [self.matched_reads[i] for i in range(len(self.matched_reads)) if i == 0 or self.matched_reads[i] != self.matched_reads[i-1]]					# remove duplicate values

				for read in self.matched_reads:
					# rule dob
					self.hb_edges.append((write[S_NO], read[S_NO]))
					self.mat.addEdge(write[S_NO], read[S_NO])
					if write[MO] == SEQ_CST and read[MO] == SEQ_CST:
						self.so_edges.append((write[S_NO], read[S_NO]))
	
	def next_release_sequence(self, i, write_rel):
		if i >= len(self.writes):
			return

		write_curr = self.writes[i]
		# sb-release-sequence: write_rel --sb--> write --rf--> read => write_rel --dob--> read
		for j in range(i, len(self.writes)):
			write = self.writes[j]
			# print(write, write_rel)
			if write[T_NO] == write_rel[T_NO] and write[ADDR] == write_rel[ADDR]:
				for read in self.reads:
					if read[RF] == write[S_NO] and read[MO] in read_models:
						self.matched_reads.append(read)
				self.next_release_sequence(j+1, write_rel)
			else:
				break
		
		# mo-release-sequence: write_rel --mo--> write --rf--> read => write_rel --dob--> read
		for mo in self.mo_edges:
			if mo[0] == write_curr[S_NO]:
				j = next(k for k,v in enumerate(self.writes) if v[S_NO] == mo[1])
				write = self.writes[j]
				# release sequence condition for ithb
				if (write[T_NO] == write_curr[T_NO]) or (write[MO] in write_models):
					for read in self.reads:
						if read[RF] == write[S_NO] and read[MO] in read_models:
							self.matched_reads.append(read)
					self.next_release_sequence(j+1, write_rel)
		
		return