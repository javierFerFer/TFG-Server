/**********************************************************************************************//**
 * @file	TFG-Server\Headers\DataBaseConnect.h.
 *
 * @brief	Declaración de la clase conexión, permite la conexión a la base de datos.\n
 * 			Declares the data base connect class.
 **************************************************************************************************/

#include "../Headers/Main.h"
#include<iostream>
#include<mysql/mysql.h>
#include <vector>
#include<stdio.h>

using namespace std;

/**********************************************************************************************//**
 * @class	DataBaseConnect
 *
 * @brief	Clase conexión.\n
 * 			Connection class.
 *
 * @author	Javier Fernández Fernández
 * @date	24/04/2020
 **************************************************************************************************/

class DataBaseConnect {

private:


	/** @brief	The connection */
	MYSQL* conn; /* Puntero de la conexión a la base de datos*/
	MYSQL_RES* res; /* puntero donde se almacena los resultados de las consultas*/
	MYSQL_ROW row;

	char* opt_host_name = "localhost"; /* host */
	char* opt_user_name = "root"; /* usuario */
	char* opt_password = "Luna1000"; /* Contraseña */
	unsigned int opt_port_num = 3306; /* puerto */
	char* opt_socket_name = NULL; /* Nombre del socket, no cambiar */
	char* opt_db_name = "tfg_server"; /* Nombre de la base de datos */
	unsigned int opt_flags = 0; /* Flags de la base de datos, no tocar */

	// Todas las tablas aquí
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

	// Todas las sentencias MYSQL aquí
	const string selectAll = "select * from ";
	const string selectCoutID = "select count(id) from ";
	const string selectJsonLength = "select JSON_LENGTH (";
	const string selectJsonExtract = "select JSON_EXTRACT (";
	const string select_id_tema = "select nombre from ";
	const string select_name_normal_model = "select nombre_modelo from ";
	const string select_id_preguta_normal = "select id from ";
	const string selectName = "select nombre from ";
	const string selectSubject = "select asignaturas.nombre_asign from ";
	const string selectThemes = "select nombre from ";
	const string selectAdminDepart = "select admin_depart from ";
	const string select_id_question_specific_subject_normal_questions = "select id, pregunta from ";
	const string select_id_specific_subject_normal_questions = "select id from ";
	const string select_id_question_specific_subject_test_questions = "select id, pregunta , respuesta_A, respuesta_B, respuesta_C, respuesta_D, respuesta_correcta from ";
	const string select_id_id_reference_question_normal = "select modificaciones_pendientes_normal.id, preguntas_tipo_normal.pregunta, modificaciones_pendientes_normal.pregunta from ";
	const string select_id_id_reference_question_test = "select id, id_reference, pregunta , respuesta_A, respuesta_B, respuesta_C, respuesta_D, respuesta_correcta from ";
	const string selectAllNormalDataModels = "select id, nombre_modelo, descripcion, autor_email from ";
	const string select_id_question_specific_normal_model = "select id, pregunta from ";
	const string select_cod_asign = "select cod_asign from ";
	const string select_id_tema_reverse = "select id_tema from ";
	const string select_id_reference = "select id_reference from ";
	const string select_max_id_theme = "select max(id_tema) from ";
	const string select_max_id_question = "select max(id) from ";

	// Todos los insert aquí
	const string insertNewThemeSentence = "insert into " + themesTableName + "(id_tema, nombre, cod_asign) values( ";
	const string insertNewQuestionQuery = "insert into " + normal_question_table + "(id, pregunta, tema_perteneciente, modelo_perteneciente) values( ";
	const string insertNewModificationQuestionQuery = "insert into " + normal_question_modification_table + "(id, id_reference, pregunta) values( ";
	const string insertNewModificationQuestionQueryTest = "insert into " + test_question_modification_table + "(id, id_reference, pregunta, respuesta_A, respuesta_B, respuesta_C, respuesta_D, respuesta_correcta) values( ";
	const string insertNewNormalModelQuery = "insert into " + normalModelsTableName + "(id, nombre_modelo, descripcion, tema_perteneciente, autor_email) values( ";
	const string insertNewTestModelQuery = "insert into " + testModelsTableName + "(id, nombre_modelo, descripcion, tema_perteneciente, autor_email) values( ";
	const string insertNewTestQuestionQuery = "insert into " + test_question_table + "(id, pregunta, respuesta_A, respuesta_B, respuesta_C, respuesta_D, respuesta_correcta, tema_perteneciente, modelo_perteneciente) values( ";


public:

