#include <my_global.h>
#include <mysql.h>

int mysql_start(void* args)
{
  char* login_info[2];
  login_info = (char**)args;

  MySQL* newConn;
  newConn = initDB(login_info[0], login_info[1]);

  if(update_port_table(newConn)==0)
    printf("Error in updating port table\n");

  
}
