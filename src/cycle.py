import networkx as nx
import networkx_modified.cycles as nxm

def cycles_aux(G, v, visited, explored, stack):
	# TODO avoid cycle 12341 if 1231 already found
	if v in explored:
		return []

	visited.add(v)
	stack.append(v)
	cycles = []

	for v_ in G.successors(v):
		if v_ in stack:
			cycle = stack[stack.index(v_):]
			cycles += [cycle]
			continue

		cycles += cycles_aux(G, v_, visited, explored, stack[:])

	return cycles

def cycles(flags, bounds, E):
	G = nx.DiGraph(E)
	cycles = nxm.simple_cycles(G, flags, bounds)
	cycles = list(cycles)

	# visited  = set()  # seen from the current source (node from where DFS starts)
	# explored = set()  # visted from all predecessors
	# cycles   = []     # all cycles in graph

	# for v in G:
	# 	if not v in visited:
	# 		cycles += cycles_aux(G, v, visited, explored, [])
	# 		visited.add(v)
	# 		explored.update(visited)
	# 		visited = set()

	return cycles