	/**********************************************************************************************//**
	 * @fn	DataBaseConnect::DataBaseConnect();
	 *
	 * @brief	Constructor por defecto de la clase.\n
	 * 			Default constructor.
	 *
	 * @author	Javier Fernández Fernández
	 * @date	24/04/2020
	 **************************************************************************************************/

	DataBaseConnect();

	/**********************************************************************************************//**
	 * @fn	bool DataBaseConnect::loginQuery(string emailParam, string passwdParam);
	 *
	 * @brief	Comprobación del login.\n
	 * 			Check login
	 *
	 * @author	Javier Fernández Fernández
	 * @date	24/04/2020
	 *
	 * @param 	emailParam, string 	Parámetro de correo del usuario.\n
	 * 								Email param.
	 *
	 * @param 	passwdParam, string Parametro de contraseña del usuario.\n
	 * 								Password param.
	 *
	 * @returns	True si los datos de login son correctos, false en caso contrario.\n
	 * 			True if all data of login are correct, false if not.
	 **************************************************************************************************/

	bool loginQuery(string emailParam, string passwdParam);

	/**********************************************************************************************//**
	 * @fn	bool DataBaseConnect::checkNameOfThemeExist(string nameOfThemeParam);
	 *
	 * @brief	Comprueba si el nombre del tema existe.\n
	 * 			Check name of theme exist.
	 *
	 * @author	Javier Fernández Fernández
	 * @date	24/04/2020
	 *
	 * @param 	nameOfThemeParam, string	Nombre del tema a buscar en la base de datos.\n
	 * 										Name of the theme parameter.
	 *
	 * @returns	True si lo encuentra, false en caso contrario.\n
	 * 			True if exist, false if not.
	 **************************************************************************************************/

	bool checkNameOfThemeExist(string nameOfThemeParam);

	/**********************************************************************************************//**
	 * @fn	bool DataBaseConnect::checkNameOfQuestionExist(string nameOfQuestionParam);
	 *
	 * @brief	Comprueba si la pregunta existe.\n
	 * 			Check if question exist.
	 *
	 * @author	Javier Fernández Fernández
	 * @date	24/04/2020
	 *
	 * @param 	nameOfQuestionParam, string	Pregunta a buscar.\n
	 * 										Question to find.
	 *
	 * @returns	True si la encuentra, false en caso contrario.\n
	 * 			True if exist, false if not.
	 **************************************************************************************************/

	bool checkNameOfQuestionExist(string nameOfQuestionParam);

	/**********************************************************************************************//**
	 * @fn	bool DataBaseConnect::checkNameOfNormalQuestionExist(string nameOfQuestionParam);
	 *
	 * @brief	Comprueba si la pregunta existe en la tabla de modificaciones de tipo normal.\n
	 * 			Check if question exists in normal modification table.
	 *
	 * @author	Javier Fernández Fernández
	 * @date	24/04/2020
	 *
	 * @param 	nameOfQuestionParam, string	Pregunta a buscar.\n
	 * 										Question to find.
	 *
	 * @returns	True si la encuentra, false en caso contrario.\n
	 * 			True if exist, false if not.
	 **************************************************************************************************/

	bool checkNameOfNormalQuestionExist(string nameOfQuestionParam);

	/**********************************************************************************************//**
	 * @fn	bool DataBaseConnect::checkNameOfTestQuestionExist(string nameOfQuestionParam);
	 *
	 * @brief	Comprueba que la pregunta exista en la tabla de modificaciones de tipo test.\n
	 * 			Check if question exist in test modification table.
	 *
	 * @author	Javier Fernández Fernández
	 * @date	24/04/2020
	 *
	 * @param 	nameOfQuestionParam, string	Pregunta a buscar.\n
	 * 										Question to find.
	 *
	 * @returns	True si la encuentra, false en caso contrario.\n
	 * 			True if exist, false if not.
	 **************************************************************************************************/

