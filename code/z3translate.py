import ast
import z3_functions as z3

class z3translate:

    def __init__(self, cycles):

        self.variables   = []             # list of formula variables ie fences
        conjunctions     = []             # list of all cycles, each cycle translated into a conjunction
        self.formula = ""
        
        self.variables = set([item for sublist in cycles for item in sublist]) # list of unique fences
        
        for cycle in cycles:
            conjunctions.append(z3.conjunct(cycle))

        self.formula = z3.disjunct(conjunctions)

        # print(self.variables, conjunctions, self.formula)

    def get(self):
        return self.variables, self.formula
