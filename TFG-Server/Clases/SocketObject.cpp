#include "../Headers/SocketObject.h"
#include "../Libraries/json.hpp"
#include "../Headers/JsonObjectArray.h"
#include "../Headers/JsonSingleData.h"
#include <csetjmp>

#include <iomanip>

using namespace std;
using namespace PoDoFo;


int no = 0;
string nameOfPDFStr;
string nameOfPDFStrWithAnswers;
string nameOfPDFStrWithOutAnswers;

void SocketObject::launchReadThread() {
	try {
	std::this_thread::sleep_for(std::chrono::milliseconds(3000));
	setlocale(LC_ALL, "");

	printf("Se ha conectado: %s:%d\n", inet_ntoa(clientSocket.sin_addr), clientSocket.sin_port);

	valread = read(sendReceiveDataSocket, buffer, 1024);

	string userData(buffer);

	if (userData.find("GetPasswd") != std::string::npos) {

		// Generación de clave
		passwd = generatePasswd();
		Poco::Crypto::Cipher::ByteVec iv{ ivString.begin(), ivString.end() };

		Poco::Crypto::Cipher::ByteVec key2{ passwd.begin(), passwd.end() };

		Poco::Crypto::CipherFactory& factory = Poco::Crypto::CipherFactory::defaultFactory();
		Poco::Crypto::CipherKey key("aes-256-cbc", key2, iv); // iterationCount = 1

		Poco::Crypto::Cipher* pCipher = factory.createCipher(key);

		// Conversión de valores a objeto antes de ser enviado al cliente
		JsonObjectArray* keyJSonObject = new JsonObjectArray();
		keyJSonObject->A_Title = "key";

		vector <string> allKeyData;
		allKeyData.push_back(passwd);
		allKeyData.push_back(ivString);

		keyJSonObject->B_Content = allKeyData;

		// Conversión de objeto con datos de encriptado a formato JSON
		nlohmann::json jsonKeydata;
		jsonKeydata["A_Title"] = keyJSonObject->A_Title;
		jsonKeydata["B_Content"] = keyJSonObject->B_Content;

		// Conversión del JSon con la clave a string y envio del mismo (0 = formato que acepta c#)
		string keyJsonToString = jsonKeydata.dump(0);

		// Conversión del string formateado a un array de char
		char arrayData[keyJsonToString.size() + 1];
		keyJsonToString.copy(arrayData, keyJsonToString.size() + 1);
		arrayData[keyJsonToString.size()] = '\0';

		// Envio de los datos correspondientes a la encriptación al cliente
		send(sendReceiveDataSocket, arrayData, strlen(arrayData), 0);


		mutex mtx;
		while (checkThreadFunction) {
				// Limpieza del buffer para no recibir datos erroneos
				memset(buffer, 0, sizeof buffer);

				valread = read(sendReceiveDataSocket, buffer, 120024);

				unique_lock<mutex> lock(mtx);
				if (timeOut < maxTimeOut) {
					// reseteo de time out
					timeOut = 0;

				// Lectura de datos enviados por el cliente
				string userData(buffer);

				// Desencriptado de datos
				std::string decrypted;


				decrypted = pCipher->decryptString(userData, Poco::Crypto::Cipher::ENC_BASE64);

				nlohmann::json jsonObjT;
				std::stringstream(decrypted) >> jsonObjT;

				JsonObjectArray* internalJsonObject = new JsonObjectArray();

				// Forma de recorrer todos los elementos del json
				for (auto& element : jsonObjT) {

					string title = element;
					// Limpieza de '"' en el título recibido
					title.erase(remove(title.begin(), title.end(), '"'), title.end());

					if (title.compare("loginCredentials") == 0) {
						// Petición de datos de usuario
						for (auto& data : jsonObjT) {
							if (data.size() != 1) {
								// Recogida del usuario y contraseña
								vector <string> userLoginData(begin(data), end(data));

								bool checkLogin = dataBaseConnection.loginQuery(userLoginData[0], userLoginData[1]);
								if (checkLogin) {
									// Datos de usuario correctos
									sendSigleMessage("loginStatus", "correct", pCipher);
									// Hilo de timeOut, de momento desactivado para testeo
									spawn();
								} else {
									// Datos de usuario introducidos inválidos
									sendSigleMessage("loginStatus", "incorrect", pCipher);
								}
							}
						}
					} else if (title.compare("test") == 0) {
						break;
					} else if (title.compare("client_disconnect") == 0) {
						// Fin de conexión
						checkThreadFunction = false;
						checkTimeOut = false;
						break;

					} else if (title.compare("getNameOfMail") == 0) {
						for (auto& element : jsonObjT) {
							string dataOfMessage = element;
							// Limpieza de '"' en el título recibido
							dataOfMessage.erase(remove(dataOfMessage.begin(), dataOfMessage.end(), '"'), dataOfMessage.end());

							if (dataOfMessage.compare("getNameOfMail") != 0) {
								string nameOfUser = dataBaseConnection.nameQuery(dataOfMessage);
								if (nameOfUser.length() != 0) {
									sendSigleMessage("userNameData", nameOfUser, pCipher);
								}
							}
						}
					} else if (title.compare("getAllSubjects") == 0) {
						for (auto& element : jsonObjT) {
							string getAllSubjects = element;
							// Limpieza de '"' en el título recibido
							getAllSubjects.erase(remove(getAllSubjects.begin(), getAllSubjects.end(), '"'), getAllSubjects.end());

							if (getAllSubjects.compare("getAllSubjects") != 0) {
								vector<string> allSubjects = dataBaseConnection.getAllNamesOfSubjects(getAllSubjects);
								sendMoreSingleDataMessage("allSubjects", allSubjects, pCipher);
							}
						}
					} else if (title.compare("getThemes") == 0) {
						for (auto& element : jsonObjT) {

							string dataOfMessage = element;
							// Limpieza de '"' en el título recibido
							dataOfMessage.erase(remove(dataOfMessage.begin(), dataOfMessage.end(), '"'), dataOfMessage.end());

							if (dataOfMessage.compare("getThemes") != 0) {
								vector <string> allThemes = dataBaseConnection.getAllNamesOfThemes(dataOfMessage);
								sendMoreSingleDataMessage("allThemesNames", allThemes, pCipher);
							}
						}
					} else if (title.compare("getAllThemesFromSignature") == 0) {
						for (auto& element : jsonObjT) {

							string dataOfMessage = element;
							// Limpieza de '"' en el título recibido
							dataOfMessage.erase(remove(dataOfMessage.begin(), dataOfMessage.end(), '"'), dataOfMessage.end());

							if (dataOfMessage.compare("getAllThemesFromSignature") != 0) {
								vector <string> allThemes = dataBaseConnection.getAllNamesOfThemes(dataOfMessage);
								sendMoreSingleDataMessage("allNamesFromSpecificSubject", allThemes, pCipher);
							}
						}
					} else if (title.compare("getAllModelsThemesFromSignature") == 0) {
						for (auto& element : jsonObjT) {

							string dataOfMessage = element;
							// Limpieza de '"' en el título recibido
							dataOfMessage.erase(remove(dataOfMessage.begin(), dataOfMessage.end(), '"'), dataOfMessage.end());

							if (dataOfMessage.compare("getAllModelsThemesFromSignature") != 0) {
								vector <string> allThemes = dataBaseConnection.getAllNamesOfThemes(dataOfMessage);
								sendMoreSingleDataMessage("allNamesModelsFromSpecificSubject", allThemes, pCipher);
							}
						}
					} else if (title.compare("getThemeForTest") == 0) {
						for (auto& element : jsonObjT) {

							string dataOfMessage = element;
							// Limpieza de '"' en el título recibido
							dataOfMessage.erase(remove(dataOfMessage.begin(), dataOfMessage.end(), '"'), dataOfMessage.end());

							if (dataOfMessage.compare("getThemeForTest") != 0) {
								vector <string> allThemes = dataBaseConnection.getAllNamesOfThemes(dataOfMessage);
								sendMoreSingleDataMessage("allThemesForTest", allThemes, pCipher);
							}
						}
					
					}else if (title.compare("findNameOfTheme") == 0) {
						for (auto& element : jsonObjT) {

							string dataOfMessage = element;
							// Limpieza de '"' en el título recibido
							dataOfMessage.erase(remove(dataOfMessage.begin(), dataOfMessage.end(), '"'), dataOfMessage.end());

							if (dataOfMessage.compare("findNameOfTheme") != 0) {
								string checkNameOfThemeExist;
								bool checkName = dataBaseConnection.checkNameOfThemeExist(dataOfMessage);
								if (checkName) {
									checkNameOfThemeExist = "true";
								} else {
									checkNameOfThemeExist = "false";
								}
								sendSigleMessage("checkIfThemeExist", checkNameOfThemeExist, pCipher);
							}
						}
					} else if (title.compare("findQuestion") == 0) {
						for (auto& element : jsonObjT) {

							string dataOfMessage = element;
							// Limpieza de '"' en el título recibido
							dataOfMessage.erase(remove(dataOfMessage.begin(), dataOfMessage.end(), '"'), dataOfMessage.end());

							if (dataOfMessage.compare("findQuestion") != 0) {
								string checkNameOfQuestionExist;
								bool checkName = dataBaseConnection.checkNameOfQuestionExist(dataOfMessage);
								if (checkName) {
									checkNameOfQuestionExist = "true";
								} else {
									checkNameOfQuestionExist = "false";
								}
								sendSigleMessage("checkIfQuestionExist", checkNameOfQuestionExist, pCipher);
							}
						}
					} else if (title.compare("findQuestionTest") == 0) {
						for (auto& element : jsonObjT) {

							string dataOfMessage = element;
							// Limpieza de '"' en el título recibido
							dataOfMessage.erase(remove(dataOfMessage.begin(), dataOfMessage.end(), '"'), dataOfMessage.end());

							if (dataOfMessage.compare("findQuestionTest") != 0) {
								string checkNameOfQuestionExist;
								bool checkName = dataBaseConnection.checkNameOfQuestionTestExist(dataOfMessage);
								if (checkName) {
									checkNameOfQuestionExist = "true";
								} else {
									checkNameOfQuestionExist = "false";
								}
								sendSigleMessage("checkTestQuestion", checkNameOfQuestionExist, pCipher);
							}
						}
					}else if (title.compare("insertNewTheme") == 0) {
						for (auto& data : jsonObjT) {
							if (data.size() != 1) {
								// Recogida del usuario y contraseña
								vector <string> newThemeData(begin(data), end(data));
								bool insertStatus = dataBaseConnection.insertNewTheme(newThemeData);
								if (insertStatus) {
									sendSigleMessage("insertNewThemeStatus", "true", pCipher);
								} else {
									sendSigleMessage("insertNewThemeStatus", "false", pCipher);
								}
							}
						}
					} else if (title.compare("insertTestNewTheme") == 0) {
						for (auto& data : jsonObjT) {
							if (data.size() != 1) {
								// Recogida del usuario y contraseña
								vector <string> newThemeData(begin(data), end(data));
								bool insertStatus = dataBaseConnection.insertNewTheme(newThemeData);
								if (insertStatus) {
									sendSigleMessage("insertNewTestStatus", "true", pCipher);
								} else {
									sendSigleMessage("insertNewTestStatus", "false", pCipher);
								}
							}
						}
					}else if (title.compare("insertNewQuestion") == 0) {
						for (auto& data : jsonObjT) {
							if (data.size() != 1) {
								// Recogida del usuario y contraseña
								vector <string> newQuestionData(begin(data), end(data));
								bool insertStatus = dataBaseConnection.insertNewQuestion(newQuestionData);
								if (insertStatus) {
								// Insercción realizada con exito
									sendSigleMessage("insertNewQuestionStatus", "true", pCipher);
								} else {
								// Mensaje de error, no se pudo insertar el dato correspondiente
									sendSigleMessage("insertNewQuestionStatus", "false", pCipher);
								}
							}
						}
					} else if (title.compare("insertNewTestQuestion") == 0) {
						for (auto& data : jsonObjT) {
							if (data.size() != 1) {
								vector <string> newQuestionData(begin(data), end(data));
								bool insertStatus = dataBaseConnection.insertNewTestQuestion(newQuestionData);
								if (insertStatus) {
									// Insercción realizada con exito
									sendSigleMessage("insertNewTestQuestion", "true", pCipher);
								} else {
									// Mensaje de error, no se pudo insertar el dato correspondiente
									sendSigleMessage("insertNewTestQuestion", "false", pCipher);
								}
							}
						}
					}else if (title.compare("selectedThemeQuestionAdd") == 0) {
						for (auto& element : jsonObjT) {

							string dataOfMessage = element;
							// Limpieza de '"' en el título recibido
							dataOfMessage.erase(remove(dataOfMessage.begin(), dataOfMessage.end(), '"'), dataOfMessage.end());

							if (dataOfMessage.compare("selectedThemeQuestionAdd") != 0) {
								string checkNameOfQuestionExist;
								bool checkName = dataBaseConnection.checkNameOfQuestionExist(dataOfMessage);
								if (checkName) {
									checkNameOfQuestionExist = "true";
								} else {
									checkNameOfQuestionExist = "false";
								}
								sendSigleMessage("checkIfQuestionSelectedTheme", checkNameOfQuestionExist, pCipher);
							}
						}
					} else if (title.compare("selectedTestThemeQuestionAdd") == 0) {
						for (auto& element : jsonObjT) {

							string dataOfMessage = element;
							// Limpieza de '"' en el título recibido
							dataOfMessage.erase(remove(dataOfMessage.begin(), dataOfMessage.end(), '"'), dataOfMessage.end());

							if (dataOfMessage.compare("selectedTestThemeQuestionAdd") != 0) {
								string checkNameOfQuestionExist;
								bool checkName = dataBaseConnection.checkNameOfQuestionTestExist(dataOfMessage);
								if (checkName) {
									checkNameOfQuestionExist = "true";
								} else {
									checkNameOfQuestionExist = "false";
								}
								sendSigleMessage("checkTestIfQuestionSelectedTheme", checkNameOfQuestionExist, pCipher);
							}
						}
					} else if (title.compare("getAllNormalQuestionsSpecificNameOfSUbject") == 0) {
						for (auto& element : jsonObjT) {

							string dataOfMessage = element;
							// Limpieza de '"' en el título recibido
							dataOfMessage.erase(remove(dataOfMessage.begin(), dataOfMessage.end(), '"'), dataOfMessage.end());

							if (dataOfMessage.compare("getAllNormalQuestionsSpecificNameOfSUbject") != 0) {

								vector <string> allQuestions = dataBaseConnection.getAllNormalQuestions(dataOfMessage);

								if (allQuestions.size() == 0) {
									sendSigleMessage("normalQuestionsNotFound", "", pCipher);
								} else {
									sendMoreSingleDataMessage("allNormalQuestionsSpecificSubject", allQuestions, pCipher);
								}
								
							}
						}
					} else if (title.compare("getAllNormalQuestionsForMod") == 0) {
						for (auto& element : jsonObjT) {

							string dataOfMessage = element;
							// Limpieza de '"' en el título recibido
							dataOfMessage.erase(remove(dataOfMessage.begin(), dataOfMessage.end(), '"'), dataOfMessage.end());

							if (dataOfMessage.compare("getAllNormalQuestionsForMod") != 0) {

								vector <string> allQuestionsForModification = dataBaseConnection.getAllNormalQuestionsForModification(dataOfMessage);

								if (allQuestionsForModification.size() == 0) {
									sendSigleMessage("normalForModificationQuestionsNotFound", "", pCipher);
								} else {
									sendMoreSingleDataMessage("allNormalForModificationQuestions", allQuestionsForModification, pCipher);
								}

							}
						}
					} else if (title.compare("getAllTestQuestionsForMod") == 0) {
						for (auto& element : jsonObjT) {

							string dataOfMessage = element;
							// Limpieza de '"' en el título recibido
							dataOfMessage.erase(remove(dataOfMessage.begin(), dataOfMessage.end(), '"'), dataOfMessage.end());

							if (dataOfMessage.compare("getAllTestQuestionsForMod") != 0) {

								vector <string> allQuestionsForModification = dataBaseConnection.getAllTestQuestionsForModification(dataOfMessage);

								if (allQuestionsForModification.size() == 0) {
									sendSigleMessage("TestForModificationQuestionsNotFound", "", pCipher);
								} else {
									sendMoreSingleDataMessage("allTestForModificationQuestions", allQuestionsForModification, pCipher);
								}

							}
						}
					} else if (title.compare("getAllTestModifications") == 0) {
						for (auto& element : jsonObjT) {

							string dataOfMessage = element;
							// Limpieza de '"' en el título recibido
							dataOfMessage.erase(remove(dataOfMessage.begin(), dataOfMessage.end(), '"'), dataOfMessage.end());

							if (dataOfMessage.compare("getAllTestModifications") != 0) {

								vector <string> allTestModifications = dataBaseConnection.getAllTestModForIdReference(dataOfMessage);

								sendMoreSingleDataMessage("allTestModifications", allTestModifications, pCipher);
							}
						}
					} else if (title.compare("updateNormalModification") == 0) {
						for (auto& element : jsonObjT) {

							string dataOfMessage = element;
							// Limpieza de '"' en el título recibido
							dataOfMessage.erase(remove(dataOfMessage.begin(), dataOfMessage.end(), '"'), dataOfMessage.end());

							if (dataOfMessage.compare("updateNormalModification") != 0) {
								dataBaseConnection.activeNormalModification(dataOfMessage);
							}
						}
					} else if (title.compare("updateTestModification") == 0) {
						for (auto& element : jsonObjT) {

							string dataOfMessage = element;
							// Limpieza de '"' en el título recibido
							dataOfMessage.erase(remove(dataOfMessage.begin(), dataOfMessage.end(), '"'), dataOfMessage.end());

							if (dataOfMessage.compare("updateTestModification") != 0) {
								dataBaseConnection.activeTestModification(dataOfMessage);
							}
						}
					} else if (title.compare("deleteNormalModification") == 0) {
						for (auto& element : jsonObjT) {

							string dataOfMessage = element;
							// Limpieza de '"' en el título recibido
							dataOfMessage.erase(remove(dataOfMessage.begin(), dataOfMessage.end(), '"'), dataOfMessage.end());

							if (dataOfMessage.compare("deleteNormalModification") != 0) {
								dataBaseConnection.deleteNormalModification(dataOfMessage);
							}
						}
					} else if (title.compare("deleteTestModification") == 0) {
						for (auto& element : jsonObjT) {

							string dataOfMessage = element;
							// Limpieza de '"' en el título recibido
							dataOfMessage.erase(remove(dataOfMessage.begin(), dataOfMessage.end(), '"'), dataOfMessage.end());

							if (dataOfMessage.compare("deleteTestModification") != 0) {
								dataBaseConnection.deleteTestModification(dataOfMessage);
							}
						}
					} else if (title.compare("deleteNormalQuestion") == 0) {
						for (auto& element : jsonObjT) {

							string dataOfMessage = element;
							// Limpieza de '"' en el título recibido
							dataOfMessage.erase(remove(dataOfMessage.begin(), dataOfMessage.end(), '"'), dataOfMessage.end());

							if (dataOfMessage.compare("deleteNormalQuestion") != 0) {
								dataBaseConnection.deleteNormalQuestion(dataOfMessage);
							}
						}
					} else if (title.compare("deleteTestQuestion") == 0) {
						for (auto& element : jsonObjT) {

							string dataOfMessage = element;
							// Limpieza de '"' en el título recibido
							dataOfMessage.erase(remove(dataOfMessage.begin(), dataOfMessage.end(), '"'), dataOfMessage.end());

							if (dataOfMessage.compare("deleteTestQuestion") != 0) {
								dataBaseConnection.deleteTestQuestion(dataOfMessage);
							}
						}
					
					} else if (title.compare("addNewNormalModification") == 0) {
						for (auto& data : jsonObjT) {
							if (data.size() != 1) {
								vector <string> dataOfNewModification(begin(data), end(data));
								bool insertStatus = dataBaseConnection.insertNewNormalModification(dataOfNewModification);
								if (insertStatus) {
									// Insercción realizada con exito
									sendSigleMessage("insertNewNormalModification", "true", pCipher);
								} else {
									// Mensaje de error, no se pudo insertar el dato correspondiente
									sendSigleMessage("insertNewNormalModification", "false", pCipher);
								}
							}
						}
					} else if (title.compare("getAllTestQuestionsSpecificNameOfSUbject") == 0) {
						for (auto& element : jsonObjT) {

							string dataOfMessage = element;
							// Limpieza de '"' en el título recibido
							dataOfMessage.erase(remove(dataOfMessage.begin(), dataOfMessage.end(), '"'), dataOfMessage.end());

							if (dataOfMessage.compare("getAllTestQuestionsSpecificNameOfSUbject") != 0) {

								vector <string> allQuestions = dataBaseConnection.getAllTestQuestions(dataOfMessage);

								if (allQuestions.size() == 0) {
									sendSigleMessage("TestQuestionsNotFound", "", pCipher);
								} else {
									sendMoreSingleDataMessage("allTestQuestionsSpecificSubject", allQuestions, pCipher);
								}

							}
						}
					} else if (title.compare("addNewTestModification") == 0) {
						for (auto& data : jsonObjT) {
							if (data.size() != 1) {
								vector <string> dataOfNeTestModification(begin(data), end(data));
								bool insertStatus = dataBaseConnection.insertNewTestModification(dataOfNeTestModification);
								if (insertStatus) {
									// Insercción realizada con exito
									sendSigleMessage("insertNewModificationTest", "true", pCipher);
								} else {
									// Mensaje de error, no se pudo insertar el dato correspondiente
									sendSigleMessage("insertNewModificationTest", "false", pCipher);
								}
							}
						}
					} else if (title.compare("getAllNormalQuestionsSpecificTheme") == 0) {
						for (auto& element : jsonObjT) {

							string dataOfMessage = element;
							// Limpieza de '"' en el título recibido
							dataOfMessage.erase(remove(dataOfMessage.begin(), dataOfMessage.end(), '"'), dataOfMessage.end());

							if (dataOfMessage.compare("getAllNormalQuestionsSpecificTheme") != 0) {

								vector <string> allQuestions = dataBaseConnection.getAllNormalQuestionsSpecificTheme(dataOfMessage);

								if (allQuestions.size() == 0) {
									sendSigleMessage("normalQuestionsCreateExamNotFound", "", pCipher);
								} else {
									sendMoreSingleDataMessage("allNormalCreateExamQuestions", allQuestions, pCipher);
								}

							}
						}
					} else if (title.compare("getAllTestQuestionsSpecificTheme") == 0) {
						for (auto& element : jsonObjT) {

							string dataOfMessage = element;
							// Limpieza de '"' en el título recibido
							dataOfMessage.erase(remove(dataOfMessage.begin(), dataOfMessage.end(), '"'), dataOfMessage.end());

							if (dataOfMessage.compare("getAllTestQuestionsSpecificTheme") != 0) {

								vector <string> allQuestions = dataBaseConnection.getAllTestQuestionsSpecificTheme(dataOfMessage);

								if (allQuestions.size() == 0) {
									sendSigleMessage("testQuestionsCreateExamNotFound", "", pCipher);
								} else {
									sendMoreSingleDataMessage("allTestCreateExamQuestions", allQuestions, pCipher);
								}

							}
						}
					} else if (title.compare("findNameNormalModel") == 0) {
						for (auto& element : jsonObjT) {

							string dataOfMessage = element;
							// Limpieza de '"' en el título recibido
							dataOfMessage.erase(remove(dataOfMessage.begin(), dataOfMessage.end(), '"'), dataOfMessage.end());

							if (dataOfMessage.compare("findNameNormalModel") != 0) {
								string checkNameOfQuestionExist;
								bool checkName = dataBaseConnection.checkNameOfNormalQuestionExist(dataOfMessage);
								if (checkName) {
									checkNameOfQuestionExist = "true";
								} else {
									checkNameOfQuestionExist = "false";
								}
								sendSigleMessage("checkNormalNameModelExist", checkNameOfQuestionExist, pCipher);
							}
						}
					} else if (title.compare("findNameTestModel") == 0) {
						for (auto& element : jsonObjT) {

							string dataOfMessage = element;
							// Limpieza de '"' en el título recibido
							dataOfMessage.erase(remove(dataOfMessage.begin(), dataOfMessage.end(), '"'), dataOfMessage.end());

							if (dataOfMessage.compare("findNameTestModel") != 0) {
								string checkNameOfQuestionExist;
								bool checkName = dataBaseConnection.checkNameOfTestQuestionExist(dataOfMessage);
								if (checkName) {
									checkNameOfQuestionExist = "true";
								} else {
									checkNameOfQuestionExist = "false";
								}
								sendSigleMessage("checkTestNameModelExist", checkNameOfQuestionExist, pCipher);
							}
						}
					} else if (title.compare("createNormalModel") == 0) {
						for (auto& data : jsonObjT) {
							if (data.size() != 1) {
								vector <string> dataOfNewModel(begin(data), end(data));
								string insertStatus = dataBaseConnection.insertNewNormalModel(dataOfNewModel);
								if (insertStatus.compare("0") != 0) {
									// Creación del modelo realizada con éxito
									sendSigleMessage("createNormalModel", insertStatus, pCipher);
								} else {
									// Creación del modelo erronea
									sendSigleMessage("createNormalModel", insertStatus, pCipher);
								}
							}
						}
					} else if (title.compare("createTestModel") == 0) {
						for (auto& data : jsonObjT) {
							if (data.size() != 1) {
								vector <string> dataOfNewModel(begin(data), end(data));
								string insertStatus = dataBaseConnection.insertNewTestModel(dataOfNewModel);
								if (insertStatus.compare("0") != 0) {
									// Creación del modelo realizada con éxito
									sendSigleMessage("createTestModel", insertStatus, pCipher);
								} else {
									// Creación del modelo erronea
									sendSigleMessage("createTestModel", insertStatus, pCipher);
								}
							}
						}
					} else if (title.compare("updateAllNormalQuestionsNewNormalModel") == 0) {
						for (auto& data : jsonObjT) {
							if (data.size() != 1) {
								vector <string> allQuestionsToUpdate(begin(data), end(data));
								bool insertStatus = dataBaseConnection.updateNormalQuestionsNewModel(allQuestionsToUpdate);
								if (insertStatus) {
									// Actualización de las preguntas realizada con éxito
									sendSigleMessage("updateNormalQuestionNewModelStatus", "true", pCipher);
								} else {
									// Creación del modelo erronea
									sendSigleMessage("updateNormalQuestionNewModelSuccess", "false", pCipher);
								}
							}
						}
					} else if (title.compare("updateAllTestQuestionsNewNormalModel") == 0) {
						for (auto& data : jsonObjT) {
							if (data.size() != 1) {
								vector <string> allQuestionsToUpdate(begin(data), end(data));
								bool insertStatus = dataBaseConnection.updateTestQuestionsNewModel(allQuestionsToUpdate);
								if (insertStatus) {
									// Actualización de las preguntas realizada con éxito
									sendSigleMessage("updateTestQuestionNewModelStatus", "true", pCipher);
								} else {
									// Creación del modelo erronea
									sendSigleMessage("updateTestQuestionNewModelSuccess", "false", pCipher);
								}
							}
						}
					} else if (title.compare("createNormalExamFiles") == 0) {
						for (auto& data : jsonObjT) {
							if (data.size() != 1) {
								vector <string> allQuestions(begin(data), end(data));

								generateNormalExam(allQuestions);
								sendSigleMessage("normalExamCreatedSucces", "", pCipher);
							}
						}
					} else if (title.compare("SelectedNormalModelcreateNormalExamFiles") == 0) {
						for (auto& data : jsonObjT) {
							if (data.size() != 1) {
								vector <string> allQuestions(begin(data), end(data));

								generateNormalExam(allQuestions);
								sendSigleMessage("normalModelExamCreated", "", pCipher);
							}
						}
					} else if (title.compare("SelectedTestModelcreateTestExamFiles") == 0) {
						for (auto& data : jsonObjT) {
							if (data.size() != 1) {
								vector <string> allQuestions(begin(data), end(data));

								generateTestExam(allQuestions, true);
								generateTestExam(allQuestions, false);

								sendSigleMessage("testModelExamCreated", "", pCipher);
							}
						}
					} else if (title.compare("createTestExamFiles") == 0) {
						for (auto& data : jsonObjT) {
							if (data.size() != 1) {
								vector <string> allQuestions(begin(data), end(data));
								generateTestExam(allQuestions, true);
								generateTestExam(allQuestions, false);

								sendSigleMessage("TestExamCreatedSucces", "", pCipher);
							}

						}
					} else if (title.compare("sendNormalExam") == 0) {
						for (auto& element : jsonObjT) {
							string serverMessage = element;
							// Limpieza de '"' en el título recibido
							serverMessage.erase(remove(serverMessage.begin(), serverMessage.end(), '"'), serverMessage.end());

							if (serverMessage.compare("sendNormalExam") != 0) {
								cout << "ACAA " << "echo «datos de examen de tipo normal» | mail -s «examen» " + serverMessage + " -A '" + nameOfPDFStr + "'" << endl;
								getResultOfCommands("echo «datos de examen de tipo normal» | mail -s «examen» " + serverMessage + " -A '" + nameOfPDFStr + "'");
								std::this_thread::sleep_for(std::chrono::milliseconds(3000));
								getResultOfCommands("rm -r '" + nameOfPDFStr + "'");

							}
						}
					} else if (title.compare("sendTestExam") == 0) {
						for (auto& element : jsonObjT) {
							string serverMessage = element;
							// Limpieza de '"' en el título recibido
							serverMessage.erase(remove(serverMessage.begin(), serverMessage.end(), '"'), serverMessage.end());

							if (serverMessage.compare("sendTestExam") != 0) {

								getResultOfCommands("echo «datos de examen de tipo Test» | mail -s «examen_con_respuesta» " + serverMessage + " -A '" + nameOfPDFStrWithAnswers + "'");
								std::this_thread::sleep_for(std::chrono::milliseconds(3000));
								getResultOfCommands("rm -r '" + nameOfPDFStrWithAnswers + "'");
								getResultOfCommands("echo «datos de examen de tipo Test» | mail -s «examen_sin_respuesta» " + serverMessage + " -A '" + nameOfPDFStrWithOutAnswers + "'");
								std::this_thread::sleep_for(std::chrono::milliseconds(3000));
								getResultOfCommands("rm -r '" + nameOfPDFStrWithOutAnswers + "'");
							}
						}
					} else if (title.compare("getAllNormalModels") == 0) {
						for (auto& element : jsonObjT) {
							string serverMessage = element;
							// Limpieza de '"' en el título recibido
							serverMessage.erase(remove(serverMessage.begin(), serverMessage.end(), '"'), serverMessage.end());

							if (serverMessage.compare("getAllNormalModels") != 0) {
								vector <string> allDataNormalModels = dataBaseConnection.getAllNormalModels(serverMessage);
								sendMoreSingleDataMessage("allNormalDataModels", allDataNormalModels, pCipher);
							}
						}
					} else if (title.compare("getAllTestModels") == 0) {
						for (auto& element : jsonObjT) {
							string serverMessage = element;
							// Limpieza de '"' en el título recibido
							serverMessage.erase(remove(serverMessage.begin(), serverMessage.end(), '"'), serverMessage.end());

							if (serverMessage.compare("getAllTestModels") != 0) {
								vector <string> allDataTestModels = dataBaseConnection.getAllTestModels(serverMessage);
								sendMoreSingleDataMessage("allTestDataModels", allDataTestModels, pCipher);
							}
						}
					
					} else if (title.compare("getAllNormalQuestionsOfSpecificNormalModel") == 0) {
						for (auto& element : jsonObjT) {

							string dataOfMessage = element;
							// Limpieza de '"' en el título recibido
							dataOfMessage.erase(remove(dataOfMessage.begin(), dataOfMessage.end(), '"'), dataOfMessage.end());

							if (dataOfMessage.compare("getAllNormalQuestionsOfSpecificNormalModel") != 0) {

								vector <string> allQuestions = dataBaseConnection.getAllQuestionsOfSpecificNormalModel(dataOfMessage);
								sendMoreSingleDataMessage("allNormalModelQuestionsList", allQuestions, pCipher);
							}
						}
					} else if (title.compare("getAllTestQuestionsOfSpecificTestModel") == 0) {
						for (auto& element : jsonObjT) {

							string dataOfMessage = element;
							// Limpieza de '"' en el título recibido
							dataOfMessage.erase(remove(dataOfMessage.begin(), dataOfMessage.end(), '"'), dataOfMessage.end());

							if (dataOfMessage.compare("getAllTestQuestionsOfSpecificTestModel") != 0) {

								vector <string> allQuestions = dataBaseConnection.getAllQuestionsOfSpecificTestModel(dataOfMessage);
								sendMoreSingleDataMessage("allTestModelQuestionsList", allQuestions, pCipher);
							}
						}
					} else if (title.compare("checkPermissions") == 0) {
						for (auto& element : jsonObjT) {

							string dataOfMessage = element;
							// Limpieza de '"' en el título recibido
							dataOfMessage.erase(remove(dataOfMessage.begin(), dataOfMessage.end(), '"'), dataOfMessage.end());

							if (dataOfMessage.compare("checkPermissions") != 0) {

								string checkPermissionsOfUser = dataBaseConnection.checkUserPermissionsToChange(dataOfMessage);
								if (checkPermissionsOfUser.length() != 0) {
									sendSigleMessage("statusPermissionsOfChanges", checkPermissionsOfUser, pCipher);
								} else {
								// No tiene permisos para los cambios
									sendSigleMessage("statusPermissionsOfChanges", "withOutPermissions", pCipher);
								}
								
							}
						}
					}
					break;
				}

			} else {
			// Cierre de conexión al pasar el tiempo de time-out
				checkThreadFunction = false;
				checkTimeOut = false;
				sendSigleMessage("connectionStatus", "closedForTimeOut", pCipher);
			}
		}

	} else {
		// Fin de conexión al no ser una petición de obtener clave de encriptación
		checkThreadFunction = false;
		checkTimeOut = false;
	}
	removeThread(this_thread::get_id());
	} catch (...) {
		// En caso de error, se corta la conexión
		// Mandar mensaje de cierre de conexión al cliente
		cout << "ha saltado el catch" << endl;
		checkThreadFunction = false;
		checkTimeOut = false;
		removeThread(this_thread::get_id());
	}
}