	bool checkNameOfTestQuestionExist(string nameOfQuestionParam);

	/**********************************************************************************************//**
	 * @fn	bool DataBaseConnect::checkNameOfQuestionTestExist(string nameOfQuestionParam);
	 *
	 * @brief	Comprueba si la pregunta existe en la tabla de modificaciones de tipo test en base a su ID.\n
	 * 			Check if question exist in test modification table based on his ID.
	 *
	 * @author	Javier Fernández Fernández
	 * @date	24/04/2020
	 *
	 * @param 	nameOfQuestionParam, string	Pregunta a buscar.\n
	 * 										Question to find.
	 *
	 * @returns	True si la encuentra, false en caso contrario.\n
	 * 			True if exist, false if not.
	 **************************************************************************************************/

	bool checkNameOfQuestionTestExist(string nameOfQuestionParam);

	/**********************************************************************************************//**
	 * @fn	bool DataBaseConnect::insertNewTheme(vector<string> allNewThemeData);
	 *
	 * @brief	Inserta el nuevo tema recibido por parámetro.\n
	 * 			Insert new theme.
	 *
	 * @author	Javier Fernández Fernández
	 * @date	24/04/2020
	 *
	 * @param 	allNewThemeData, vector<string>	Datos del nuevo tema.\n
	 * 											Date of new theme.
	 *
	 * @returns	True si lo inserta correctamente, false si hay algún problema.\n
	 * 			True if insert successfully, false in the opposite case.
	 **************************************************************************************************/

	bool insertNewTheme(vector<string> allNewThemeData);

	/**********************************************************************************************//**
	 * @fn	bool DataBaseConnect::insertNewQuestion(vector <string> allNewQuestionData);
	 *
	 * @brief	Inserta una nueva pregunta de tipo normal./n
	 * 			Insert new normal question.
	 *
	 * @author	Javier Fernández Fernández
	 * @date	24/04/2020
	 *
	 * @param 	allNewQuestionData, vector<string> Datos de la nueva pregunta a insertar.\n
	 * 											   All data of new question to insert.
	 *
	 * @returns	True si la inserta correctamente, false si hay algún problema.\n
	 * 			True if insert successfully, false in the opposite case.
	 **************************************************************************************************/

	bool insertNewQuestion(vector <string> allNewQuestionData);

	/**********************************************************************************************//**
	 * @fn	bool DataBaseConnect::insertNewTestQuestion(vector <string> allNewQuestionData);
	 *
	 * @brief	Inserta una nueva pregunta de tipo test./n
	 * 			Insert new test question.
	 *
	 * @author	Javier Fernández Fernández
	 * @date	24/04/2020
	 *
	 * @param 	allNewQuestionData, vector<string>	Datos de la nueva pregunta a insertar.\n
	 * 												All data of new question to insert.
	 *
	 * @returns	True si la inserta correctamente, false si hay algún problema.\n
	 * 			True if insert successfully, false in the opposite case.
	 **************************************************************************************************/

	bool insertNewTestQuestion(vector <string> allNewQuestionData);

	/**********************************************************************************************//**
	 * @fn	bool DataBaseConnect::insertNewNormalModification(vector <string> allDataNewModification);
	 *
	 * @brief	Inserta una nueva modificación de tipo normal.\n
	 * 			Insert new normal modification.
	 *
	 * @author	Javier Fernández Fernández
	 * @date	24/04/2020
	 *
	 * @param 	allDataNewModification, vector<string>	Todos los datos de la nueva modificación.\n
	 * 													All data of new modification.
	 *
	 * @returns	True si la inserta correctamente, false si hay algún problema.\n
	 * 			True if insert successfully, false in the opposite case.
	 * 			
	 **************************************************************************************************/

	bool insertNewNormalModification(vector <string> allDataNewModification);

	/**********************************************************************************************//**
	 * @fn	bool DataBaseConnect::insertNewTestModification(vector <string> allDataNewModification);
	 *
	 * @brief	Inserta una nueva modificación de tipo test.\n
	 * 			Insert new test modification.
	 *
	 * @author	Javier Fernández Fernández
	 * @date	24/04/2020
	 *
	 * @param 	allDataNewModification, vector<string>	Todos los datos de la nueva modificación.\n
	 * 													All data of new Modification.
	 *
	 * @returns	True si la inserta correctamente, false si hay algún problema.\n
	 * 			True if insert successfully, false in the opposite case.
	 **************************************************************************************************/

