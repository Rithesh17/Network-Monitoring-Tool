classdef nn_unit
    %A node of the Neural Network.
    %
    %
    %Properties: bias: The bias value of the node.
    %            
    %            weights: An array of weights between the current node and 
    %                     the nodes of previous level.
    %                     
    %            level: The layer value to which the node belongs. By
    %                   convention the inputs are labelled as level 1, and
    %                   the output layer has the highest value.
    %
    %
    %Methods: setLevel: Sets the layer number of the node. The value of
    %                   the layer is to be passed as the parameter.
    %          
    
    properties (Access = private)
        bias = 0;
        weights;
        level = 1;
    end
    methods
        function obj = setLevel(obj, level_val)
            obj.level = level_val;
        end
        
        function obj = setBias(obj, bias_val)
            obj.bias = bias_val;
        end
        
        function obj = setWeights(obj, wt)
            obj.weights = wt;
        end

        function [obj, hypo] = getHypothesis(obj, input_val)
            hypo = 1/(1+exp(-(sum(input_val.*obj.weights) - obj.bias)));
        end
        
        function level_out = getLevel(obj)
            level_out = obj.level;
        end
    end
end
