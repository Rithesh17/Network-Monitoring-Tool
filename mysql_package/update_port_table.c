//This function is to fill the service_port table.
//The table will be filled only once, and will
//be referred to if required.

#include<my_global.h>
#include<mysql.h>

//This is the error function for errors found in
//the update_port_table() and send_query()
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
// serv_port table
//
// Parameters: query: The query for MySQL in the form of a string
//             dbConn: The connection handler of MySQL
//             SerNo: The tuple number of the new tuple (Primary key)
//             serv_port: The port number of the service
//             service: The name of the service
//             protocol: TCP or UDP service
//
// Return Value: If successful, returns 1. Else, returns 0;

int send_query(char* query, MYSQL* dbConn, int SerNo, int serv_port, \
                char* service, char* protocol)
{
  if(sprintf(query, "INSERT INTO service_port(SerNo, \
                   serv_port, service, protocol) VALUES \
                   (%d, %d, \"%s\", \"%s\");", SerNo, \
                    serv_port, service, protocol)==0)
    return error_sql(dbConn);

  int len = strlen(query);

  if(mysql_real_query(dbConn, query, len)!=0)
    return error_sql(dbConn);

  return 1;
}

//Function to update the service_port table of
//packet_analyser database.
//
//Parameters: dbConn: The connection handler to MySQL
//
//Reture Value: If successful, returns 1.
//              Else, returns 0.

int update_port_table(MYSQL* dbConn)
{

  if(mysql_query(dbConn, "USE packet_analyser")!=0)
    return error_sql(dbConn);

  char query[200];

  int Ser = 1;

  if(send_query(query, dbConn, Ser, 21, "ftp", "tcp")==0)
    return 0;
  Ser++;
  if(send_query(query, dbConn, Ser, 21, "fsp", "udp")==0)
    return 0;
  Ser++;
  if(send_query(query, dbConn, Ser, 22, "ssh", "tcp")==0)
    return 0;
  Ser++;
  if(send_query(query, dbConn, Ser, 22, "ssh", "udp")==0)
    return 0;
  Ser++;
  if(send_query(query, dbConn, Ser, 23, "telnet", "tcp")==0)
    return 0;
  Ser++;
  if(send_query(query, dbConn, Ser, 25, "smtp", "tcp")==0)
    return 0;
  Ser++;
  if(send_query(query, dbConn, Ser, 80, "http", "tcp")==0)
    return 0;
  Ser++;
  if(send_query(query, dbConn, Ser, 80, "http", "udp")==0)
    return 0;
  Ser++;
  if(send_query(query, dbConn, Ser, 109, "pop2", "tcp")==0)
    return 0;
  Ser++;
  if(send_query(query, dbConn, Ser, 109, "pop2", "udp")==0)
    return 0;

  return 1;
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

  update_port_table(&dbConn);

  mysql_close(&dbConn);

  return 1;
}
