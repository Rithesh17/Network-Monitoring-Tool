import numpy as np;
import numpy.random;
import sklearn.metrics;
import math;

class NeuralNetwork:
    def __init__(self, nodes, weights=None):
        self.nodes = nodes;
        if weights==None:
            self.weights=[];
            for i in range(0,len(nodes)-1):
                self.weights.append(np.random.uniform(size=(nodes[i]+1, nodes[i+1])).tolist());
            self.weights = self.weights;
        else:
            self.weights = weights;
        print(self.weights[0][0][0]);

    def propagate(self, input_data):
        level = np.array(input_data).T.tolist();

        for i in range(0, len(self.nodes)-1):
          level1=[];
          prev_level = np.insert(np.array(level), 0, 1, 0).tolist();
          for j in range(0, self.nodes[i+1]):
            hypo = 0;
            for k in range(0, len(prev_level)):
              hypo = hypo + self.weights[i][j][k] * prev_level[k];
            level1.append(1/(1+math.exp(-hypo)));
          level = level1.copy();
        return level;

    def setData(self, total_input_data, total_output_data):
        self.ip = total_input_data;
        self.op = total_output_data;

    def cost(self):
        cost_val = 0;
        for i in range(0, len(self.ip)):
            exp_output = self.propagate(self.ip[i]);
            cost_val = cost_val + sklearn.metrics.mean_squared_error(self.ip[i], exp_output);
        return cost_val;

    def setPosition(self, pos):
        p = 0;
        for i in range(0, len(nodes)-1):
            for j in range(0, nodes[i]+1):
                for k in range(0, nodes[i+1]):
                    self.weights[i][j][k] = pos[p].copy();
                    p=p+1;


    def getPosition(self):
        pos = [];
        for i in range(0, len(nodes)-1):
            for j in range(0, nodes[i]+1):
                for k in range(0, nodes[i+1]):
                    pos.append(self.weights[i][j][k]);

        return pos;
