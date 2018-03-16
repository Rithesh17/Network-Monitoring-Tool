/*






*/


#include"icmp_handle.h"

void pkp_read_icmp_header(const unsigned char *packet) {

  pkp_icmp_header = (struct icmphdr *)(packet + 14 + 20);
  
}
