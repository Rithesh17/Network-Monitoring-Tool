import random
import csv

file1 = open('new_dataset.csv', 'r');
file2 = open('part_dataset.csv', 'w');

i=0;

read1 = csv.reader(file1);
write1 = csv.writer(file2);

reader = list(read1);

for i in range(0, 5000):
    a = random.randint(0, len(reader));
    write1.writerow(reader[a]);