void SocketObject::timeOutData() {
	mutex mtx;
	while (checkTimeOut) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		unique_lock<mutex> lock(mtx);
		timeOut += 1;
		if (timeOut >= maxTimeOut) {
			checkTimeOut = false;
		}
		cout << timeOut << endl;
	}
}

void SocketObject::spawn() {
	thread timeOutThread(&SocketObject::timeOutData, this);
	timeOutThread.detach();
}

void SocketObject::sendSigleMessage(string titleParam, string messageParam, Poco::Crypto::Cipher* cipherParam) {
	JsonSingleData* jSonObject = new JsonSingleData();
	jSonObject->A_Title = titleParam;

	jSonObject->B_Content = messageParam;

	// Conversión de objeto con datos de encriptado a formato JSON
	nlohmann::json jsonData;
	jsonData["A_Title"] = jSonObject->A_Title;
	jsonData["B_Content"] = jSonObject->B_Content;


	// Conversión del JSon con la clave a string y envio del mismo (0 = formato que acepta c#)
	string keyJsonToString = cipherParam->encryptString(jsonData.dump(0), Poco::Crypto::Cipher::ENC_BASE64);

	// Conversión del string formateado a un array de char
	char arrayData[keyJsonToString.size() + 1];
	keyJsonToString.copy(arrayData, keyJsonToString.size() + 1);
	arrayData[keyJsonToString.size()] = '\0';

	// Envio de los datos correspondientes a la encriptación al cliente
	send(sendReceiveDataSocket, arrayData, strlen(arrayData), 0);
}

