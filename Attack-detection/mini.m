function [min_val, min_index] = mini(array)
    %The function returning minimm value and index
    %in the array.
    [min_value, min_ind] = min(array);
    min_val = min_value(1);
    min_index = min_ind(1);
end