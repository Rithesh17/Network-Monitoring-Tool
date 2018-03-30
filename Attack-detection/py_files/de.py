
import numpy as np;
import numpy.random;
import random;

class DifferentialEvolution:
  def __init__(self, n_net, cr=0.15, mut=0.2):
    self.cr = cr;
    self.n_net = n_net;
    self.mut = mut;

    self.upp_val = np.zeros(shape=len(n_net[0])).tolist();
    self.low_val = self.upp_val.copy();

    for i in range(0, len(n_net[0])):
      self.upp_val[i] = float('-inf');
      self.low_val[i] = float('inf');
      for j in range(0, len(n_net)):
        if self.upp_val[i] < self.n_net[j][i]:
          self.upp_val[i] = self.n_net[j][i];
        if self.low_val[i] > self.n_net[j][i]:
          self.low_val[i] = self.n_net[j][i];


  def mutation():

    mut_gen = [];

    for i in range(0, len(self.n_net)):
      curr_part = self.n_net.pop(i);

      random_part = np.array(random.sample(self.n_net, 3));
      for j in range(0,3):
        random_part[j] = np.array(random_part[j].getPosition());

      self.n_net.insert(i, curr_part);

      mut_gen.append((random_part[0]+mut*(random_part[1] - random_part[2])).tolist());

    return mut_gen;


  def recombination(mut_gen):

    next_gen = [];

    for i in range(0, len(self.n_net)):
      next_gen_param = [];
      rand_i = random.randint(1, len(self.n_net[i]));

      for j in range(0, len(self.n_net[i])):
        if random.randint(0,1)<=self.cr or j == rand_i:
          next_jen_param.append(mut_gen[i][j]);
        else
          next_gen_param.append(self.n_net[i][j]);

      next_gen.append(next_gen_param);

    return next_gen;


  def selection(next_gen):
    final_gen = [];
    new_gen = NeuralNetwork(n_net.nodes);
    for i in range(0, len(self.n_net)):
      new_gen.setPosition(next_gen[i]);

      if new_gen.cost < self.n_net[i].cost:
        final_gen.append(new_gen);
      else
        final_gen.append(self.n_net[i]);

    return final_gen;

  def optimise(self):
    mut_gen = mutation();
    next_gen = recombination(mut_gen);
    return selection(next_gen);
