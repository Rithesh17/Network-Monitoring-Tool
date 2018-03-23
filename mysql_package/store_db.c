#include <my_global.h>
#include <mysql.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>

#include "store_db.h"

#define ECHOFLAGS (ECHO | ECHOE | ECHOK | ECHONL)

int set_disp_mode(int fd,int option)
{
   int err;
   struct termios term;
   if(tcgetattr(fd,&term)==-1){
     perror("Cannot get the attribution of the terminal");
     return 1;
   }
   if(option)
        term.c_lflag|=ECHOFLAGS;
   else
        term.c_lflag &=~ECHOFLAGS;
   err=tcsetattr(fd,TCSAFLUSH,&term);
   if(err==-1 && err==EINTR){
        perror("Cannot set the attribution of the terminal");
        return 1;
   }
   return 0;
}

int store_db(char* pcap_file, char* tcp_csv_file, char* icmp_csv_file)
{
  char db_username[30], db_passwd[30];
  printf("To store the packet information in the database, we need access \
permissions\n\nEnter the username: ");
  scanf("%s", db_username);

  set_disp_mode(STDIN_FILENO, 0);
  printf("Enter the password: ");
  scanf("%s", db_passwd);
  set_disp_mode(STDIN_FILENO, 1);
  printf("\n\n");

  MYSQL* dbConn;

  dbConn = initDB(db_username, db_passwd);

//  update_pcap_table(pcap_file, dbConn, 1);
  update_tcp_packet(tcp_csv_file, dbConn, 1);
//  update_icmp_packet(icmp_csv_file, dbConn, 1);
}

int main()
{
  store_db("test1.txt", "tcp.csv", "icmp.csv");
}