	bool insertNewTestModification(vector <string> allDataNewModification);

	/**********************************************************************************************//**
	 * @fn	bool DataBaseConnect::updateNormalQuestionsNewModel(vector <string> allQuestionsNewNormalModel);
	 *
	 * @brief	Actualiza el modelo al que pertenecen las preguntas de tipo normal pasadas por parámetro.\n
	 * 			Update the model of normal questions passed by parameter.
	 *
	 * @author	Javier Fernández Fernández
	 * @date	24/04/2020
	 *
	 * @param 	allQuestionsNewNormalModel, vector<string>	Todas las preguntas a actualizar.\n
	 * 														All questions to update.
	 *
	 * @returns	True si las actualiza correctamente, false si hay algún problema.\n
	 * 			True if update successfully, false in the opposite case.
	 **************************************************************************************************/

	bool updateNormalQuestionsNewModel(vector <string> allQuestionsNewNormalModel);

	/**********************************************************************************************//**
	 * @fn	bool DataBaseConnect::updateTestQuestionsNewModel(vector <string> allQuestionsNewTestModel);
	 *
	 * @brief	Actualiza el modelo al que pertenecen las preguntas de tipo test pasadas por parámetro.\n
	 * 			Update the model of test questions passed by parameter.
	 *
	 * @author	Javier Fernández Fernández
	 * @date	24/04/2020
	 *
	 * @param 	allQuestionsNewTestModel, vector<string>	Todas las preguntas a modificar.\n
	 * 														All questions to update.
	 *
	 * @returns	True si las actualiza correctamente, false si hay algún problema.\n
	 * 			True if update successfully, false in the opposite case.
	 **************************************************************************************************/

	bool updateTestQuestionsNewModel(vector <string> allQuestionsNewTestModel);

	/**********************************************************************************************//**
	 * @fn	void DataBaseConnect::activeNormalModification(string idNormalModification);
	 *
	 * @brief	Actualiza la pregunta de tipo normal con los datos de la modificación.\n
	 * 			Update normal question with all data of normal modification.
	 *
	 * @author	Javier Fernández Fernández
	 * @date	24/04/2020
	 *
	 * @param 	idNormalModification, string	ID de la modificación.
	 * 											ID of modification.
	 **************************************************************************************************/

	void activeNormalModification(string idNormalModification);

	/**********************************************************************************************//**
	 * @fn	void DataBaseConnect::activeTestModification(string idTestModification);
	 *
	 * @brief	Actualiza la pregunta de tipo test con los datos de la modificación.\n
	 * 			Update test question with all data of test modification.
	 *
	 * @author	Javier Fernández Fernández
	 * @date	24/04/2020
	 *
	 * @param 	idTestModification, string	ID de la modificación.\n
	 * 										ID of modification.
	 **************************************************************************************************/

	void activeTestModification(string idTestModification);

	/**********************************************************************************************//**
	 * @fn	void DataBaseConnect::deleteNormalModification(string idNormalModification);
	 *
	 * @brief	Borra la modificación de tipo normal.\n
	 * 			Delete normal modification.
	 *
	 * @author	Javier Fernández Fernández
	 * @date	24/04/2020
	 *
	 * @param 	idNormalModification, string	ID de la modificación.\n
	 * 											ID of modification.
	 **************************************************************************************************/

	void deleteNormalModification(string idNormalModification);

	/**********************************************************************************************//**
	 * @fn	void DataBaseConnect::deleteTestModification(string idTestModification);
	 *
	 * @brief	Borra la modificación de tipo test.\n
	 * 			Delete test modification.
	 *
	 * @author	Javier Fernández Fernández
	 * @date	24/04/2020
	 *
	 * @param 	idTestModification, string	ID de la modificación.\n
	 * 										ID of modification.
	 **************************************************************************************************/

	void deleteTestModification(string idTestModification);

