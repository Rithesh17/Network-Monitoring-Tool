/*
 * File: icmp_handle.h
 *
 * Contents: This contains the macros and routines defined to parse the icmp header.
 */

#include<netinet/ip_icmp.h>
#include<netinet/ip.h>
#include<netinet/in.h>

#include"pkp/packcap.h"


void pkp_read_icmp_header(const unsigned char *packet);
