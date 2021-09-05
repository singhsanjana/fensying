# --------------------------------------------------------

# --------------------------------------------------------

from z3_functions import constant
from pre_calculators.graph import Graph
from constants import *

class mo:

	def __init__(self, trace, hb_matrix, size, to_edges, traceno=1):
		self.trace = trace
		self.hb_matrix = hb_matrix
		self.size = size
		self.to_edges = to_edges

		self.writes = []
		self.reads = []
		self.mo_edges = []                      								# list of mo edges

		# self.preprocessing(trace)

		# self.rule1()
		# self.rule2()
		# self.rule3()
		# self.rule4()

		# self.mo_edges = list(dict.fromkeys(self.mo_edges))

		# open exec files
		# for event in trace:
		# 	print(event)
		# print('finding mo for trace', traceno)
		self.mo_edges = self.get_mo_from_dot(traceno)

		# print("mo edges=",self.mo_edges)

	def get(self):
		return self.mo_edges, self.to_edges

	def get_mo_from_dot(self, traceno):
		mo_found = []
		filename= file_info.CDS_FOLDER_PATH + '/exec%04d.dot' % (traceno)
		# print(filename)
		with open(filename) as file:
			lines=file.readlines()
			for line in lines:
				index = line.find('->')
				if index != -1 and 'N0' not in line:
					mo_found.append((int(line[1:index-1]), int(line[index+4:-2])))
		return mo_found

	def preprocessing(self, trace):
		for t in trace:
			if t[TYPE] == WRITE or t[TYPE] == INIT or t[TYPE] == RMW:
				self.writes.append(t)
			if t[TYPE] == READ or t[TYPE] == RMW:
				self.reads.append(t)
	
	def rule1(self):
		for a in self.writes:
			for b in self.writes:
				if a[ADDR] == b[ADDR] and self.hb_matrix.containsEdge(a[S_NO],b[S_NO]):		# checking if variable operated upon is same and if there is hb
					self.mo_edges.append((a[S_NO],b[S_NO]))

					# if they are of memory order sc, then they need to also be counted as TO edges
					if a[MO] == SEQ_CST and b[MO] == SEQ_CST:
						self.to_edges.append((a[S_NO], b[S_NO]))
	
	def rule2(self):
		for a in self.reads:
			x = a[RF] # "RF" of a refers to the serial number of the write instruction that it is reading from
			for b in self.reads:
				if a[ADDR] == b[ADDR] and b[RF] != x and self.hb_matrix.containsEdge(a[S_NO],b[S_NO]): 	# checking if variable operated upon is same and if there is hb
					y = b[RF]
					self.mo_edges.append((x,y))

					# if they are of memory order sc, then they need to also be counted as TO edges
					k = 0
					for write in self.writes:
						if write[S_NO] == x and write[MO] == SEQ_CST:
							k += 1
						if write[S_NO] == y and write[MO] == SEQ_CST:
							k += 1
					if k == 2:
						self.to_edges.append((x,y))

	def rule3(self):
		for a in self.reads:
			x = a[RF]
			for b in self.writes:
				if b[ADDR] == a[ADDR] and self.hb_matrix.containsEdge(a[S_NO],b[S_NO]): 		# checking if variable operated upon is same and if there is hb
					self.mo_edges.append((x,b[S_NO]))

					# if they are of memory order sc, then they need to also be counted as TO edges
					k = 0
					for write in self.writes:
						if write[S_NO] == x and write[MO] == SEQ_CST:
							k += 1
					if k == 1 and b[MO] == SEQ_CST:
						self.to_edges.append((x,b[S_NO]))
	
	def rule4(self):
		for x in self.writes:
			for b in self.reads:
				if x[ADDR] == b[ADDR] and self.hb_matrix.containsEdge(x[S_NO],b[S_NO]) and b[RF] != x[S_NO]:
					y = b[RF]
					self.mo_edges.append((x[S_NO],y))

					# if they are of memory order sc, then they need to also be counted as TO edges
					k = 0
					for write in self.writes:
						if write[S_NO] == y and write[MO] == SEQ_CST:
							k += 1
					if k == 1 and x[MO] == SEQ_CST:
						self.to_edges.append((x[S_NO],y))

	# unused
	# to get all the transitive mo relations as well
	def all_mo(self):
		flag = 0
		temp_edges = self.mo_edges

		while flag != 2:
			for e0 in self.mo_edges:
				for e1 in self.mo_edges:
					if e0[1] == e1[0] and (e0[0],e1[1]) not in self.mo_edges:
						self.mo_edges.append((e0[0],e1[1]))


			if self.mo_edges == temp_edges:
				flag += 1
			else:
				temp_edges = self.mo_edges