	/**********************************************************************************************//**
	 * @fn	void DataBaseConnect::deleteNormalQuestion(string idNormalModification);
	 *
	 * @brief	Borra la pregunta de tipo normal.\n
	 * 			Delete normal question.
	 *
	 * @author	Javier Fernández Fernández
	 * @date	24/04/2020
	 *
	 * @param 	idNormalModification, string	ID de la pregunta.\n
	 * 											ID of question.
	 **************************************************************************************************/

	void deleteNormalQuestion(string idNormalModification);

	/**********************************************************************************************//**
	 * @fn	void DataBaseConnect::deleteTestQuestion(string idTestModification);
	 *
	 * @brief	Borra la pregunta de tipo test.\n
	 * 			Delete test question.
	 *
	 * @author	Javier Fernández Fernández
	 * @date	24/04/2020
	 *
	 * @param 	idTestModification, string	ID de la pregunta.\n
	 * 										ID of question.
	 **************************************************************************************************/

	void deleteTestQuestion(string idTestModification);

	/**********************************************************************************************//**
	 * @fn	string DataBaseConnect::insertNewNormalModel(vector <string> allDataNormalModel);
	 *
	 * @brief	Inserta un modelo de tipo normal.\n
	 * 			Insert new normal model.
	 *
	 * @author	Javier Fernández Fernández
	 * @date	24/04/2020
	 *
	 * @param 	allDataNormalModel, vector<string>	Todos los datos del modelo normal.\n
	 * 												All data of normal model.
	 *
	 * @returns	string, Si todo va bien, retorna el ID del modelo insertado.\n
	 * 					Si algo falla, retorna 0.
	 **************************************************************************************************/

	string insertNewNormalModel(vector <string> allDataNormalModel);

	/**********************************************************************************************//**
	 * @fn	string DataBaseConnect::insertNewTestModel(vector <string> allDataTestModel);
	 *
	 * @brief	Inserta un modelo de tipo test.\n
	 * 			Insert new test model.
	 *
	 * @author	Javier Fernández Fernández
	 * @date	24/04/2020
	 *
	 * @param 	allDataTestModel, vector<string>	Todos los datos del modelo test.\n
	 * 												All data of test model.
	 *
	 * @returns string, Si todo va bien, retorna el ID del modelo insertado.\n
	 * 					Si algo falla, retorna 0.
	 **************************************************************************************************/

	string insertNewTestModel(vector <string> allDataTestModel);

	/**********************************************************************************************//**
	 * @fn	string DataBaseConnect::nameQuery(string emailParam);
	 *
	 * @brief	Busca el nombre asociado al correo recibido por parámetro.\n
	 * 			Search the name asociated to email param.
	 *
	 * @author	Javier Fernández Fernández
	 * @date	24/04/2020
	 *
	 * @param 	emailParam, string	Email del usuario a buscar.
	 * 								User email to use.
	 *
	 * @returns	string, Si lo encuentra, retorna el correo del usuario.\n
	 * 					Si no lo encuentra, retorna un string vacio --> ""
	 **************************************************************************************************/

	string nameQuery(string emailParam);

	/**********************************************************************************************//**
	 * @fn	string DataBaseConnect::select(string value);
	 *
	 * @brief	Permite hacer una consulta a la tabla dual de la base de datos.\n
	 * 			It used to do queries in dual table of database.
	 *
	 * @author	Javier Fernández Fernández
	 * @date	24/04/2020
	 *
	 * @param 	value, string	Valor para la consulta.\n
	 * 							Value for to query.
	 *
	 * @returns string, Valor recibido de la consulta.
	 * 					Value of query.
	 **************************************************************************************************/

	string select(string value);

	/**********************************************************************************************//**
	 * @fn	string DataBaseConnect::checkUserPermissionsToChange(string emailParam);
	 *
	 * @brief	Comprueba si el usuario tiene permisos para trabajar con las peticiciones de borrado/modificación.\n
	 * 			Check if user has permissions to work with modify/delete request.
	 *
	 * @author	Javier Fernández Fernández
	 * @date	24/04/2020
	 *
	 * @param 	emailParam, string	Email del usuario a buscar.\n
	 * 								Email of user to find.
	 *
	 * @returns	string, Si lo encuentra, retorna el correo, si no lo encuentra, retorna un string vacio --> ""
	 * 					If find, return email of user, if not, return empty string like this --> ""
	 **************************************************************************************************/

