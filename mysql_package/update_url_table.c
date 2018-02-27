//This function inserts a tuple to the ip_url table
//every time a new ip_addr is found.

#include<my_global.h>
#include <mysql.h>

//Function to update the ip_ether table of
//packet_analyser database
//
//Parameters: newConn: The connection handler of MYSQL
//            ip: The ip address to be added to the table
//           url: The URL address to be added to the table
//         tup_no: The tuple number of the last added tuple
//
//Returm value: If successful, returns the row value of
//              the last added tuple. Else, returns 0
//

int update_url_table(MYSQL* newConn, char* ip, char* url, int tup_no)
{
  //Using the database 'packet_analyser'
  if(mysql_query(newConn, "USE packet_analyser")!=0)
  {
    printf("Error: %s\n", mysql_error(newConn));
    mysql_close(newConn);
    return 0;
  }

  //To store the query in the form of a string
  char query[200];

  //Here, we are checking if there is any tuple with the
  //same ip_addr as the one to be added. If
  //yes, then we need nt update the table
  sprintf(query, "SELECT * FROM ip_url WHERE ip_addr=\"%s\";", ip);

  int len = strlen(query);

  //The query is called here
  if(mysql_real_query(newConn, query, len)!=0)
  {
    printf("Error: %s\n", mysql_error(newConn));
    mysql_close(newConn);
    return 0;
  }

  //The result of the query is captured
  MYSQL_RES* result = mysql_store_result(newConn);

  if(result==NULL)
  {
    printf("Error: %s\n", mysql_error(newConn));
    mysql_close(newConn);
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
    sprintf(query, "INSERT INTO ip_url(SerNo, ip_addr, url_addr)\
                    VALUES (%d, \"%s\", \"%s\");", tup_no++, ip, url);

    int len = strlen(query);

    if(mysql_real_query(newConn, query, len)!=0)
    {
      printf("Error: %s\n", mysql_error(newConn));
      mysql_close(newConn);
      return 0;
    }
  }
  return tup_no;
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

  int tup_no = 1;
  tup_no = update_url_table(&newConn, "172.16.230.254", "https://www.google.com", tup_no);

  mysql_close(&newConn);
  return 1;
}
