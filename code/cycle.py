import networkx as nx

def Cycles(edges):

	# Create Directed Graph with a list of all edges
	G=nx.DiGraph(edges)

	cycles = nx.simple_cycles(G)
	cycles = list(cycles)

	return cycles
