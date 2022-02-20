from itertools import cycle
from constants import f_tags as ft

def collapse_mo(mo1, mo2):
	mo_score = {ft.sc:3, ft.ar:2, ft.r:1, ft.a:1}

	if mo1 == mo2:
		return mo1
	if mo_score(mo1) > mo_score(mo2):
		return mo1
	if mo_score(mo2) > mo_score(mo1):
		return mo2
	return ft.ar # same score but mo not same ie ft.r and ft.a

def same_location_fences(fence1, fence2):
	prefix1, s_no1, line_no1, filename1, tag1 = fence1
	prefix2, s_no2, line_no2, filename2, tag2 = fence2
	return (prefix1 == prefix2 and line_no1 == line_no2 and filename1 == filename2)

def expand_fence(fence):
	filename = fence.split('@')[1]
	fence    = fence.split('@')[0]

	line_no = fence.split('-')[1]
	fence   = fence.split('-')[0]

	s_no   = fence[ fence.rfind('_')+1: ]
	prefix = fence[ :fence.rfind('_') ]

	return (prefix, s_no, line_no, filename)

def expand_fence_tag(fence, tag):
	prefix, s_no, line_no, filename = expand_fence(fence)
	return (prefix, s_no, line_no, filename, tag)

def weak_collapse(cycle_tags):
	# for different occurrences of a fence at a location (due to loops)
	# collapse all such occurrences into 1 with the strongest tag
	expanded_fences = [ expand_fence_tag(fence, cycle_tags[fence]) for fence in cycle_tags.keys() ]
	final_cycle_tags = {}

	collapsed_indices = []
	for i in range(len(expanded_fences)):
		if i in collapsed_indices:
			continue

		prefix, s_no, line_no, filename, tag = expanded_fences[i]
		mo = tag
		for j in range(i+1, len(expanded_fences)):
			if same_location_fences(expanded_fences[i], expanded_fences[j]):
				mo = collapse_mo(mo, expanded_fences[j][4])
				collapsed_indices[j]

		collapsed_fence = prefix + '_' + line_no + '@' + filename
		final_cycle_tags[collapsed_fence] = mo
	
	return final_cycle_tags

def compute_relaxed_tags(cycles, swdob_edges):
	all_cycles_tag   = []
	cycles_to_remove = []
	
	for cycle in cycles:
		tags_for_cycle = {}
		to_remove = False

		for i in range(len(cycle)):
			event = cycle[i]
			if type(event) is not str: # not a fence
				continue

			prev = (cycle[(i-1)%len(cycle)], event) # incoming edge: * --> event
			next = (event, cycle[(i+1)%len(cycle)]) # outgoing edge: event --> *
			
			if (prev in swdob_edges) and (next in swdob_edges):
				tags_for_cycle[event] = ft.ar
			elif (prev in swdob_edges) and (next not in swdob_edges):
				tags_for_cycle[event] = ft.a
			elif next in swdob_edges:
				tags_for_cycle[event] = ft.r
			else:
				# prev and next are sb edges. There should be a smaller 
				# cycle in cycles that does not include event. 
				# We can delete this cycle from cycles.
				to_remove = True
				break

		if to_remove:
			cycles_to_remove.append(cycle)
		elif len(tags_for_cycle) != 0:
			all_cycles_tag.append(weak_collapse(tags_for_cycle))
		# tags_for_cycle can be empty only if the cycle has no fence. 
		# Such a cycle is invalid program behavior and should not come in any cds trace
		else:
			print('ERROR: found a weak cycle without fences:', cycle)
			exit(0)
		
	for cycle in cycles_to_remove:
		cycles.remove(cycle)

	return all_cycles_tag

def strong_collapse(cycles):
	collapsed_cycles = []
	for cycle in cycles:
		collapsed_cycle = set()
		for event in cycle:
			if type(event) != str: # not fence
				continue

			prefix, s_no, line_no, filename = expand_fence(event)
			collapsed_fence = prefix + '_' + line_no + '@' + filename
			collapsed_cycle.add(collapsed_fence)

		collapsed_cycle = list(collapsed_cycle)
		collapsed_cycles.append(collapsed_cycle)
		
	return collapsed_cycles

def reduce(cycles, sb_edges):
	# remove non-minimal cycles
	# non-minimal cycle: cycles with a --so--> b --so--> c where a --sb--> b --sb--> c
	cycles_to_remove = []
	for cycle in cycles:
		for i in range(len(cycle)):
			prev = cycle[(i-1)%len(cycle)] #(i-1)th fence
			curr = cycle[i]                #(i)th fence
			next = cycle[(i+1)%len(cycle)] #(i+1)th fence
			if (prev,curr) in sb_edges and (curr,next) in sb_edges:
				cycles_to_remove.append(cycle)
				break
	
	for cycle in cycles_to_remove:
		cycles.remove(cycle)

def compute_strong_tags(cycles, sb_edges):
	cycles = strong_collapse(cycles) # remove s_no from fences of all cycles
	reduce(cycles, sb_edges) # if a --so--> b --so--> c also a --sb--> b --sb--> c then remove the cycle from cycles

	all_cycles_tag = []
	for cycle in cycles:
		tags_for_cycle = {}
		for event in cycle:
			if type(event) is not str:
				continue
			tags_for_cycle[event] = ft.sc
		all_cycles_tag.append(tags_for_cycle)
	return all_cycles_tag