void SocketObject::sendMoreSingleDataMessage(string titleParam, vector<string> messageParam, Poco::Crypto::Cipher* cipherParam) {
	JsonObjectArray* jSonObject = new JsonObjectArray();
	jSonObject->A_Title = titleParam;

	jSonObject->B_Content = messageParam;

	// Conversión de objeto con datos de encriptado a formato JSON
	nlohmann::json jsonData;
	jsonData["A_Title"] = jSonObject->A_Title;
	jsonData["B_Content"] = jSonObject->B_Content;

	// Conversión del JSon con la clave a string y envio del mismo (0 = formato que acepta c#)
	string keyJsonToString = cipherParam->encryptString(jsonData.dump(0), Poco::Crypto::Cipher::ENC_BASE64);



	// Conversión del string formateado a un array de char
	char arrayData[keyJsonToString.size() + 1];
	keyJsonToString.copy(arrayData, keyJsonToString.size() + 1);
	arrayData[keyJsonToString.size()] = '\0';

	// Envio de los datos correspondientes a la encriptación al cliente
	send(sendReceiveDataSocket, arrayData, strlen(arrayData), 0);
}

void SocketObject::generateNormalExam(vector<string> allQuestions) {
	nameOfPDFStr = "Tema ";
	nameOfPDFStr.append(allQuestions[allQuestions.size() - 1]);
	nameOfPDFStr.append(" - ");
	nameOfPDFStr.append(passwd);
	nameOfPDFStr.append(".pdf");

	PdfStreamedDocument document(nameOfPDFStr.c_str());
	PdfPainter painter;

	string tempStringAppend;

	int externalCounter = 0;

	float valueOfQuestion = 10.0 / ((allQuestions.size() - 1) / 2);

	std::stringstream stream;
	stream << std::fixed << std::setprecision(3) << valueOfQuestion;
	std::string valueOfQuestionRounded = stream.str();

	float checkIfDecimal = stof(valueOfQuestionRounded);

	if (checkIfDecimal == (int)checkIfDecimal) {
		valueOfQuestionRounded = to_string((int)checkIfDecimal);
	}

	string valueOfQuestionString = " \n(Valor de la pregunta: ";
	valueOfQuestionString.append(valueOfQuestionRounded);
	valueOfQuestionString.append(")");


	for (int counterPages = 0; counterPages < ((allQuestions.size() - 1) / 2); counterPages++) {
		PdfPage* pPage;
		const PdfEncoding* pEncoding = new PdfIdentityEncoding(); // required for UTF8 characterspodo
		PdfFont* pFont = document.CreateFont("Arial"); // LiberationSerif has polish characters 
		const char* tempString;

		if (counterPages == 0) {
			// Primera página en ser credada
			pPage = document.CreatePage(PdfPage::CreateStandardPageSize(ePdfPageSize_A4));
			painter.SetPage(pPage);

			tempString = "Fecha:\n\nNombre:\n\nApellidos: ";


			PdfString pString(reinterpret_cast<const pdf_utf8*>(tempString)); // Necesario para castear el texto a UTF-8

			pFont->SetFontSize(12);
			painter.SetFont(pFont);
			painter.Rectangle(60.0, pPage->GetPageSize().GetHeight() - 140.0, 300.0, 100.0);
			painter.DrawMultiLineText(60.0, pPage->GetPageSize().GetHeight() - 140.0, 300.0, 100.0, pString);

			tempStringAppend = "Tema: ";
			tempStringAppend.append(allQuestions[allQuestions.size() - 1]);
			tempString = tempStringAppend.c_str();

			pFont->SetFontSize(20);
			PdfString subjectPDF(reinterpret_cast<const pdf_utf8*>(tempString)); // Necesario para castear el texto a UTF-8
			painter.Rectangle(pPage->GetPageSize().GetWidth() * 0.25, pPage->GetPageSize().GetHeight() - 270.0, 300.0, 100.0);
			painter.DrawMultiLineText(pPage->GetPageSize().GetWidth() * 0.25, pPage->GetPageSize().GetHeight() - 270.0, 300.0, 100.0, subjectPDF);

			externalCounter++;
			if (((allQuestions.size() - 1) / 2) == 1) {
			int index = externalCounter;

			double size = 350;

			pFont->SetFontSize(12);
			tempStringAppend = "";
			tempStringAppend.append(allQuestions[externalCounter]);
			tempStringAppend.append(valueOfQuestionString);

			tempString = tempStringAppend.c_str();

			PdfString tempPString(reinterpret_cast<const pdf_utf8*>(tempString)); // Necesario para castear el texto a UTF-8
			painter.Rectangle(60.0, pPage->GetPageSize().GetHeight() - size, 500.0, 100.0);
			painter.DrawMultiLineText(60.0, pPage->GetPageSize().GetHeight() - size, 500.0, 100.0, tempPString);


			pFont->SetFontSize(12);
			index = externalCounter + 2;
			if (index == allQuestions.size()) {
				break;
			}
			tempStringAppend = "";

			tempStringAppend.append(allQuestions[externalCounter + 2]);
			tempStringAppend.append(valueOfQuestionString);

			PdfString tempPStringSecondQuestion(reinterpret_cast<const pdf_utf8*>(tempStringAppend.c_str())); // Necesario para castear el texto a UTF-8
			painter.Rectangle(60.0, pPage->GetPageSize().GetHeight() - 650.0, 500.0, 100.0);
			painter.DrawMultiLineText(60.0, pPage->GetPageSize().GetHeight() - 650.0, 500.0, 100.0, tempPStringSecondQuestion);

			externalCounter += 2;
			}
		} else if (counterPages % 2 == 0) {

			int index = externalCounter + 2;
			if (index == allQuestions.size()) {
				break;
			}

			pPage = document.CreatePage(PdfPage::CreateStandardPageSize(ePdfPageSize_A4));
			painter.SetPage(pPage);

			pFont->SetFontSize(12);
			tempStringAppend = "";


			tempStringAppend.append(allQuestions[externalCounter + 2]);
			tempStringAppend.append(valueOfQuestionString);

			tempString = tempStringAppend.c_str();

			PdfString tempPString(reinterpret_cast<const pdf_utf8*>(tempString)); // Necesario para castear el texto a UTF-8
			painter.Rectangle(60.0, pPage->GetPageSize().GetHeight() - 250.0, 500.0, 100.0);
			painter.DrawMultiLineText(60.0, pPage->GetPageSize().GetHeight() - 250.0, 500.0, 100.0, tempPString);


			pFont->SetFontSize(12);
			tempStringAppend = "";
			index = externalCounter + 4;
			if (index == allQuestions.size()) {
				break;
			}

			tempStringAppend.append(allQuestions[externalCounter + 4]);
			tempStringAppend.append(valueOfQuestionString);

			tempString = tempStringAppend.c_str();

			PdfString tempPStringSecondQuestion(reinterpret_cast<const pdf_utf8*>(tempString)); // Necesario para castear el texto a UTF-8
			painter.Rectangle(60.0, pPage->GetPageSize().GetHeight() - 650.0, 500.0, 100.0);
			painter.DrawMultiLineText(60.0, pPage->GetPageSize().GetHeight() - 650.0, 500.0, 100.0, tempPStringSecondQuestion);

			externalCounter += 6;
			if (externalCounter == allQuestions.size()) {
				break;
			}

		} else if (counterPages % 2 != 0) {
			int index = externalCounter;
			if (index == allQuestions.size()) {
				break;
			}
			double size = 350;
			if (counterPages != 1) {
				pPage = document.CreatePage(PdfPage::CreateStandardPageSize(ePdfPageSize_A4));
				painter.SetPage(pPage);
				size = 250;
			}

			pFont->SetFontSize(12);
			tempStringAppend = "";
			tempStringAppend.append(allQuestions[externalCounter]);
			tempStringAppend.append(valueOfQuestionString);

			tempString = tempStringAppend.c_str();

			PdfString tempPString(reinterpret_cast<const pdf_utf8*>(tempString)); // Necesario para castear el texto a UTF-8
			painter.Rectangle(60.0, pPage->GetPageSize().GetHeight() - size, 500.0, 100.0);
			painter.DrawMultiLineText(60.0, pPage->GetPageSize().GetHeight() - size, 500.0, 100.0, tempPString);


			pFont->SetFontSize(12);
			index = externalCounter + 2;
			if (index == allQuestions.size()) {
				break;
			}
			tempStringAppend = "";

			tempStringAppend.append(allQuestions[externalCounter + 2]);
			tempStringAppend.append(valueOfQuestionString);

			PdfString tempPStringSecondQuestion(reinterpret_cast<const pdf_utf8*>(tempStringAppend.c_str())); // Necesario para castear el texto a UTF-8
			painter.Rectangle(60.0, pPage->GetPageSize().GetHeight() - 650.0, 500.0, 100.0);
			painter.DrawMultiLineText(60.0, pPage->GetPageSize().GetHeight() - 650.0, 500.0, 100.0, tempPStringSecondQuestion);

			externalCounter += 2;
		}
	}
	painter.FinishPage();
	document.Close();
}

