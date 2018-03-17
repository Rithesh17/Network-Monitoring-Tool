//This function updates the icmp_packet table periodically.

#include<my_global.h>
#include<mysql.h>

//
//This is the error function for errors found in
//the update_icmp_packet() and send_query()
//functions.
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

int getTupleNum(MYSQL* dbConn)
{
  sprintf(query, "SELECT * FROM icmp_packet;", ip, ether);

  int len = strlen(query);

  //The query is called here
  if(mysql_real_query(dbConn, query, len)!=0)
  {
    printf("Error: %s\n", mysql_error(dbConn));
    mysql_close(dbConn);
    return 0;
  }

  //The result of the query is captured
  MYSQL_RES* result = mysql_store_result(dbConn);

  if(result==NULL)
  {
    printf("Error: %s\n", mysql_error(dbConn));
    mysql_close(dbConn);
    return 0;
  }

  int num_rows = 0;

  //We are finding the number of rows in the 'result'
  //table.
  while(mysql_fetch_row(result))
  {
    num_rows++;// break;
  }

  return num_rows++;
}

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

int send_query(char* query, MYSQL* dbConn, int SerNo, char* type,\
               int seq, char* ip_gateway)
{
  if(sprintf(query, "INSERT INTO icmp_packet(SerNo, type, seq, ip_gateway)\
                      VALUES (%d, \"%s\", %d, \"%s\");", \
                      SerNo, type, seq, ip_gateway)==0)
    return error_sql(dbConn);

  int len = strlen(query);

  if(mysql_real_query(dbConn, query, len)!=0)
    return error_sql(dbConn);

  sprintf(query, '\0');

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

int update_icmp_packet(const char* csv_file, MYSQL* dbConn, int tup_no)
{
    FILE* csv_file_des = fopen(csv_file, "r");
    char buff[1024], query[1024];

    if(tup_no==-1)
      tup_no = getTupleNum(dbConn);

    while(fgets(buff, 1024, (FILE*)csv_file_des) != NULL)
        if(send_query(query, dbConn, tup_no, strtok(buff,","), \
          atoi(strtok(buff,",")), strtok(buff,","))==0)
            return 0;

    return tup_no;
}
