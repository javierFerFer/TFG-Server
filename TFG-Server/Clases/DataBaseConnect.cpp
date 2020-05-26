#include "../Headers/DataBaseConnect.h"
using namespace std;


DataBaseConnect::DataBaseConnect() {

    /*
    Inicialización de la conexión con la base de datos
    Se establece la codificación en UTF-8
    */

    conn = mysql_init(NULL);
    mysql_options(conn, MYSQL_SET_CHARSET_NAME, "utf8mb4");
    mysql_options(conn, MYSQL_INIT_COMMAND, "SET NAMES utf8mb4");

    

    /*
    Conexión con la base de datos
    */
    mysql_real_connect(conn, opt_host_name, opt_user_name, opt_password,
        opt_db_name, opt_port_num, opt_socket_name, opt_flags);
    /* show tables in the database (test for errors also) */
    

    /*
    Desconexión con la base de datos
    */
    //mysql_close(conn);
}

bool DataBaseConnect::loginQuery(string emailParam, string passwdParam) {

    string loginQueryString = selectAll + teachersTableName + "where email = BINARY " + "'" + emailParam + "' and passwd = " + "BINARY '" + passwdParam +"'";
    mysql_query(conn, loginQueryString.data());
    res = mysql_store_result(conn);

    

    // Obtiene el número de columnas
    int num_fields = mysql_num_fields(res);
    // Almacena todos los resultados de la fila
    if ((row = mysql_fetch_row(res))) {
        return true;
    } else {
        return false;
    }
}



bool DataBaseConnect::checkNameOfThemeExist(string nameOfThemeParam) {
    string loginQuery = select_id_tema + themesTableName + "where nombre = BINARY " + "'" + nameOfThemeParam + "'";
    mysql_query(conn, loginQuery.data());
    res = mysql_store_result(conn);

    int num_fields = mysql_num_fields(res);

    if ((row = mysql_fetch_row(res))) {
        return true;
    } else {
        return false;
    }
}

bool DataBaseConnect::checkNameOfQuestionExist(string nameOfQuestionParam) {
    string loginQuery = select_id_preguta_normal + normal_question_table + "where pregunta = BINARY " + "'" + nameOfQuestionParam + "'";
    mysql_query(conn, loginQuery.data());
    res = mysql_store_result(conn);

    int num_fields = mysql_num_fields(res);

    if ((row = mysql_fetch_row(res))) {
        return true;
    } else {
        return false;
    }
}

bool DataBaseConnect::checkNameOfNormalQuestionExist(string nameOfNormalModel) {
    string queryString = select_name_normal_model + normalModelsTableName + "where nombre_modelo = BINARY " + "'" + nameOfNormalModel + "'";
    mysql_query(conn, queryString.data());
    res = mysql_store_result(conn);


    int num_fields = mysql_num_fields(res);

    if ((row = mysql_fetch_row(res))) {
        return true;
    } else {
        return false;
    }
}

