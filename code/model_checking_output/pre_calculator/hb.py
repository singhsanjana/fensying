# --------------------------------------------------------

# --------------------------------------------------------

from model_checking_output.pre_calculator.graph import Graph
from constants import *

class hb:

	def __init__(self, trace, so_edges):
		self.hb_edges = []
		self.rf_edges = []
		self.so_edges = so_edges

		# self.sb(trace)
		self.swrf(trace)
		
		self.hb_edges = list(set(self.hb_edges))

	def get(self):
		return self.hb_edges, self.rf_edges, self.so_edges

	# [snj]: function not in use
	def sb(self, trace):
		# getting a list of sb as tuples of two
		for i in range(len(trace)-1):
			if trace[i][T_NO] == trace[i+1][T_NO]:
				self.hb_edges.append((trace[i][S_NO],trace[i+1][S_NO]))
			
	def swrf(self,trace):
		for i in range(1, len(trace)): # 1st event is not hb after any event 
			# read rf (if any)
			if trace[i][RF] != 'NA': # has rf details
				v1 = trace[i][RF]
				v2 = trace[i][S_NO]
				self.rf_edges.append((v1, v2))

				if self.get_mem_order(trace, v1) == SEQ_CST and trace[i][MO] == SEQ_CST:
					self.so_edges.append((v1,v2))

			# read sw using CV
			for j in range(len(trace[i][CV])):
				if j+1 == trace[i][T_NO]: # sb computed after inserting fences (valid threads start at 1)
					continue

				if trace[i][CV][j] == 0: # lack of information on hb (program threads start from 1 not 0)
					continue

				if trace[i-1][T_NO] == trace[i][T_NO]: 
					if len(trace[i-1][CV]) > j and trace[i-1][CV][j] == trace[i][CV][j]: # a transitive hb relation
							continue

				v1 = int(trace[i][CV][j])
				v2 = int(trace[i][S_NO])
				self.hb_edges.append((v1,v2))
				
				if (self.get_mem_order(trace, v1) == SEQ_CST and trace[i][MO] == SEQ_CST):
					self.so_edges.append((v1,v2))
					
				
	def get_mem_order(self, trace, s_no):
		for i in range(len(trace)):
			if int(trace[i][S_NO]) == s_no:
				return trace[i][MO]