function ANN_PSO
    %This is the main function for Artificial Neural Network
    %optimised using PSO
    
    tic;

    %Enter the file name and the Sheet nuber of .xls file
    %containing the dataset
    file_n = 'train.xlsx';
    sheet1 = 'Sheet1';
    sheet2 = 'Sheet2';

    input_data = xlsread(file_n, sheet1);
    output_data = xlsread(file_n, sheet2);
    
    input_data = input_data';
    output_data = output_data';
    
    b = length(input_data(:,1));
    c = length(output_data(:,1));
    
    a = 4;
    netw = feedforwardnet(a);
    netw = configure(netw, input_data, output_data);
    
    %For the PSO, let's set the upper-bound to 1
    %and lower bound to -1
    upper=1; lower=-1;
    
    %Set some value for the population size
    popn_size = 15;
    param_size = a*b+a+a+c;
    
    x_val = zeros(popn_size, param_size);
    
    for i = 1:popn_size
        for j = 1:param_size
            x_val(i, j) = lower + rand * (upper - lower);
        end
    end
    
    fit_val_1 = zeros(3,500);
    
    for trials = 1:1
        
        %Initialisation of PSO
        v_val = 0.1*x_val;
        
        fit_val = zeros(popn_size);
        
        for i = 1:popn_size
            [fit_val(i),~] = neural_net(x_val(i,:), a, b, c, netw, input_data, output_data);
        end
        [glob_min_fit, min_index] = mini(fit_val);
        
        p_best = x_val;
        g_best = x_val(min_index, :);
        
        %PSO Algorithm
        c1 = 2; c2 = 2;
        
        fprintf('Iteration\tBest Particle\tFitness\n');
        
        itern = 1;
        while itern <= 700
            
            for i = 1:popn_size
               for j = 1:param_size
                   v_val(i,j) = v_val(i,j) + c1*rand*(p_best(i,j)-x_val(i,j)) + c2*rand*(g_best(j)-x_val(i,j));
                   x_val(i,j) = x_val(i,j) + v_val(i,j);
                   
                   if(x_val(i,j)<lower)
                       x_val(i,j) = lower;
                   elseif(x_val(i,j)>upper)
                       x_val(i,j) = upper;
                   end
               end
            end
            
            new_fit = zeros(popn_size);
            
            for i = 1:popn_size
                [new_fit(i),~] = neural_net(x_val(i, :), a, b, c, netw, input_data, output_data);
                if new_fit(i) < fit_val(i)
                    fit_val(i) = new_fit(i);
                    p_best(i,:) = x_val(i,:);
                end
            end
            
            [new_min_fit, new_min_index] = mini(fit_val);
            %disp(itern);
            fit_val_1(trials, itern) = new_min_fit;
            
            if new_min_fit < glob_min_fit
                glob_min_fit = new_min_fit;
                g_best = p_best(new_min_index,:);
            end
            
            %disp(fit_val_1(trials,itern));
            %disp(glob_min_fit(1));
 
            fprintf('%d\t%g\t%g\n', itern, new_min_index, glob_min_fit);
            itern = itern + 1;
        end
        
        x_best(trials,:) = g_best;
        y_best(trials) = glob_min_fit;
    end
    fprintf('For the PSO: ');
    toc;
    fprintf('\n');
    
    final_netw = feedforwardnet(a);
    final_netw = configure(final_netw, input_data, output_data);
    
    [~, index_of_best] = mini(y_best);
    x_best_of_best = x_best(index_of_best,:);
    
    neural_net(x_best_of_best, a, b, c, final_netw, input_data, output_data);
    
    fprintf('Testing the ANN...\n');
    
    input_data = xlsread('test.xlsx', 'Sheet1');
    output_data = xlsread('test.xlsx', 'Sheet2');
    
    %view(final_netw);
    
    count=0;
    
    for i=1:length(output_data(:,1))
        hypo = final_netw(input_data(1,:));
        %disp(hypo);
        %disp(floor(hypo));
        if(floor(hypo) ~= output_data(i,1))
            count=count+1;
        end
    end
    
    error = count/length(output_data(:,1));
    
    fprintf('Error bits: %d\nSuccess rate: %f\n',count, 1-error);
end