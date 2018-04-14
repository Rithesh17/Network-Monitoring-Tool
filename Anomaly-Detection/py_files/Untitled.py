
# coding: utf-8

# In[1]:


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

    def propagate(self, input_data):
        level = np.array(input_data).T.tolist();

        for i in range(0, len(self.nodes)-1):
            level1=[];
            prev_level = np.insert(np.array(level), 0, 1, 0).tolist();
            #print([[len(self.weights), len(self.weights[0]), len(self.weights[0][0])], len(prev_level)]);
            for j in range(0, self.nodes[i+1]):
                hypo = 0;
                for k in range(0, len(prev_level)):
                    hypo = hypo + self.weights[i][k][j] * float(prev_level[k]);
                #print(hypo);
                if hypo >= 20:
                    level1.append(1);
                elif hypo <= -20:
                    level1.append(-1);
                else:
                    level1.append(1/(1+math.exp(-hypo)));
            level = level1[:];
        return level;

    def setData(self, total_input_data, total_output_data):
        self.ip = total_input_data;
        self.op = total_output_data;

    def cost(self):
        cost_val = 0;
        for i in range(0, len(self.ip)):
            exp_output = self.propagate(self.ip[i]);
            #print([self.op[i], exp_output]);
            cost_val = cost_val + (exp_output[0] - self.op[i])**2;
        cost_val = cost_val / len(self.op);
        return cost_val;

    def setPosition(self, pos):
        p = 0;
        for i in range(0, len(nodes)-1):
            for j in range(0, nodes[i]+1):
                for k in range(0, nodes[i+1]):
                    self.weights[i][j][k] = pos[p];
                    p=p+1;


    def getPosition(self):
        pos = [];
        for i in range(0, len(nodes)-1):
            for j in range(0, nodes[i]+1):
                for k in range(0, nodes[i+1]):
                    pos.append(self.weights[i][j][k]);

        return pos;


# In[2]:




import numpy as np
import numpy.random

class ParticleSwarmOptimiser:
    def __init__(self, n_net, ip, op, chi=0.73, p_phi=2, g_phi=2):
        self.n_net = n_net;
        self.ip = ip;
        self.op = op;

        self.chi = chi;
        self.p_phi = p_phi;
        self.g_phi = g_phi;

        self.popn = len(self.n_net);
        self.dim = len(self.n_net[0].getPosition());

        self.pos = [];
        for i in range(0, len(n_net)):
            self.pos.append(np.array(n_net[i].getPosition()));
        self.pos = np.array(self.pos);

        self.vel = np.random.uniform(size=(len(n_net), len(n_net[0].getPosition())));

        self.p_best = self.pos.copy();

        self.fit=[];
        for i in range(0, len(n_net)):
            self.fit.append(n_net[i].cost());

        self.g_best = self.p_best[self.fit.index(min(self.fit))];
        self.best_fit = min(self.fit);


    def swarm_improve(self):
        p_random = np.random.uniform(size=(self.popn, self.dim));
        g_random = np.random.uniform(size=(self.popn, self.dim));

        self.vel = self.chi * (self.vel + self.p_phi * p_random * (self.p_best - self.pos)                               + self.g_phi * g_random * (self.g_best - self.pos));

        self.pos = self.pos + self.vel;

        nn = [];

        for i in range(0, len(self.n_net)):
            nn.append(NeuralNetwork(self.n_net[0].nodes));
            nn[i].setData(self.ip, self.op);
            nn[i].setPosition(self.pos[i].tolist());

        de = DifferentialEvolution(nn, self.ip, self.op);
        nn = de.optimise();

        for i in range(0, len(self.n_net)):
            curr_fit = nn[i].cost();
            if curr_fit < self.fit[i]:
                self.p_best[i] = self.pos[i].copy();
                self.fit[i] = curr_fit;

        self.g_best = self.p_best[self.fit.index(min(self.fit))];
        self.best_fit = min(self.fit);

    def optimise(self, tol=0.0001, max_iter=20):
        iterate = 0;
        while self.best_fit > tol and iterate < max_iter:
            self.swarm_improve();
            print(self.best_fit);
            iterate = iterate + 1;


# In[3]:



