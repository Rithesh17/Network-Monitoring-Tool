#include<stdio.h>
#include<stdlib.h>
#include<pcap.h>

int main(int argc , char **argv) {

	/* Finding the default device(NIC) to sniff on. */


	char *device  , error_buffer[PCAP_ERRBUF_SIZE];

	device = pcap_lookupdev(error_buffer);
	if(device == NULL) {
		fprintf(stderr , "Couldn't find the default device\n");
		exit(2);
	}
	printf("Device = %s\n" , device);
	
	/*Finding the device successful*/

	/* 
	 * Opening the device for sniffing. 
	 * This requires root privileges. 
	 */

	pcap_t *handle;
	handle = pcap_open_live(device , BUFSIZ , 1 , 1000 , error_buffer);		if(handle == NULL) {
			fprintf(stderr , "Couldn't open device %s : %s\n" , device , error_buffer);
			exit(1);
		}

	printf("%s , %p \n" , handle , handle);
	/*
	 * The pcap_open_live returned the handle.
	 * Device opened for sniffing.
	 * We have a working sniffing session.
	 */

	/*
	 * Checking the link layer support.
	 */

	if(pcap_datalink(handle) != DLT_EN10MB) {
		fprintf(stderr , "Device %s does not provide Ethernet headers - not supported\n" , device);
		exit(1);
	}

	return 0;
	
}










