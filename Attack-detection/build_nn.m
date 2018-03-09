function n_netw = build_nn(node, bias, weights)
    
    n_netw(sum(node)) = nn_unit;
    
    for level = 1:length(node)
        for j = 1:node(level)
            a = n_netw(sum(node(1:level-1))+j);
            a = a.setLevel(level);
            n_netw(sum(node(1:level-1))+j) = a;
        end
    end
    
    
end