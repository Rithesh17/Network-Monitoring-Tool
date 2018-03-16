#include<netinet/ip.h>
#include<netinet/in.h>
int main() {
	printf("%d\n" , sizeof(struct iphdr));
	return 0;
}
