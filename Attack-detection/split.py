import random;

file_split = open("IRIS.csv", "r");
file_tr = open("train.csv", "w");
file_test = open("test.csv", "w");

for line in file_split.readlines():
    rand = random.randint(1,2);
    if rand == 1:
        file_tr.write(line);
    else:
        file_test.write(line);
