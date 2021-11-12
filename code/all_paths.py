import networkx as nx

def all_path(edges, src, tgt):

	# Create Directed Graph with a list of all edges
	G=nx.DiGraph(edges)

	paths = nx.all_simple_paths(G, source=src, target=tgt)
	paths = list(paths)

	return paths
