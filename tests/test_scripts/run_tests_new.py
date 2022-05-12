from ast import Pass
from cgi import test
# from cProfile import run
import os
import subprocess
import sys
import glob
import enum
from math import inf

class Test_Params:
    res_dir = 'tests/test_scripts/result'
    test_dirs = [
        'tests/benchmarks/VBMCbench/configs/extra'
    ]

    N = 5       # no. of runs per tests
    T = [1, inf]  # values to be passed to -t flags. should be in increasing order
    Parallel = False

class Timeouts:
    ## if these values are changed, need to change the corresponding values in constants.py
	mc   = 900   # model checking  timeout of 15 minutes
	tool = 900   # tool processing timeout of 15 minutes (excludes time of model checker and SAT solver)

class Status(enum.Enum):
    Unknown = 'Unknown'
    CanNotFix = 'Abort'
    MCTO = 'MCTO'
    FTO = 'FTO'
    NoBug = 'NOBUG'
    Fail = 'FAIL'
    OK = 'OK'


class Test_Stats:
    def __init__(self, filepath, t=inf, d=inf, f=inf) -> None:
        self.total_synthesized = 0
        self.min_synthesized = inf
        self.max_synthesized = 0
        self.total_strengthened  = 0
        self.min_strengthened  = inf
        self.max_strengthened  = 0
        self.total_iterations = 0
        self.min_iterations = inf
        self.max_iterations = 0
        self._time_ceg      = ''
        self._time_z3       = ''
        self._time_fensying = ''
        self._time_total    = ''
        self.num_finished = 0
        self.num_mcto = 0
        self.num_fto = 0

        self._status = Status.Unknown
        self._aborted = False            # could bot fix test (execution successful)
        self._result_generated = False   # execution success

        self.execute_test(filepath, t, d, f)
        self.compute_final_result()


    def add(self, val1, val2):
        if val1 == '':
            return val2
        if val2 == '':
            return val1
        return str(float(val1) + float(val2))


    def avg(self, val, num=Test_Params.N):
        if val == '':
            return '0'
        return str( round(float(val) / num, 2) )


    def compute_total_times(self, time_ceg, time_z3, time_fensying):
        self._time_ceg = self.add(self._time_ceg, time_ceg)
        self._time_z3  = self.add(self._time_z3, time_z3)
        self._time_fensying = self.add(self._time_fensying, time_fensying)
        return


    def compute_total_fences(self, synthesized, strengthened, iterations):
        # take sum of synthesized and strengthened for all itr
        if synthesized == '' or strengthened == '':
            print('got empty string for #fence')
            exit(0)
        else:
            self.total_synthesized = self.total_synthesized + int(synthesized)
            self.min_synthesized = min(self.min_synthesized, int(synthesized))
            self.max_synthesized = max(self.max_synthesized, int(synthesized))
            self.total_strengthened = self.total_strengthened + int(strengthened)
            self.min_strengthened = min(self.min_strengthened, int(strengthened))
            self.max_strengthened = max(self.max_strengthened, int(strengthened))
            if iterations != '':
                self.total_iterations = self.total_iterations + int(iterations)
                self.min_iterations = min(self.min_iterations, int(iterations))
                self.max_iterations = max(self.max_iterations, int(iterations))
        return


    def update_status(self, status):
        """Compute the new values of status, abort and result generated based on older runs and current run"""
        self._aborted = self._aborted or self._status == Status.CanNotFix
        self._result_generated = self._result_generated or status == Status.OK or status == Status.CanNotFix or status == Status.MCTO
        # if program has already run once then status is older one
        if self._status == Status.Unknown and status == Status.OK:
            self._status = Status.OK
        elif status == Status.MCTO:
            self._status = Status.MCTO
        elif status == Status.FTO:
            self._status = Status.FTO
        elif status == Status.CanNotFix or status == Status.NoBug or status == Status.Fail:
            print('update status should not have been called. _status=', self._status, ' status=', status)
            sys.exit(0)
        return


    def compute_final_result(self):
        # self.print_stats()
        if self._result_generated:
            self._time_ceg = self.avg(self._time_ceg)
            self._time_z3  = self.avg(self._time_z3)
            self._time_fensying = self.avg(self._time_fensying)
            self._time_total = self.add(self._time_ceg, self.add(self._time_z3, self._time_fensying))
            self.total_synthesized = self.avg(self.total_synthesized, self.num_finished + self.num_mcto)
            self.total_strengthened = self.avg(self.total_strengthened, self.num_finished + self.num_mcto)
            self.total_iterations = self.avg(self.total_iterations, self.num_finished + self.num_mcto)


    def execute_test(self, filepath, t, d, f):
        for i in range(Test_Params.N):
            run_once = Run_Test(filepath, t, d, f)
            further_run, status, synthesized, strengthened, iterations, time_ceg, time_z3, time_fensying = run_once.get()
            
            if not further_run:
                self._status = status
                return

            if status == Status.MCTO:
                self.compute_total_times(Timeouts.mc, time_z3, time_fensying)
                self.update_status(status)
                self.compute_total_fences(synthesized, strengthened, iterations)
                self.num_mcto += 1
            elif status == Status.FTO:
                self.compute_total_times(time_ceg, Timeouts.tool, time_fensying)
                self.update_status(status)
                self.num_fto +=1
            else:
                self.compute_total_times(time_ceg, time_z3, time_fensying)
                self.update_status(status)
                self.compute_total_fences(synthesized, strengthened, iterations)
                self.num_finished += 1


    def get_string(self):
        # self.print_stats()
        return self._status.name + ',' + str(self.total_synthesized) + ',' \
                + str(self.min_synthesized) + ',' \
                + str(self.max_synthesized) + ',' \
                + str(self.total_strengthened) + ',' \
                + str(self.min_strengthened) + ',' \
                + str(self.max_strengthened) + ',' \
                + str(self.total_iterations) + ',' \
                + str(self.min_iterations) + ',' \
                + str(self.max_iterations) + ',' \
                + self._time_ceg + ',' \
                + self._time_z3 + ',' \
                + self._time_fensying + ',' \
                + self._time_total + ',' \
                + str(self.num_finished) + ',' \
                + str(self.num_mcto) + ',' \
                + str(self.num_fto) + ','

    def print_stats(self):
        print('STATS :', end='')
        print('status: ', self._status.name,            \
            'tot_syn:', str(self.total_synthesized),    \
            'min_syn:', str(self.min_synthesized),      \
            'max_syn:', str(self.max_synthesized),      \
            'tot_str:', str(self.total_strengthened),   \
            'min_str:', str(self.min_strengthened),     \
            'max_str:', str(self.max_strengthened),     \
            'tot_itr:', str(self.total_iterations),     \
            'min_itr:', str(self.min_iterations),       \
            'max_itr:', str(self.max_iterations),       \
            'ceg:'    , self._time_ceg,                 \
            'z3:'     , self._time_z3,                  \
            'fen:'    , self._time_fensying,            \
            'tot:'    , self._time_total,               \
            '#fin:'   , str(self.num_finished),         \
            '#mcto:'  , str(self.num_mcto),             \
            '#fto:'   , str(self.num_fto))