import numpy as np;
import numpy.random;
import random;

class DifferentialEvolution:
    def __init__(self, n_net, ip, op, cr=0.15, mut=0.2):
        self.cr = cr;
        self.n_net = n_net;
        self.mut = mut;
        self.ip = ip;
        self.op = op;

        self.upp_val = np.zeros(shape=len(n_net[0].getPosition())).tolist();
        self.low_val = self.upp_val[:];

        for i in range(0, len(n_net[0].getPosition())):
            self.upp_val[i] = float('-inf');
            self.low_val[i] = float('inf');
            for j in range(0, len(n_net)):
                arr = self.n_net[j].getPosition()
                if self.upp_val[i] < arr[i]:
                      self.upp_val[i] = arr[i];
                if self.low_val[i] > arr[i]:
                      self.low_val[i] = arr[i];


    def mutation(self):

        mut_gen = [];

        for i in range(0, len(self.n_net)):
            curr_part = self.n_net.pop(i);

            random_part = np.array(random.sample(self.n_net, 3));
            for j in range(0,3):
                random_part[j] = np.array(random_part[j].getPosition());

            self.n_net.insert(i, curr_part);

            mut_gen.append((random_part[0]+self.mut*(random_part[1] - random_part[2])).tolist());

        return mut_gen;


    def recombination(self, mut_gen):

        next_gen = [];

        for i in range(0, len(self.n_net)):
            next_gen_param = [];
            rand_i = random.randint(1, len(self.n_net[i].getPosition()));

            for j in range(0, len(self.n_net[i].getPosition())):
                if random.randint(0,1)<=self.cr or j == rand_i:
                      next_gen_param.append(mut_gen[i][j]);
                else:
                      next_gen_param.append((self.n_net[i].getPosition())[j]);

            next_gen.append(next_gen_param);

        return next_gen;


    def selection(self, next_gen):
        final_gen = [];
        new_gen = NeuralNetwork(self.n_net[0].nodes);
        new_gen.setData(self.ip, self.op);
        for i in range(0, len(self.n_net)):
            new_gen.setPosition(next_gen[i]);

            if new_gen.cost() < self.n_net[i].cost():
                final_gen.append(new_gen);
            else:
                final_gen.append(self.n_net[i]);

        return final_gen;

    def optimise(self):
        mut_gen = self.mutation();
        next_gen = self.recombination(mut_gen);
        return self.selection(next_gen);


# In[4]:




if __name__ == '__main__':

    import csv;
    import numpy as np;
    import sklearn.metrics;
    from sklearn.model_selection import train_test_split;
    from datetime import datetime;

    file_d = open('red2_corrected.csv', 'r');
    readl = csv.reader(file_d);

    data = []; target = [];

    for row in readl:
        data.append(row[0:-1]);
        target.append(row[-1]);

    target = [int(x) for x in target];

    train_in, test_in, train_out, test_out = sklearn.model_selection.train_test_split(data, target);

    popn = 100;

    n_net_pop = [];

    nodes = [len(train_in[0]), 8, 4, 1];

    startTime = datetime.now()
    
    for i in range(0, popn):
        nn = NeuralNetwork(nodes);
        nn.setData(train_in, train_out);
        n_net_pop.append(nn);

    pso = ParticleSwarmOptimiser(n_net_pop, train_in, train_out);

    i=0;
    best_val = [[i, pso.best_fit]];
    print([i,pso.best_fit]);

    pso.optimise();
    '''while i < 500 and pso.best_fit > 1e-6:
        pso.optimise();
        i=i+1;
        if pso.best_fit < best_val[-1][1]:
            best_val.append([i, pso.best_fit]);
            print([i, pso.best_fit]);'''

    print('Time taken to train model: ', datetime.now() - startTime);
    
    best_weight = pso.g_best;
    best_nn = NeuralNetwork(nodes);
    best_nn.setData(test_in, test_out);
    best_nn.setPosition(best_weight);

    predict_out = [];
    for i in range(0, len(test_out)):
        if best_nn.propagate(test_in[i])[0] >= 0.5:
            predict_out.append(1);
        else:
            predict_out.append(0);

    print (sklearn.metrics.classification_report(test_out, predict_out));
    
