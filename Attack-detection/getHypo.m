function [n_netw, hypo] = getHypo(n_netw, input_data, node)
    data = input_data;
    
    temp = [];
    for level = 2:length(node)
        for j = 1:node(level)
            hypo = n_netw(sum(node(1:level-1))+j).getHypothesis(data);
            temp = [temp, hypo];
        end
        data = temp;
        temp = [];
    end
    hypo = data;
end