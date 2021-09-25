# --------------------------------------------------------
# check benchmarks/misc/relseq_test.cc
# MO doesn't include some transitive edges
# Trace1 MO: [(2, 5), (2, 8), (2, 10), (2, 11), (2, 13), (5, 8), (8, 10), (11, 13), (13, 10)]
# Here (5,10) and (11,10) are missing
# --------------------------------------------------------

from constants import *
import networkx as nx

class mo:

	def __init__(self, trace, so_edges, traceno):
		self.trace = trace
		self.so_edges = so_edges
		self.mo_edges = []                      								# list of mo edges

		self.mo_edges = self.get_mo_from_dot(traceno)
		# print("mo edges=",self.mo_edges)
		self.rs_edges = self.get_rs_from_mo()

	def get(self):
		return self.mo_edges, self.so_edges, self.rs_edges
	
	def get_rs_from_mo(self):
		G = nx.DiGraph(self.mo_edges)
		print(self.mo_edges)
		rs_paths = dict(nx.all_pairs_bellman_ford_path(G, weight=lambda x,y,d:-1))
		rs_edges = self.remove_rlx_paths(rs_paths)			
		# [div]: TODO: assign to rs_edges in the required format. What is required format
		print(rs_edges)
		return rs_edges

	def remove_rlx_paths(self, rs_paths):
		rs_edges={}
		for rs_head,rs_tail in rs_paths.items():
			if self.get_mem_order(rs_head) != RLX:
				rs_head_thrd = self.get_thread_no(rs_head)
				for rs_end, rs_seq in rs_tail.items():
					# [div]: TODO use traces to get thread num here?
					if self.get_mem_order(rs_end) != RLX or rs_head_thrd == self.get_thread_no(rs_end):
						for rs_write in rs_seq[1:-1]:
							if self.get_mem_order(rs_write) != RLX or rs_head_thrd == self.get_thread_no(rs_write):
								if rs_head in rs_edges:
									rs_edges[rs_head][rs_end] = rs_seq
								else:
									rs_edges[rs_head]= {rs_end: rs_seq}
		return rs_edges


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

					if (self.get_mem_order(x) == SEQ_CST and self.get_mem_order(y)):
						self.so_edges.append((x, y))
		return mo_found

	def get_mem_order(self, s_no):
		for i in range(len(self.trace)):
			if int(self.trace[i][S_NO]) == s_no:
				return self.trace[i][MO]
	
	def get_thread_no(self, s_no):
		for i in range(len(self.trace)):
			if int(self.trace[i][S_NO]) == s_no:
				return self.trace[i][T_NO]
	

	