	string checkUserPermissionsToChange(string emailParam);

	/**********************************************************************************************//**
	 * @fn	vector <string> DataBaseConnect::getAllNamesOfSubjects(string emailParam);
	 *
	 * @brief	Obtiene todos los nombres de las asignaturas asociadas al profesor.\n
	 * 			Get all names of subjects associated of teacher.
	 *
	 * @author	Javier Fernández Fernández
	 * @date	24/04/2020
	 *
	 * @param 	emailParam, string	Email del profesor.
	 * 								Email of theacher.
	 *
	 * @returns	vector<string>, Todos los nombres de las asignaturas.
	 * 							All names of subjects.
	 **************************************************************************************************/

	vector <string> getAllNamesOfSubjects(string emailParam);

	/**********************************************************************************************//**
	 * @fn	vector <string> DataBaseConnect::getAllNamesOfThemes(string subjectParam);
	 *
	 * @brief	Obtiene todos los nombres de los temas asociados a la asignatura.\n
	 * 			Get all names of themes associated of subject.
	 *
	 * @author	Javier Fernández Fernández
	 * @date	24/04/2020
	 *
	 * @param 	subjectParam, string	Tema a buscar.\n
	 * 									Theme to find.
	 *
	 * @returns	vector<string>, Todos los nombres de los temas.
	 * 							All names of themes.
	 **************************************************************************************************/

	vector <string> getAllNamesOfThemes(string subjectParam);

	/**********************************************************************************************//**
	 * @fn	vector <string> DataBaseConnect::getAllNormalQuestions(string nameOfSubjectParam);
	 *
	 * @brief	Obtiene todas las preguntas de tipo normal asociadas al tema.\n
	 * 			Get all normal questions associated the theme.
	 *
	 * @author	Javier Fernández Fernández
	 * @date	24/04/2020
	 *
	 * @param 	nameOfSubjectParam,string	Nombre del tema.\n
	 * 										Name of theme.
	 *
	 * @returns	vector<string>, Todos los datos de las preguntas.\n
	 * 							All data of questions.
	 **************************************************************************************************/

	vector <string> getAllNormalQuestions(string nameOfSubjectParam);

	/**********************************************************************************************//**
	 * @fn	vector <string> DataBaseConnect::getAllNormalQuestionsForModification(string emailUser);
	 *
	 * @brief	Obtiene todas las modificaciones de tipo normal de todas los temas de las asignaturas asociadas al correo del profesor.\n
	 * 			Get all normal modifications of all themes of all subjects associated to email teacher.
	 *
	 * @author	Javier Fernández Fernández
	 * @date	24/04/2020
	 *
	 * @param 	emailUser, string	Correo del profesor.\n
	 * 								Teacher email.
	 *
	 * @returns	vector<string>, Todos los datos de las modificaciones.\n
	 * 							All data of modifications.
	 **************************************************************************************************/

	vector <string> getAllNormalQuestionsForModification(string emailUser);

	/**********************************************************************************************//**
	 * @fn	vector <string> DataBaseConnect::getAllTestQuestionsForModification(string emailUser);
	 *
	 * @brief	Obtiene todas las modificaciones de tipo test de todas los temas de las asignaturas asociadas al correo del profesor.\n
	 * 			Get all test modifications of all themes of all subjects associated to email teacher.
	 *
	 * @author	Javier Fernández Fernández
	 * @date	24/04/2020
	 *
	 * @param 	emailUser, string	Correo del profesor.\n
	 * 								Teacher email.
	 *
	 * @returns	vector<string>, Todos los datos de las modificaciones.\n
	 * 							All data of modifications.
	 **************************************************************************************************/

	vector <string> getAllTestQuestionsForModification(string emailUser);

	/**********************************************************************************************//**
	 * @fn	vector <string> DataBaseConnect::getAllTestQuestions(string nameOfSubjectParam);
	 *
	 * @brief	Obtiene todas las preguntas de tipo test asociadas a un tema.\n
	 * 			Get all test questions associated to theme.
	 *
	 * @author	Javier Fernández Fernández
	 * @date	24/04/2020
	 *
	 * @param 	nameOfSubjectParam, string	Nombre del tema.\n
	 * 										Name of theme.
	 *
	 * @returns	vector<string>, Todos los datos de las preguntas.\n
	 * 							All data of questions.
	 **************************************************************************************************/

