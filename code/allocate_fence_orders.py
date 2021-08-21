from constants import f_tags as ft

def allocate_fence_orders(req_fences, all_cycles_by_trace, cycles_tags_by_trace):
	# print(req_fences, all_cycles, cycles_tags)
	candidate_sol = get_cycles_with_only_fences(all_cycles_by_trace[0])
	candidate_sol_tags = cycles_tags_by_trace[0]
	for t in range(1,len(all_cycles_by_trace)):
		trace = all_cycles_by_trace[t]
		for i in range(len(trace)):
			cycle = trace[i]
			cycle_with_only_fences = [c for c in cycle if type(c) is str]
			# check only the min_cycles, where all fences of the cycle are in the min_model
			if set(cycle_with_only_fences).issubset(set(req_fences)):
				for j in range(len(candidate_sol)):
					cycle_ = candidate_sol[j]
					for f in cycle_with_only_fences:
						if f in cycle_:
							if get_fence_score(cycles_tags_by_trace[t][i][f]) > get_fence_score(candidate_sol_tags[j][f]):
								candidate_sol_tags[j][f] = cycles_tags_by_trace[t][i][f]
						else:
							candidate_sol[j].append(f)
							candidate_sol_tags[j][f] = cycles_tags_by_trace[t][i][f]
		for i in range(len(candidate_sol)):
			cycle = candidate_sol[i]
			for j in range(len(candidate_sol)):
				cycle_ = candidate_sol[j]
				if (i != j) and (set(cycle) == set(cycle_)):
					cycle_score = get_cycle_score(cycle)
					cycle_score_ = get_cycle_score(cycle_)
					if cycle_score <= cycle_score_:
						del candidate_sol[j]
						del candidate_sol_tags[j]
	selected_cycle = candidate_sol_tags[0]
	for cs in candidate_sol_tags:
		if get_cycle_score(cs) < get_cycle_score(selected_cycle):
			selected_cycle = cs
	print(selected_cycle)
	return selected_cycle

def get_cycles_with_only_fences(all_cycles):
	cycles_with_only_fences = []
	for cyc in all_cycles:
		cwof = [c for c in cyc if type(c) is str]
		cycles_with_only_fences.append(cwof)

	return cycles_with_only_fences

def get_cycle_score(cycle_dict):
	orders = cycle_dict.values()
	score = 0
	for order in orders:
		score += get_fence_score(order)

	return score

def get_fence_score(order):
	if order == ft.a or order == ft.r:
		return 1
	elif order == ft.ar:
		return 2
	elif order == ft.sc:
		return 3