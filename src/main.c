/*
 * File: main.c
 *
 * Contents:
 * 	1. This is the main sourcefile around which everything is built.
 * 	2. Basic device detection , opening a sniffing session and calling the sniff handler happens in this Sourcefile.
 */


#include"pkp/packcap.h"
#include"pkp/ether_handle.h"


int main(int argc , char **argv) {

	if(argc != 3) {
		fprintf(stderr , "Usage: $%s <pcapdumpfile.pcap> <name_of_csvfile.csv> \n" , argv[0]);
		exit(1);
	}
	pkp_csv_file = argv[2];
	pkp_dump_file = argv[1];

/*
 * Finding default device .
 * Routine: pcap_lookupdev()
 */
	pkp_device.name = pcap_lookupdev(pkp_device.error_buffer);
	if(pkp_device.name == NULL)
		pkp_err_exit("Error in finding default device. \nRoutine: pcap_loopupdev()" , pkp_device.error_buffer);


/*
 * Getting details of the device found .
 * Routine: pcap_lookupnet()
 */


	if(pcap_lookupnet(pkp_device.name , &pkp_device.raw_ip_addr , &pkp_device.raw_subnet_mask , pkp_device.error_buffer) == -1)
		pkp_err_exit("Error in finding details about the device. \nRoutine: pcap_lookupnet()" , pkp_device.error_buffer);

/*
 * Converting Network forms of IP Address and Subnet Mask into human-readable strings.
 * Routine: inet routines.
 */

	pkp_device.ip_address.s_addr = pkp_device.raw_ip_addr;
	strcpy(pkp_device.str_ip_addr , inet_ntoa(pkp_device.ip_address));
	if(pkp_device.str_ip_addr == NULL)
		pkp_err_exit("Error: Unable to convert raw IP address into human readable form. Routine: inet_ntoa()" , pkp_device.error_buffer);


	pkp_device.ip_address.s_addr = pkp_device.raw_subnet_mask;
	strcpy(pkp_device.str_subnet_mask , inet_ntoa(pkp_device.ip_address));
	if(pkp_device.str_subnet_mask == NULL)
		pkp_err_exit("Error: Unable to covert raw Subnet mask into human readable form. Routine: inet_ntoa()" , pkp_device.error_buffer);

	printf("Device = %s\n" 	     , 	pkp_device.name);
	printf("Ip Address = %s\n"   , 	pkp_device.str_ip_addr);
	printf("Subnet mask = %s\n\n"  , 	pkp_device.str_subnet_mask);


/*
 * Opening a live sniffing session. Get a handle to manage the session.
 * Routine: pcap_open_live()
 */

	pkp_sniff.handle = pcap_open_live(pkp_device.name , BUFSIZ , pkp_sniff.packet_count_limit , pkp_sniff.timeout_limit , pkp_device.error_buffer);
	if(pkp_sniff.handle == NULL)
		pkp_err_exit("Error:  Unable to open a live sniffing session in the device specified. Routine: pcap_open_live()" , pkp_device.error_buffer);

/*
 * Checking if the default device uses/supports Ethernet headers. Or Checking if the Data-Link Protocol is the Ethernet Protocol.
 * Routine: pcap_datalink()
 */

	if(pcap_datalink(pkp_sniff.handle) != DLT_EN10MB)
		pkp_err_exit("Error: Device does not support Ethernet Headers." , pkp_device.error_buffer);

/*
 * XXX: Checking if sniffing happens properly. This will be commented.
 * Sniffed 1 packet .
 * Routine: pcap_next()
 *
 *
	pkp_packet = pcap_next(pkp_sniff_handle , &pkp_packet_header);
	if(pkp_packet == NULL)
		err_exit("Error: A single packet did not get sniffed." , pkp_error_buffer);

	print_packet_len(pkp_packet , pkp_packet_header);

 */


	//if((pkp_device.dumpfile = pcap_dump_open(pkp_sniff.handle , pkp_dump_file)) == NULL)
	//	pkp_err_exit("Error in dumping the packets into a dump(pcap) file" , pkp_device.error_buffer);


//	if(pcap_dump_fopen(pkp_sniff.handle , "pcapfile.pcap") == NULL)
	//	pkp_err_exit("Error dumping into pcap file" , pkp_device.error_buffer);


/*
 * Call the sniff handler . DEFAULT_PACKET_COUNT_LIMIT = 1. So , the sniffing happens till an error occurs.
 * Routine: pcap_loop()
 */
	//pcap_loop(pkp_sniff.handle , DEFAULT_PACKET_COUNT_LIMIT , pkp_pcap_dump_handler , (unsigned char *)(pkp_device.dumpfile));
	pcap_loop(pkp_sniff.handle , DEFAULT_PACKET_COUNT_LIMIT , pkp_packet_handler , NULL);

	return 0;

}
