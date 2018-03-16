This is the database for the Network monitoring software

Files contained in this zip folder (in the order to be executed):

1. init_DB: The file which creates the database 'packet_analyser'
            and initialises all the required tables.

2. update_port_table: This function is to be executed only once.
                      The file is not yet ready (having some nonsense
                      errors)

3. update_pcap_table: This function is to be executed every time
                      a packet is captured and parsed.

4. update_ether_table: This function is to be executed every time a
                       a packet is captured and parsed, but the table
                       will occasionally be updated (only when a new
                       ip address arrives).

5. update_url_table: This function is also to be executed every time,
                     but the table is occasionally updated (only when
                     a new ip address arrives).

6. update_pkt_info: This function is to be executed in frequent periodic
                    fashion. It reads the contents of the .csv file and
                    adds it to the database.

Please note that:
a. Each file has its own 'main' file. It was intended for unit
   tests, and must be removed while integrating with other modules.

b. Read the comments in each of the file to get the functioning, the
   arguments to be passed, and the return value of the function.

c. For accessing the MySQL database, root permissions are required. Also,
   the username and the password of the authorised user is needed to login
   to the database.
