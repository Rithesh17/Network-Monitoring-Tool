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


struct pkp_device_details {
	char *name;
	char error_buffer[PCAP_ERRBUF_SIZE];
	char str_ip_addr[13];
	char str_subnet_mask[13];
	struct in_addr ip_address;
	bpf_u_int32 		raw_ip_addr;			/*IP Address in raw/network form. */
	bpf_u_int32 		raw_subnet_mask;

};
struct pkp_device_details pkp_device;


struct pkp_sniff_session {
	int packet_count_limit;
	int timeout_limit;
	pcap_t *handle;
};
struct pkp_sniff_session pkp_sniff;



struct pkp_packet_global {

	struct pcap_pkthdr *pkp_pkt_header;
	long int raw_timestamp;
	char current_time[64];
	unsigned short int length;
	struct ether_header *pkp_eth_header;
	char src_eth_addr[10];
	char dest_eth_addr[10];
};

struct pkp_packet_global pkp_packet;

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

void pkp_packet_handler(unsigned char *arg , struct pcap_pkthdr *packet_header , const unsigned char *packet) ;

#endif
