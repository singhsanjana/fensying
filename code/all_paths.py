import networkx as nx

def compute_paths(G, src, tgt):
	paths = nx.all_simple_paths(G, source=src, target=tgt)
	return list(paths)

def make_graph(edges):
	return nx.DiGraph(edges)