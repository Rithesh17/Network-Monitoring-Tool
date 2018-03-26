//This function updates the icmp_packet table periodically.

#include<my_global.h>
#include<mysql.h>

#include "store_db.h"

// Function to send the query to MySQL, to create instances of the
// icmp_packet table
//
// Parameters: query: The query for MySQL in the form of a string.
//             dbConn: The connection handler of MySQL.
//             SerNo: The tuple number of the new tuple (Primary key).
//             type:
//             seq: The sequence number of the packet.
//             ip_gateway: The IP address of the gateway.
//
// Return Value: If successful, returns 1. Else, returns 0;

int send_icmp_query(char* query, MYSQL* dbConn, int SerNo, char* buff)
{
  char* type = strtok(buff, ",");
  int seq = atoi(strtok(NULL, ","));
  char* ip_gateway = strtok(NULL, "\n");

  if(sprintf(query, "INSERT INTO icmp_packet(SerNo, type, seq, ip_gateway) \
VALUES (%d, '%s', %d, '%s');", SerNo, type, seq, ip_gateway)==0)
    return error_sql(dbConn);

    printf("%s\n", query);

  if(mysql_query(dbConn, (const char*)query)!=0)
    return error_sql(dbConn);

  strcpy(query, "\0");

  return 1;
}

//Function to update the icmp_packet table of
//packet_analyser database
//
//Parameters: csv_file: The file name of .csv file where the
//                      packet information are stored.
//            dbConn: The connection handler of MYSQL
//            tup_no: The value of the last tuple
//
//Returm value: If successful, returns the row value of
//              the last added tuple. Else, returns 0
//

int update_icmp_packet(char* csv_file, MYSQL* dbConn, int tup_no)
{
    FILE* csv_file_des = fopen(csv_file, "r");
    char buff[1024], query[1024];

    if(tup_no==-1)
      tup_no = getTupleNum(dbConn, "icmp_packet");

    while(fgets(buff, 1024, (FILE*)csv_file_des) != NULL)
        if(send_icmp_query(query, dbConn, tup_no++, buff)==0)
            return 0;

    return tup_no;
}
