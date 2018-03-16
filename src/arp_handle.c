/*
 * File: arp_handle.c
 *
 * Contents:
 * 	1. Will contain routines which will parse the ARP header.
 */

#include"pkp/packcap.h"
#include"pkp/arp_handle.h"


void pkp_read_arp_header(const unsigned char *packet) {

  pkp_arp_packet.header = (struct arphdr *)(packet + 14);

  /*That 14 is the size of ethernet header. It should be reverted back to sizeof(ether_header) instead of 14. */

  
}
