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
#include<time.h>
#include<pcap.h>

/*
 * A few constants used.
 */

#define DEFAULT_PACKET_COUNT_LIMIT -1



/*
 * TODO: If possible , put implementations of all the functions in another directory , create a dynamic shared object , and put only their declarations into a header files(Standard method).
 *
 */

/*
 * Error Message printing routine of packcap.
 */
void pkp_err_exit(char *errmsg , char *error_buffer) ;

/*
 * This Routine prints the packet length.
 */

void pkp_print_packet_len(const unsigned char *packet , struct pcap_pkthdr *packet_header); 

/*
 * This is the packcap packet handler. 
 * XXX: Thoughts:
 * 	1. Planning to have different handlers for different types of packets.
 * 	2. Have one universal handler and then distribute work among other relevant routines.
 */

void pkp_packet_handler_1(unsigned char *arg , struct pcap_pkthdr *packet_header , const unsigned char *packet) ;

#endif 
