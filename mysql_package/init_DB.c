//Tis is the initialisation of the connection with
//the MySQL database, and initialisation of the
//database and the tables required for our project

//Tables required: .pcap table(file_path, inode, timestamp)
//

//When an error is found, it is displayed on a window and
//NULL is returned, instead of exiting at that point. This
//is done so as not to disturb other functions of the
//packet sniffer.

#include<stdio.h>
#include<mysql.h>
#include<my_global.h>

//This is the error function for any errors
//found in the initDB() function.
//
//Parameters: conn: The connection handler to MySQL
//
//Return Value: None

void mysqlError(MYSQL* conn)
{
  printf("Error: %s\n", mysql_error(conn));
  mysql_close(conn);
}

//This is the actual function involved in establishing
//a connection with MySQL and creating the database with
//required tables

//Parameters: user: The username of the user accessing the database
//          passwd: The password of the user accessing the database

//Return value: On success, the function returns the handler of the
//              connection. If error found, it is displayed on a
//              window and NULL is returned

MYSQL* initDB(char* user, char* passwd)
{
  MYSQL newConn;

  //Initialising the handler
  if(mysql_init(&newConn) == NULL)
  {
    printf("Error: %s\n", mysql_error(&newConn));
    return NULL;
  }

  //Connecting to MySQL through the handler.
  if(mysql_real_connect(&newConn, "localhost", user, passwd, NULL, 3306, NULL, 0)==NULL)
  {
    printf("Error: %s\n", mysql_error(&newConn));
    return NULL;
  }

  //Creating a database of the name 'packet_analyser' to store the
  //tables, if it does not exist
  if(mysql_query(&newConn, "CREATE DATABASE IF NOT EXISTS packet_analyser;")!=0)
  {
    mysqlError(&newConn);
    return NULL;
  }

  //Using the contents of the database packet_analyser
  mysql_query(&newConn, "USE packet_analyser;");

  //Creating the p_cap table if it does not exist
  if(mysql_query(&newConn, "CREATE TABLE IF NOT EXISTS p_cap\
                            (SerNo INT PRIMARY KEY, timestamp INT,\
                            file_path TEXT, i_node INT);")!=0)
  {
    mysqlError(&newConn);
    return NULL;
  }

  //Creating the service_port table if it does not exist
  if(mysql_query(&newConn, "CREATE TABLE IF NOT EXISTS service_port\
                            (SerNo INT PRIMARY KEY, serv_port INT,\
                            service TEXT, protocol TEXT);")!=0)
  {
    mysqlError(&newConn);
    return NULL;
  }

  //Creating the ip_url table if it does not exist
  if(mysql_query(&newConn, "CREATE TABLE IF NOT EXISTS ip_url\
                            (SerNo INT PRIMARY KEY, ip_addr TEXT,\
                            url_addr TEXT);")!=0)
  {
    mysqlError(&newConn);
    return NULL;
  }

  //Creating the ip_ether table if it does not exist
  if(mysql_query(&newConn, "CREATE TABLE IF NOT EXISTS ip_ether\
                            (SerNo INT PRIMARY KEY, ip_addr TEXT,\
                            ether_addr TEXT);")!=0)
  {
    mysqlError(&newConn);
    return NULL;
  }

  return &newConn;
}

int main()
{
  //Please enter the username and password in initDB
  initDB(user, passwd);
  return 0;
}
