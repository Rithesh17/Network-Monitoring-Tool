classdef nn_unit
    properties (Access = private)
        bias = 0;
        inputs;
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
            obj.inputs = input_val;
            hypo = 1/(1+exp(-(sum(obj.inputs.*obj.weights) - obj.bias)));
        end
    end
end
