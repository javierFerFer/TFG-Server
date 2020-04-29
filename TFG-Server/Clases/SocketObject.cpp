#include "../Headers/SocketObject.h"
#include "../Libraries/json.hpp"
#include "../Headers/JsonObjectArray.h"
#include "../Headers/JsonSingleData.h"
using namespace std;



void SocketObject::launchReadThread() {
	//try {
	std::this_thread::sleep_for(std::chrono::milliseconds(3000));


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

				valread = read(sendReceiveDataSocket, buffer, 1024);

				unique_lock<mutex> lock(mtx);
				if (timeOut < maxTimeOut) {
					// reseteo de time out
					timeOut = 0;

				// Lectura de datos enviados por el cliente
				string userData(buffer);

				// Desencriptado de datos
				std::string decrypted;


				decrypted = pCipher->decryptString(userData, Poco::Crypto::Cipher::ENC_BASE64);



				//cout << "AQUI BRO" << decrypted <<endl;


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
						// nada
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
								} else {
									// Nombre no encontrado, cierre de conexión del cliente
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
					}
					// Solo debe leer el título
					break;
					//if (element.size() != 1) {
					//	// Guarda datos recibidos
					//	vector <string> allData(begin(element), end(element));
					//	internalJsonObject->B_Content = allData;
					//} else {
					//	// Guarda titulo
					//	string title = element;
					//	internalJsonObject->A_Title  = element;
					//}
					//break;
				}

				// crear Json con los datos de la clase
				//nlohmann::json jsonObjVicer;
				//jsonObjVicer["Title"] = internalJsonObject->A_Title;
				//jsonObjVicer["Content"] = internalJsonObject->B_Content;


				// Revisión del contenido

				// Convertimos el json en un string
				//string bar = jsonObjVicer.dump();

			} else {
				checkThreadFunction = false;
				checkTimeOut = false;
				sendSigleMessage("connectionStatus", "closedForTimeOut", pCipher);
				// Mandará mensaje de time out al cliente y se cierra
				// la conexión
			}
		}

	} else {
		// Fin de conexión al no ser una petición de obtener clave de encriptación
		checkThreadFunction = false;
		checkTimeOut = false;
	}
	// Encriptado/desencriptado
	//std::string encrypted = pCipher->encryptString(plainText, Poco::Crypto::Cipher::ENC_BASE64); //Base64-encoded output
	//cout << "plainText=" << plainText << endl;
	//cout << "encrypted=" << encrypted << endl;
	//std::string decrypted = pCipher->decryptString(encrypted, Poco::Crypto::Cipher::ENC_BASE64);
	//cout << "decrypted=" << decrypted << endl;
	removeThread(this_thread::get_id());
	//} catch (...) {
	//	// En caso de error, se corta la conexión
	//	// Mandar mensaje de cierre de conexión al cliente
	//	cout << "ha saltado el catch" << endl;
	//	checkThreadFunction = false;
	//	checkTimeOut = false;
	//	removeThread(this_thread::get_id());
	//}
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

string SocketObject::generatePasswd() {
	srand((unsigned)time(0));
	int randomNumber;
	string randomPasswd;

	for (int index = 0; index < 32; index++) {
		randomNumber = (rand() % 9) + 1;
		randomPasswd += to_string(randomNumber);
	}
	//string clave = "01234567891234560123456789123456";
	return randomPasswd;
}

void SocketObject::removeThread(thread::id id) {
	mutex internalMtx;
	lock_guard<mutex> lock(internalMtx);
	auto iter = find_if(allSockets.begin(), allSockets.end(), [=](thread& t) { return (t.get_id() == id); });
	if (iter != allSockets.end()) {
		iter->detach();
		allSockets.erase(iter);
		//cout << sendReceiveDataSocket << endl;
	}
	cout << "conexiones activas " << allSockets.size() << endl;
}
