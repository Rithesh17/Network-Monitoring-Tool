
#include"pkp/packcap.h"
#include"pkp/ether_handle.h"
/*
 * TODO: If possible , put implementations of all the functions in another directory , create a dynamic shared object , and put only their declarations into a header files(Standard method).
 *
 */

void pkp_err_exit(char *errmsg , char *error_buffer) {
	fprintf(stderr , "%s\n%s\n" , errmsg , error_buffer);
	exit(1);
}
/*
 * The pkp_print_packet_len() was written for testing purposes.
 * It will remain here. Can be used if required.
 *
 * This is used to print the timestamp and packet length in actual capture.
 */

void pkp_print_packet_len(const unsigned char *packet , struct pcap_pkthdr *packet_header) {
	
	static int count = 0;

	time_t raw_timestamp = packet_header->ts.tv_sec;
	struct tm *current_time = localtime(&raw_timestamp);
	char tm_buffer[64] , buffer[64];
	strftime(tm_buffer , sizeof(tm_buffer) , "%Y-%m-%d %H:%M:%S" , current_time);
	snprintf(buffer , sizeof(buffer) , "%s.%ld" , tm_buffer , packet_header->ts.tv_usec);

	printf("%d.\t" , ++count);
	printf("%s\t" , buffer);	
	printf("len: %d\n" , packet_header->len);			/*Total length(len) will be equal to Capture Length(caplen) because maxsize of a packet is set to 65535. */
//	printf("Packet capture length: %d\t" , packet_header->caplen);
//	printf("Packet total length: %d\n" , packet_header->len);
}

void pkp_packet_handler_1(unsigned char *arg , struct pcap_pkthdr *packet_header , const unsigned char *packet) {
	pkp_print_packet_len(packet , packet_header);
	pkp_read_eth_header(packet);
}

