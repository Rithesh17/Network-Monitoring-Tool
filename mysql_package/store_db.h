#ifndef __STORE_DB_H__
#define __STORE_DB_H__

#include <my_global.h>
#include <mysql.h>

MYSQL* initDB(char* user, char* passwd);

int update_icmp_packet(char* csv_file, MYSQL* dbConn, int tup_no);

int update_pcap_table(char* filename, MYSQL* dbConn, int tup_no);

int update_tcp_packet(char* csv_file, MYSQL* dbConn, int tup_no);

int error_sql(MYSQL* dbConn);

int getTupleNum(MYSQL* dbConn, char* table_name);

#endif
