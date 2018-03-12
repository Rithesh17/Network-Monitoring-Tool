#include<stdio.h>
#include<pcap.h>

int main(int argc , char **argv) {
	
	if(argc != 2) {
		fprintf(stderr , "Usage: $%s <device>name>\n" , argv[0]);
		exit(1);
	}
	
	char *device;
	device = argv[1];
	printf("Device: %s\n" , device);

	return 0; 
}
