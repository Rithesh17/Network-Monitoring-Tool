//This function updates the tcp_packet table periodically.

#include<my_global.h>
#include<mysql.h>

//
//This is the error function for errors found in
//the update_tcp_packet() and send_query()
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
  char* query[100];
  sprintf(query, "SELECT * FROM tcp_packet;");

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
// tcp_packet table
//
// Parameters: query: The query for MySQL in the form of a string.
//             dbConn: The connection handler of MySQL.
//             SerNo: The tuple number of the new tuple (Primary key).
//             ip_src: The source ip address.
//             ip_dst: The destination ip address.
//             ether_src: The source ethernet address.
//             ether_dst: The destination ethernet address.
//             protocol: The protocol used by the packet.
//             length: The length of the packet.
//             timestamp: The time of capture of packet.
//
// Return Value: If successful, returns 1. Else, returns 0;

int send_query(char* query, MYSQL* dbConn, int SerNo, char* timestamp,\
               int length, char* ether_src, char* ether_dst, \
               char* protocol3, char* ip_src, char* ip_dst, char* protocol4, \
               int port_src, int port_dst)
{
  if(sprintf(query, "INSERT INTO tcp_packet(SerNo, timestamp, length, \
                      ether_src, ether_dst, protocol3, ip_src, ip_dst, protocol4,\
                      port_src, port_dst) VALUES (%d, \"%s\", %d, \"%s\", \"%s\"\
                      , \"%s\", \"%s\", \"%s\", \"%s\", %d, %d);", \
                      SerNo, timestamp, length, ether_src, ether_dst, \
                      protocol3, ip_src, ip_dst, protocol4, port_src, \
                      port_dst)==0)
    return error_sql(dbConn);

  int len = strlen(query);

  if(mysql_real_query(dbConn, query, len)!=0)
    return error_sql(dbConn);

  sprintf(query, '\0');

  return 1;
}

//Function to update the tcp_packet table of
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

int update_tcp_packet(const char* csv_file, MYSQL* dbConn, int tup_no)
{
    FILE* csv_file_des = fopen(csv_file, "r");
    char buff[1024], query[1024];

    if(tup_no==-1)
      tup_no = getTupleNum(dbConn);

    while(fgets(buff, 1024, (FILE*)csv_file_des) != NULL)
        if(send_query(query, dbConn, ++tup_no, strtok(buff,","), atoi(strtok(buff,",")),\
            strtok(buff,","), strtok(buff,","),  strtok(buff,","), \
            strtok(buff,","), strtok(buff,","), strtok(buff,","), \
            atoi(strtok(buff,",")), atoi(strtok(buff,",")))==0)
            return 0;

    return tup_no;
}

int main()
{
  MYSQL dbConn;

  //Initialising the handler
  if(mysql_init(&dbConn) == NULL)
  {
    printf("Error: %s\n", mysql_error(&dbConn));
    return 0;
  }

  //Connecting to MySQL through the handler.
  if(mysql_real_connect(&dbConn, "localhost", "root", "nirmala17", NULL, 3306, NULL, 0)==NULL)
  {
    printf("Error: %s\n", mysql_error(&dbConn));
    return 0;
  }

  mysql_query(&dbConn, "USE packet_analyser;");

  int tup_no = 1;
  tup_no = update_tcp_packet("new.csv",&dbConn, -1);

  mysql_close(&dbConn);
  return 1;
}
