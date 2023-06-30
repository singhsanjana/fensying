from cProfile import run
import os
import subprocess
import sys
import glob
from math import inf
import threading
from importlib_metadata import csv
import time


filename  = sys.argv[1]

with open(filename,'r') as file:
    test_list = file.read().replace("'","").strip().split(' ')

print(test_list)
for test in test_list:
    filename, ext = os.path.splitext(os.path.basename(test))
    print(filename)