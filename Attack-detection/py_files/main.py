#This is the main file for the PSO-DE optimiser.

if __name__ == '__main__':

    import csv;
    import numpy as np;
    import sklearn.metrics;
    from sklearn.model_selection import train_test_split;

    file_d = open('new_dataset.csv', 'r');
    readl = csv.reader(file_d);

    data = []; target = [];

    for row in readl:
        data.append(row[0:-1]);
        target.append(row[-1]);

    train_in, test_in, train_out, test_out = sklearn.model_selection.train_test_split(data, target);

    popn = 100;

    n_net_pop = [];

    nodes = [len(train_in[0]), 8, 4, 1];

    for i in range(0, popn):
        nn = NeuralNetwork(nodes);
        nn.setData(train_in, train_out);
        n_net_pop.append(nn);

    pso = ParticleSwarmOptimiser(n_net_pop);

    i=0;
    best_val = [[i, pso.best_fit]];
    print([i,pso.best_fit]);
    while i < 500 and pso.best_fit > 1e-6:
        pso.optimise();
        i=i+1;
        if pso.best_fit < best_val[-1][1]:
            best_val.append([i, pso.best_fit]);
            print([i, pso.best_fit]);

    best_weight = pso.g_best;
    best_nn = NeuralNetwork(nodes);
    best_nn.setWeights(best_weight);

    predict_out = [];
    for i in range(0, len(test_out)):
        predict_out.append(best_nn.propagate(test_in[i]));

    print (sklearn.metrics.classification_report(test_out, predict_out));
