#include "../Headers/SocketObject.h"
#include "../Libraries/json.hpp"
#include "../Headers/JsonObjectArray.h"
#include "../Headers/JsonSingleData.h"
#include "../Headers/DataBaseConnect.h"
using namespace std;



void SocketObject::launchReadThread() {

	std::this_thread::sleep_for(std::chrono::milliseconds(3000));
	bool checkThreadFunction = true;

	printf("Se ha conectado: %s:%d\n", inet_ntoa(clientSocket.sin_addr), clientSocket.sin_port);

	// Conexión de pruebas a la base de datos MySQL
	//DataBaseConnect testDB;


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
		JsonObjectArray *keyJSonObject = new JsonObjectArray();
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
		
		// Hilo de timeOut, de momento desactivado para testeo
		//spawn();


		mutex mtx;
		while (checkThreadFunction) {

			valread = read(sendReceiveDataSocket, buffer, 1024);

			// Lectura de datos enviados por el cliente
			string userData(buffer);

			// Desencriptado de datos
			std::string decrypted = pCipher->decryptString(userData, Poco::Crypto::Cipher::ENC_BASE64);

			nlohmann::json jsonObjT;
			std::stringstream(decrypted) >> jsonObjT;

			JsonObjectArray*internalJsonObject = new JsonObjectArray();
			
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
							DataBaseConnect test;
							bool checkLogin = test.loginQuery(userLoginData[0], userLoginData[1]);
							if (checkLogin) {
								// Datos de usuario correctos
								sendMessage("loginStatus", "correct", pCipher);
							} else {
							// Error de login
							}
						}
					}
				}
				if (element.size() != 1) {
					// Guarda datos recibidos
					vector <string> allData(begin(element), end(element));
					internalJsonObject->B_Content = allData;
				} else {
					// Guarda titulo
					string title = element;
					internalJsonObject->A_Title  = element;
				}
				break;
			}

			// crear Json con los datos de la clase
			nlohmann::json jsonObjVicer;
			jsonObjVicer["Title"] = internalJsonObject->A_Title;
			jsonObjVicer["Content"] = internalJsonObject->B_Content;


			// Revisión del contenido

			// Convertimos el json en un string
			string bar = jsonObjVicer.dump();


			unique_lock<mutex> lock(mtx);
			if (timeOut < maxTimeOut) {
				timeOut = 0;
				if (userData.find("javi") != string::npos) {
					// Manda mensaje de finalización de conexión al cliente antes de finalizar
					checkThreadFunction = false;
					checkTimeOut = false;
				}
			} else {
				checkThreadFunction = false;
				checkTimeOut = false;
				// Mandará mensaje de time out al cliente y se cierra
				// la conexión
			}
		}

	} else {
	// Error al no ser el mensaje error de get passwd
	}
	// Encriptado/desencriptado
	//std::string encrypted = pCipher->encryptString(plainText, Poco::Crypto::Cipher::ENC_BASE64); //Base64-encoded output
	//cout << "plainText=" << plainText << endl;
	//cout << "encrypted=" << encrypted << endl;
	//std::string decrypted = pCipher->decryptString(encrypted, Poco::Crypto::Cipher::ENC_BASE64);
	//cout << "decrypted=" << decrypted << endl;
	removeThread(this_thread::get_id());
}

void SocketObject::timeOutData() {
	mutex mtx;
	while (checkTimeOut) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		unique_lock<mutex> lock(mtx);
		timeOut += 1;
	}
}

void SocketObject::spawn() {
	thread timeOutThread(&SocketObject::timeOutData, this);
	timeOutThread.detach();
}

void SocketObject::sendMessage(string titleParam, string messageParam, Poco::Crypto::Cipher *cipherParam) {
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
	//cout << allSockets.size() << endl;
}
