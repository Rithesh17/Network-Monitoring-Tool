#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>
#include<pcap.h>


void print_packet_info(const u_char *packet , struct pcap_pkthdr packet_header) ;

/*
	All the above stuff should go to the header file .
*/


int main(int argc , char **argv) {


	char *device  , error_buffer[PCAP_ERRBUF_SIZE];
	char ip_addr[13];
	char subnet_mask[13];
	bpf_u_int32 ip_addr_raw;	//Ip Addr as an Integer.
	bpf_u_int32 subnet_mask_raw;	//Subnet mask as Integer.
	int lookup_return_code;
	struct in_addr ip_address;
	int packet_count_limit = 1;
	int timeout_limit = 100000;
	const u_char *packet;
	struct pcap_pkthdr packet_header;
	
 	/* Finding the default device(NIC) to sniff on. */

	device = pcap_lookupdev(error_buffer);
	if(device == NULL) {
		fprintf(stderr , "Couldn't find the default device\n");
		exit(2);
	}
//	printf("Device = %s\n" , device);
	
	/*Finding the device successful*/

	

	/* Get Device information.*/


	lookup_return_code = pcap_lookupnet(device , &ip_addr_raw , &subnet_mask_raw , error_buffer);
	
	if(lookup_return_code == -1) {
		fprintf(stderr , "Error in pcpa_lookupnet\n");
		exit(1);
	}

	/*Getting device info successful*/

	/*Convert the device information into human readable form*/

	ip_address.s_addr = ip_addr_raw;
	strcpy(ip_addr , inet_ntoa(ip_address));
	if(ip_addr == NULL) {
		fprintf(stderr , "Error in inet_ntoa\n");
		exit(1);
	}

	ip_address.s_addr = subnet_mask_raw;
	strcpy(subnet_mask , inet_ntoa(ip_address));
	if(subnet_mask == NULL) {
		fprintf(stderr , "Error in inet_ntoa\n");
		exit(1);
	}

	printf("Device = %s\n" , device);
	printf("Ip Address = %s\n" , ip_addr);
	printf("Subnet mask = %s\n" , subnet_mask);

	/* Collecting information about the host done!*/



 	/* 
	 * Opening the device for sniffing. 
	 * This requires root privileges. 
	 */

	pcap_t *handle;
	handle = pcap_open_live(device , BUFSIZ , packet_count_limit , timeout_limit , error_buffer);		if(handle == NULL) {
			fprintf(stderr , "Couldn't open device %s : %s\n" , device , error_buffer);
			exit(1);
		}

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
	
	/* 	
	 * Data link layer support present!
	 */

	packet = pcap_next(handle , &packet_header);
	if(packet == NULL) {
		fprintf(stderr , "No packet found\n");
		exit(1);	
	}

	print_packet_info(packet , packet_header);
	

	return 0;
	
}


void print_packet_info(const u_char *packet , struct pcap_pkthdr packet_header) {
	printf("Packet capture length: %d\n" , packet_header.caplen);
	printf("Packet total length: %d\n" , packet_header.len);

}







