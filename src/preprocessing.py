from os import write
from constants import *

def preprocessing_new(order,fence_list):
  reads = []
  writes = []

  for i in range(len(order)):
    if order[i][TYPE] == INIT: # initial value (considered as a write), does not need fences
      writes.append(order[i])

    if order[i][TYPE] == WRITE or order[i][TYPE] == RMW:
      if order[i][LINE_NO] in fence_list:
        # print(order[i])
        if not (len(writes) > 0 and writes[-1] == order[i-1]): # not (common fence between this and previous write)
          writes.append(order[i-1]) # fence before write
        writes.append(order[i])     # write
        writes.append(order[i+1])   # fence after write
      else:
        writes.append(order[i])

    if order[i][TYPE] == READ or order[i][TYPE] == RMW:
      if order[i][LINE_NO] in fence_list:
        # print(order[i])
        if not (len(reads) > 0 and reads[-1] == order[i-1]): # not (common fence between this and previous read)
          reads.append(order[i-1]) # fence before read
        reads.append(order[i])     # read
        reads.append(order[i+1])   # fence after read
      else:
        reads.append(order[i])
  
  return reads, writes

def preprocessing(order):
  reads = []
  writes = []

  for i in range(len(order)):
    if order[i][TYPE] == INIT: # initial value (considered as a write), does not need fences
      writes.append(order[i])

    if order[i][TYPE] == WRITE or order[i][TYPE] == RMW:
      if not (len(writes) > 0 and writes[-1] == order[i-1]): # not (common fence between this and previous write)
        writes.append(order[i-1]) # fence before write
      writes.append(order[i])     # write
      writes.append(order[i+1])   # fence after write

    if order[i][TYPE] == READ or order[i][TYPE] == RMW:
      if not (len(reads) > 0 and reads[-1] == order[i-1]): # not (common fence between this and previous read)
        reads.append(order[i-1]) # fence before read
      reads.append(order[i])     # read
      reads.append(order[i+1])   # fence after read
  
  return reads, writes

  