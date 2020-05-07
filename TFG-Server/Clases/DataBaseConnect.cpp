#include "../Headers/DataBaseConnect.h"
using namespace std;





DataBaseConnect::DataBaseConnect() {

    /* INITIALIZE CONNECTION HANDLER, DO NOT CHANGE */

    conn = mysql_init(NULL);
    mysql_options(conn, MYSQL_SET_CHARSET_NAME, "utf8mb4");
    mysql_options(conn, MYSQL_INIT_COMMAND, "SET NAMES utf8mb4");

    

    /* THIS CONNECTS TO SERVER, DO NOT CHANGE ANYTHING HERE */
    mysql_real_connect(conn, opt_host_name, opt_user_name, opt_password,
        opt_db_name, opt_port_num, opt_socket_name, opt_flags);
    /* show tables in the database (test for errors also) */
    

    /* disconnect from server */
    //mysql_close(conn);
}

bool DataBaseConnect::loginQuery(string emailParam, string passwdParam) {

    string loginQueryString = selectAll + teachersTableName + "where email = BINARY " + "'" + emailParam + "' and passwd = " + "BINARY '" + passwdParam +"'";
    cout << "Consulta " << loginQueryString << endl;
    mysql_query(conn, loginQueryString.data());
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

bool DataBaseConnect::checkNameOfNormalQuestionExist(string nameOfNormalModel) {
    string queryString = select_name_normal_model + normalModelsTableName + "where nombre_modelo = BINARY " + "'" + nameOfNormalModel + "'";
    cout << "Consulta AQUII" << queryString << endl;
    mysql_query(conn, queryString.data());
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

bool DataBaseConnect::checkNameOfQuestionTestExist(string nameOfQuestionParam) {
    string loginQuery = select_id_preguta_normal + test_question_table + "where pregunta = BINARY " + "'" + nameOfQuestionParam + "'";
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

bool DataBaseConnect::insertNewTestQuestion(vector<string> allNewQuestionData) {
    try {
        string max_ID;
        string id_tema;

        
            
                string questionToInsert = allNewQuestionData[0];
                string get_max_id_query = select_max_id_question + test_question_table;

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

                string get_cod_theme = select_id_tema_reverse + themesTableName + " where nombre = BINARY " + "'" + allNewQuestionData[allNewQuestionData.size() -1] + "'";
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
            
                string insertQuestion = insertNewTestQuestionQuery + to_string(stoi(max_ID) + 1) + ", '" + allNewQuestionData[0]  + "', '" + allNewQuestionData[1]  + "', '" + allNewQuestionData[2] + "', '" + allNewQuestionData[3]  + "', '" + allNewQuestionData[4]  + "', '" + allNewQuestionData[5] + "' , " + id_tema + ", null)";
                cout << "CONSULTA: " << insertQuestion << endl;
                
                mysql_query(conn, insertQuestion.data());
                res = mysql_store_result(conn);
        
        return true;
    } catch (...) {
        return false;
    }
}

bool DataBaseConnect::insertNewNormalModification(vector<string> allDataNewModification) {
    try {
        string max_ID;

        for (int counter = 0; counter < allDataNewModification.size(); counter++) {
            if (counter == 0) {
                string questionToInsert = allDataNewModification[0];
                string get_max_id_query = select_max_id_question + normal_question_modification_table;

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

            } else {
                string insertQuestion = insertNewModificationQuestionQuery + to_string(stoi(max_ID) + 1) + ", " + allDataNewModification[0] + " , '" + allDataNewModification[1] + "')";
                mysql_query(conn, insertQuestion.data());
                res = mysql_store_result(conn);
            }
        }
        return true;
    } catch (...) {
        return false;
    }
}

bool DataBaseConnect::insertNewTestModification(vector<string> allDataNewModification) {
    try {
        string max_ID;

                string questionToInsert = allDataNewModification[0];
                string get_max_id_query = select_max_id_question + test_question_modification_table;

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

                string insertQuestion = insertNewModificationQuestionQueryTest + to_string(stoi(max_ID) + 1) + ", " + allDataNewModification[0] + " , '" + allDataNewModification[1] + "'" + " , '" + allDataNewModification[2] + "'" + " , '" + allDataNewModification[3] + "'" + " , '" + allDataNewModification[4] + "'" + " , '" + allDataNewModification[5] + "'" + " , '" + allDataNewModification[6] + "'" +")";
                
                mysql_query(conn, insertQuestion.data());
                res = mysql_store_result(conn);
        return true;
    } catch (...) {
        return false;
    }
}

bool DataBaseConnect::updateNormalQuestionsNewModel(vector<string> allQuestionsNewNormalModel) {
    try {

        string ID_model = allQuestionsNewNormalModel[0];

        for (int i = 1; i < allQuestionsNewNormalModel.size(); i+=2) {
            string updateQuery = "update " + normal_question_table + " set modelo_perteneciente = CASE WHEN modelo_perteneciente is null THEN '" + '"' + ID_model + '"' + "' ELSE JSON_MERGE(modelo_perteneciente, '" + '"' + ID_model + '"' + "') END where (id = " + allQuestionsNewNormalModel[i] + ")";
            mysql_query(conn, updateQuery.data());
            res = mysql_store_result(conn);
        }
        return true;
    } catch (...) {
        return false;
    }
}

string DataBaseConnect::insertNewNormalModel(vector<string> allDataNormalModel) {
    try {
        string max_ID;
        string id_tema;

        string get_max_id_query = select_max_id_question + normalModelsTableName;


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

        string get_cod_theme = select_id_tema_reverse + themesTableName + " where nombre = BINARY " + "'" + allDataNormalModel[2] + "'";

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

        string insertQuestion = insertNewNormalModelQuery + to_string(stoi(max_ID) + 1) + ", '" + allDataNormalModel[0] + "' , '" + allDataNormalModel[1] + "'" + " , " + id_tema + " , '" + allDataNormalModel[3] + "')";

        mysql_query(conn, insertQuestion.data());
        res = mysql_store_result(conn);
        return to_string(stoi(max_ID) + 1);
    } catch (...) {
        return "0";
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

string DataBaseConnect::select(string value) {
    string loginQuery = "select '" + value + "' from dual";
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

    string namesQuery = selectThemes + themesTableName + "where cod_asign = ( "  + select_cod_asign + subjectsTableNameWithOutSpaces + " where nombre_asign = '" + subjectParam + "')";
    cout << "Consulta " << namesQuery << endl;
    
    mysql_query(conn, namesQuery.data());
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

vector<string> DataBaseConnect::getAllNormalQuestions(string nameOfSubjectParam) {
    vector <string> allQuestionsSpecificSubject;

    string query = select_id_question_specific_subject_normal_questions + normal_question_table + "where tema_perteneciente in ( " + select_id_tema_reverse + 
        themesTableName + " where cod_asign = (" + select_cod_asign + subjects + " where nombre_asign = '" + nameOfSubjectParam + "'))";
    cout << "Consulta  " << query << endl;

    mysql_query(conn, query.data());
    res = mysql_store_result(conn);

    // get the number of the columns
    int num_fields = mysql_num_fields(res);
    // Fetch all rows from the result
    while ((row = mysql_fetch_row(res))) {
        for (int i = 0; i < num_fields; i++) {
            // Make sure row[i] is valid!
            if (row[i] != NULL) {
                allQuestionsSpecificSubject.push_back(row[i]);
            }
        }
    }


    return allQuestionsSpecificSubject;
}

vector<string> DataBaseConnect::getAllTestQuestions(string nameOfSubjectParam) {
    vector <string> allQuestionsSpecificSubject;

    string query = select_id_question_specific_subject_test_questions + test_question_table + "where tema_perteneciente in ( " + select_id_tema_reverse +
        themesTableName + " where cod_asign = (" + select_cod_asign + subjects + " where nombre_asign = '" + nameOfSubjectParam + "'))";
    cout << "Consulta  ACA" << query << endl;

    mysql_query(conn, query.data());
    res = mysql_store_result(conn);

    // get the number of the columns
    int num_fields = mysql_num_fields(res);
    // Fetch all rows from the result
    while ((row = mysql_fetch_row(res))) {
        for (int i = 0; i < num_fields; i++) {
            // Make sure row[i] is valid!
            if (row[i] != NULL) {
                allQuestionsSpecificSubject.push_back(row[i]);
            }
        }
    }


    return allQuestionsSpecificSubject;
}

vector<string> DataBaseConnect::getAllNormalQuestionsSpecificTheme(string nameOfTheme) {

    vector <string> allNormalQuestionsSpecificTheme;

    string namesQuery = select_id_question_specific_subject_normal_questions + normal_question_table + "where tema_perteneciente = ( " + select_id_tema_reverse + themesTableName + " where nombre = '" + nameOfTheme + "')";
    cout << "Consulta" << namesQuery << endl;

    mysql_query(conn, namesQuery.data());
    res = mysql_store_result(conn);

    // get the number of the columns
    int num_fields = mysql_num_fields(res);
    // Fetch all rows from the result
    while ((row = mysql_fetch_row(res))) {
        for (int i = 0; i < num_fields; i++) {
            // Make sure row[i] is valid!
            if (row[i] != NULL) {
                allNormalQuestionsSpecificTheme.push_back(row[i]);
            }
        }
    }

    return allNormalQuestionsSpecificTheme;
}