	vector <string> getAllTestQuestions(string nameOfSubjectParam);

	/**********************************************************************************************//**
	 * @fn	vector <string> DataBaseConnect::getAllTestQuestionsFromID(string id);
	 *
	 * @brief	Obtiene todos los datos de la pregunta tipo test con el ID de parámetro.\n
	 * 			Get all data of test question with ID by parameter.
	 *
	 * @author	Javier Fernández Fernández
	 * @date	24/04/2020
	 *
	 * @param 	id, string	ID de la pregunta.
	 * 						ID of question.
	 *
	 * @returns	vector<string>, Todos los datos de la pregunta encontrada.
	 **************************************************************************************************/

	vector <string> getAllTestQuestionsFromID(string id);

	/**********************************************************************************************//**
	 * @fn	vector <string> DataBaseConnect::getAllNormalQuestionsSpecificTheme(string nameOfTheme);
	 *
	 * @brief	Obtiene todas las preguntas de tipo normal asociadas a un tema.\n
	 * 			Get all normal questions associated a one theme.
	 *
	 * @author	Javier Fernández Fernández
	 * @date	24/04/2020
	 *
	 * @param 	nameOfTheme, string	Nombre del tema.\n
	 * 								Name of theme.
	 *
	 * @returns	vector<string>, Todos los datos de las preguntas.\n
	 * 							All data of questions.
	 **************************************************************************************************/

	vector <string> getAllNormalQuestionsSpecificTheme(string nameOfTheme);

	/**********************************************************************************************//**
	 * @fn	vector <string> DataBaseConnect::getAllNormalQuestionsSpecificThemeWithoutName(string nameOfTheme);
	 *
	 * @brief	Obtiene todoss los ID's  de las preguntas de tipo normal asociadas a un tema.\n
	 * 			Get all normal questions ID associated a one theme.
	 *
	 * @author	Javier Fernández Fernández
	 * @date	24/04/2020
	 *
	 * @param 	nameOfTheme, string	Nombre del tema.\n
	 * 								Name of theme.
	 *
	 * @returns	vector<string>, Todos los ID's de las preguntas.\n
	 * 							All ID's of questions.
	 **************************************************************************************************/

	vector <string> getAllNormalQuestionsSpecificThemeWithoutName(string nameOfTheme);

	/**********************************************************************************************//**
	 * @fn	vector <string> DataBaseConnect::getAllTestQuestionsSpecificThemeWithoutName(string nameOfTheme);
	 *
	 * @brief	Obtiene todoss los ID's  de las preguntas de tipo test asociadas a un tema.\n
	 * 			Get all test questions ID associated a one theme.
	 *
	 * @author	Javier Fernández Fernández
	 * @date	24/04/2020
	 *
	 * @param 	nameOfTheme, string	Nombre del tema.\n
	 * 								Name of theme.
	 *
	 * @returns	vector<string>, Todos los ID's de las preguntas.\n
	 * 							All ID's of questions.
	 **************************************************************************************************/

	vector <string> getAllTestQuestionsSpecificThemeWithoutName(string nameOfTheme);

	/**********************************************************************************************//**
	 * @fn	vector <string> DataBaseConnect::getAllTestQuestionsSpecificTheme(string nameOfTheme);
	 *
	 * @brief	Obtiene todas las preguntas de tipo test asociadas a un tema.\n
	 * 			Get all test questions associated a one theme.
	 *
	 * @author	Javier Fernández Fernández
	 * @date	24/04/2020
	 *
	 * @param 	nameOfTheme, string	Nombre del tema.\n
	 * 								Name of theme.
	 *
	 * @returns	vector<string>, Todos los datos de las preguntas.\n
	 * 							All data of questions.
	 **************************************************************************************************/

	vector <string> getAllTestQuestionsSpecificTheme(string nameOfTheme);