class Run_Test:
    """Runs and computes the results of a single run"""
    def __init__(self, filepath, t, d, f) -> None:
        self.status = Status.OK
        self.synthesized   = ''
        self.strengthened  = ''
        self.iterations    = ''
        self.time_ceg      = ''
        self.time_z3       = ''
        self.time_fensying = ''
        
        self.result_generated = False
        self.further_runs = True

        self.run_test_once(filepath, t, d, f)

        
    def read_result(self, lines):
        i=0
        for i in range(len(lines)):
            if 'ABORT' in lines[i]:
                self.status = Status.CanNotFix
            if 'Model Checking time exceeded' in lines[i]:
                self.status = Status.MCTO
            if 'Tool time exceeded' in lines[i]:
                self.status = Status.FTO
                break
            if 'time_handler' in lines[i]:
                self.status = Status.FTO
                break
            if 'RuntimeError' in lines[i]:
                self.status = Status.FTO
                break
            if 'No buggy traces. Nothing to do.' in lines[i]:
                self.status = Status.NoBug
                break
            if 'RESULT SUMMARY' in lines[i]:
                self.result_generated = True
                break
        
        if i == len(lines)-1:   # either FTO or NUBUG or result not generated
            if not self.result_generated:
                self.status = Status.Fail
            return

        for j in range(i,len(lines)): # remaining lines after line containing 'RESULT SUMMARY'
            words = lines[j].split()
            if len(words) == 0: # empty line
                continue
            if 'synthesized:' in words:
                self.synthesized = words[5]
                continue
            if 'strengthened:' in words:
                self.strengthened = words[5]
                continue
            if 'BTG:' in words and 'Avg.' not in words :
                self.time_ceg = words[2]
                continue
            if 'Z3:' in words and 'Avg.' not in words :
                self.time_z3 = words[2]
                continue
            if 'Fensying:' in words and 'Avg.' not in words :
                self.time_fensying = words[2]
                continue   
            if 'Total' in words and 'iterations:' in words:
                self.iterations = int(words[2])
                continue
        return
    

    def run_test_once(self, filepath, t, d, f):
        process_command = ['python3', 'main.py', filepath]
        if t != inf:
            process_command.append('-t')
            process_command.append(str(t))
        if d != inf:
            process_command.append('-d')
            process_command.append(str(d))
        if f != inf:
            process_command.append('-f')
            process_command.append(str(f))
        if Test_Params.Parallel:
            process_command.append('-p')
        print('executing: ', process_command)

        process = subprocess.Popen(process_command, stdout=subprocess.PIPE, bufsize=1, universal_newlines=True, stderr=subprocess.STDOUT)
        lines = process.stdout.readlines()
        self.read_result(lines)
        self.print_results()

        if self.status == Status.NoBug:
            self.further_runs = False
        elif self.status == Status.Fail:
            self.further_runs = False
            # return 'STOP', 'Failed to run.' + ',,,,,,,,,,,,,,'
        elif self.status == Status.CanNotFix:
            self.further_runs = False
            # return 'STOP', 'Cannot fix.' + ',,,,,,,,,,,,,,'
        elif t == inf and (self.status == Status.MCTO or self.status == Status.FTO):
            self.further_runs = False
        return
    

    def print_results(self):
        print('RESULTS : ', end='')
        print('status:', self.status.name, 'syn:', self.synthesized, 'str:', self.strengthened, 'itr:', self.iterations, 'ceg', self.time_ceg, 'z3:', self.time_z3, 'fen:', self.time_fensying)

    
    def get(self):
        return self.further_runs, self.status, self.synthesized, self.strengthened, self.iterations, self.time_ceg, self.time_z3, self.time_fensying


