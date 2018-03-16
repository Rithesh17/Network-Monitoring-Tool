/*
 * File: packcap.c
 *
 * Contents:
 * 	1. Contains implementations of routines declared in pkp/packcap.h
 *
 * 	2.
 */
#include"pkp/packcap.h"
#include"pkp/ether_handle.h"

 /* TODO: If possible , put implementations of all the functions in another directory , create a dynamic shared object , and put only their declarations into a header files(Standard method).
 *
 */


void pkp_err_exit(char *errmsg , char *error_buffer) {
	fprintf(stderr , "%s\n%s\n" , errmsg , error_buffer);
	exit(1);
}

/*
 * This is used to print the timestamp and packet length in actual capture.
 */

void pkp_print_packet_len(const unsigned char *packet , struct pcap_pkthdr *packet_header) {

	static int count = 0;

	pkp_packet.raw_timestamp = packet_header->ts.tv_sec;
	struct tm *current_time = localtime(&pkp_packet.raw_timestamp);
	char tm_buffer[64];
	strftime(tm_buffer , sizeof(tm_buffer) , "%Y-%m-%d %H:%M:%S" , current_time);
	snprintf(pkp_packet.current_time, sizeof(pkp_packet.current_time) , "%s.%ld" , tm_buffer , pkp_packet.raw_timestamp);

	pkp_packet.length = packet_header->len;
	printf("%d. LEN: %d\t" , ++count , pkp_packet.length);
	pkp_packet.length = packet_header->len;


	/*
	 * packet_header->len = packet_header->caplen in our tool.
	 * Reason: The capture buffer size = 65535. The theoritically largest size of a frame.
	 */
}

void pkp_packet_handler(unsigned char *arg , struct pcap_pkthdr *packet_header , const unsigned char *packet) {
	pkp_print_packet_len(packet , packet_header);
	pkp_read_eth_header(packet);
}
