import networkx as nx
import networkx_modified.simple_cycles as nxm

def cycles(flags, bounds, E, essential=None):
	G = nx.DiGraph(E)
	cycles = nxm.simple_cycles(G, flags, bounds, essential)
	return cycles