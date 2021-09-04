# --------------------------------------------------------

# --------------------------------------------------------

from constants import *

class mo:

	def __init__(self, trace, hb_matrix, size, so_edges, writes, reads, traceno=1):
		self.trace = trace
		self.hb_matrix = hb_matrix
		self.size = size
		self.so_edges = so_edges

		self.writes = writes
		self.reads = reads
		self.mo_edges = []                      								# list of mo edges

		# self.rule1()
		# self.rule2()
		# self.rule3()
		# self.rule4()

		# self.mo_edges = list(dict.fromkeys(self.mo_edges))
		# print('finding mo for trace', traceno)
		self.mo_edges = self.get_mo_from_dot(traceno)
		# print("mo edges=",self.mo_edges)

	def get(self):
		return self.mo_edges, self.so_edges

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


	def rule1(self):
		for a in self.writes:
			for b in self.writes:
				if a[ADDR] == b[ADDR] and self.hb_matrix.containsEdge(a[S_NO],b[S_NO]):		# checking if variable operated upon is same and if there is hb
					self.mo_edges.append((a[S_NO],b[S_NO]))
					if a[MO] == SEQ_CST and b[MO] == SEQ_CST:
						self.so_edges.append((a[S_NO], b[S_NO]))
	
	def rule2(self):
		for a in self.reads:
			x = next(v for i,v in enumerate(self.writes) if v[S_NO] == a[RF])
			for b in self.reads:
				if a[ADDR] == b[ADDR] and b[RF] != x[S_NO] and self.hb_matrix.containsEdge(a[S_NO],b[S_NO]): 	# checking if variable operated upon is same and if there is hb
					y = next(v for i,v in enumerate(self.writes) if v[S_NO] == b[RF])
					self.mo_edges.append((x[S_NO], y[S_NO]))
					if x[MO] == SEQ_CST and y[MO] == SEQ_CST:
							self.so_edges.append((x[S_NO], y[S_NO]))

	def rule3(self):
		for a in self.reads:
			x = next(v for i,v in enumerate(self.writes) if v[S_NO] == a[RF])
			for b in self.writes:
				if b[ADDR] == a[ADDR] and self.hb_matrix.containsEdge(a[S_NO],b[S_NO]): 		# checking if variable operated upon is same and if there is hb
					self.mo_edges.append((x[S_NO], b[S_NO]))
					if x[MO] == SEQ_CST and b[MO] == SEQ_CST:
						self.so_edges.append((x[S_NO], b[S_NO]))
	
	def rule4(self):
		for x in self.writes:
			for b in self.reads:
				if x[ADDR] == b[ADDR] and self.hb_matrix.containsEdge(x[S_NO],b[S_NO]) and b[RF] != x[S_NO]:
					y = next(v for i,v in enumerate(self.writes) if v[S_NO] == b[RF])
					self.mo_edges.append((x[S_NO], y[S_NO]))
					if x[MO] == SEQ_CST and y[MO] == SEQ_CST:
						self.so_edges.append((x[S_NO], y[S_NO]))

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