import csv;
import random;

fd1 = open('corrected.csv', 'r');
fd2 = open('new_corrected.csv', 'w');
fd3 = open('red_corrected.csv', 'w');
fd4 = open('red2_corrected.csv', 'w');

A = [];
for line in csv.reader(fd1):
    if(line[1] == 'tcp'):
        line[1] = 0;
    elif(line[1] == 'udp'):
        line[1] = 1;
    else:
        line[1] = 2;

    del line[2];
    del line[2];

    out = line.pop();

    if(out == 'normal.'):
        line = line + [0,0,0];
    elif(out in ['back.', 'land.', 'neptune.', 'pod.', 'smurf.', 'teardrop.']):
        line = line + [0,0,1];
    elif(out in ['buffer_overflow.', 'loadmodule.', 'rootkit.', 'perl.']):
        line = line + [0,1,0];
    elif(out in ['ipsweep.', 'nmap.', 'portsweep.', 'satan.']):
        line = line + [0,1,1];
    else:
        line = line + [1,0,0];

    csv.writer(fd2).writerow(line);
    A.append(line);

r = random.sample(A, 10000);

for line in r:
    csv.writer(fd3).writerow(line);

r = random.sample(A, 100);

for line in r:
    csv.writer(fd4).writerow(line);

fd1.close();
fd2.close();
fd3.close();
fd4.close();
