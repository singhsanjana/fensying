# import argparse
# import os
# import fileinput

# parser = argparse.ArgumentParser()
# parser.add_argument("--file", "-f", type=str, required=True)

# args = parser.parse_args()
# filename = args.file

from operator import itemgetter

class insert:
	def __init__(self,loc,filename):
		self.loc = loc
		self.filename = filename

		self.sort_locs()
		self.insert_fences()

	def sort_locs(self):
		
		self.loc = sorted(self.loc,key=itemgetter('thread','no_in_thread'))
		print("Number of fences inserted:",len(self.loc))
		# print("req locs=",self.loc)


	def insert_fences(self):

		with open(self.filename) as f:
			lines = f.readlines()
			# lines = [line.strip() for line in f]

		self.filename = self.filename[:-3]
		filename_new = self.filename+'_fence.cc'
		temp = open(filename_new,'w')
		fence_instr = 'atomic_thread_fence(memory_order_seq_cst);\n'

		# thread_order = []
		k = -1
		line_no = 0
		in_thread = 0
		curly = 0																	# check for and avoid curly braces
		for w in range(len(self.loc)):
			# new = {}
			location = self.loc[w]
			t = location['thread']-1
			fn = 'void t'+str(t)
			fence_line = location['no_in_thread']

			for i in range(line_no,len(lines)):
				line = lines[i]
				temp.writelines(line)

				if in_thread:
					if '{' in line:
						curly += 1
					elif '}' in line:
						curly -= 1

					k += 1

					if curly == 0:
						k = -1
						in_thread = 0
						t += 1
						continue

					# new[k] = line

					if k == fence_line:
						temp.writelines(fence_instr)
						line_no = i+1
						break

				if fn in line:
					in_thread = 1

		for w in range(line_no,len(lines)):
			temp.writelines(lines[w])