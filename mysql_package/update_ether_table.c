//This function inserts a tuple to the ip_ether table
//every time a new ip_addr or ether_addr is found.

#include<my_global.h>
#include <mysql.h>

//Function to update the ip_ether table of
//packet_analyser database
//
//Parameters: dbConn: The connection handler of MYSQL
//            ip: The ip address to be added to the table
//         ether: The ethernet address to be added to the table
//         tup_no: The tuple number of the last added tuple
//
//Returm value: If successful, returns the row value of
//              the last added tuple. Else, returns 0
//

int update_ether_table(MYSQL* dbConn, char* ip, char* ether, int tup_no)
{
  //Using the database 'packet_analyser'
  if(mysql_query(dbConn, "USE packet_analyser")!=0)
  {
    printf("Error: %s\n", mysql_error(dbConn));
    mysql_close(dbConn);
    return 0;
  }

  //To store the query in the form of a string
  char query[200];

  //Here, we are checking if there is any tuple with the
  //same ip_addr and ether_addr as the one to be added. If
  //yes, then we need not update the table
  sprintf(query, "SELECT * FROM ip_ether WHERE ip_addr=\"%s\" \
                          AND ether_addr=\"%s\";", ip, ether);

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

  //Only if the nuber of rows are 0 (that is, there is
  //no tuple) we need to add the tuple into the table
  if(num_rows==0)
  {
    sprintf(query, "INSERT INTO ip_ether(SerNo, ip_addr, ether_addr)\
                    VALUES (%d, \"%s\", \"%s\");", tup_no++, ip, ether);

    int len = strlen(query);

    if(mysql_real_query(dbConn, query, len)!=0)
    {
      printf("Error: %s\n", mysql_error(dbConn));
      mysql_close(dbConn);
      return 0;
    }
  }
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

  int tup_no = 1;
  tup_no = update_ether_table(&dbConn, "10.42.0.5", "10.50.22.35", 1);

  mysql_close(&dbConn);
  return 1;
}
