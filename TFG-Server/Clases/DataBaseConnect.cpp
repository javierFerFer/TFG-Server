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

bool DataBaseConnect::checkNameOfThemeExist(string nameOfThemeParam) {
    string loginQuery = select_id_tema + themesTableName + "where nombre = BINARY " + "'" + nameOfThemeParam + "'";
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

bool DataBaseConnect::checkNameOfQuestionExist(string nameOfQuestionParam) {
    string loginQuery = select_id_preguta_normal + normal_question_table + "where pregunta = BINARY " + "'" + nameOfQuestionParam + "'";
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

bool DataBaseConnect::insertNewTheme(vector<string> allNewThemeData) {
    try {
        string max_ID;
        string cod_asign;

        for (int counter = 0; counter < allNewThemeData.size(); counter++) {
            if (counter == 0) {
                string nameOfTheme = allNewThemeData[0];
                string get_max_id_query = select_max_id_theme + themesTableName;
                mysql_query(conn, get_max_id_query.data());
                res = mysql_store_result(conn);

                // get the number of the columns
                int num_fields = mysql_num_fields(res);
                // Fetch all rows from the result
                if ((row = mysql_fetch_row(res))) {
                    for (int i = 0; i < num_fields; i++) {
                        // Make sure row[i] is valid!
                        if (row[i] != NULL) {
                            max_ID = row[i];
                        } else {
                            max_ID = "0";
                        }
                    }
                }

                string get_cod_asign_query = select_cod_asign + subjectsTableNameWithOutSpaces + " where nombre_asign = BINARY " + "'" + allNewThemeData[1] + "'";
                mysql_query(conn, get_cod_asign_query.data());
                res = mysql_store_result(conn);

                // get the number of the columns
                num_fields = mysql_num_fields(res);
                // Fetch all rows from the result
                if ((row = mysql_fetch_row(res))) {
                    for (int i = 0; i < num_fields; i++) {
                        // Make sure row[i] is valid!
                        if (row[i] != NULL) {
                            cod_asign = row[i];
                        }
                    }
                }
            } else {
                string insertData = insertNewThemeSentence + to_string(stoi(max_ID) + 1) + ", '" + allNewThemeData[0] + "' , " + cod_asign + " )";
                mysql_query(conn, insertData.data());
                res = mysql_store_result(conn);
            }
        }
        return true;
    } catch (...) {
        return false;
    }
}

bool DataBaseConnect::insertNewQuestion(vector<string> allNewQuestionData) {
    try {
        string max_ID;
        string id_tema;

        for (int counter = 0; counter < allNewQuestionData.size(); counter++) {
            if (counter == 0) {
                string questionToInsert = allNewQuestionData[0];
                string get_max_id_query = select_max_id_question + normal_question_table;
                
                mysql_query(conn, get_max_id_query.data());
                res = mysql_store_result(conn);

                // get the number of the columns
                int num_fields = mysql_num_fields(res);
                // Fetch all rows from the result
                if ((row = mysql_fetch_row(res))) {
                    for (int i = 0; i < num_fields; i++) {
                        // Make sure row[i] is valid!
                        if (row[i] != NULL) {
                            max_ID = row[i];
                        } else {
                            max_ID = "0";
                        }
                    }
                }

                string get_cod_theme = select_id_tema_reverse + themesTableName + " where nombre = BINARY " + "'" + allNewQuestionData[1] + "'";
                mysql_query(conn, get_cod_theme.data());
                res = mysql_store_result(conn);

                // get the number of the columns
                num_fields = mysql_num_fields(res);
                // Fetch all rows from the result
                if ((row = mysql_fetch_row(res))) {
                    for (int i = 0; i < num_fields; i++) {
                        // Make sure row[i] is valid!
                        if (row[i] != NULL) {
                            id_tema = row[i];
                        }
                    }
                }
            } else {
                string insertQuestion = insertNewQuestionQuery + to_string(stoi(max_ID) + 1) + ", '" + allNewQuestionData[0] + "' , " + id_tema + ", null)";
                mysql_query(conn, insertQuestion.data());
                res = mysql_store_result(conn);
            }
        }
        return true;
    } catch (...) {
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
