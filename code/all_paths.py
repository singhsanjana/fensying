import networkx as nx

def compute_paths(G, src, tgt):
	paths = nx.all_simple_paths(G, source=src, target=tgt)
	return list(paths)

def make_graph(edges):
	return nx.DiGraph(edges)

def all_path(edges, source_nodes, target_nodes):
	# Create Directed Graph with a list of all edges
	G=make_graph(edges)

	hb_paths = {}

	for e1 in source_nodes:
		print('next source', e1)
		is_target = False
		if e1 in target_nodes:
			is_target = True
			target_nodes.remove(e1) # note: all_paths fails if e1 is in the list of target nodes

		paths = compute_paths(G, e1, target_nodes)
		
		if is_target:
			target_nodes.add(e1) # add e1 back for next iteration

		hb_paths[e1] = {}
		for path in paths:
			e2 = path[-1] # target event
			if not e2 in hb_paths[e1]: # first path from e1 to e2
				hb_paths[e1][e2] = []

			hb_paths[e1][e2].append(path)

	return hb_paths
