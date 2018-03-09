function n_netw = build_nn(node, bias, weights)
    %Build a Neural Network with given bias and weights
    %
    %
    %Input: node: An array denoting the nodes in each layer
    %             Each element of the array holds the number
    %             of nodes in that layer. First layer is the
    %             input layer, and the last layer is the
    %             output layer.
    %
    %       bias: An array holding the bias values of each node.
    %             The input is to be given linearly, starting 
    %             from the bias for the layer next to input layer
    %             till the bias for the output layer.
    %
    %    weights: An array holding the values of the weights for 
    %             every connection. The input is to be given linearly, 
    %             starting from the weights for the layer next to input 
    %             layer (weights between the input layer and the layer 
    %             next to it) till the weights for the output layer.
    %
    %
    %Output: n_netw: The neural network built. It is an array of 'nn_unit'
    %                objects defined by the bias and the weights given.
    %                For more information on the methods of the 'nn_unit'
    %                objects refer to the class 'nn_unit'.
    %
    
    n_netw(sum(node)) = nn_unit;
    
    for level = 2:length(node)
        for j = 1:node(level)
            a = n_netw(sum(node(1:level-1))+j);
            a = a.setLevel(level);
            n_netw(sum(node(1:level-1))+j) = a;
        end
    end
    
    for unit = node(1)+1:sum(node)
        n_netw(unit) = n_netw(unit).setBias(bias(unit-node(1)));
    end
    
    for level = 2:length(node)
        for j = 1:node(level)
            a = n_netw(sum(node(1:level-1))+j);
            a = a.setWeights(weights(sum(node(1:level-2))+1:sum(node(1:level-1))));
            n_netw(sum(node(1:level-1))+j) = a;
        end
    end
end