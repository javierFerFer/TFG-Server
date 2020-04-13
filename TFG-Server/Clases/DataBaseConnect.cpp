#include "../Headers/DataBaseConnect.h"
using namespace std;


static char* opt_host_name = "localhost"; /* HOST */
static char* opt_user_name = "root"; /* USERNAME */
static char* opt_password = "Luna1000"; /* PASSWORD */
static unsigned int opt_port_num = 3306; /* PORT */
static char* opt_socket_name = NULL; /* SOCKET NAME, DO NOT CHANGE */
static char* opt_db_name = "test"; /* DATABASE NAME */
static unsigned int opt_flags = 0; /* CONNECTION FLAGS, DO NOT CHANGE */


DataBaseConnect::DataBaseConnect() {

    MYSQL* conn; /* pointer to connection handler */
    MYSQL_RES* res; /* holds the result set */
    MYSQL_ROW row;



    /* INITIALIZE CONNECTION HANDLER, DO NOT CHANGE */
    conn = mysql_init(NULL);

    /* THIS CONNECTS TO SERVER, DO NOT CHANGE ANYTHING HERE */
    mysql_real_connect(conn, opt_host_name, opt_user_name, opt_password,
        opt_db_name, opt_port_num, opt_socket_name, opt_flags);
    /* show tables in the database (test for errors also) */
    mysql_query(conn, "SELECT * from testeo");
    res = mysql_store_result(conn);


    // get the number of the columns
    int num_fields = mysql_num_fields(res);
    // Fetch all rows from the result
    while ((row = mysql_fetch_row(res))) {
        // Print all columns
        for (int i = 0; i < num_fields; i++) {
            // Make sure row[i] is valid!
            if (row[i] != NULL) {
                cout << row[i] << endl;
                cout << "here" << endl;
            } else {
                cout << "NULL" << endl;
            }

            // Also, you can use ternary operator here instead of if-else
            // cout << row[i] ? row[i] : "NULL" << endl;
        }
    }

    // DON'T FORGET TO CLEAN RESULT AFTER YOU DON'T NEED IT 
    // ANYMORE

    if (res != NULL)
        mysql_free_result(res);

    /* disconnect from server */
    mysql_close(conn);
}
