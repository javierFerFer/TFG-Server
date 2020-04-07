//todos los include de la clase
#include "../Headers/Main.h"
#include "../Headers/Menu.h"
#include <sstream>


/*
Main del programa

*/

// Constructor de la clase, se ejecuta al crear un objeto de la misma
Menu::Menu(){
	checkPort();
	//mostrarMenu();
}

void Menu::checkPort() {
	resultCommand = getResultOfCommand(ufwCommand);
	vector<string> resultCommandSplit = splitLineToLine(resultCommand);
	checkProgramPort(resultCommandSplit);
	
	//resetPortVariables();
}

void Menu::mostrarMenu() {
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

vector<string> Menu::splitLineToLine(string sentenceParam) {
	stringstream ss(sentenceParam);
	string to;
	vector <string> splitResult;
	if (sentenceParam != "") {
		while (getline(ss, to, '\n')) {
			splitResult.push_back(to);
		}
	}
	return splitResult;
}

void Menu::checkProgramPort(vector<string> vectorParam) {
	for (int counterLines = 0; counterLines < vectorParam.size(); counterLines++) {
		if (vectorParam[counterLines].find(port) != string::npos) {
			checkProgramPortBool = true;
			if (vectorParam[counterLines].find(statusPortOpen) != string::npos) {
				cout << "Encontrado y abierto"<< endl;
				checkProgramPortOpenOrClose = true;
				break;
			} else if (vectorParam[counterLines].find(statusPortClose) != string::npos) {
				cout << "Encontrado, pero cerrado" << endl;
				break;
			}
			break;
		}
	}

	if (!checkProgramPortBool) {
		cout << "Puerto no encontrado" << endl;
	}
}

void Menu::resetPortVariables() {
	checkProgramPortBool = false;
	checkProgramPortOpenOrClose = false;
}






string Menu::getResultOfCommand(string cmdCommandParam) {

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
