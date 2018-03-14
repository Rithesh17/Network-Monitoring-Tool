/*
 * File: ether_handle.c
 *
 * Contents:
 *
 * 1. Basic Ethernet header parsing and decoding routines.
 * 2. Support present for most generally used protocols.
 * At present ,
 * 	a. IPv4
 * 	b. IPv6
 * 	c. ARP
 * 	d. REARP
 * 	e. VLAN		Not sure about this.
 * 	f. LOOPBACK
 */

#include"pkp/packcap.h"
#include"pkp/ether_handle.h"

void pkp_read_eth_header(const unsigned char *packet) {

	pkp_packet.pkp_eth_header = (struct ether_header *)(packet);
	//pkp_packet.pkp_eth_header = (struct ether_header *)(packet);
/*
 * XXX: That 24 actually is sizeof(struct pcap_pkthdr). HACK! Which should be reverted back. Not working if sizeof() is used.
 *
 * This is how the packet looks like if libpcap is used to capture the packet.
 *
 *  	PACKET_HEADER + PACKET_DATA.
 *
 *  Size of PACKET_HEADER is sizeof(pcap_pkthdr).
 *  PACKET_DATA is the beginning of data or the ethernet header comes first after the pcap packet header.
 */

	char *temp_eth_addr = ether_ntoa(pkp_packet.pkp_eth_header->ether_shost);

	memset(pkp_packet.src_eth_addr , '\0' , sizeof(pkp_packet.src_eth_addr));
	sprintf( pkp_packet.src_eth_addr, "%s" , temp_eth_addr);


	temp_eth_addr  = ether_ntoa(pkp_packet.pkp_eth_header->ether_dhost);
	memset(pkp_packet.dest_eth_addr , '\0' , sizeof(pkp_packet.dest_eth_addr));
	sprintf(pkp_packet.dest_eth_addr , "%s" , temp_eth_addr);



 /*
 * XXX: Never ever use this strcpy routine to copy stuff.
 * This is causing stack smashes and a lot of chaos.
 * Definitely , if it was used , the tool would be a source of stack overflow vulnerabilities.
 * As this tool is run as root user , The attacker gets the root shell. SHIT!
 *
 * In fact , the stack got smashed. So , definitely security critical part.
 *
 * REMEDY: Using strncpy() for now. TODO: But still not happy with it.
 *
 */
	char *l3_protocol;
	switch((unsigned short int)(pkp_packet.pkp_eth_header->ether_type)) {
		case 0x0008:
			l3_protocol = "IPv4";
			break;
		case 0xdd86:
			l3_protocol = "IPv6";
			break;
		case 0x0608:
			l3_protocol = "ARP";
			break;
		case 0x3580:
			l3_protocol = "ReARP";
			break;
		case 0x0090:
			l3_protocol = "LoopBack";
			break;
		default:
			l3_protocol = "Unkwown/Not Supported";
	}

/*
 * XXX: Respect endianess of the architecture while writing the switch cases. This is what happened.
 * case 0x0800: ...		. This was written because in net/ethernet.h , #define ETHERTYPE_IP 0x800. 	But this is in network order.
 * Should write it in little_endian order.
 *
 * Got this while looking through hexdump of the pcap file.
 *
 */

	printf(" %s 	> 	%s ; protocol: %s\n" , pkp_packet.src_eth_addr , pkp_packet.dest_eth_addr , l3_protocol);
}
