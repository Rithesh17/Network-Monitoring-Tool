#ifndef __PACKCAP_H__
#define __PACKCAP_H__

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<pcap.h>

/*
 * A few constants used.
 */

#define DEFAULT_PACKET_COUNT_LIMIT -1



/*
 * TODO: If possible , put implementations of all the functions in another directory , create a dynamic shared object , and put only their declarations into a header files(Standard method).
 *
 */

void err_exit(char *errmsg , char *error_buffer) {
	fprintf(stderr , "%s\n%s\n" , errmsg , error_buffer);
	exit(1);
}


void print_packet_len(const unsigned char *packet , struct pcap_pkthdr packet_header) {
	printf("Packet capture length: %d\n" , packet_header.caplen);
	printf("Packet total length: %d\n" , packet_header.len);
}


#endif 
