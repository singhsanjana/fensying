from constants import f_tags as ft

def compute_relaxed_tags(cycles, swdob_edges):
	all_cycles_tag = []
	# deleteing a cycle within loop causes some issues.
	cycles_to_delete = []
	for cycle in cycles:
		tags_for_cycle = {}
		should_delete=False
		for i in range(len(cycle)):
			element = cycle[i]
			if type(element) is not str:
				# not a fence
				continue
			prev = (cycle[(i-1)%len(cycle)], element)
			next = (element, cycle[(i+1)%len(cycle)])
			
			if (prev in swdob_edges) and (next in swdob_edges):
				tags_for_cycle[element] = ft.ar
			elif (prev in swdob_edges) and (next not in swdob_edges):
				tags_for_cycle[element] = ft.a
			elif next in swdob_edges:
				tags_for_cycle[element] = ft.r
			else:
				# prev and next are sb edges. There should be a smaller 
				# cycle in cycles that does not include element. 
				# We can delete this cycle from cycles.
				should_delete = True
				break
		if should_delete:
			cycles_to_delete.append(cycle)
		elif len(tags_for_cycle) != 0:
			all_cycles_tag.append(tags_for_cycle)
		# tags_for_cycle can be empty only if the cycle has no fence. 
		# Such a cycle is invalid program behavior and should not come in any cds trace
		else:
			print('ERROR: found a weak cycle without fences:', cycle)
			exit(0)
		
	for cycle in cycles_to_delete:
		cycles.remove(cycle)

	return all_cycles_tag

def compute_strong_tags(cycles):
	# [snj]: TODO if a --so--> b --so--> c also a --sb--> b --sb--> c then remove the cycle that has these edges
	all_cycles_tag = []
	for cycle in cycles:
		tags_for_cycle = {}
		for element in cycle:
			if type(element) is not str:
				continue
			tags_for_cycle[element] = ft.sc
		all_cycles_tag.append(tags_for_cycle)
	return all_cycles_tag