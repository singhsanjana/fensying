from constants import *
import networkx as nx

class mo:

	def __init__(self, trace, so_edges, traceno):
		self.trace = trace
		self.so_edges = so_edges
		self.mo_edges = []                      		# list of mo edges
		self.rs_edges = {}								# dict of rs paths

		immediate_mo_edges = self.get_mo_from_dot(traceno)
		# print("mo edges=",self.mo_edges)
		# self.get_rs_from_mo()
		self.get_transitive_mo(immediate_mo_edges)
		# print(self.mo_edges)


	def get(self):
		return self.mo_edges, self.so_edges, self.rs_edges
	
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
					isDotted = line.find('dotted') != -1
					if isDotted:
						y = int(line[index+4 : (line.find('[')-1)])
					else:
						y = int(line[index+4:-2])
					mo_found.append((x, y))

					if (self.get_mem_order(x) == SEQ_CST and self.get_mem_order(y)):
						self.so_edges.append((x, y))
		return mo_found

	def get_mem_order(self, s_no):
		for i in range(len(self.trace)):
			if int(self.trace[i][S_NO]) == s_no:
				return self.trace[i][MO]

	def get_transitive_mo(self, immediate_mo_edges):
		# create a graph over mo_edges
		G=nx.DiGraph(immediate_mo_edges)
		# compute all paths over mo_edges. the format is {from: {to: 1/0, ...}, ...}
		paths = nx.all_pairs_node_connectivity(G)
		for fr,to_list in paths.items():
			for to, has_path in to_list.items():
				if has_path == 1:
					self.mo_edges.append((fr,to))
	