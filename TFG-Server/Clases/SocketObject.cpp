#include "../Headers/SocketObject.h"
#include "../Libraries/json.hpp"
#include "../Headers/JsonObject.h"
#include "../Headers/DataBaseConnect.h"
using namespace std;



void SocketObject::launchReadThread() {

	std::this_thread::sleep_for(std::chrono::milliseconds(3000));
	bool checkThreadFunction = true;

	// Conexión de pruebas a la base de datos MySQL
	DataBaseConnect testDB;

	spawn();
	mutex mtx;
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

		// Convertimos el json en un string
	
		string bar = jsonObjVicer.dump();
		

		cout << "valores del vector" << endl;
		for (int i = 0; i < internalJsonObject->content.size(); i++) {
			cout << internalJsonObject->content[i] << endl;
		}

		cout << "titulo" << endl;
		cout<< internalJsonObject->title<<endl;
		
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
