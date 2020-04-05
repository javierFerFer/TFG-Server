//todos los include de la clase
#include "../Headers/Main.h"
#include "../Headers/ThreadMenu.h"
#include <iostream>
#include <sstream>
using namespace std;


/*
Main del programa

*/

// Constructor de la clase, se ejecuta al crear un objeto de la misma
ThreadMenu::ThreadMenu(){
	mostrarMenu();
}

void ThreadMenu::mostrarMenu() {
	while (checkMenu) {
		cout << "Opciones del menu:" << endl;
		cout << "1. Activar el servidor" << endl;
		cout << "2. Ver estado del servidor" << endl;
		cout << "3. Desactivar el servidor" << endl;
		cout << "4. Modificar clave RSA" << endl;
		cout << "5. Salir" << endl;
		cout << endl;
		cout << "Introduce el índice de la acción que desea realizar" << endl;
		cin >> userValue;

		if (userValue == "1") {
			// Activa el servidor
		} else if (userValue == "2") {
			// ver el estado del servidor
			string foo = getResultOfCommand("ufw status");
			splitLineToLine(foo);
		} else if (userValue == "3") {
			// Desactivar el servidor
		} else if (userValue == "4") {
			// Modificar la clave RSA
		} else if (userValue == "5") {
			exit(0);
		} else {
			// Error de valor introducido
			cout << "Ha introducido un valor inválido"<<endl;
		}
	}
}

void ThreadMenu::splitLineToLine(string sentenceParam) {
	std::stringstream ss(sentenceParam);
	std::string to;

	if (sentenceParam != "") {
		while (std::getline(ss, to, '\n')) {
			
			if (to.find("12345") != std::string::npos) {
				if (to.find("ALLOW") != std::string::npos) {
					cout << "El servidor está activado" << endl;
					cout << endl;
					break;
				} else if (to.find("DENY") != std::string::npos) {
					cout << "El servidor está desactivado" << endl;
					cout << endl;
					break;
				}
				break;
			}
		}
	}
}

string ThreadMenu::getResultOfCommand(string cmdCommandParam) {

	string data;
	FILE* stream;
	const int max_buffer = 256;
	char buffer[max_buffer];
	cmdCommandParam.append(" 2>&1");

	stream = popen(cmdCommandParam.c_str(), "r");
	if (stream) {
		while (!feof(stream))
			if (fgets(buffer, max_buffer, stream) != NULL) data.append(buffer);
		pclose(stream);
	}
	return data;
}
