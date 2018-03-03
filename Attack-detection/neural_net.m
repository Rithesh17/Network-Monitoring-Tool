function [fitness,hypo] = neural_net(val, a, b, c, netw, input_data, output_data)
    k=1;
    
    val_1 = zeros(a, b);
    bias_1 = zeros(a,1);
    bias_2 = zeros(c,1);
    
    for i = 1:a
        for j = 1:b
            val_1(i, j) = val(k);
            k=k+1;
        end
    end
    
    for i = 1:a
        val_2(i) = val(k);
        bias_1(i,1) = val(k+a);
        k=k+1;
    end
    
    k=k+a;
    
    for i = 1:c
        bias_2(i,1) = val(k);
        k = k+1;
    end
        
    netw.b{1} = bias_1;
    netw.b{2} = bias_2;
    netw.iw{1,1} = val_1;
    netw.lw{2,1} = val_2;
    
    hypo = netw(input_data);
    fitness = sum((hypo-output_data).^2)/length(input_data);
end