import networkx as nx

def make_graph(edges):
	return nx.DiGraph(edges)

def compute_cycles(G):
	cycles = nx.simple_cycles(G)
	cycles = list(cycles)

	return cycles

def Cycles(edges):
	G=make_graph(edges)
	cycles = nx.simple_cycles(G)
	cycles = list(cycles)

	return cycles
