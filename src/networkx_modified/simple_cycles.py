from collections import defaultdict
import networkx as nx

import multiprocessing as mp

essential = None
flags  = None
bounds = None

class scc_meta_bundle:
    def __init__(self, scc):
        self.fences = [f for f in scc if type(f) == str]
        self.events = [e for e in scc if type(e) == int]
        self.count_fences = len(self.fences)

def next_fences(count_fences, bound, indices, all_fences):
    pivot = bound
    while pivot >= 0:
        pivot -= 1
        indices[pivot] += 1
        for p in range(pivot+1, min(bound, pivot+bound)):
            indices[p] = indices[pivot] + (p - pivot)
        if indices[-1] < count_fences:
            next_fences_combination = []
            for i in range(bound):
                next_fences_combination.append(all_fences[indices[i]])
            return next_fences_combination

    return [] # no more combinations

def _unblock(thisnode, blocked, B):
    stack = {thisnode}
    while stack:
        node = stack.pop()
        if node in blocked:
            blocked.remove(node)
            stack.update(B[node])
            B[node].clear()

def simple_cycles_scc(scc, subG):
    sccG = subG.subgraph(scc)
    cycles = []

    # order of scc determines ordering of nodes
    startnode = scc.pop()
    # Processing node runs "circuit" routine from recursive version
    path = [startnode]
    blocked = set()  # vertex: blocked from search?
    closed = set()  # nodes involved in a cycle
    blocked.add(startnode)
    B = defaultdict(set)  # graph portions that yield no elementary circuit
    stack = [(startnode, list(sccG[startnode]))]  # sccG gives comp nbrs
    while stack:
        if flags['depth_bound'] and bounds['max_depth'] == len(stack): # Fensying depth bounding
            break

        thisnode, nbrs = stack[-1]
        if nbrs:
            nextnode = nbrs.pop()
            if nextnode == startnode:
                cycles.append(path[:])
                closed.update(path)
            
            elif nextnode not in blocked:
                path.append(nextnode)
                stack.append((nextnode, list(sccG[nextnode])))
                closed.discard(nextnode)
                blocked.add(nextnode)
                continue
        # done with nextnode... look for more neighbors
        if not nbrs:  # no more nbrs
            if thisnode in closed:
                _unblock(thisnode, blocked, B)
            else:
                for nbr in sccG[thisnode]:
                    if thisnode not in B[nbr]:
                        B[nbr].add(thisnode)
            stack.pop()
            path.pop()

    return cycles

def simple_cycles_sccs(sccs, subG):
    cycles = []

    _scc_ = None
    idx = []

    while sccs or _scc_:
        if not _scc_:
            scc = sccs.pop()
            if essential: # essential edge (every cycle must have this edge) is provided
                if len([edge for edge in essential if not edge[0] in scc or not edge[1] in scc]) > 0:
                    # if essential edges(s) are missing from scc
                    continue # skip this scc
        
            if flags['fence_bound']:
                _scc_ = scc_meta_bundle(scc)
                if _scc_.count_fences < bounds['max_fences']:
                    _scc_ = None # no bound proceed normally
                else:
                    scc_iter_fences = _scc_.fences[0:bounds['max_fences']] # _scc_.fences at [0, 1, ..., fence_bound-1]
                    scc = set(_scc_.events + scc_iter_fences) # 1st set all_events + 1st combination of fences
                    idx = list(range(bounds['max_fences']))  # [0, 1, ..., fence_bound-1]
        
        else:
            scc_iter_fences = next_fences(_scc_.count_fences, bounds['max_fences'], idx, _scc_.fences)
            if scc_iter_fences == []: # no more combinations
                _scc_ = None
                continue
            scc = set(_scc_.events + scc_iter_fences)
        
        cycles += simple_cycles_scc(scc, subG)
        
        # done processing this node
        H = subG.subgraph(scc)  # make smaller to avoid work in SCC routine
        sccs.extend(scc for scc in nx.strongly_connected_components(H) if len(scc) > 1)

    return cycles

def parallel_cycles_sccs_aux(scc, subG):
    _scc_ = None
    idx = []
    cycles = []

    if essential: # essential edge (every cycle must have this edge) is provided
        if len([edge for edge in essential if not edge[0] in scc or not edge[1] in scc]) > 0:
            # if essential edges(s) are missing from scc
            return [],[]
        
    if flags['fence_bound']:
        _scc_ = scc_meta_bundle(scc)
        if _scc_.count_fences < bounds['max_fences']:
            _scc_ = None # no bound proceed normally
        else:
            scc_iter_fences = _scc_.fences[0:bounds['max_fences']] # _scc_.fences at [0, 1, ..., fence_bound-1]
            scc = set(_scc_.events + scc_iter_fences) # 1st set all_events + 1st combination of fences
            idx = list(range(bounds['max_fences']))  # [0, 1, ..., fence_bound-1]

    while True:
        scc_cycles = simple_cycles_scc(scc, subG)
        cycles += scc_cycles
        
        if _scc_ == None:
            break

        scc_iter_fences = next_fences(_scc_.count_fences, bounds['max_fences'], idx, _scc_.fences)
        if scc_iter_fences == []: # no more combinations
            break

        scc = set(_scc_.events + scc_iter_fences)

    # # done processing this node
    H = subG.subgraph(scc)  # make smaller to avoid work in SCC routine
    new_sccs = [scc for scc in nx.strongly_connected_components(H)]

    return cycles, new_sccs

def parallel_cycles_sccs(sccs, subG):
    global parallel_cycles_sccs_aux
    cycles = []
    new_sccs = []
    
    pool = mp.Pool(mp.cpu_count())
    while True:
        for scc in sccs:
            cyc, new_sccs_ = pool.apply(parallel_cycles_sccs_aux, args=(scc, subG))
            cycles += cyc
            new_sccs += new_sccs_

        if not new_sccs:
            break
        sccs = new_sccs
        new_sccs = []

    pool.close()
    pool.join()

    return cycles

# @not_implemented_for("undirected")
def simple_cycles(G, F, B, Es=None):
    """Find simple cycles (elementary circuits) of a directed graph.

    A `simple cycle`, or `elementary circuit`, is a closed path where
    no node appears twice. Two elementary circuits are distinct if they
    are not cyclic permutations of each other.

    This is a nonrecursive, iterator/generator version of Johnson's
    algorithm [1]_.  There may be better algorithms for some cases [2]_ [3]_.

    Parameters
    ----------
    G : NetworkX DiGraph
       A directed graph
    flags : Fensying flags
    bounds : Fensying bounds
    essential : all returned cycles must have these edges

    Returns
    -------
    list of cycles: each cycle is list of graph nodes
    """

    global essential
    global flags
    global bounds

    essential = Es
    flags  = F
    bounds = B

    if type(essential) == tuple:
        essential = [essential]

    # Johnson's algorithm requires some ordering of the nodes.
    # We assign the arbitrary ordering given by the strongly connected comps
    # There is no need to track the ordering as each node removed as processed.
    # Also we save the actual graph so we can mutate it. We only take the
    # edges because we do not want to copy edge and node attributes here.
    subG = type(G)(G.edges())
    sccs = [scc for scc in nx.strongly_connected_components(subG) if len(scc) > 1]
    
    # NOTE: G will not have single length cycles

    if flags['parallel']:
        cycles = parallel_cycles_sccs(sccs, subG)
    else:
        cycles = simple_cycles_sccs(sccs, subG)

    return cycles