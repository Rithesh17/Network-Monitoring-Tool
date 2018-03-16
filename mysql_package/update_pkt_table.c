//This function updates the pkt_info table periodically.

#include<my_global.h>
#include<mysql.h>

//
//This is the error function for errors found in
//the update_pkt_info() and send_query()
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

// Function to send the query to MySQL, to create instances of the
// pkt_info table
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

int send_query(char* query, MYSQL* dbConn, int SerNo, char* ip_src, \
               char* ip_dst, char* ether_src, char* ether_dst, \
               char* protocol, int length, char* timestamp)
{
  if(sprintf(query, "INSERT INTO packet_info(SerNo, \
                    src_ip, dst_ip, protocol, length, timestamp) VALUES \
                    (%d, \"%s\", \"%s\", \"%s\", \"%s\", \"%s\", %d, \"%s\"\
                );", SerNo, ip_src, ip_dst, ether_src, ether_dst, \
                    protocol, length, timestamp)==0)
    return error_sql(dbConn);

  int len = strlen(query);

  if(mysql_real_query(dbConn, query, len)!=0)
    return error_sql(dbConn);

  sprintf(query, '\0');

  return 1;
}

//Function to update the pkt_info table of
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

int update_pkt_info(const char* csv_file, MySQL* dbConn, int tup_no)
{
    FILE* csv_file_des = fopen(csv_file, "r");
    char buff[1024], query[1024];

    while(fgets(buff, 1024, (FILE*)csv_file_des) != NULL)
        if(send_query(query, dbConn, (int) strtok(buff,","), strtok(buff,","),\
            strtok(buff,","), strtok(buff,","),  strtok(buff,","), \
            strtok(buff,","),(int) strtok(buff,","), strtok(buff,","))==0)
            return 0;

    return tup_no;
}

//The main file is only for unit test. While integrating, this
//part will be commented.
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

    update_pkt_info("test.csv", &dbConn, 1);

    mysql_close(&dbConn);

    return 1;
}
