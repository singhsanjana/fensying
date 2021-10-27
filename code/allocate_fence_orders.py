from constants import f_tags as ft

def max_order(order1, order2):
	if order1 == order2:
		return order1
	if fence_score(order1) > fence_score(order2):
		return order1
	if fence_score(order1) < fence_score(order2):
		return order2
	if fence_score(order1) == fence_score(order2): # wkt order1 != order2
		return ft.ar

def fence_score(fence_order):
	if fence_order == ft.a:
		return 1
	if fence_order == ft.r:
		return 1
	if fence_order == ft.ar:
		return 2
	if fence_order == ft.sc:
		return 3

def cycle_score(tagged_cycle):
	score = 0
	for order in tagged_cycle.values():
		score += fence_score(order)
	return score

def reduce(cycles_list):
	reduced_list = [i for i in range(len(cycles_list))]

	for i in range(len(cycles_list)-1):
		if not i in reduced_list:
			continue

		for j in range(i+1, len(cycles_list)):
			if not j in reduced_list:
				continue

			if cycles_list[i].keys() == cycles_list[j].keys(): # both cycle have same fences
				if cycle_score(cycles_list[i]) < cycle_score(cycles_list[j]):
					reduced_list.remove(j)
					continue
				if cycle_score(cycles_list[i]) > cycle_score(cycles_list[j]):
					reduced_list.remove(i)
					break

	reduced_cycles = []
	for idx in reduced_list:
		reduced_cycles.append(cycles_list[idx])

	return reduced_cycles

def get_min_cycles_of_trace(min_model, trace_cycles):
	min_cycles = []
	for cycle in trace_cycles:
		is_min_cycle = True
		for fence in cycle:
			if 'at' in fence:
				continue
			if not fence in min_model:
				is_min_cycle = False
				break
		if is_min_cycle:
			min_cycles.append(cycle)

	return min_cycles

def allocate_fence_orders(min_model, cycles_tags_by_trace):
	# initialize candidate_solutions to min_cycle of trace 0
	candidate_solutions = get_min_cycles_of_trace(min_model, cycles_tags_by_trace[0]) 

	# coalesce min_cycles of remaining traces with candidate_solutions
	for i in range(1, len(cycles_tags_by_trace)): 
		trace_min_cycles = get_min_cycles_of_trace(min_model, cycles_tags_by_trace[i]) # min_cycles of ith trace

		new_candidate_solutions = []
		for cycle1 in candidate_solutions:
			for cycle2 in trace_min_cycles:
				new_cycle = {}
				for fence in cycle1:
					if fence in cycle2: # if fence in both then choose the higher order of the two
						new_cycle[fence] = max_order(cycle1[fence], cycle2[fence])
					else:
						new_cycle[fence] = cycle1[fence]

				for fence in cycle2:
					if not fence in cycle1: 
						# if fence in cycle2 but not cycle1 then add the fence to the solution with cycle2's order
						new_cycle[fence] = cycle2[fence]

				new_candidate_solutions.append(new_cycle)

		candidate_solutions = reduce(new_candidate_solutions)

	return candidate_solutions[0] # if there are >1 solutions return any 1