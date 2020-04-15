#include "../Headers/SocketObject.h"
#include "../Libraries/json.hpp"
#include "../Headers/JsonObject.h"
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

		// Creamos JSon con la clave y se lo enviamos al cliente
		nlohmann::json keyJson;
		keyJson["a_Title"] = "key";
		keyJson["b_Content"] = passwd;

		// Conversión del JSon con la clave a string y envio del mismo (0 = formato que acepta c#)
		string keyJsonToString = keyJson.dump(0);

		cout << "here" << endl;
		cout << keyJsonToString << endl;
		//std::this_thread::sleep_for(std::chrono::milliseconds(30000));

		// Conversión del string formateado a un array de char
		char arrayData[keyJsonToString.size() + 1];
		keyJsonToString.copy(arrayData, keyJsonToString.size() + 1);
		arrayData[keyJsonToString.size()] = '\0';
		send(sendReceiveDataSocket, arrayData, strlen(arrayData), 0);
		

		spawn();
		mutex mtx;

		// AQUI

		while (checkThreadFunction) {

			valread = read(sendReceiveDataSocket, buffer, 1024);

			string userData(buffer);


			nlohmann::json jsonObjT;
			std::stringstream(buffer) >> jsonObjT;

			JsonObject* internalJsonObject = new JsonObject();

			for (auto& element : jsonObjT) {
				if (element.size() != 1) {
					// Guarda datos recibidos
					vector <string> allData(begin(element), end(element));
					internalJsonObject->content = allData;
				} else {
					// Guarda titulo
					string title = element;
					internalJsonObject->title = element;
				}
			}

			// crear Json con los datos de la clase
			nlohmann::json jsonObjVicer;
			jsonObjVicer["Title"] = internalJsonObject->title;
			jsonObjVicer["Content"] = internalJsonObject->content;


			// Revisión del contenido

			// Convertimos el json en un string
			string bar = jsonObjVicer.dump();


			cout << "valores del vector" << endl;
			for (int i = 0; i < internalJsonObject->content.size(); i++) {
				cout << internalJsonObject->content[i] << endl;
			}

			cout << "titulo" << endl;
			cout << internalJsonObject->title << endl;

			cout << "objeto convertido a string" << endl;
			cout << bar << endl;

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
		cout << timeOut << endl;
	}
}

void SocketObject::spawn() {
	thread timeOutThread(&SocketObject::timeOutData, this);
	timeOutThread.detach();
}

string SocketObject::generatePasswd() {
	int generatePasswd = 111111111111111111111111111111 + (std::rand() % (999999999999999999999999999999 - 111111111111111111111111111111 + 1));
	return to_string(generatePasswd);
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
	cout << allSockets.size() << endl;
}
