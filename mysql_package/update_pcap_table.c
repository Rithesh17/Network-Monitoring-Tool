//This function is to add a tuple to the p_cap table
//every time a .pcap file is created.

#include<my_global.h>
#include<mysql.h>
#include<sys/stat.h>

//Function to update the p_cap table of
//packet_analyser database
//
//Parameters: filename: The file name of .pcap file
//            conn: The connection handler of MYSQL
//           tup_no: The value of the last tuple
//
//Returm value: If successful, returns the row value of
//              the last added tuple. Else, returns 0
//
int update_pcap_table(const char* filename, MYSQL* dbConn, int tup_no)
{
  //Using stat to get the timestamp and the inode value
  //of the file

  struct stat fileStat;
  if(stat(filename, &fileStat)!= 0)
  {
    printf("Error: file stat not obtained");
    return 0;
  }

  //Using the contents of the database packet_analyser
  mysql_query(dbConn, "USE packet_analyser;");

  char query[1000];

  int s = sprintf(query, "INSERT INTO p_cap(SerNo, timestamp, file_path,\
                          i_node) VALUES (%d, %d, \"%s\", %d);", tup_no,\
                          fileStat.st_mtim.tv_sec, filename, \
                          (int)fileStat.st_ino);

  int len = strlen(query);

  if(mysql_real_query(dbConn, query, len)!=0)
  {
    printf("Error: %s\n", mysql_error(dbConn));
    mysql_close(dbConn);
    return 0;
  }

  tup_no++;
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

  update_pcap_table("/home/ritesh/Desktop/prio_with_aging.c", &dbConn, 1);
}
