#include "../Headers/DataBaseConnect.h"
using namespace std;





DataBaseConnect::DataBaseConnect() {

    /* INITIALIZE CONNECTION HANDLER, DO NOT CHANGE */
    conn = mysql_init(NULL);

    /* THIS CONNECTS TO SERVER, DO NOT CHANGE ANYTHING HERE */
    mysql_real_connect(conn, opt_host_name, opt_user_name, opt_password,
        opt_db_name, opt_port_num, opt_socket_name, opt_flags);
    /* show tables in the database (test for errors also) */
    

    /* disconnect from server */
    //mysql_close(conn);
}

bool DataBaseConnect::loginQuery(string emailParam, string passwdParam) {
    string loginQuery = selectAll + teachersTableName + "where email = BINARY " + "'" + emailParam + "' and passwd = " + "BINARY '" + passwdParam +"'";
    cout << "Consulta " << loginQuery << endl;
    mysql_query(conn, loginQuery.data());
    res = mysql_store_result(conn);

    

    // get the number of the columns
    int num_fields = mysql_num_fields(res);
    // Fetch all rows from the result
    if ((row = mysql_fetch_row(res))) {
        return true;
    } else {
        return false;
    }
}
