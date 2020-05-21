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
	const string normalModelsTableName = "modelo_normal ";
	const string testModelsTableName = "modelo_test ";
	const string normal_question_table = "preguntas_tipo_normal ";
	const string normal_question_modification_table = "modificaciones_pendientes_normal ";
	const string test_question_modification_table = "modificaciones_pendientes_test ";
	const string test_question_table = "preguntas_tipo_test ";

	// All MySQL sentences
	const string selectAll = "select * from ";
	const string select_id_tema = "select nombre from ";
	const string select_name_normal_model = "select nombre_modelo from ";
	const string select_id_preguta_normal = "select id from ";
	const string selectName = "select nombre from ";
	const string selectSubject = "select asignaturas.nombre_asign from ";
	const string selectThemes = "select nombre from ";
	const string selectAdminDepart = "select admin_depart from ";
	const string select_id_question_specific_subject_normal_questions = "select id, pregunta from ";
	const string select_id_question_specific_subject_test_questions = "select id, pregunta , respuesta_A, respuesta_B, respuesta_C, respuesta_D, respuesta_correcta from ";
	const string selectAllNormalDataModels = "select id, nombre_modelo, descripcion, autor_email from ";
	const string select_id_question_specific_normal_model = "select id, pregunta from ";
	const string select_cod_asign = "select cod_asign from ";
	const string select_id_tema_reverse = "select id_tema from ";
	const string select_max_id_theme = "select max(id_tema) from ";
	const string select_max_id_question = "select max(id) from ";

	const string insertNewThemeSentence = "insert into " + themesTableName + "(id_tema, nombre, cod_asign) values( ";
	const string insertNewQuestionQuery = "insert into " + normal_question_table + "(id, pregunta, tema_perteneciente, modelo_perteneciente) values( ";
	const string insertNewModificationQuestionQuery = "insert into " + normal_question_modification_table + "(id, id_reference, pregunta) values( ";
	const string insertNewModificationQuestionQueryTest= "insert into " + test_question_modification_table + "(id, id_reference, pregunta, respuesta_A, respuesta_B, respuesta_C, respuesta_D, respuesta_correcta) values( ";
	const string insertNewNormalModelQuery= "insert into " + normalModelsTableName + "(id, nombre_modelo, descripcion, tema_perteneciente, autor_email) values( ";
	const string insertNewTestModelQuery= "insert into " + testModelsTableName + "(id, nombre_modelo, descripcion, tema_perteneciente, autor_email) values( ";
	const string insertNewTestQuestionQuery = "insert into " + test_question_table + "(id, pregunta, respuesta_A, respuesta_B, respuesta_C, respuesta_D, respuesta_correcta, tema_perteneciente, modelo_perteneciente) values( ";

public:

	DataBaseConnect();

	bool loginQuery(string emailParam, string passwdParam);
	bool checkNameOfThemeExist(string nameOfThemeParam);
	bool checkNameOfQuestionExist(string nameOfQuestionParam);
	bool checkNameOfNormalQuestionExist(string nameOfQuestionParam);
	bool checkNameOfTestQuestionExist(string nameOfQuestionParam);
	bool checkNameOfQuestionTestExist(string nameOfQuestionParam);
	bool insertNewTheme(vector<string> allNewThemeData);
	bool insertNewQuestion(vector <string> allNewQuestionData);
	bool insertNewTestQuestion(vector <string> allNewQuestionData);
	bool insertNewNormalModification(vector <string> allDataNewModification);
	bool insertNewTestModification(vector <string> allDataNewModification);
	bool updateNormalQuestionsNewModel(vector <string> allQuestionsNewNormalModel);
	bool updateTestQuestionsNewModel(vector <string> allQuestionsNewTestModel);

	string insertNewNormalModel(vector <string> allDataNormalModel);
	string insertNewTestModel(vector <string> allDataTestModel);
	string nameQuery(string emailParam);
	string select(string value);
	string checkUserPermissionsToChange(string emailParam);

	vector <string> getAllNamesOfSubjects(string emailParam);
	vector <string> getAllNamesOfThemes(string subjectParam);
	vector <string> getAllNormalQuestions(string nameOfSubjectParam);
	vector <string> getAllTestQuestions(string nameOfSubjectParam);
	vector <string> getAllNormalQuestionsSpecificTheme(string nameOfTheme);
	vector <string> getAllTestQuestionsSpecificTheme(string nameOfTheme);
	vector <string> getAllNormalModels(string nameOfSubject);
	vector <string> getAllTestModels(string nameOfSubject);
	vector <string> getAllQuestionsOfSpecificNormalModel(string idModel);
	vector <string> getAllQuestionsOfSpecificTestModel(string idModel);
};

