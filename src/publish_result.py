from constants import ADDR, MO, RF, S_NO, T_NO, TYPE, VALUE, output_colours as oc
from constants import file_info as fi
from prettytable import PrettyTable
from tabulate import tabulate


def iteration_result_summary(mc_time, z3_time, count_added_fences, count_modified_fences):
    print("Time- CEG:\t\t",round(mc_time, 2))        # CDS time + relations computation time
    print("Time- Z3:\t\t",round(z3_time, 2))
    print("Fences synthesized: \t",count_added_fences)
    print("Fences strengthened:\t",count_modified_fences)

def pretty_mo(mo):
    if mo == 'r':
        return 'release'
    if mo == 'a':
        return 'acquire'
    if mo == 'ar':
        return 'acq-rel'
    return 'seq_cst'

def pretty_pos(pos):
    attr = pos.split('_') # 'F', (before/after/at), line no.
    return attr[1] + ' line no. ' + attr[2]
    

def synthesis_summary(fences_and_tags_by_files):
    pt = PrettyTable()
    print (oc.OKBLUE + oc.BOLD + "\n\n================= SYNTHESIS SUMMARY =================" + oc.ENDC)
    pt.field_names = ['FILENAME', 'POSITION', 'M.O.', 'TYPE']

    for file in fences_and_tags_by_files:
        mod_list = []

        for (fence_pos, mo) in fences_and_tags_by_files[file]:
            if fence_pos.startswith('at'):
                mod_list.append( (fence_pos, mo) )
                continue

            pt.add_row([file, pretty_pos(fence_pos), pretty_mo(mo), 'synthesized'])

        for (fence_pos, mo) in mod_list:
            pt.add_row([file, pretty_pos(fence_pos), pretty_mo(mo), 'strengthened'])

    print(pt)

def final_result_summary(total_time, mc_time, z3_time, count_added_fences, count_modified_fences, 
                        batching, total_iterations, print_synthesis_summary, fences_and_tags):
    print(oc.OKBLUE + oc.BOLD + "\n\n================= RESULT SUMMARY =================" + oc.ENDC)
    
    print(oc.OKGREEN, oc.BOLD, "Total fences synthesized:  \t", count_added_fences, oc.ENDC)
    print(oc.OKGREEN, oc.BOLD, "Total fences strengthened: \t", count_modified_fences, oc.ENDC)

    print("Time- CEG:\t\t",round(mc_time, 2)) # CDS time + relations cpmputation time
    print("Time- Z3:\t\t",round(z3_time, 2))
    # print("\nTime- Total:\t\t",round(total_time, 2))
    print("Time- Fensying:\t\t",round(total_time - (mc_time + z3_time), 2), '\n\n')
    if batching:
        print("Total iterations:\t",total_iterations)
        print("Time- avg per iter:\t",round(total_time/total_iterations, 2), '\n\n')

    if print_synthesis_summary:
        synthesis_summary(fences_and_tags)

    if z3_time > 0:
        modified_files = list(fences_and_tags.keys())
        fixed_files = []

        for i in range(len(modified_files)):
            filename = modified_files[i]
            fenced_filename = filename[:filename.rfind('.')] + fi.OUTPUT_FILE_APPEND_STRING
            fixed_files.append(fenced_filename)

        if len(fixed_files) == 1:
            print(oc.OKBLUE, oc.BOLD, 'Fixed input file(s) at:', fixed_files[0], '\n', oc.ENDC)
            return
        print(oc.OKBLUE, oc.BOLD, 'Fixed input file(s) at:', str(fixed_files).replace("'", ""), '\n', oc.ENDC)

def print_trace(trace):
    for i in range(len(trace)):
        event = trace[i]
        if event[TYPE] == 'read' or event[TYPE] == 'rmw':
            trace[i] = event[:7]
        elif event[TYPE] == 'write':
            trace[i] = event[:6] + ['']
        else:
            trace[i] = event[:5] + ['', '']
        trace[i].append(event[9])

    header_values = ['#', 'Thread', 'Type', 'MO', 'Location', 'Value', 'RF', 'HB']
    print(tabulate(trace, headers=header_values))