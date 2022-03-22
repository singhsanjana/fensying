from constants import f_tags as ft

def get_prog_mo(fence):
	if '_at_' in fence:
		fence_prog_order = fence[2 : fence.find(')')]
		return fence_prog_order
	return ''

def mo_score(mo):
	if mo == ft.a:
		return 1
	if mo == ft.r:
		return 1
	if mo == ft.ar:
		return 2
	if mo == ft.sc:
		return 3

def max_order(order1, order2):
	if order1 == order2:
		return order1
	if mo_score(order1) > mo_score(order2):
		return order1
	if mo_score(order1) < mo_score(order2):
		return order2
	if mo_score(order1) == mo_score(order2): # wkt order1 != order2
		return ft.ar

def fence_score(fence, fence_order):
	order_score = mo_score(fence_order)

	if '_at_' in fence:
		fence_prog_order = get_prog_mo(fence)
		prog_order_score = mo_score(fence_prog_order)
		if prog_order_score > order_score or fence_order == fence_prog_order:
			return 0
		else: # strengthened program fence
			return (order_score - prog_order_score)

	return order_score

def cycle_score(tagged_cycle):
	score = 0
	for fence in tagged_cycle:
		order = tagged_cycle[fence]
		score += fence_score(fence, order)
	return score

def preference(tagged_cycle):
	synthesized  = False
	strengthened = False

	for fence in tagged_cycle:
		if '_at_' in fence:
			current_mo_score = mo_score( get_prog_mo(fence) )
			new_mo_score = mo_score( tagged_cycle[fence] )
			if new_mo_score > current_mo_score:
				strengthened = True
		else:
			synthesized = True

		if synthesized and strengthened: # nothing left to discover
			break

	if synthesized and strengthened:
		return 0
	if strengthened:
		return 1
	return 2 # only synthesized

def reduce(cycles_list):
	reduced_list = [i for i in range(len(cycles_list))]

	for i in range(len(cycles_list)-1):
		if not i in reduced_list:
			continue

		for j in range(i+1, len(cycles_list)):
			if not j in reduced_list:
				continue

			if cycles_list[i].keys() == cycles_list[j].keys(): # both cycle have same fences
				if cycles_list[i] == cycles_list[j]:
					reduced_list.remove(j)
					continue
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
			if '_at_' in fence: # program fence, min-model has only synthesized fences
				continue
			if not fence in min_model:
				is_min_cycle = False
				break
		if is_min_cycle:
			min_cycles.append(cycle)

	return min_cycles

def weakest(solutions):
	weakest_solution = solutions[0]
	weakest_score    = cycle_score(solutions[0])
	
	for i in range(1,len(solutions)):
		solution_score = cycle_score(solutions[i])
		if solution_score < weakest_score:
			weakest_solution = solutions[i]
			weakest_score    = solution_score
		elif solution_score == weakest_score and preference(solutions[i]) > preference(weakest_solution): 
			weakest_solution = solutions[i]
			weakest_score    = solution_score

	return weakest_solution

def solution_by_files(solution): # solution dict: fence -> mo
	solution_files = {}
	for fence in solution:
		filename = fence.split('@')[1]
		fence_pos = fence.split('@')[0]
        
		if filename in solution_files:
			solution_files[filename].append( (fence_pos, solution[fence]) )
		else:
			solution_files[filename] = [ (fence_pos, solution[fence]) ]
	
	return solution_files

def allocate_fence_orders(min_model, cycles_tags_by_trace):
	# initialize candidate_solutions to min_cycle of trace 0
	candidate_solutions = get_min_cycles_of_trace(min_model, cycles_tags_by_trace[0]) 
	candidate_solutions = reduce(candidate_solutions)

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

	return solution_by_files(weakest(candidate_solutions)) # if there are >1 solutions return the weakest