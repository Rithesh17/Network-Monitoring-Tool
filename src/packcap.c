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


void pkp_err_exit(char *errmsg) {
	fprintf(stderr , "%s\n" , errmsg);
	exit(1);
}

/*
 * This is used to print the timestamp and packet length in actual capture.
 */

void pkp_print_packet_details(const unsigned char *packet , struct pcap_pkthdr *packet_header) {

	static int count = 0;

	pkp_frame.raw_timestamp = packet_header->ts.tv_sec;
	struct tm *current_time = localtime(&pkp_frame.raw_timestamp);
	char tm_buffer[64];
	strftime(tm_buffer , sizeof(tm_buffer) , "%Y-%m-%d %H:%M:%S" , current_time);


	snprintf(pkp_frame.current_time, sizeof(pkp_frame.current_time) , "%s.%ld" , tm_buffer , pkp_frame.raw_timestamp);
	pkp_frame.length = packet_header->len;

	/*
	 * packet_header->len = packet_header->caplen in our tool.
	 * Reason: The capture buffer size = 65535. The theoritically largest size of a frame.
	 */


	fprintf(fs_csv , "%s , %d , %s , %s , %s , %s , %s , %s , %u , %u \n" , pkp_frame.current_time , pkp_frame.length , pkp_frame.src_eth_addr , pkp_frame.dest_eth_addr , pkp_frame.l3_protocol , pkp_ipv4_packet.src_ipv4_addr , pkp_ipv4_packet.dest_ipv4_addr , pkp_ipv4_packet.l4_protocol , pkp_tcp_segment.src_port , pkp_tcp_segment.dest_port);
	fprintf(stdout , "%s , %d , %s , %s , %s , %s , %s , %s , %u , %u \n" , pkp_frame.current_time , pkp_frame.length , pkp_frame.src_eth_addr , pkp_frame.dest_eth_addr , pkp_frame.l3_protocol , pkp_ipv4_packet.src_ipv4_addr , pkp_ipv4_packet.dest_ipv4_addr , pkp_ipv4_packet.l4_protocol , pkp_tcp_segment.src_port , pkp_tcp_segment.dest_port);
	if(!strcmp(pkp_ipv4_packet.l4_protocol, "ICMP")) {
		printf("ICMP packets detected\n" );
	}
}



void pkp_packet_handler(unsigned char *arg , struct pcap_pkthdr *packet_header , const unsigned char *packet) {
	pkp_read_eth_header(packet);
	pkp_read_ipv4_header(packet);
	pkp_read_tcp_header(packet);


	fs_csv = fopen(pkp_csv_file , "a+");
	pkp_print_packet_details(packet , packet_header);
	fclose(fs_csv);



}


void pkp_dumpinto_file() {

	memset(pkp_dump_file , '\0' , sizeof(pkp_dump_file));
	printf("\nEnter the dumpfile name:(Maximum of 40 characters) ");
	scanf("%s" , pkp_dump_file);

	pkp_sniff.dumpfile = pcap_dump_open(pkp_sniff.handle , pkp_dump_file);
	if(pkp_sniff.dumpfile == NULL)
		pkp_err_exit("Error in creating the pcap_dumper_t structure.");

	if(pcap_loop(pkp_sniff.handle ,DEFAULT_PACKET_COUNT_LIMIT , &pcap_dump  , (char *)pkp_sniff.dumpfile) < 0)
		pkp_err_exit("Error in capturing packets. routine: pcap_loop()");

}


void pkp_live_relay() {
	memset(pkp_csv_file , '\0' , sizeof(pkp_csv_file));
	printf("Enter the csv file name:(Maximum of 40 characters) ");
	scanf("%s" , pkp_csv_file);

	pcap_loop(pkp_sniff.handle , DEFAULT_PACKET_COUNT_LIMIT , pkp_packet_handler , NULL);

}
