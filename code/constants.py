# memory orders
SEQ_CST = "seq_cst"
REL = "release"
ACQ = "acquire"
ACQ_REL = "acq_rel"
RLX = "relaxed"

# instruction types and thread operations
RMW = "rmw"
WRITE = "write"
INIT = "init"
READ = "read"
FENCE = "fence"
JOIN = "join"
CREATE = "create"
START = "start"
FINISH = "finish"

# instruction indices
S_NO = 0					# instruction serial number
T_NO = 1					# instruction thread number
TYPE = 2					# instruction type or thread operation
MO	= 3						# memory order
ADDR = 4					# memory address that instr is accessing
VALUE = 5					# value of variable/thread in the instruction
RF = 6						# S_NO of source write
LINE_NO = 7					# line number in source code
CV = 8						# clock vector representing hb relation

# models
strong_write_models = [REL, ACQ_REL, SEQ_CST]
strong_read_models = [ACQ, ACQ_REL, SEQ_CST]

# fence tags
class f_tags:
	a = 'a'
	r = 'r'
	ar = 'ar'
	sc = 'sc'

# files and CDS Checker related constants
class file_info:
	Z3_FILE = "compute_fences"
	FILE_EXTENSION = ".cc"
	CDS_MADE_FILE_EXTENSIONS = [".o", ".o.d"]
	OUTPUT_FILE_APPEND_STRING = "_fenced"+FILE_EXTENSION
	FENCE_INSTRUCTIONS = {
		f_tags.sc: "atomic_thread_fence(memory_order_seq_cst);\n",
		f_tags.a: "atomic_thread_fence(memory_order_acquire);\n",
		f_tags.r: "atomic_thread_fence(memory_order_release);\n",
		f_tags.ar: "atomic_thread_fence(memory_order_acq_rel);\n"
	}
	CDS_FOLDER_PATH = "../model-checker/cds-checker"
	CDS_TEST_FOLDER_PATH = CDS_FOLDER_PATH+"/test"

class output_colours:
	HEADER = '\033[95m'
	OKBLUE = '\033[94m'
	OKGREEN = '\033[92m'
	WARNING = '\033[93m'
	FAIL = '\033[91m'
	ENDC = '\033[0m'
	BOLD = '\033[1m'
	UNDERLINE = '\033[4m'

def time_handler(signum, frame):
	raise RuntimeError