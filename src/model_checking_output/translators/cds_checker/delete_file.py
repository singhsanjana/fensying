# --------------------------------------------------------
# Cleans up all generated files:
# 1. Generated file when -t optimization flag is used and
# 	the solution cannot be found after a few iterations
# 2. Copied and generated model checker files
# --------------------------------------------------------

import os
from constants import file_info as fi

def delete_generated_file(filename):
	if fi.OUTPUT_FILE_APPEND_STRING in filename:
		file_path = filename
		if os.path.exists(file_path):
			os.remove(file_path)