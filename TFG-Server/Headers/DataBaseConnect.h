#include "../Headers/Main.h"
#include<iostream>
#include<mysql/mysql.h>
#include <vector>
#include<stdio.h>

using namespace std;

class DataBaseConnect {

private:

	MYSQL* conn; /* pointer to connection handler */
	MYSQL_RES* res; /* holds the result set */
	MYSQL_ROW row;

	char* opt_host_name = "localhost"; /* HOST */
	char* opt_user_name = "root"; /* USERNAME */
	char* opt_password = "Luna1000"; /* PASSWORD */
	unsigned int opt_port_num = 3306; /* PORT */
	char* opt_socket_name = NULL; /* SOCKET NAME, DO NOT CHANGE */
	char* opt_db_name = "tfg_server"; /* DATABASE NAME */
	unsigned int opt_flags = 0; /* CONNECTION FLAGS, DO NOT CHANGE */

	// All tables here
	const string teachersTableName = " profesorado ";
	const string teachersTableNameWithOutSpaces = "profesorado";
	const string subjects = " asignaturas ";
	const string subjectsTableNameWithOutSpaces = "asignaturas";
	const string themesTableName = "temas ";
	const string normal_question_table = "preguntas_tipo_normal ";
	const string normal_question_modification_table = "modificaciones_pendientes_normal ";
	const string test_question_table = "preguntas_tipo_test ";

	// All MySQL sentences
	const string selectAll = "select * from ";
	const string select_id_tema = "select nombre from ";
	const string select_id_preguta_normal = "select id from ";
	const string selectName = "select nombre from ";
	const string selectSubject = "select asignaturas.nombre_asign from ";
	const string selectThemes = "select nombre from ";
	const string select_id_question_specific_subject_normal_questions = "select id, pregunta from ";
	const string select_id_question_specific_subject_test_questions = "select id, pregunta , respuesta_A, respuesta_B, respuesta_C, respuesta_D, respuesta_correcta from ";
	const string select_cod_asign = "select cod_asign from ";
	const string select_id_tema_reverse = "select id_tema from ";
	const string select_max_id_theme = "select max(id_tema) from ";
	const string select_max_id_question = "select max(id) from ";

	const string insertNewThemeSentence = "insert into " + themesTableName + "(id_tema, nombre, cod_asign) values( ";
	const string insertNewQuestionQuery = "insert into " + normal_question_table + "(id, pregunta, tema_perteneciente, modelo_perteneciente) values( ";
	const string insertNewModificationQuestionQuery = "insert into " + normal_question_modification_table + "(id, id_reference, pregunta) values( ";
	const string insertNewTestQuestionQuery = "insert into " + test_question_table + "(id, pregunta, respuesta_A, respuesta_B, respuesta_C, respuesta_D, respuesta_correcta, tema_perteneciente, modelo_perteneciente) values( ";

public:

	DataBaseConnect();

	bool loginQuery(string emailParam, string passwdParam);
	bool checkNameOfThemeExist(string nameOfThemeParam);
	bool checkNameOfQuestionExist(string nameOfQuestionParam);
	bool checkNameOfQuestionTestExist(string nameOfQuestionParam);
	bool insertNewTheme(vector<string> allNewThemeData);
	bool insertNewQuestion(vector <string> allNewQuestionData);
	bool insertNewTestQuestion(vector <string> allNewQuestionData);
	bool insertNewNormalModification(vector <string> allDataNewModification);
	string nameQuery(string emailParam);
	vector <string> getAllNamesOfSubjects(string emailParam);
	vector <string> getAllNamesOfThemes(string subjectParam);
	vector <string> getAllNormalQuestions(string nameOfSubjectParam);
	vector <string> getAllTestQuestions(string nameOfSubjectParam);
};

