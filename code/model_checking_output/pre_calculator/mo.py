# --------------------------------------------------------

# --------------------------------------------------------

from constants import *

class mo:

	def __init__(self, trace, so_edges, traceno):
		self.trace = trace
		self.so_edges = so_edges
		self.mo_edges = []                      								# list of mo edges

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
					x = int(line[1:index-1])
					y = int(line[index+4:-2])
					mo_found.append((x, y))

					if (self.get_mem_order(self.trace, x) == SEQ_CST and self.get_mem_order(self.trace, y)):
						self.so_edges.append((x, y))
		return mo_found

	def get_mem_order(self, trace, s_no):
		for i in range(len(trace)):
			if int(trace[i][S_NO]) == s_no:
				return trace[i][MO]