
# coding: utf-8

# In[118]:


import math

def cost(pos, option):
    if option == 0:
        return rast_cost(pos);
    elif option == 1:
        return sph_cost(pos);
    elif option == 2:
        return ack_cost(pos);
    elif option == 3:
        return eggholder_cost(pos);

def rast_cost(pos):
    #Returns the cost of each particle based on Rastrigin function.
    #Param: pos: The position of the particle.
    #Return: val: The cost of the particle.
    val = 10*len(pos);
    
    for item in pos:
        val += item**2 - 10*math.cos(2*math.pi*item);
    
    return val;

def sph_cost(pos):
    #Return the cost of each particle based on Sphere function.
    #Param: pos: The position of the particle.
    #Return: val: The cost of the particle.
    
    val = 0;
    
    for item in pos:
        val += item**2;
    
    return val;


def ack_cost(pos):
    #Return the cost of each particle based on Ackley function.
    #Param: pos: The position of the particle.
    #Return: val: The cost of the particle.
 
    val = -20*math.exp(-0.2*math.sqrt(0.5*(pos[0]**2+pos[1]**2))) -     math.exp(0.5*(math.cos(2*math.pi*pos[0]) + math.cos(2*math.pi*pos[1]))) + math.e + 20;
    
    return val;

def eggholder_cost(pos):
    #Return the cost of each particle based on Ackley function.
    #Param: pos: The position of the particle.
    #Return: val: The cost of the particle.

    return -(pos[1]+47)*math.sin(math.sqrt(math.fabs((pos[0]/2)+pos[1]+47)))             - pos[0]*math.sin(math.sqrt(math.fabs(pos[0]-pos[1]+47)));


# In[119]:



import numpy as np
import numpy.random

class ParticleSwarmOptimiser:
    def __init__(self, pos, option, chi=0.73, p_phi=2, g_phi=2):
        
        self.chi = chi;
        self.p_phi = p_phi;
        self.g_phi = g_phi;
        self.option = option;
        
        self.popn = len(pos);
        self.dim = len(pos[0]);
        
        self.pos = pos;
        
        self.vel = np.random.uniform(size=(self.popn, self.dim));
        
        self.p_best = self.pos.copy();
        
        self.fit=[];
        for i in range(0, len(self.pos)):
            self.fit.append(cost(self.pos[i], option));
            
        self.g_best = self.p_best[self.fit.index(min(self.fit))];
        self.best_fit = min(self.fit);
        
        
    def swarm_improve(self):
        p_random = np.random.uniform(size=(self.popn, self.dim));
        g_random = np.random.uniform(size=(self.popn, self.dim));
        
        self.pos = np.array(self.pos);
        self.vel = np.array(self.vel);
        self.p_best = np.array(self.p_best);
        self.g_best = np.array(self.pos);
        
        self.vel = self.chi * (self.vel + self.p_phi * p_random * (self.p_best - self.pos)                               + self.g_phi * g_random * (self.g_best - self.pos));
        
        self.pos = self.pos + self.vel;
        
        self.pos = self.pos.tolist();
        self.vel = self.vel.tolist();
        self.p_best = self.p_best.tolist();
        self.g_best = self.g_best.tolist();
            
        de = DifferentialEvolution(self.pos, self.option);
        self.pos = de.optimise();
            
        for i in range(0, len(self.pos)):
            curr_fit = cost(self.pos[i], self.option)
            if curr_fit < self.fit[i]:
                self.p_best[i] = self.pos[i].copy();
                self.fit[i] = curr_fit;
            
        self.g_best = self.p_best[self.fit.index(min(self.fit))];
        self.best_fit = min(self.fit);
    
    def optimise(self, tol=1e-6, max_iter=500):
        iterate = 0;
        self.rate = [];
        while iterate < max_iter:
            self.swarm_improve();
            #print(self.best_fit);
            self.rate.append(self.best_fit);
            iterate = iterate + 1;



# In[120]:



import numpy as np;
import numpy.random;
import random;

class DifferentialEvolution:
    def __init__(self, pos, option, cr=0.15, mut=0.2):
        self.cr = cr;
        self.mut = mut;
        self.pos = pos;
        self.option = option;
    
        self.upp_val = np.zeros(shape=len(pos[0]));
        self.low_val = self.upp_val.copy();
    
        for i in range(0, len(pos[0])):
            self.upp_val[i] = float('-inf');
            self.low_val[i] = float('inf');
            for j in range(0, len(pos)):
                arr = self.pos[j]
                if self.upp_val[i] < arr[i]:
                      self.upp_val[i] = arr[i];
                if self.low_val[i] > arr[i]:
                      self.low_val[i] = arr[i];
          
    
    def mutation(self):
    
        mut_gen = [];
    
        for i in range(0, len(self.pos)):
            curr_part = self.pos.pop(i);
            
            random_part = np.array(random.sample(self.pos, 3));
            for j in range(0,3):
                random_part[j] = np.array(self.pos[j]);
        
            self.pos.insert(i, curr_part);
      
            mut_gen.append((random_part[0]+self.mut*(random_part[1] - random_part[2])).tolist());
    
        return mut_gen;
  

    def recombination(self, mut_gen):

        next_gen = [];

        for i in range(0, len(self.pos)):
            next_gen_param = [];
            rand_i = random.randint(1, len(self.pos[i]));

            for j in range(0, len(self.pos[i])):
                if random.randint(0,1)<=self.cr or j == rand_i:
                      next_gen_param.append(mut_gen[i][j]);
                else:
                      next_gen_param.append(self.pos[i][j]);

            next_gen.append(next_gen_param);

        return next_gen;

    
    def selection(self, next_gen):
        final_gen = [];
        
        for i in range(0, len(self.pos)):

            if cost(next_gen[i], self.option) < cost(self.pos[i], self.option):
                final_gen.append(next_gen[i]);
            else:
                final_gen.append(self.pos[i]);

        return final_gen;
    
    def optimise(self):
        mut_gen = self.mutation();
        next_gen = self.recombination(mut_gen);
        return self.selection(next_gen);



# In[117]:


if __name__ == '__main__':
    import random
    import matplotlib.pyplot as plt
    
    popn = 500;
    
    part = [];
    for i in range(0, popn):
        pos = [];
        for j in range(0, 2):
            pos.append(random.uniform(-5.12, 5.12));
        part.append(pos);
    
    pso = ParticleSwarmOptimiser(part, 0);
    pso.optimise();
    
    print([pso.g_best, pso.best_fit]);
    
    plt.plot(pso.rate[2:]);
    plt.ylabel('Eggholder function');
    plt.show();

