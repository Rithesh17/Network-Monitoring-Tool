//This function is to add a tuple to the p_cap table
//every time a .pcap file is created.

#include<my_global.h>
#include<mysql.h>
#include<sys/stat.h>

#include "store_db.h"

//Function to update the p_cap table of
//packet_analyser database
//
//Parameters: filename: The file name of .pcap file
//            conn: The connection handler of MYSQL
//           tup_no: The value of the last tuple
//
//Returm value: If successful, returns the row value of
//              the last added tuple. Else, returns 0
//
int update_pcap_table(char* filename, MYSQL* dbConn, int tup_no)
{
  //Using stat to get the timestamp and the inode value
  //of the file

  struct stat fileStat;
  if(stat(filename, &fileStat)!= 0)
  {
    printf("Error: file stat not obtained");
    return 0;
  }

  char query[1024];
  //
  // if(tup_no == -1)
  //   tup_no = getTupleNum(dbConn, "p_cap");

  int s = sprintf(query, "INSERT INTO p_cap(SerNo, timestamp, file_path, \
i_node) VALUES (%d, %ld, '%s', %d);", tup_no,\
                          fileStat.st_mtim.tv_sec, filename, \
                          (int)fileStat.st_ino);

  int len = strlen(query);

  printf("%s\n", query);

  if(mysql_query(dbConn, (const char*)query)!=0)
  {
    printf("Error: %s\n", mysql_error(dbConn));
    mysql_close(dbConn);
    return 0;
  }
  printf("Return 1.\n");
  return 1;
}

//The main file is only for unit test. While integrating, this
//part will be commented.
// int main()
// {
//   MYSQL dbConn;
//
//   //Initialising the handler
//   if(mysql_init(&dbConn) == NULL)
//   {
//     printf("Error: %s\n", mysql_error(&dbConn));
//     return 0;
//   }
//
//   //Connecting to MySQL through the handler.
//   if(mysql_real_connect(&dbConn, "localhost", "root", "nirmala17", NULL, 3306, NULL, 0)==NULL)
//   {
//     printf("Error: %s\n", mysql_error(&dbConn));
//     return 0;
//   }
//
//   update_pcap_table("/home/ritesh/Desktop/prio_with_aging.c", &dbConn, 1);
// }
