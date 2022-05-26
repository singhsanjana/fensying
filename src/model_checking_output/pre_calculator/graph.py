class Graph:
    def __init__(self, size):
        self.adjMatrix = []
        for i in range(size):
            self.adjMatrix.append([0 for i in range(size)])
        self.size = size
    def addEdge(self, v1, v2):
        self.adjMatrix[v1][v2] = 1
    def containsEdge(self, v1, v2):
        return True if self.adjMatrix[v1][v2] > 0 else False
    def __len__(self):
        return self.size
        
    def toString(self):
        for row in self.adjMatrix:
            print(row)