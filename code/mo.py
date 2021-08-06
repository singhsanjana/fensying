from constants import *

class mo:
	def __init__(self, writes, reads, hb_edges, sc_edges):
		self.reads = reads
		self.writes = writes
		self.sc_edges = sc_edges
		self.hb_edges = hb_edges
		self.mo_edges = []

		self.compute()

	def get(self):
		return self.mo_edges, self.sc_edges

	def compute(self):
		for edge in self.hb_edges:
			if (type(edge[0]) is int) and (type(edge[1]) is int):
				try:
					w1 = next(v for i,v in enumerate(self.writes) if type(v) is list and v[S_NO] == edge[0])

					# write-write coherence rule
					try:
						w2 = next(v for i,v in enumerate(self.writes) if type(v) is list and v[S_NO] == edge[1])
						if w1[ADDR] == w2[ADDR]:
							self.mo_edges.append((w1[S_NO], w2[S_NO]))
							if w1[MO] == SEQ_CST and w2[MO] == SEQ_CST:
								self.sc_edges.append((w1[S_NO], w2[S_NO]))
					except: continue

					# write-read coherence rule
					try:
						r1 = next(v for i,v in enumerate(self.reads) if type(v) is list and v[S_NO] == edge[1])
						if r1[RF] != w1[S_NO]:
							w2_S_NO = r1[RF]
							w2 = next(v for i,v in enumerate(self.writes) if type(v) is list and v[S_NO] == w2_S_NO)
							if w1[ADDR] == w2[ADDR]:
								self.mo_edges.append((w1[S_NO], w2[S_NO]))
								if w1[MO] == SEQ_CST and w2[MO] == SEQ_CST:
									self.sc_edges.append((w1[S_NO], w2[S_NO]))
					except: continue
				except: continue

				try:
					r1 = next(v for i,v in enumerate(self.reads) if type(v) is list and v[S_NO] == edge[0])

					# read-read coherence rule
					try:
						r2 = next(v for i,v in enumerate(self.reads) if type(v) is list and v[S_NO] == edge[1])
						w1 = next(v for i,v in enumerate(self.writes) if type(v) is list and v[S_NO] == r1[RF])
						if r2[RF] != w1[S_NO]:
							w2 = next(v for i,v in enumerate(self.writes) if type(v) is list and v[S_NO] == r2[RF])
							if w1[ADDR] == w2[ADDR]:
								self.mo_edges.append((w1[S_NO], w2[S_NO]))
								if w1[MO] == SEQ_CST and w2[MO] == SEQ_CST:
									self.sc_edges.append((w1[S_NO], w2[S_NO]))
					except: continue

					# read-write coherence rule
					try:
						w1 = next(v for i,v in enumerate(self.writes) if type(v) is list and v[S_NO] == edge[1])
						if r1[RF] != w1[S_NO]:
							w2 = next(v for i,v in enumerate(self.writes) if type(v) is list and v[S_NO] == r1[RF])
							if w1[ADDR] == w2[ADDR]:
								self.mo_edges.append((w1[S_NO], w2[S_NO]))
								if w1[MO] == SEQ_CST and w2[MO] == SEQ_CST:
									self.sc_edges.append((w1[S_NO], w2[S_NO]))
					except: continue
				except: continue