def run_all_config(filename):
    os.chdir(cwd + '/src')
    csv_row = ''
    t_succ = True

    # for t=1 and inf
    for i in range(len(Test_Params.T)): 
        # config1: no fence and depth bounding
        t_conf = Test_Params.T[i]
        exec_test = Test_Stats(filename, t=t_conf)
        test_cols = exec_test.get_string()
        print('test_cols:', test_cols)
        csv_row += test_cols
    os.chdir(cwd)
    csv_row += '\n'
    return csv_row
        

def write_csv_header(csv_file_name):
    csv_header = 'Test Name,'
    for t in Test_Params.T:
        # for no fence and depth bounding
        csv_header_t = 'status, avg_synthesized, min_synthesized, max_synthesized, avg_strengthened, min_strengthened, max_strengthened,avg_itr, min_itr, max_itr, Time-BTG, Time-Z3, Time-fensying, Time-total,#num_succ, #num_mcto, #num_fto,'
        csv_header_t = csv_header_t.replace(',', 
                        '(t=' + str(t) + '),')
        csv_header += (csv_header_t)
    csv_header = csv_header[:-1] + '\n'

    csv_file = open(os.path.join(Test_Params.res_dir, csv_file_name), 'w')
    csv_file.write(csv_header)
    csv_file.close()
    return

def run_single_test(dir_path, file):
    print('Testing ' + dir_path + '/' + file[:-2])
    filepath = '../' + os.path.join(dir_path, file)

    input_file = glob.glob(dir_path + '/' + file[:-2]+'.c*')[0]
    input_ext  = input_file[ input_file.rfind('.')+1 : ]

    f = filepath + '$' + input_ext

    csv_row = file + ',' + run_all_config(f)
    return csv_row

def test_dir(dir_path, csv_file_name='', create_new_csv=True):
    if create_new_csv:
        csv_file_name = 'result-' + str(dir_path).replace('/','_') + '.csv'
        write_csv_header(csv_file_name)
    for file in os.listdir(dir_path):
        if os.path.isdir(os.path.join(dir_path, file)):
            test_dir(os.path.join(dir_path, file), csv_file_name, False)
        if file[-2:] != '.o':
            continue 
        if '_fixed' in file:
            continue
              
        if csv_file_name == '':
            print('Something went wrong with result file name')
            exit(0)
        csv_file = open(os.path.join(Test_Params.res_dir, csv_file_name), 'a')
        full_path = str(dir_path).replace('/','_')
        test_name_for_csv = full_path[len(csv_file_name[len('result-'):-4]):]
        csv_row = run_single_test(dir_path, file)
        csv_row = test_name_for_csv + "/" + csv_row
        csv_row = csv_row[:-1] + '\n'
        csv_file.write(csv_row)
        csv_file.close()
    return 

cwd = os.getcwd()
    
if not os.path.exists(Test_Params.res_dir):
    os.system('mkdir ' + Test_Params.res_dir)

for dir in Test_Params.test_dirs:
    print('\nENTERING ' + dir)
    test_dir(dir)
    print('LEAVING ' + dir)