bool DataBaseConnect::checkNameOfTestQuestionExist(string nameOfTestModel) {
    string queryString = select_name_normal_model + testModelsTableName + "where nombre_modelo = BINARY " + "'" + nameOfTestModel + "'";
    mysql_query(conn, queryString.data());
    res = mysql_store_result(conn);


    int num_fields = mysql_num_fields(res);

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


    int num_fields = mysql_num_fields(res);

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

                int num_fields = mysql_num_fields(res);

                if ((row = mysql_fetch_row(res))) {
                    for (int i = 0; i < num_fields; i++) {

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

                num_fields = mysql_num_fields(res);

                if ((row = mysql_fetch_row(res))) {
                    for (int i = 0; i < num_fields; i++) {

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

                int num_fields = mysql_num_fields(res);

                if ((row = mysql_fetch_row(res))) {
                    for (int i = 0; i < num_fields; i++) {

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

                num_fields = mysql_num_fields(res);

                if ((row = mysql_fetch_row(res))) {
                    for (int i = 0; i < num_fields; i++) {

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

                int num_fields = mysql_num_fields(res);

                if ((row = mysql_fetch_row(res))) {
                    for (int i = 0; i < num_fields; i++) {

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

                num_fields = mysql_num_fields(res);

                if ((row = mysql_fetch_row(res))) {
                    for (int i = 0; i < num_fields; i++) {

                        if (row[i] != NULL) {
                            id_tema = row[i];
                        }
                    }
                }
            
                string insertQuestion = insertNewTestQuestionQuery + to_string(stoi(max_ID) + 1) + ", '" + allNewQuestionData[0]  + "', '" + allNewQuestionData[1]  + "', '" + allNewQuestionData[2] + "', '" + allNewQuestionData[3]  + "', '" + allNewQuestionData[4]  + "', '" + allNewQuestionData[5] + "' , " + id_tema + ", null)";
                
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

                int num_fields = mysql_num_fields(res);

                if ((row = mysql_fetch_row(res))) {
                    for (int i = 0; i < num_fields; i++) {

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

                int num_fields = mysql_num_fields(res);

                if ((row = mysql_fetch_row(res))) {
                    for (int i = 0; i < num_fields; i++) {

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

bool DataBaseConnect::updateTestQuestionsNewModel(vector<string> allQuestionsNewTestModel) {
    try {

        string ID_model = allQuestionsNewTestModel[0];

        for (int i = 1; i < allQuestionsNewTestModel.size(); i += 7) {
            string updateQuery = "update " + test_question_table + " set modelo_perteneciente = CASE WHEN modelo_perteneciente is null THEN '" + '"' + ID_model + '"' + "' ELSE JSON_MERGE(modelo_perteneciente, '" + '"' + ID_model + '"' + "') END where (id = " + allQuestionsNewTestModel[i] + ")";
            mysql_query(conn, updateQuery.data());
            res = mysql_store_result(conn);
        }
        return true;
    } catch (...) {
        return false;
    }
}

void DataBaseConnect::activeNormalModification(string idNormalModification) {
    try {
        string updateQuery = "update " + normal_question_table + " left join " + normal_question_modification_table + " on preguntas_tipo_normal.id = modificaciones_pendientes_normal.id_reference set preguntas_tipo_normal.pregunta = modificaciones_pendientes_normal.pregunta where modificaciones_pendientes_normal.id = " + idNormalModification;
        mysql_query(conn, updateQuery.data());

        string deleteModification = "delete from " + normal_question_modification_table + " where id = " + idNormalModification;
        mysql_query(conn, deleteModification.data());
    } catch (...) {
        
    }
}

void DataBaseConnect::activeTestModification(string idTestModification) {
    try {
        string updateQuery = "update " + test_question_table + " left join " + test_question_modification_table + " on preguntas_tipo_test.id = modificaciones_pendientes_test.id_reference set preguntas_tipo_test.pregunta = modificaciones_pendientes_test.pregunta, preguntas_tipo_test.respuesta_A = modificaciones_pendientes_test.respuesta_A, preguntas_tipo_test.respuesta_B = modificaciones_pendientes_test.respuesta_B, preguntas_tipo_test.respuesta_C = modificaciones_pendientes_test.respuesta_C, preguntas_tipo_test.respuesta_D = modificaciones_pendientes_test.respuesta_D, preguntas_tipo_test.respuesta_correcta = modificaciones_pendientes_test.respuesta_correcta where modificaciones_pendientes_test.id = " + idTestModification;
        mysql_query(conn, updateQuery.data());

        string deleteModification = "delete from " + test_question_modification_table + " where id = " + idTestModification;
        mysql_query(conn, deleteModification.data());
    } catch (...) {

    }
}

void DataBaseConnect::deleteNormalModification(string idNormalModification) {
    try {
        string deleteModification = "delete from " + normal_question_modification_table + " where id = " + idNormalModification;
        mysql_query(conn, deleteModification.data());
    } catch (...) {

    }
}

void DataBaseConnect::deleteTestModification(string idTestModification) {
    try {
        string deleteModification = "delete from " + test_question_modification_table + " where id = " + idTestModification;
        mysql_query(conn, deleteModification.data());
    } catch (...) {

    }
}

void DataBaseConnect::deleteNormalQuestion(string idNormalModification) {
    try {
        string internalID;
        string lengthJSONModels;

        string query = select_id_reference + normal_question_modification_table + " where id = " + idNormalModification;
                
        mysql_query(conn, query.data());
        res = mysql_store_result(conn);

        int num_fields = mysql_num_fields(res);
        if ((row = mysql_fetch_row(res))) {
            for (int i = 0; i < num_fields; i++) {

                if (row[i] != NULL) {
                    internalID = row[i];
                } else {
                    internalID = "0";
                }
            }
        }

        if(internalID.compare("0") != 0){
            string removeAllNormalModificationOfQuestions = "delete from " + normal_question_modification_table + " where id_reference = " + internalID;

            mysql_query(conn, removeAllNormalModificationOfQuestions.data());
            res = mysql_store_result(conn);

            string getLengthJSON = selectJsonLength + "modelo_perteneciente) from " + normal_question_table + " where id = " + internalID;

            mysql_query(conn, getLengthJSON.data());
            res = mysql_store_result(conn);

            num_fields = mysql_num_fields(res);
            if ((row = mysql_fetch_row(res))) {
                for (int i = 0; i < num_fields; i++) {

                    if (row[i] != NULL) {
                        lengthJSONModels = row[i];
                    } else {
                        lengthJSONModels = "0";
                    }
                }
            }
            if (lengthJSONModels.compare("0") != 0) {
                vector <string> allNumbersOfModels;
                vector <string> allNumbersOfModelsToDelete;

                for (int counterOfModels = 0; counterOfModels < stoi(lengthJSONModels); counterOfModels++) {
                    string getNumberOfModel = selectJsonExtract + "modelo_perteneciente, '$[" + to_string(counterOfModels) + "]') from " + normal_question_table +  " where id = " + internalID;

                    mysql_query(conn, getNumberOfModel.data());
                    res = mysql_store_result(conn);


                    num_fields = mysql_num_fields(res);
                    if ((row = mysql_fetch_row(res))) {
                        for (int i = 0; i < num_fields; i++) {
                            if (row[i] != NULL) {
                                string tempData = row[i];
                                tempData = tempData.substr(1, tempData.size() - 2);
                                allNumbersOfModels.push_back(tempData);
                            }
                        }
                    }
                }

                // Borrado de la pregunta normal
                string deleteNormalQuest = "delete from " + normal_question_table + " where id = " + internalID;

                
                mysql_query(conn, deleteNormalQuest.data());
                res = mysql_store_result(conn);

                // Comprobación de que el modelo/s a los que pertenecía dicha pregunta tienen al menos una pregunta asociada
                for (int counterMod = 0; counterMod < allNumbersOfModels.size(); counterMod++) {
                    string getCountOfQuestions = selectCoutID + normal_question_table +  " where JSON_CONTAINS(modelo_perteneciente, " + "'" + '"' + allNumbersOfModels[counterMod] + '"' + "'" + ")";

                    mysql_query(conn, getCountOfQuestions.data());
                    res = mysql_store_result(conn);


                    num_fields = mysql_num_fields(res);
                    if ((row = mysql_fetch_row(res))) {
                        for (int i = 0; i < num_fields; i++) {
                            string tempData = row[i];
                            if (tempData.compare("0") == 0) {
                                // Borrado del modelo
                                allNumbersOfModelsToDelete.push_back(allNumbersOfModels[counterMod]);
                            }
                        }
                    }
                }

                if (allNumbersOfModels.size() != 0) {
                    for (int counterOfModelsToDelete = 0; counterOfModelsToDelete < allNumbersOfModelsToDelete.size(); counterOfModelsToDelete++) {

                        // Borrado de todos los modelos normales sin preguntas asociadas
                        string deleteQuery = "delete from modelo_normal where id = " + allNumbersOfModelsToDelete[counterOfModelsToDelete];

                        mysql_query(conn, deleteQuery.data());
                        res = mysql_store_result(conn);
                    }
                }

            } else {
            // Borrado de la pregunta en base a su ID
                string deleteNormalQuest = "delete from " + normal_question_table + " where id = " + internalID;


                mysql_query(conn, deleteNormalQuest.data());
                res = mysql_store_result(conn);
            }
        }

    } catch (...) {

    }
}

void DataBaseConnect::deleteTestQuestion(string idTestModification) {
    try {
        string internalID;
        string lengthJSONModels;

        string query = select_id_reference + test_question_modification_table + " where id = " + idTestModification;

        mysql_query(conn, query.data());
        res = mysql_store_result(conn);

        int num_fields = mysql_num_fields(res);
        if ((row = mysql_fetch_row(res))) {
            for (int i = 0; i < num_fields; i++) {

                if (row[i] != NULL) {
                    internalID = row[i];
                } else {
                    internalID = "0";
                }
            }
        }

        if (internalID.compare("0") != 0) {
            string removeAllNormalModificationOfQuestions = "delete from " + test_question_modification_table + " where id_reference = " + internalID;

            mysql_query(conn, removeAllNormalModificationOfQuestions.data());
            res = mysql_store_result(conn);

            string getLengthJSON = selectJsonLength + "modelo_perteneciente) from " + test_question_table + " where id = " + internalID;

            mysql_query(conn, getLengthJSON.data());
            res = mysql_store_result(conn);

            num_fields = mysql_num_fields(res);
            if ((row = mysql_fetch_row(res))) {
                for (int i = 0; i < num_fields; i++) {

                    if (row[i] != NULL) {
                        lengthJSONModels = row[i];
                    } else {
                        lengthJSONModels = "0";
                    }
                }
            }
            if (lengthJSONModels.compare("0") != 0) {
                vector <string> allNumbersOfModels;
                vector <string> allNumbersOfModelsToDelete;

                for (int counterOfModels = 0; counterOfModels < stoi(lengthJSONModels); counterOfModels++) {
                    string getNumberOfModel = selectJsonExtract + "modelo_perteneciente, '$[" + to_string(counterOfModels) + "]') from " + test_question_table + " where id = " + internalID;

                    mysql_query(conn, getNumberOfModel.data());
                    res = mysql_store_result(conn);

                    num_fields = mysql_num_fields(res);
                    if ((row = mysql_fetch_row(res))) {
                        for (int i = 0; i < num_fields; i++) {
                            if (row[i] != NULL) {
                                string tempData = row[i];
                                tempData = tempData.substr(1, tempData.size() - 2);
                                allNumbersOfModels.push_back(tempData);
                            }
                        }
                    }
                }

                // Borrado de la pregunta normal
                string deleteNormalQuest = "delete from " + test_question_table + " where id = " + internalID;


                mysql_query(conn, deleteNormalQuest.data());
                res = mysql_store_result(conn);

                // Comprobación de que el modelo/s a los que pertenecía dicha pregunta tienen al menos una pregunta asociada

                for (int counterMod = 0; counterMod < allNumbersOfModels.size(); counterMod++) {
                    string getCountOfQuestions = selectCoutID + test_question_table + " where JSON_CONTAINS(modelo_perteneciente, " + "'" + '"' + allNumbersOfModels[counterMod] + '"' + "'" + ")";

                    mysql_query(conn, getCountOfQuestions.data());
                    res = mysql_store_result(conn);

                    num_fields = mysql_num_fields(res);
                    if ((row = mysql_fetch_row(res))) {
                        for (int i = 0; i < num_fields; i++) {
                            string tempData = row[i];
                            if (tempData.compare("0") == 0) {

                                allNumbersOfModelsToDelete.push_back(allNumbersOfModels[counterMod]);
                            }
                        }
                    }
                }

                if (allNumbersOfModels.size() != 0) {
                    for (int counterOfModelsToDelete = 0; counterOfModelsToDelete < allNumbersOfModelsToDelete.size(); counterOfModelsToDelete++) {
                        // Borrado de todos los modelos normales sin preguntas asociadas
                        string deleteQuery = "delete from modelo_test where id = " + allNumbersOfModelsToDelete[counterOfModelsToDelete];

                        mysql_query(conn, deleteQuery.data());
                        res = mysql_store_result(conn);
                    }
                }

            } else {
                // Borrado de la pregunta en base a su ID
                string deleteNormalQuest = "delete from " + test_question_table + " where id = " + internalID;


                mysql_query(conn, deleteNormalQuest.data());
                res = mysql_store_result(conn);
            }
        }

    } catch (...) {

    }
}

string DataBaseConnect::insertNewNormalModel(vector<string> allDataNormalModel) {
    try {
        string max_ID;
        string id_tema;

        string get_max_id_query = select_max_id_question + normalModelsTableName;


        mysql_query(conn, get_max_id_query.data());
        res = mysql_store_result(conn);

        int num_fields = mysql_num_fields(res);
        if ((row = mysql_fetch_row(res))) {
            for (int i = 0; i < num_fields; i++) {
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

        num_fields = mysql_num_fields(res);

        if ((row = mysql_fetch_row(res))) {
            for (int i = 0; i < num_fields; i++) {

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

string DataBaseConnect::insertNewTestModel(vector<string> allDataTestModel) {
    try {
        string max_ID;
        string id_tema;

        string get_max_id_query = select_max_id_question + testModelsTableName;


        mysql_query(conn, get_max_id_query.data());
        res = mysql_store_result(conn);

        int num_fields = mysql_num_fields(res);

        if ((row = mysql_fetch_row(res))) {
            for (int i = 0; i < num_fields; i++) {

                if (row[i] != NULL) {
                    max_ID = row[i];
                } else {
                    max_ID = "0";
                }
            }
        }

        string get_cod_theme = select_id_tema_reverse + themesTableName + " where nombre = BINARY " + "'" + allDataTestModel[2] + "'";

        mysql_query(conn, get_cod_theme.data());
        res = mysql_store_result(conn);

        num_fields = mysql_num_fields(res);

        if ((row = mysql_fetch_row(res))) {
            for (int i = 0; i < num_fields; i++) {

                if (row[i] != NULL) {
                    id_tema = row[i];
                }
            }
        }

        string insertQuestion = insertNewTestModelQuery + to_string(stoi(max_ID) + 1) + ", '" + allDataTestModel[0] + "' , '" + allDataTestModel[1] + "'" + " , " + id_tema + " , '" + allDataTestModel[3] + "')";

        mysql_query(conn, insertQuestion.data());
        res = mysql_store_result(conn);
        return to_string(stoi(max_ID) + 1);
    } catch (...) {
        return "0";
    }
}

string DataBaseConnect::nameQuery(string emailParam) {
    string loginQuery = selectName + teachersTableName + "where email = BINARY " + "'" + emailParam + "'";
    mysql_query(conn, loginQuery.data());
    res = mysql_store_result(conn);

    int num_fields = mysql_num_fields(res);

    if ((row = mysql_fetch_row(res))) {
        for (int i = 0; i < num_fields; i++) {

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

    int num_fields = mysql_num_fields(res);

    if ((row = mysql_fetch_row(res))) {
        for (int i = 0; i < num_fields; i++) {

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

string DataBaseConnect::checkUserPermissionsToChange(string emailParam) {

    string query = selectAdminDepart + teachersTableName + " where email = admin_depart AND email = '" + emailParam + "'";

    mysql_query(conn, query.data());
    res = mysql_store_result(conn);

    int num_fields = mysql_num_fields(res);

    if ((row = mysql_fetch_row(res))) {
        for (int i = 0; i < num_fields; i++) {

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
        mysql_query(conn, allSubjectsQuery.data());
        res = mysql_store_result(conn);

        int num_fields = mysql_num_fields(res);

        if ((row = mysql_fetch_row(res))) {
            for (int i = 0; i < num_fields; i++) {

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
    
    mysql_query(conn, namesQuery.data());
    res = mysql_store_result(conn);

    int num_fields = mysql_num_fields(res);
    while ((row = mysql_fetch_row(res))) {
        for (int i = 0; i < num_fields; i++) {
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

    mysql_query(conn, query.data());
    res = mysql_store_result(conn);

    int num_fields = mysql_num_fields(res);
    while ((row = mysql_fetch_row(res))) {
        for (int i = 0; i < num_fields; i++) {
            if (row[i] != NULL) {
                allQuestionsSpecificSubject.push_back(row[i]);
            }
        }
    }


    return allQuestionsSpecificSubject;
}

vector<string> DataBaseConnect::getAllNormalQuestionsForModification(string emailUser) {
    vector <string> allData;
    vector <string> allNormalQuestionsID;
    vector <string> allNamesOfThemes;

    vector <string> allSubjectsOfUser = getAllNamesOfSubjects(emailUser);

    for (int counterOfSubjects = 0; counterOfSubjects < allSubjectsOfUser.size(); counterOfSubjects++) {
        vector <string> NamesOfThemes = getAllNamesOfThemes(allSubjectsOfUser[counterOfSubjects]);
        allNamesOfThemes.insert(allNamesOfThemes.end(), NamesOfThemes.begin(), NamesOfThemes.end());
    }

    for (int counterOfThemes = 0; counterOfThemes < allNamesOfThemes.size(); counterOfThemes++) {
        vector <string> normalQuesions = getAllNormalQuestionsSpecificThemeWithoutName(allNamesOfThemes[counterOfThemes]);
        allNormalQuestionsID.insert(allNormalQuestionsID.end(), normalQuesions.begin(), normalQuesions.end());
    }

    for (int counterQuestions = 0; counterQuestions < allNormalQuestionsID.size(); counterQuestions++) {
        vector <string> tempData = getAllNormalModificationsQuestions(allNormalQuestionsID[counterQuestions]);
        allData.insert(allData.end(), tempData.begin(), tempData.end());

    }

    return allData;

}

vector<string> DataBaseConnect::getAllTestQuestionsForModification(string emailUser) {
    vector <string> allData;
    vector <string> allTestQuest;
    vector <string> allNamesOfThemes;
    vector <string> allSubjectsOfUser = getAllNamesOfSubjects(emailUser);


    for (int counterOfSubjects = 0; counterOfSubjects < allSubjectsOfUser.size(); counterOfSubjects++) {
        vector <string> NamesOfThemes = getAllNamesOfThemes(allSubjectsOfUser[counterOfSubjects]);
        allNamesOfThemes.insert(allNamesOfThemes.end(), NamesOfThemes.begin(), NamesOfThemes.end());
    }

    // ID de preguntas que tienen al menos una modificación pendiente
    for (int counterOfThemes = 0; counterOfThemes < allNamesOfThemes.size(); counterOfThemes++) {
        vector <string> testQuesions = getAllTestQuestionsSpecificThemeWithoutName(allNamesOfThemes[counterOfThemes]);
        allTestQuest.insert(allTestQuest.end(), testQuesions.begin(), testQuesions.end());
    }

    for (int counterQuestions = 0; counterQuestions < allTestQuest.size(); counterQuestions++) {
        vector <string> tempData = getAllTestQuestionsFromID(allTestQuest[counterQuestions]);
        allData.insert(allData.end(), tempData.begin(), tempData.end());

    }

    return allData;

}

vector<string> DataBaseConnect::getAllTestQuestions(string nameOfSubjectParam) {
    vector <string> allQuestionsSpecificSubject;

    string query = select_id_question_specific_subject_test_questions + test_question_table + "where tema_perteneciente in ( " + select_id_tema_reverse +
        themesTableName + " where cod_asign = (" + select_cod_asign + subjects + " where nombre_asign = '" + nameOfSubjectParam + "'))";

    mysql_query(conn, query.data());
    res = mysql_store_result(conn);

    int num_fields = mysql_num_fields(res);
    while ((row = mysql_fetch_row(res))) {
        for (int i = 0; i < num_fields; i++) {
            if (row[i] != NULL) {
                allQuestionsSpecificSubject.push_back(row[i]);
            }
        }
    }


    return allQuestionsSpecificSubject;
}

vector<string> DataBaseConnect::getAllTestQuestionsFromID(string id) {
    vector <string> allTestQuestions;

    string query = select_id_question_specific_subject_test_questions + test_question_table + "where id = " + id;

    mysql_query(conn, query.data());
    res = mysql_store_result(conn);

    int num_fields = mysql_num_fields(res);
    while ((row = mysql_fetch_row(res))) {
        for (int i = 0; i < num_fields; i++) {
            if (row[i] != NULL) {
                allTestQuestions.push_back(row[i]);
            }
        }
    }


    return allTestQuestions;
}

vector<string> DataBaseConnect::getAllNormalQuestionsSpecificTheme(string nameOfTheme) {

    vector <string> allNormalQuestionsSpecificTheme;

    string namesQuery = select_id_question_specific_subject_normal_questions + normal_question_table + "where tema_perteneciente = ( " + select_id_tema_reverse + themesTableName + " where nombre = '" + nameOfTheme + "')";

    mysql_query(conn, namesQuery.data());
    res = mysql_store_result(conn);

    int num_fields = mysql_num_fields(res);
    while ((row = mysql_fetch_row(res))) {
        for (int i = 0; i < num_fields; i++) {
            if (row[i] != NULL) {
                allNormalQuestionsSpecificTheme.push_back(row[i]);
            }
        }
    }

    return allNormalQuestionsSpecificTheme;
}

vector<string> DataBaseConnect::getAllNormalQuestionsSpecificThemeWithoutName(string nameOfTheme) {

    vector <string> allNormalQuestionsSpecificTheme;

    string namesQuery = select_id_specific_subject_normal_questions + normal_question_table + "where tema_perteneciente = ( " + select_id_tema_reverse + themesTableName + " where nombre = '" + nameOfTheme + "')";

    mysql_query(conn, namesQuery.data());
    res = mysql_store_result(conn);

    int num_fields = mysql_num_fields(res);
    while ((row = mysql_fetch_row(res))) {
        for (int i = 0; i < num_fields; i++) {
            if (row[i] != NULL) {
                allNormalQuestionsSpecificTheme.push_back(row[i]);
            }
        }
    }

    return allNormalQuestionsSpecificTheme;
}

vector<string> DataBaseConnect::getAllTestQuestionsSpecificThemeWithoutName(string nameOfTheme) {
    vector <string> allNormalQuestionsSpecificTheme;

    string namesQuery = "select DISTINCT preguntas_tipo_test.id from " + test_question_table + " left join " + test_question_modification_table + " on preguntas_tipo_test.id = modificaciones_pendientes_test.id_reference where preguntas_tipo_test.tema_perteneciente = (select id_tema from temas where nombre = '" + nameOfTheme + "') AND preguntas_tipo_test.id = modificaciones_pendientes_test.id_reference";

    mysql_query(conn, namesQuery.data());
    res = mysql_store_result(conn);

    int num_fields = mysql_num_fields(res);
    while ((row = mysql_fetch_row(res))) {
        for (int i = 0; i < num_fields; i++) {
            if (row[i] != NULL) {
                allNormalQuestionsSpecificTheme.push_back(row[i]);
            }
        }
    }

    return allNormalQuestionsSpecificTheme;
}

vector<string> DataBaseConnect::getAllTestQuestionsSpecificTheme(string nameOfTheme) {
    vector <string> allTestQuestionsSpecificTheme;

    string namesQuery = select_id_question_specific_subject_test_questions + test_question_table + "where tema_perteneciente = ( " + select_id_tema_reverse + themesTableName + " where nombre = '" + nameOfTheme + "')";

    mysql_query(conn, namesQuery.data());
    res = mysql_store_result(conn);

    int num_fields = mysql_num_fields(res);
    while ((row = mysql_fetch_row(res))) {
        for (int i = 0; i < num_fields; i++) {
            if (row[i] != NULL) {
                allTestQuestionsSpecificTheme.push_back(row[i]);
            }
        }
    }

    return allTestQuestionsSpecificTheme;
}

vector<string> DataBaseConnect::getAllNormalModels(string nameOfSubject) {
	
    vector <string> allDataNormalModels;

    string query = selectAllNormalDataModels + normalModelsTableName + "where tema_perteneciente = (select id_tema from temas where nombre = BINARY '" + nameOfSubject + "')";

    mysql_query(conn, query.data());
    res = mysql_store_result(conn);

    int num_fields = mysql_num_fields(res);
    while ((row = mysql_fetch_row(res))) {
        for (int i = 0; i < num_fields; i++) {
            if (row[i] != NULL) {
                allDataNormalModels.push_back(row[i]);
            }
        }
    }
    return allDataNormalModels;
}

vector<string> DataBaseConnect::getAllTestModels(string nameOfSubject) {
    vector <string> allDataTestModels;

    string query = selectAllNormalDataModels + testModelsTableName + "where tema_perteneciente = (select id_tema from temas where nombre = BINARY '" + nameOfSubject + "')";

    mysql_query(conn, query.data());
    res = mysql_store_result(conn);

    int num_fields = mysql_num_fields(res);
    while ((row = mysql_fetch_row(res))) {
        for (int i = 0; i < num_fields; i++) {
            if (row[i] != NULL) {
                allDataTestModels.push_back(row[i]);
            }
        }
    }
    return allDataTestModels;
}

vector<string> DataBaseConnect::getAllQuestionsOfSpecificNormalModel(string idModel) {
    vector <string> allDataNormalModelsQuestions;

    string query = select_id_question_specific_normal_model + normal_question_table + "where JSON_CONTAINS(modelo_perteneciente, '" + '"' + idModel + '"' + "')";

    mysql_query(conn, query.data());
    res = mysql_store_result(conn);

    int num_fields = mysql_num_fields(res);
    while ((row = mysql_fetch_row(res))) {
        for (int i = 0; i < num_fields; i++) {
            if (row[i] != NULL) {
                allDataNormalModelsQuestions.push_back(row[i]);
            }
        }
    }
    return allDataNormalModelsQuestions;
}

vector<string> DataBaseConnect::getAllQuestionsOfSpecificTestModel(string idModel) {
    vector <string> allDataTestModelsQuestions;

    string query = select_id_question_specific_subject_test_questions + test_question_table + "where JSON_CONTAINS(modelo_perteneciente, '" + '"' + idModel + '"' + "')";

    mysql_query(conn, query.data());
    res = mysql_store_result(conn);

    int num_fields = mysql_num_fields(res);
    while ((row = mysql_fetch_row(res))) {
        for (int i = 0; i < num_fields; i++) {
            if (row[i] != NULL) {
                allDataTestModelsQuestions.push_back(row[i]);
            }
        }
    }
    return allDataTestModelsQuestions;
}

vector<string> DataBaseConnect::getAllNormalModificationsQuestions(string idReference) {
    vector <string> allDataNormalModifications;

    string query = select_id_id_reference_question_normal + normal_question_modification_table + "left join preguntas_tipo_normal on modificaciones_pendientes_normal.id_reference = preguntas_tipo_normal.id where preguntas_tipo_normal.id = " + idReference;

    mysql_query(conn, query.data());
    res = mysql_store_result(conn);

    int num_fields = mysql_num_fields(res);
    while ((row = mysql_fetch_row(res))) {
        for (int i = 0; i < num_fields; i++) {
            if (row[i] != NULL) {
                allDataNormalModifications.push_back(row[i]);
            }
        }
    }
    return allDataNormalModifications;
}

vector<string> DataBaseConnect::getAllTestModForIdReference(string idReference) {
    vector <string> allDataTestModifications;

    string query = select_id_id_reference_question_test + test_question_modification_table + " where id_reference = " + idReference;

    mysql_query(conn, query.data());
    res = mysql_store_result(conn);

    int num_fields = mysql_num_fields(res);
    while ((row = mysql_fetch_row(res))) {
        for (int i = 0; i < num_fields; i++) {
            if (row[i] != NULL) {
                allDataTestModifications.push_back(row[i]);
            }
        }
    }
    return allDataTestModifications;
}
