import csv;

fd = open('corrected.csv', 'r');

normal = 0;
dos = 0;
r2l = 0;
probe = 0;
u2r = 0;

for line in csv.reader(fd):
    out = line.pop();

    if(out == 'normal.'):
        normal = normal + 1;
    elif(out in ['back.', 'land.', 'neptune.', 'pod.', 'smurf.', 'teardrop.']):
        dos = dos + 1;
    elif(out in ['buffer_overflow.', 'loadmodule.', 'rootkit.', 'perl.']):
        r2l = r2l + 1;
    elif(out in ['ipsweep.', 'nmap.', 'portsweep.', 'satan.']):
        probe = probe + 1;
    else:
        u2r=u2r+1;

print([normal, dos, r2l, probe, u2r]);
