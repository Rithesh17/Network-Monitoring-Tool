import random;
import csv;

file_split = open("corrected.csv", "r");

prot = [];

for line in csv.reader(file_split):
    if line[2] not in prot:
        prot.append(line[2]);

print(prot);

# for line in csv.reader(file_split):
#
#     del line[2:4];
#
#     prot = line.pop(1);
#     if prot=='tcp':
#         line.insert(1, 0);
#     elif prot=='udp':
#         line.insert(1, 1);
#     else:
#         line.insert(1, 2);
#
#     out = line.pop();
#     if out=='normal.':
#         line.append(0);
#     else:
#         line.append(1);
#
#     csv.writer(file_tr).writerow(line);
