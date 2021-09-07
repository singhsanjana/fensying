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
		for i in range(1, len(trace)): # 1st event is not hb after any event 
			for j in range(len(trace[i][CV])):
				if j+1 == trace[i][T_NO]: # sb computed in another function (valid threads start at 1)
					continue

				if trace[i][CV][j] == 0: # lack of information on hb (program threads start from 1 not 0)
					continue

				if trace[i-1][T_NO] == trace[i][T_NO]: 
					if len(trace[i-1][CV]) == len(trace[i][CV]) and trace[i-1][CV][j] == trace[i][CV][j]: # a transitive hb relation
						continue

				v1 = int(trace[i][CV][j])
				v2 = int(trace[i][S_NO])
				self.mat.addEdge(v1,v2)
				self.hb_edges.append((v1,v2))
				
				if (self.get_mo(trace, v1) == SEQ_CST and trace[i][MO] == SEQ_CST):
					self.so_edges.append((v1,v2))
					
				
	def get_mo(self, trace, s_no):
		for i in range(len(trace)):
			if int(trace[i][S_NO]) == s_no:
				return trace[i][MO]