	/**********************************************************************************************//**
	 * @fn	vector <string> DataBaseConnect::getAllNormalModels(string nameOfSubject);
	 *
	 * @brief	Obtiene todos los datos de los modelos normales del tema especificado.\n
	 * 			Get all data of normal models associated to theme by parameter.
	 *
	 * @author	Javier Fernández Fernández
	 * @date	24/04/2020
	 *
	 * @param 	nameOfSubject, string	Nombre del tema.\n
	 * 									Name of theme.
	 *
	 * @returns	vector<string>, Todos los datos de los modelos encontrados.\n
	 * 							All data of normla models found.
	 **************************************************************************************************/

	vector <string> getAllNormalModels(string nameOfSubject);

	/**********************************************************************************************//**
	 * @fn	vector <string> DataBaseConnect::getAllTestModels(string nameOfSubject);
	*
	 * @brief	Obtiene todos los datos de los modelos de tipo test del tema especificado.\n
	 * 			Get all data of test models associated to theme by parameter.
	 *
	 * @author	Javier Fernández Fernández
	 * @date	24/04/2020
	 *
	 * @param 	nameOfSubject, string	Nombre del tema.\n
	 * 									Name of theme.
	 *
	 * @returns	vector<string>, Todos los datos de los modelos encontrados.\n
	 * 							All data of normla models found.
	 **************************************************************************************************/

	vector <string> getAllTestModels(string nameOfSubject);

	/**********************************************************************************************//**
	 * @fn	vector <string> DataBaseConnect::getAllQuestionsOfSpecificNormalModel(string idModel);
	 *
	 * @brief	Obtiene todas las preguntas de tipo normal asociadas a un modelo.\n
	 * 			Get all normal questions associated a one model.
	 *
	 * @author	Javier Fernández Fernández
	 * @date	24/04/2020
	 *
	 * @param 	idModel, string	ID del modelo.\n
	 * 							ID of model.
	 *
	 * @returns	vector<string>, Todas las preguntas asociadas al modelo.
	 * 							All questions of model.
	 **************************************************************************************************/

	vector <string> getAllQuestionsOfSpecificNormalModel(string idModel);

	/**********************************************************************************************//**
	 * @fn	vector <string> DataBaseConnect::getAllQuestionsOfSpecificTestModel(string idModel);
	 *
	 * @brief	Obtiene todas las preguntas de tipo test asociadas a un modelo.\n
	 * 			Get all test questions associated a one model.
	 *
	 * @author	Javier Fernández Fernández
	 * @date	24/04/2020
	 *
	 * @param 	idModel, string	ID del modelo.\n
	 * 							ID of model.
	 *
	 * @returns	vector<string>, Todas las preguntas asociadas al modelo.
	 * 							All questions of model.
	 **************************************************************************************************/

	vector <string> getAllQuestionsOfSpecificTestModel(string idModel);

	/**********************************************************************************************//**
	 * @fn	vector <string> DataBaseConnect::getAllNormalModificationsQuestions(string idReference);
	 *
	 * @brief	Obtiene todas las preguntas de tipo normal con su correspondiente modificación.\n
	 * 			Get all normal questions with specific modifications.
	 *
	 * @author	Javier Fernández Fernández
	 * @date	24/04/2020
	 *
	 * @param 	idReference,string	ID de referencia de la modificación.\n
	 * 								ID of reference of modification.
	 *
	 * @returns	vector<string>, Todos los datos de la preguntas y sus modificaciones.\n
	 * 							All data of questions and their modifications.
	 **************************************************************************************************/

	vector <string> getAllNormalModificationsQuestions(string idReference);

	/**********************************************************************************************//**
	 * @fn	vector <string> DataBaseConnect::getAllTestModForIdReference(string idReference);
	 *
	 * @brief	Obtiene todas las preguntas de tipo test con su correspondiente modificación.\n
	 * 			Get all test questions with specific modifications.
	 *
	 * @author	Javier Fernández Fernández
	 * @date	24/04/2020
	 *
	 * @param 	idReference,string	ID de referencia de la modificación.\n
	 * 								ID of reference of modification.
	 *
	 * @returns	vector<string>, Todos los datos de la preguntas y sus modificaciones.\n
	 * 							All data of questions and their modifications.
	 **************************************************************************************************/

	vector <string> getAllTestModForIdReference(string idReference);
};

