from os import write
from constants import *

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