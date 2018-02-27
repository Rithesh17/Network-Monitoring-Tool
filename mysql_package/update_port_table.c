//This function is to fill the service_port table.
//The table will be filled only once, and will
//be referred to if required.

#include<my_global.h>
#include<mysql.h>

//This is the error function for errors found in
//the update_port_table() and send_query()
//functions.
//
//Parameters: newConn: The connection handler to MySQL
//
//Return Value: 0

int error_sql(MYSQL* newConn)
{
    printf("Error: %s\n", mysql_error(newConn));
    mysql_close(newConn);
    return 0;
}

// Function to send the query to MySQL, to create instances of the
// serv_port table
//
// Parameters: query: The query for MySQL in the form of a string
//             newconn: The connection handler of MySQL
//             SerNo: The tuple number of the new tuple (Primary key)
//             serv_port: The port number of the service
//             service: The name of the service
//             protocol: TCP or UDP service
//
// Return Value: If successful, returns 1. Else, returns 0;

int send_query(char* query, MYSQL* newconn, int SerNo, int serv_port, \
                char* service, char* protocol)
{
  if(sprintf(query, "INSERT INTO service_port(SerNo, \
                   serv_port, service, protocol) VALUES \
                   (%d, %d, \"%s\", \"%s\");", SerNo++, \
                    serv_port, service, protocol)==0)
    return error_sql(newconn);

  int len = strlen(query);

  if(mysql_real_query(newconn, query, len)!=0)
    return error_sql(newconn);

  return 1;
}

//Function to update the service_port table of
//packet_analyser database.
//
//Parameters: newConn: The connection handler to MySQL
//
//Reture Value: If successful, returns 1.
//              Else, returns 0.

int update_port_table(MYSQL* newConn)
{

  if(mysql_query(newConn, "USE packet_analyser")!=0)
    return error_sql(newConn);

  char query[200];

  int Ser = 1;

  if(send_query(query, newConn, Ser, 21, "ftp", "tcp")==0)
    return 0;
  Ser++;
  if(send_query(query, newConn, Ser, 21, "fsp", "udp")==0)
    return 0;
  Ser++;
  if(send_query(query, newConn, Ser, 22, "ssh", "tcp")==0)
    return 0;
  Ser++;
  if(send_query(query, newConn, Ser, 22, "ssh", "udp")==0)
    return 0;
  Ser++;
  if(send_query(query, newConn, Ser, 23, "telnet", "tcp")==0)
    return 0;
  Ser++;
  if(send_query(query, newConn, Ser, 25, "smtp", "tcp")==0)
    return 0;
  Ser++;
  if(send_query(query, newConn, Ser, 80, "http", "tcp")==0)
    return 0;
  Ser++;
  if(send_query(query, newConn, Ser, 80, "http", "udp")==0)
    return 0;
  Ser++;
  if(send_query(query, newConn, Ser, 109, "pop2", "tcp")==0)
    return 0;
  Ser++;
  if(send_query(query, newConn, Ser, 109, "pop2", "udp")==0)
    return 0;
  Ser++;

  return Ser;
}

int main()
{
  MYSQL newConn;

  //Initialising the handler
  if(mysql_init(&newConn) == NULL)
  {
    printf("Error: %s\n", mysql_error(&newConn));
    return 0;
  }

  //Connecting to MySQL through the handler.
  if(mysql_real_connect(&newConn, "localhost", "root", "nirmala17", NULL, 3306, NULL, 0)==NULL)
  {
    printf("Error: %s\n", mysql_error(&newConn));
    return 0;
  }

  update_port_table(&newConn);

  mysql_close(&newConn);

  return 1;
}
