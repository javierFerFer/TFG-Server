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

string DataBaseConnect::nameQuery(string emailParam) {
    string loginQuery = selectName + teachersTableName + "where email = BINARY " + "'" + emailParam + "'";
    cout << "Consulta " << loginQuery << endl;
    mysql_query(conn, loginQuery.data());
    res = mysql_store_result(conn);



    // get the number of the columns
    int num_fields = mysql_num_fields(res);
    // Fetch all rows from the result
    if ((row = mysql_fetch_row(res))) {
        for (int i = 0; i < num_fields; i++) {
            // Make sure row[i] is valid!
            if (row[i] != NULL) {
                return row[i];
            } else {
                return "";
            }
        }
    } else {
        return "";
    }
}

vector<string> DataBaseConnect::getAllNamesOfSubjects(string emailParam) {
    vector <string> allSubjects;

    for (int counter = 0; counter < 5; counter++) {
        string allSubjectsQuery = selectSubject + teachersTableName + ", " + subjects + "where " + teachersTableNameWithOutSpaces + ".email = '" + emailParam + "' and "
            + teachersTableNameWithOutSpaces + ".asign_" + to_string((counter + 1)) + " = " + subjectsTableNameWithOutSpaces + ".cod_asign";
        cout << "Consulta " << allSubjectsQuery << endl;
        mysql_query(conn, allSubjectsQuery.data());
        res = mysql_store_result(conn);

        // get the number of the columns
        int num_fields = mysql_num_fields(res);
        // Fetch all rows from the result
        if ((row = mysql_fetch_row(res))) {
            for (int i = 0; i < num_fields; i++) {
                // Make sure row[i] is valid!
                if (row[i] != NULL) {
                    allSubjects.push_back(row[i]);
                }
            }
        }
    }

    return allSubjects;

}

vector<string> DataBaseConnect::getAllNamesOfThemes(string subjectParam) {
    vector <string> allThemes;

    string loginQuery = selectThemes + themesTableName + "where cod_asign = ( "  + select_cod_asign + subjectsTableNameWithOutSpaces + " where nombre_asign = '" + subjectParam + "')";
    cout << "Consulta " << loginQuery << endl;
    
    mysql_query(conn, loginQuery.data());
    res = mysql_store_result(conn);

    // get the number of the columns
    int num_fields = mysql_num_fields(res);
    // Fetch all rows from the result
    while ((row = mysql_fetch_row(res))) {
        for (int i = 0; i < num_fields; i++) {
            // Make sure row[i] is valid!
            if (row[i] != NULL) {
                allThemes.push_back(row[i]);
            }
        }
    }

    return allThemes;
}
