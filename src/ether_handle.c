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
	struct ether_header *eth_header;
	eth_header = (struct ether_header *)(packet);
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

	char *src_eth_addr = ether_ntoa(eth_header->ether_shost);

	char *dest_eth_addr = ether_ntoa(eth_header->ether_dhost);

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
	char *network_layer_protocol;
	switch((unsigned short int)(eth_header->ether_type)) {
		case 0x0008:
			network_layer_protocol = "IPv4";
			break;
		case 0xdd86:
			network_layer_protocol = "IPv6";
			break;
		case 0x0608:
			network_layer_protocol = "ARP";
			break;
		case 0x3580:
			network_layer_protocol = "ReARP";
			break;
		case 0x0090:
			network_layer_protocol = "LoopBack";
			break;
		default:
			network_layer_protocol = "Unkwown/Not Supported";
	}
			 
/*
 * XXX: Respect endianess of the architecture while writing the switch cases. This is what happened.
 * case 0x0800: ...		. This was written because in net/ethernet.h , #define ETHERTYPE_IP 0x800. 	But this is in network order.
 * Should write it in little_endian order. 
 *
 * Got this while looking through hexdump of the pcap file.
 *
 */

	printf("l2_src: %s > l2_dest: %s ; protocol: %s\n" , src_eth_addr , dest_eth_addr , network_layer_protocol);
}

