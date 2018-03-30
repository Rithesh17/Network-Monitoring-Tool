import numpy as np
import numpy.random

class ParticleSwarmOptimiser:
    def __init__(self, n_net, ip, op, chi=0.73, p_phi=2, g_phi=2):
        self.n_net = n_net;

        self.chi = chi;
        self.p_phi = p_phi;
        self.g_phi = g_phi;

        self.popn = len(self.n_net);
        self.dim = len(self.n_net[0]);

        self.pos = [];
        for i in range(0, len(n_net)):
            self.pos.append(np.array(n_net[i].getPosition()));
        self.pos = np.array(self.pos);

        self.vel = np.random.uniform(size=(len(n_net), len(n_net[0].getPosition())));

        self.p_best = self.pos.copy();

        self.fit=[];
        for i in range(0, len(n_net)):
            self.fit.append(n_net[i].cost());

        self.g_best = self.p_best[self.fit.argmin()];
        self.best_fit = self.fit.min();


    def swarm_improve(self):
        p_random = np.random.uniform(size=(self.popn, self.dim));
        g_random = np.random.uniform(size=(self.popn, self.dim));

        self.vel = self.chi * (self.vel + self.p_phi * p_random * (self.p_best - self.pos)\
                               + self.g_phi * g_random * (self.g_best - self.pos));

        self.pos = self.pos + self.vel;

        nn = [];

        for i in range(0, len(n_net)):
            nn.append(NeuralNetwork(n_net[0].nodes));
            nn[i].setData(ip, op);
            nn[i].setPosition(self.pos[i].tolist());

        de = DifferentialEvolution(nn);
        nn = de.optimise();

        for i in range(0, len(n_net)):
            curr_fit = nn[i].cost();
            if cuur_fit < self.fit[i]:
                self.p_best[i] = self.pos[i].copy();
                self.fit[i] = curr_fit;

        self.g_best = self.p_best[self.fit.argmin()].copy();
        self.best_fit = self.fit.min();

    def optimise(self, tol=0.0001, max_iter=500):
        iterate = 0;
        while self.best_fit > tol and iterate < max_iter:
            self.swarm_improve();
            iterate = iterate + 1;
