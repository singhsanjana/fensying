# --------------------------------------------------------
# check benchmarks/misc/relseq_test.cc
# MO doesn't include some transitive edges
# Trace1 MO: [(2, 5), (2, 8), (2, 10), (2, 11), (2, 13), (5, 8), (8, 10), (11, 13), (13, 10)]
# Here (5,10) and (11,10) are missing
# --------------------------------------------------------
# Check benchmarks/misc/relseq_test.cc exec0004.dot
# 8 has two direct edges to 10 and 11. 
# So there are two relseq for 8, 8 : { 10 : [8, 10] } { 11 : [8, 11] } 
# The MO here is not complete. This can have two possible MO: [8,10,11] or [8,11,10]
# Both of these can be relseq. It can potentially change other relseq 
# Currently, [11,13] is a relseq. But if 10 comes after 11, 10 will break it.
# To discuss: How to handle it?
# --------------------------------------------------------

from constants import *
import networkx as nx

class mo:

	def __init__(self, trace, so_edges, traceno):
		self.trace = trace
		self.so_edges = so_edges
		self.mo_edges = []                      		# list of mo edges
		self.rs_edges = {}								# dict of rs paths

		self.mo_edges = self.get_mo_from_dot(traceno)
		# print("mo edges=",self.mo_edges)
		self.get_rs_from_mo()
		self.get_transitive_mo()

	def get(self):
		return self.mo_edges, self.so_edges, self.rs_edges
	
	def get_rs_from_mo(self):
		G = nx.DiGraph(self.mo_edges)
		print(self.mo_edges)
		rs_paths = dict(nx.all_pairs_bellman_ford_path(G, weight=lambda x,y,d:-1))
		print('all rs_paths')
		self.print_rs(rs_paths)
		self.remove_rlx_paths(rs_paths)
		# print('rs_edges', self.rs_edges)
		self.print_rs(self.rs_edges)	
		# [div]: TODO: assign to rs_edges in the required format. What is required format?
		# print(rs_edges)
		# return rs_edges

	def remove_rlx_paths(self, rs_paths):
		for rs_head,rs_tail in rs_paths.items():
			if self.get_mem_order(rs_head) != RLX:
				rs_head_thrd = self.get_thread_no(rs_head)
				for rs_end, rs_seq in rs_tail.items():
					# rs_end is not a rlx store from different thread
					if self.get_mem_order(rs_end) != RLX or rs_head_thrd == self.get_thread_no(rs_end):
						is_relseq = True
						for rs_write in rs_seq[1:-1]:
							if self.get_mem_order(rs_write) == RLX and rs_head_thrd != self.get_thread_no(rs_write):
								is_relseq = False
						if is_relseq and len(rs_seq) > 1:
							self.add_to_rs_edges(rs_head, rs_end, rs_seq)
		return

	def add_to_rs_edges(self, rs_head, rs_end, rs_seq):
		if rs_head in self.rs_edges:
			self.rs_edges[rs_head][rs_end] = rs_seq
		else:
			self.rs_edges[rs_head]= {rs_end: rs_seq}
		return

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
	
	def print_rs(self, rs_to_print):
		print('rs edges')
		for rs_head, rs_tail in rs_to_print.items():
			print(rs_head, ':', end=' ')
			for rs_end, rs_seq in rs_tail.items():
				print ('{', rs_end, ':', rs_seq, '}', end=' ')
			print()

	def get_transitive_mo(self):
		pass

	