import sys
from turing_machine import *

if len(sys.argv) > 1:
    input_file = sys.argv[1]
else:
    input_file = "input.txt"

tm = TuringMachine(input_file)
tm.parseFile()
tm.makeTransitionTables()
tm.turingSimulator()

