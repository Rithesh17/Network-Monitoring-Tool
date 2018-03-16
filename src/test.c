#include<stdio.h>
#include<net/ethernet.h>

int main() {
	printf("%d\n" , sizeof(struct ether_header));
	return 0;
}
