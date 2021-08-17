from constants import f_tags as ft

def compute_relaxed_tags(cycles, swdob_edges):
	cycles_tags = []
	for cycle in cycles:
		cycle_dict = {}
		for i in range(len(cycle)):
			element = cycle[i]
			if type(element) is not str:
				continue
			prev = (cycle[i-1], element)
			if i != len(cycle)-1 :
				nex = (element, cycle[i+1])
			else:
				nex = (element, cycle[0])
			
			if (prev in swdob_edges) and (nex in swdob_edges):
				cycle_dict[element] = ft.ar
			elif (prev in swdob_edges) and (nex not in swdob_edges):
				cycle_dict[element] = ft.a
			elif (prev not in swdob_edges) and (nex in swdob_edges):
				cycle_dict[element] = ft.r
			else:
				cycle_dict[element] = ft.sc
		cycles_tags.append(cycle_dict)
	return cycles_tags

def compute_strong_tags(cycles):
	cycles_tags = []
	for cycle in cycles:
		cycle_dict = {}
		for element in cycle:
			if type(element) is not str:
				continue
			cycle_dict[element] = ft.sc
		cycles_tags.append(cycle_dict)
	return cycles_tags