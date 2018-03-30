// The header file connecting the initDB function, the store_db function
// and the update_all_tables function. All the three functions are in
// different files, and hence this header file connects them.

#ifndef __STORE_DB_H__
#define __STORE_DB_H__

#include <my_global.h>
#include <mysql.h>
#include <sys/stat.h>

extern MYSQL* initDB(char* user, char* passwd);

extern int update_all_tables(char* pcap_filename, char* csv_file, MYSQL* dbConn);

#endif