void SocketObject::generateTestExam(vector <string> allQuestions, bool generateWithNotAnswer) {
	if (generateWithNotAnswer) {
		nameOfPDFStr = "Tema ";
		nameOfPDFStr.append(allQuestions[allQuestions.size() - 1]);
		nameOfPDFStr.append(" - ");
		nameOfPDFStr.append(passwd);
		nameOfPDFStr.append("_con_respuesta.pdf");
	} else {
		nameOfPDFStrWithOutAnswers = "Tema ";
		nameOfPDFStrWithOutAnswers.append(allQuestions[allQuestions.size() - 1]);
		nameOfPDFStrWithOutAnswers.append(" - ");
		nameOfPDFStrWithOutAnswers.append(passwd);
		nameOfPDFStrWithOutAnswers.append("_sin_respuesta.pdf");
		nameOfPDFStrWithAnswers = nameOfPDFStr;
		nameOfPDFStr = nameOfPDFStrWithOutAnswers;

	}

	PdfStreamedDocument document(nameOfPDFStr.c_str());
	PdfPainter painter;

	string tempStringAppend;

	int externalCounter = 0;

	float valueOfQuestion = 10.0 / ((allQuestions.size() - 1) / 7);

	std::stringstream stream;
	stream << std::fixed << std::setprecision(3) << valueOfQuestion;
	std::string valueOfQuestionRounded = stream.str();

	float checkIfDecimal = stof(valueOfQuestionRounded);

	if (checkIfDecimal == (int)checkIfDecimal) {
		valueOfQuestionRounded = to_string((int)checkIfDecimal);
	}

	string valueOfQuestionString = " \n(Valor de la pregunta: ";
	valueOfQuestionString.append(valueOfQuestionRounded);
	valueOfQuestionString.append(")");

	// Revisar ese -1 al final
	for (int counterPages = 0; counterPages < ((allQuestions.size() - 1) / 7); counterPages++) {
		PdfPage* pPage;
		const PdfEncoding* pEncoding = new PdfIdentityEncoding(); // Necesario para castear el texto a UTF-8
		PdfFont* pFont = document.CreateFont("Arial");
		const char* tempString;

		if (counterPages == 0) {
			if (generateWithNotAnswer) {
				// Primera página en ser credada
				pPage = document.CreatePage(PdfPage::CreateStandardPageSize(ePdfPageSize_A4));
				painter.SetPage(pPage);

				pFont->SetFontSize(12);
				painter.SetFont(pFont);

				tempStringAppend = "Tema: ";
				tempStringAppend.append(allQuestions[allQuestions.size() - 1]);
				tempString = tempStringAppend.c_str();

				pFont->SetFontSize(20);
				PdfString subjectPDF(reinterpret_cast<const pdf_utf8*>(tempString)); // Necesario para castear el texto a UTF-8
				painter.Rectangle(pPage->GetPageSize().GetWidth() * 0.25, pPage->GetPageSize().GetHeight() - 180.0, 300.0, 100.0);
				painter.DrawMultiLineText(pPage->GetPageSize().GetWidth() * 0.25, pPage->GetPageSize().GetHeight() - 180.0, 300.0, 100.0, subjectPDF);

				externalCounter++;
				if (((allQuestions.size() - 1) / 7) == 1) {
					int index = externalCounter;

					double size = 350;


					pFont->SetFontSize(12);
					tempStringAppend = "";
					tempStringAppend.append("Pregunta: ");
					tempStringAppend.append(allQuestions[externalCounter]);
					tempStringAppend.append("\n");
					tempStringAppend.append("\n");
					tempStringAppend.append("A    ");
					tempStringAppend.append(allQuestions[externalCounter + 1]);
					tempStringAppend.append("\n");
					tempStringAppend.append("B    ");
					tempStringAppend.append(allQuestions[externalCounter + 2]);
					tempStringAppend.append("\n");
					tempStringAppend.append("C    ");
					tempStringAppend.append(allQuestions[externalCounter + 3]);
					tempStringAppend.append("\n");
					tempStringAppend.append("D    ");
					tempStringAppend.append(allQuestions[externalCounter + 4]);
					tempStringAppend.append("\n");
					tempStringAppend.append("\n");
					if (generateWithNotAnswer) {
						tempStringAppend.append("Opción correcta:   ");
						tempStringAppend.append(allQuestions[externalCounter + 5]);
						tempStringAppend.append("\n");
					}
					tempStringAppend.append(valueOfQuestionString);

					tempString = tempStringAppend.c_str();

					PdfString tempPString(reinterpret_cast<const pdf_utf8*>(tempString)); // Necesario para castear el texto a UTF-8
					painter.Rectangle(60.0, pPage->GetPageSize().GetHeight() - size, 500.0, 170.0);
					painter.DrawMultiLineText(60.0, pPage->GetPageSize().GetHeight() - size, 500.0, 170.0, tempPString);
				}

			} else {
				// Primera página en ser credada
				pPage = document.CreatePage(PdfPage::CreateStandardPageSize(ePdfPageSize_A4));
				painter.SetPage(pPage);

				tempString = "Fecha:\n\nNombre:\n\nApellidos: ";

				PdfString pString(reinterpret_cast<const pdf_utf8*>(tempString)); // Necesario para castear el texto a UTF-8

				pFont->SetFontSize(12);
				painter.SetFont(pFont);
				painter.Rectangle(60.0, pPage->GetPageSize().GetHeight() - 140.0, 300.0, 100.0);
				painter.DrawMultiLineText(60.0, pPage->GetPageSize().GetHeight() - 140.0, 300.0, 100.0, pString);

				tempStringAppend = "Tema: ";
				tempStringAppend.append(allQuestions[allQuestions.size() - 1]);
				tempString = tempStringAppend.c_str();

				pFont->SetFontSize(20);
				PdfString subjectPDF(reinterpret_cast<const pdf_utf8*>(tempString)); // Necesario para castear el texto a UTF-8
				painter.Rectangle(pPage->GetPageSize().GetWidth() * 0.25, pPage->GetPageSize().GetHeight() - 230.0, 300.0, 100.0);
				painter.DrawMultiLineText(pPage->GetPageSize().GetWidth() * 0.25, pPage->GetPageSize().GetHeight() - 230.0, 300.0, 100.0, subjectPDF);
				externalCounter++;

				if (((allQuestions.size() - 1) / 7) == 1) {
					int index = externalCounter;

					double size = 350;


					pFont->SetFontSize(12);
					tempStringAppend = "";
					tempStringAppend.append("Pregunta: ");
					tempStringAppend.append(allQuestions[externalCounter]);
					tempStringAppend.append("\n");
					tempStringAppend.append("\n");
					tempStringAppend.append("A    ");
					tempStringAppend.append(allQuestions[externalCounter + 1]);
					tempStringAppend.append("\n");
					tempStringAppend.append("B    ");
					tempStringAppend.append(allQuestions[externalCounter + 2]);
					tempStringAppend.append("\n");
					tempStringAppend.append("C    ");
					tempStringAppend.append(allQuestions[externalCounter + 3]);
					tempStringAppend.append("\n");
					tempStringAppend.append("D    ");
					tempStringAppend.append(allQuestions[externalCounter + 4]);
					tempStringAppend.append("\n");
					tempStringAppend.append("\n");
					if (generateWithNotAnswer) {
						tempStringAppend.append("Opción correcta:   ");
						tempStringAppend.append(allQuestions[externalCounter + 5]);
						tempStringAppend.append("\n");
					}
					tempStringAppend.append(valueOfQuestionString);

					tempString = tempStringAppend.c_str();

					PdfString tempPString(reinterpret_cast<const pdf_utf8*>(tempString)); // Necesario para castear el texto a UTF-8
					painter.Rectangle(60.0, pPage->GetPageSize().GetHeight() - size, 500.0, 170.0);
					painter.DrawMultiLineText(60.0, pPage->GetPageSize().GetHeight() - size, 500.0, 170.0, tempPString);
				}
			}
			

		} else if (counterPages % 2 == 0) {

			int index = externalCounter;
			if (index == allQuestions.size()) {
				break;
			}

			pPage = document.CreatePage(PdfPage::CreateStandardPageSize(ePdfPageSize_A4));
			painter.SetPage(pPage);

			pFont->SetFontSize(12);
			tempStringAppend = "";


			tempStringAppend.append("Pregunta: ");
			tempStringAppend.append(allQuestions[externalCounter]);
			tempStringAppend.append("\n");
			tempStringAppend.append("\n");
			tempStringAppend.append("A    ");
			tempStringAppend.append(allQuestions[externalCounter + 1]);
			tempStringAppend.append("\n");
			tempStringAppend.append("B    ");
			tempStringAppend.append(allQuestions[externalCounter + 2]);
			tempStringAppend.append("\n");
			tempStringAppend.append("C    ");
			tempStringAppend.append(allQuestions[externalCounter + 3]);
			tempStringAppend.append("\n");
			tempStringAppend.append("D    ");
			tempStringAppend.append(allQuestions[externalCounter + 4]);
			tempStringAppend.append("\n");
			tempStringAppend.append("\n");
			if (generateWithNotAnswer) {
				tempStringAppend.append("Opción correcta:   ");
				tempStringAppend.append(allQuestions[externalCounter + 5]);
				tempStringAppend.append("\n");
			}
			tempStringAppend.append(valueOfQuestionString);

			tempString = tempStringAppend.c_str();

			PdfString tempPString(reinterpret_cast<const pdf_utf8*>(tempString)); // Necesario para castear el texto a UTF-8
			painter.Rectangle(60.0, pPage->GetPageSize().GetHeight() - 250.0, 500.0, 170.0);
			painter.DrawMultiLineText(60.0, pPage->GetPageSize().GetHeight() - 250.0, 500.0, 170.0, tempPString);


			pFont->SetFontSize(12);
			tempStringAppend = "";
			index = externalCounter + 7;
			if (index == allQuestions.size()) {
				break;
			}

			pFont->SetFontSize(12);
			tempStringAppend = "";

			tempStringAppend.append("Pregunta: ");
			tempStringAppend.append(allQuestions[externalCounter + 7]);
			tempStringAppend.append("\n");
			tempStringAppend.append("\n");
			tempStringAppend.append("A    ");
			tempStringAppend.append(allQuestions[externalCounter + 8]);
			tempStringAppend.append("\n");
			tempStringAppend.append("B    ");
			tempStringAppend.append(allQuestions[externalCounter + 9]);
			tempStringAppend.append("\n");
			tempStringAppend.append("C    ");
			tempStringAppend.append(allQuestions[externalCounter + 10]);
			tempStringAppend.append("\n");
			tempStringAppend.append("D    ");
			tempStringAppend.append(allQuestions[externalCounter + 11]);
			tempStringAppend.append("\n");
			tempStringAppend.append("\n");
			if (generateWithNotAnswer) {
				tempStringAppend.append("Opción correcta:   ");
				tempStringAppend.append(allQuestions[externalCounter + 12]);
				tempStringAppend.append("\n");
			}
			tempStringAppend.append(valueOfQuestionString);

			tempString = tempStringAppend.c_str();

			PdfString tempPStringTwo(reinterpret_cast<const pdf_utf8*>(tempString)); // Necesario para castear el texto a UTF-8
			painter.Rectangle(60.0, pPage->GetPageSize().GetHeight() - 450, 500.0, 170.0);
			painter.DrawMultiLineText(60.0, pPage->GetPageSize().GetHeight() - 450, 500.0, 170.0, tempPStringTwo);


			pFont->SetFontSize(12);
			index = externalCounter + 14;
			if (index == allQuestions.size()) {
				break;
			}
			tempStringAppend = "";

			tempStringAppend.append("Pregunta: ");
			tempStringAppend.append(allQuestions[externalCounter + 14]);
			tempStringAppend.append("\n");
			tempStringAppend.append("\n");
			tempStringAppend.append("A    ");
			tempStringAppend.append(allQuestions[externalCounter + 15]);
			tempStringAppend.append("\n");
			tempStringAppend.append("B    ");
			tempStringAppend.append(allQuestions[externalCounter + 16]);
			tempStringAppend.append("\n");
			tempStringAppend.append("C    ");
			tempStringAppend.append(allQuestions[externalCounter + 17]);
			tempStringAppend.append("\n");
			tempStringAppend.append("D    ");
			tempStringAppend.append(allQuestions[externalCounter + 18]);
			tempStringAppend.append("\n");
			tempStringAppend.append("\n");
			if (generateWithNotAnswer) {
				tempStringAppend.append("Opción correcta:   ");
				tempStringAppend.append(allQuestions[externalCounter + 19]);
				tempStringAppend.append("\n");
			}
			tempStringAppend.append(valueOfQuestionString);

			PdfString tempPStringSecondQuestion(reinterpret_cast<const pdf_utf8*>(tempStringAppend.c_str())); // Necesario para castear el texto a UTF-8
			painter.Rectangle(60.0, pPage->GetPageSize().GetHeight() - 650.0, 500.0, 170.0);
			painter.DrawMultiLineText(60.0, pPage->GetPageSize().GetHeight() - 650.0, 500.0, 170.0, tempPStringSecondQuestion);

			externalCounter += 21;
			if (externalCounter == allQuestions.size()) {
				break;
			}

		} else if (counterPages % 2 != 0) {
			int index = externalCounter;
			if (index == allQuestions.size()) {
				break;
			}
			double size = 350;
			if (counterPages != 1) {
				pPage = document.CreatePage(PdfPage::CreateStandardPageSize(ePdfPageSize_A4));
				painter.SetPage(pPage);
				size = 250;
			}

			pFont->SetFontSize(12);
			tempStringAppend = "";
			tempStringAppend.append("Pregunta: ");
			tempStringAppend.append(allQuestions[externalCounter]);
			tempStringAppend.append("\n");
			tempStringAppend.append("\n");
			tempStringAppend.append("A    ");
			tempStringAppend.append(allQuestions[externalCounter + 1]);
			tempStringAppend.append("\n");
			tempStringAppend.append("B    ");
			tempStringAppend.append(allQuestions[externalCounter + 2]);
			tempStringAppend.append("\n");
			tempStringAppend.append("C    ");
			tempStringAppend.append(allQuestions[externalCounter + 3]);
			tempStringAppend.append("\n");
			tempStringAppend.append("D    ");
			tempStringAppend.append(allQuestions[externalCounter + 4]);
			tempStringAppend.append("\n");
			tempStringAppend.append("\n");
			if (generateWithNotAnswer) {
				tempStringAppend.append("Opción correcta:   ");
				tempStringAppend.append(allQuestions[externalCounter + 5]);
				tempStringAppend.append("\n");
			}
			tempStringAppend.append(valueOfQuestionString);

			tempString = tempStringAppend.c_str();

			PdfString tempPString(reinterpret_cast<const pdf_utf8*>(tempString)); // Necesario para castear el texto a UTF-8
			painter.Rectangle(60.0, pPage->GetPageSize().GetHeight() - size, 500.0, 170.0);
			painter.DrawMultiLineText(60.0, pPage->GetPageSize().GetHeight() - size, 500.0, 170.0, tempPString);

			if (counterPages != 1) {
				index = externalCounter + 7;
				if (index == allQuestions.size()) {
					break;
				}
				pFont->SetFontSize(12);
				tempStringAppend = "";

				tempStringAppend.append("Pregunta: ");
				tempStringAppend.append(allQuestions[externalCounter + 7]);
				tempStringAppend.append("\n");
				tempStringAppend.append("\n");
				tempStringAppend.append("A    ");
				tempStringAppend.append(allQuestions[externalCounter + 8]);
				tempStringAppend.append("\n");
				tempStringAppend.append("B    ");
				tempStringAppend.append(allQuestions[externalCounter + 9]);
				tempStringAppend.append("\n");
				tempStringAppend.append("C    ");
				tempStringAppend.append(allQuestions[externalCounter + 10]);
				tempStringAppend.append("\n");
				tempStringAppend.append("D    ");
				tempStringAppend.append(allQuestions[externalCounter + 11]);
				tempStringAppend.append("\n");
				tempStringAppend.append("\n");
				if (generateWithNotAnswer) {
					tempStringAppend.append("Opción correcta:   ");
					tempStringAppend.append(allQuestions[externalCounter + 12]);
					tempStringAppend.append("\n");
				}
				tempStringAppend.append(valueOfQuestionString);

				tempString = tempStringAppend.c_str();

				PdfString tempPStringTwo(reinterpret_cast<const pdf_utf8*>(tempString)); // Necesario para castear el texto a UTF-8
				painter.Rectangle(60.0, pPage->GetPageSize().GetHeight() - 450, 500.0, 170.0);
				painter.DrawMultiLineText(60.0, pPage->GetPageSize().GetHeight() - 450, 500.0, 170.0, tempPStringTwo);


				pFont->SetFontSize(12);
				index = externalCounter + 14;
				if (index == allQuestions.size()) {
					break;
				}
				tempStringAppend = "";

				tempStringAppend.append("Pregunta: ");
				tempStringAppend.append(allQuestions[externalCounter + 14]);
				tempStringAppend.append("\n");
				tempStringAppend.append("\n");
				tempStringAppend.append("A    ");
				tempStringAppend.append(allQuestions[externalCounter + 15]);
				tempStringAppend.append("\n");
				tempStringAppend.append("B    ");
				tempStringAppend.append(allQuestions[externalCounter + 16]);
				tempStringAppend.append("\n");
				tempStringAppend.append("C    ");
				tempStringAppend.append(allQuestions[externalCounter + 17]);
				tempStringAppend.append("\n");
				tempStringAppend.append("D    ");
				tempStringAppend.append(allQuestions[externalCounter + 18]);
				tempStringAppend.append("\n");
				tempStringAppend.append("\n");
				if (generateWithNotAnswer) {
					tempStringAppend.append("Opción correcta:   ");
					tempStringAppend.append(allQuestions[externalCounter + 19]);
					tempStringAppend.append("\n");
				}
				tempStringAppend.append(valueOfQuestionString);

				PdfString tempPStringSecondQuestion(reinterpret_cast<const pdf_utf8*>(tempStringAppend.c_str())); // Necesario para castear el texto a UTF-8
				painter.Rectangle(60.0, pPage->GetPageSize().GetHeight() - 650.0, 500.0, 170.0);
				painter.DrawMultiLineText(60.0, pPage->GetPageSize().GetHeight() - 650.0, 500.0, 170.0, tempPStringSecondQuestion);

				externalCounter += 21;
			} else {
				pFont->SetFontSize(12);
				index = externalCounter + 7;
				if (index == allQuestions.size()) {
					break;
				}
				tempStringAppend = "";

				tempStringAppend.append("Pregunta: ");
				tempStringAppend.append(allQuestions[externalCounter + 7]);
				tempStringAppend.append("\n");
				tempStringAppend.append("\n");
				tempStringAppend.append("A    ");
				tempStringAppend.append(allQuestions[externalCounter + 8]);
				tempStringAppend.append("\n");
				tempStringAppend.append("B    ");
				tempStringAppend.append(allQuestions[externalCounter + 9]);
				tempStringAppend.append("\n");
				tempStringAppend.append("C    ");
				tempStringAppend.append(allQuestions[externalCounter + 10]);
				tempStringAppend.append("\n");
				tempStringAppend.append("D    ");
				tempStringAppend.append(allQuestions[externalCounter + 11]);
				tempStringAppend.append("\n");
				tempStringAppend.append("\n");
				if (generateWithNotAnswer) {
					tempStringAppend.append("Opción correcta:   ");
					tempStringAppend.append(allQuestions[externalCounter + 12]);
					tempStringAppend.append("\n");
				}
				tempStringAppend.append(valueOfQuestionString);

				PdfString tempPStringSecondQuestion(reinterpret_cast<const pdf_utf8*>(tempStringAppend.c_str())); // Necesario para castear el texto a UTF-8
				painter.Rectangle(60.0, pPage->GetPageSize().GetHeight() - 650.0, 500.0, 170.0);
				painter.DrawMultiLineText(60.0, pPage->GetPageSize().GetHeight() - 650.0, 500.0, 170.0, tempPStringSecondQuestion);

				externalCounter += 14;
			}


		}
	}
	painter.FinishPage();
	document.Close();

}

