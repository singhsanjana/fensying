import networkx as nx
import networkx_modified.cycles as nxm

def cycles(flags, bounds, E, essential=None):
	G = nx.DiGraph(E)
	cycles = nxm.simple_cycles(G, flags, bounds, essential)
	return list(cycles)