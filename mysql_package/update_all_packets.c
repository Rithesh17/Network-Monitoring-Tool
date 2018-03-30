//This function updates the packet table periodically.

// #include<my_global.h>
// #include<mysql.h>

#include "store_db.h"

//
//This is the error function for errors found in
//the update_all_tables() function.
//
//Parameters: dbConn: The connection handler to MySQL
//
//Return Value: 0

int error_sql(MYSQL* dbConn)
{
    printf("Error: %s\n", mysql_error(dbConn));
    mysql_close(dbConn);
    return 0;
}

//
//Returns the tuple number of the last tuple.
//Used to get the tuple number of the next tuple to be added.
//
//Parameters: dbConn: The connection handler to MySQL.
//            table_name: The name of the table whose last tuple number
//                        is needed.
//
//Return value: If successful, returns the tuple number of the last tuple.
//              Else, returns 0.s

int getTupleNum(MYSQL* dbConn, char* table_name)
{
  char query[100];
  sprintf(query, "SELECT * FROM %s;", table_name);

  int len = strlen(query);

  //The query is called here
  if(mysql_real_query(dbConn, query, len)!=0)
    error_sql(dbConn);


  //The result of the query is captured
  MYSQL_RES* result = mysql_store_result(dbConn);

  if(result==NULL)
    error_sql(dbConn);

   int num_rows = 0;

  //We are finding the number of rows in the 'result' table.
  while(mysql_fetch_row(result))
    num_rows++;// break;

  //It is important to clear the result after using it.
  mysql_free_result(result);

  return ++num_rows;
}

//
// Function to send the query to MySQL, to create instances of the
// p_cap table
//
// Parameters: stat_file: The stat of the .pcap file.
//             pcap_filename: The name of the .pcap file.
//             tup_no: The tuple number of the tuple to be added
//                      to the p_cap table.
//             dbConn: The connection handler of MySQL.
//             query: The query for MySQL in the form of a string.
//
// Return Value: If successful, returns 1. Else, returns 0;

int send_pcap_query(struct stat* stat_file, char* pcap_filename, \
  int tup_no, MYSQL* dbConn, char* query)
{
  struct stat fileStat = *stat_file;

  //The query required for adding the tuple to the p_cap table.
  int s = sprintf(query, "INSERT INTO p_cap(SerNo, timestamp, file_path, \
i_node) VALUES (%d, %ld, '%s', %d);", tup_no,\
                          fileStat.st_mtim.tv_sec, pcap_filename, \
                          (int)fileStat.st_ino);

  int len = strlen(query);

  //The query is called here.
  if(mysql_query(dbConn, query)!=0)
    error_sql(dbConn);

  strcpy(query, "\0");

}

// Function to send the query to MySQL, to create instances of the
// packet table
//
// Parameters: query: The query for MySQL in the form of a string.
//             dbConn: The connection handler of MySQL.
//             SerNo: The tuple numberto be added to the table
//             buff: The buffer holding the row details from the csv file.
//
// Return Value: If successful, returns 1. Else, returns 0;

int send_pack_query(char* query, MYSQL* dbConn, int SerNo, char* buff)
{

  //The following lines is to split the buffer string based on commas
  //(This is to separate cells of csv file).
  char* timestamp = strtok(buff, ",");
  int length = atoi(strtok(NULL, ","));
  char* ether_src = strtok(NULL, ",");
  char* ether_dst = strtok(NULL, ",");
  char* protocol3 = strtok(NULL, ",");
  char* ip_src = strtok(NULL, ",");
  char* ip_dst = strtok(NULL, ",");
  char* protocol4 = strtok(NULL, ",");
  int port_src = atoi(strtok(NULL, ","));
  int port_dst = atoi(strtok(NULL, "\n"));

    //The query required for adding the tuple to the packet table.
  if(sprintf(query, "INSERT INTO packet(SerNo, timestamp, length, \
ether_src, ether_dst, protocol3, ip_src, ip_dst, protocol4,\
port_src, port_dst) VALUES (%d, '%s', %d, '%s', '%s' \
, '%s', '%s', '%s', '%s', %d, %d);", \
SerNo, timestamp, length, ether_src, ether_dst, \
protocol3, ip_src, ip_dst, protocol4, port_src, \
port_dst)==0)
    return error_sql(dbConn);

  //The query is called here.
  if(mysql_query(dbConn, query)!=0)
    return error_sql(dbConn);

  //This is required to clear the query buffer.
  strcpy(query, "\0");

  return 1;
}

//Function to update all the tables of
//packet_analyser database
//
//Parameters: pcap_filename: The name of the .pcap files.
//            csv_file: The name of .csv file where the
//                          packet information are stored.
//            dbConn: The connection handler of MYSQL
//
//Returm value: If successful, returns 1. Else, returns 0
//

int update_all_tables(char* pcap_filename, char* csv_file, MYSQL* dbConn)
{
  //Opening the csv files in read mode.
  FILE* csv_file_des = fopen(csv_file, "r");

  //buff: To read the csv file row-wise into this buffer.
  //query: To write the query string for modifying the database tables.
  char buff[1024], query[1024];

  //Getting the statistics of the .pcap file to get the
  //time of creaton and the inode of the file.
  struct stat fileStat;
  if(stat(pcap_filename, &fileStat)!= 0)
  {
    printf("Error: file stat not obtained\n");
    return 0;
  }

  //Getting the last added tuple number of the p_cap table.
  int tup_no = getTupleNum(dbConn, "p_cap");

  //Send query to modify p_cap table.
  if(send_pcap_query(&fileStat, pcap_filename, tup_no, dbConn, query)==0)
    return 0;
  printf("PCAP table updated successfully\n");

  //Getting the last added tuple number of the packet table.
  tup_no = getTupleNum(dbConn, "packet");

  //Send query to modify packet table.
   while(fgets(buff, sizeof(buff), (FILE*)csv_file_des) != NULL)
      if(send_pack_query(query, dbConn, tup_no++, buff)==0)
          return 0;
  printf("Packet table updated successfully\n");

  //Closing all the csv files.
  fclose(csv_file_des);

  return 1;
}