string SocketObject::generatePasswd() {
	srand((unsigned)time(0));
	int randomNumber;
	string randomPasswd;

	for (int index = 0; index < 32; index++) {
		randomNumber = (rand() % 9) + 1;
		randomPasswd += to_string(randomNumber);
	}
	return randomPasswd;
}

bool SocketObject::getResultOfCommands(string consoleCommand) {
	try {
		FILE* stream;
		const int max_buffer = 256;
		char buffer[max_buffer];
		consoleCommand.append(" 2>&1");

		stream = popen(consoleCommand.c_str(), "r");
		return true;
	} catch (...) {
		return false;
	}
}

vector<string> SocketObject::splitLineToLine(string resultOfCommand) {
	stringstream ss(resultOfCommand);
	string to;
	vector <string> splitResult;
	if (resultOfCommand != "") {
		while (getline(ss, to, '\n')) {
			splitResult.push_back(to);
		}
	}
	return splitResult;
}

void SocketObject::removeThread(thread::id id) {
	try {
		mutex internalMtx;
		lock_guard<mutex> lock(internalMtx);
		auto iter = find_if(allSockets.begin(), allSockets.end(), [=](thread& t) { return (t.get_id() == id); });
		if (iter != allSockets.end()) {
			iter->detach();
			allSockets.erase(iter);
		}
		cout << "conexiones activas " << allSockets.size() << endl;
	} catch (...) {
		cout << "No se ha podido borrar el objeto del array de conexiones" << endl;
	}
}
