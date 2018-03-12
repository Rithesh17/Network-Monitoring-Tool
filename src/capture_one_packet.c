/*
 * Program No.3 .
 * These are the things implemented in this program:
 * 1. Find the Default device using pcap_lookupdev().
 * 2. Find the Details(Ip Address , Subnet Mask in raw form) using pcap_lookupnet().
 * 3. Convert Raw information into human readable information.Using simple inet routines.
 * 4. Open the "device" for sniffing using pcap_open_live() routine.(Requires root permission.
 * 5. Checked whether the "device" chosen supports Ethernet header or not using pcap_datalink() routine.
 * 
 * 6. Finally , Captured 1 packet using pcap_next() routine and printed it's length.
 *
 */

#include"packcap.h"


int main(int argc , char **argv) {


	char 			*pkp_dev_name;				
	char     		pkp_error_buffer[PCAP_ERRBUF_SIZE];
	char 			pkp_str_ip_addr[13];
	char 			pkp_str_subnet_mask[13];
	struct in_addr 		pkp_ip_address;
	int 			pkp_packet_count_limit = DEFAULT_PACKET_COUNT_LIMIT;			
	int 			pkp_timeout_limit = 100000;			//TODO: Should change. Don't know what. Will come back.
	const unsigned char 	*pkp_packet;
	bpf_u_int32 		pkp_raw_ip_addr;				
	bpf_u_int32 		pkp_raw_subnet_mask;			
	struct pcap_pkthdr 	pkp_packet_header;
	pcap_t 			*pkp_sniff_handle;
	

	pkp_dev_name = pcap_lookupdev(pkp_error_buffer);
	if(pkp_dev_name == NULL)	
		err_exit("Error in finding default device. \nRoutine: pcap_loopupdev()" , pkp_error_buffer);
	

	if(pcap_lookupnet(pkp_dev_name , &pkp_raw_ip_addr , &pkp_raw_subnet_mask , pkp_error_buffer) == -1)
		err_exit("Error in finding details about the device. \nRoutine: pcap_lookupnet()" , pkp_error_buffer);
		

	pkp_ip_address.s_addr = pkp_raw_ip_addr;
	strcpy(pkp_str_ip_addr , inet_ntoa(pkp_ip_address));
	if(pkp_str_ip_addr == NULL) 
		err_exit("Error: Unable to convert raw IP address into human readable form. Routine: inet_ntoa()" , pkp_error_buffer);


	pkp_ip_address.s_addr = pkp_raw_subnet_mask;
	strcpy(pkp_str_subnet_mask , inet_ntoa(pkp_ip_address));
	if(pkp_str_subnet_mask == NULL) 
		err_exit("Error: Unable to covert raw Subnet mask into human readable form. Routine: inet_ntoa()" , pkp_error_buffer);

	printf("Device = %s\n" 	     , 	pkp_dev_name);
	printf("Ip Address = %s\n"   , 	pkp_str_ip_addr);
	printf("Subnet mask = %s\n"  , 	pkp_str_subnet_mask);

	
	pkp_sniff_handle = pcap_open_live(pkp_dev_name , BUFSIZ , pkp_packet_count_limit , pkp_timeout_limit , pkp_error_buffer);		
	if(pkp_sniff_handle == NULL) 
		err_exit("Error:  Unable to open a live sniffing session in the device specified. Routine: pcap_open_live()" , pkp_error_buffer);


	if(pcap_datalink(pkp_sniff_handle) != DLT_EN10MB) 
		err_exit("Error: Device does not support Ethernet Headers." , pkp_error_buffer);	

	
	
	pkp_packet = pcap_next(pkp_sniff_handle , &pkp_packet_header);
	if(pkp_packet == NULL) 
		err_exit("Error: A single packet did not get sniffed." , pkp_error_buffer);

	print_packet_len(pkp_packet , pkp_packet_header);
	

	return 0;
	
}
