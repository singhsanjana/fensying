# --------------------------------------------------------

# --------------------------------------------------------

from graph import Graph

class mo:

	def __init__(self,trace,mat,size):
		self.mat = mat
		self.size = size

		self.mo_edges = []                      								# list of mo edges

		self.rule1(trace)
		self.rule2(trace)
		self.rule3(trace)
		self.rule4(trace)
		self.mo_edges = list(dict.fromkeys(self.mo_edges))
		# print("mo edges=",self.mo_edges)

	def get(self):
		return self.mo_edges

	def rule1(self,trace):
		for i in range(self.size):
			for j in range(self.size):
				if self.mat.containsEdge(i,j):
					# set flag for checking if both are write commands
					flag = 0
					si = str(i)
					sj = str(j)
					for k in trace:
						if k[0] == si and (k[2] == 'write' or k[2] == "init"):
							a = k[4]										# check if the variable being operated upon is the same
							flag += 1
						if k[0] == sj and k[2] == 'write':
							b = k[4]
							flag += 1

					if flag == 2 and a == b:
						self.mo_edges.append((si,sj))

	def rule2(self,trace):
		for i in trace:
			if i[2] == 'read':
				a_no = i[0]

				for j in trace:
					if j[2] == 'read':
						b_no = j[0]
						a_var = i[4]
						b_var = j[4]

						if a_var == b_var:
							a = int(a_no)
							b = int(b_no)

							if self.mat.containsEdge(a,b):
								x = i[6]
								y = j[6]

								if not x == y:
									self.mo_edges.append((x,y))

	def rule3(self,trace):
		for i in trace:
			if i[2] == 'read':
				a_no = i[0]

				for j in trace:
					if j[2] == 'write':
# IDEA: hb's are gonna be more than say write commands, that's why first checking for write cmd and only then checking if hb
						b_no = j[0]

						a_var = i[4]
						b_var = j[4]

						if a_var == b_var:
							a = int(a_no)
							b = int(b_no)

							if self.mat.containsEdge(a,b):
								x = i[6]
								self.mo_edges.append((x,b_no))

	def rule4(self,trace):
		for i in trace:
			if i[2] == 'write':
				x_no = i[0]

				for j in trace:
					if j[2] == 'read':
						b_no = j[0]

						x_var = i[4]
						b_var = j[4]

						if x_var == b_var:
							x = int(x_no)
							b = int(b_no)

							if self.mat.containsEdge(x,b):
								b_rf = j[6]

								if not b_rf == x_no:
									self.mo_edges.append((x_no,b_rf))
