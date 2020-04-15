//todos los include de la clase
#include "../Headers/Main.h"
#include "../Headers/Menu.h"
#include <sstream>
#include <thread>

#define Crypto_CipherFactory_INCLUDED;

/*
Main del programa

*/

// Constructor de la clase, se ejecuta al crear un objeto de la misma
Menu::Menu(){

	checkPort();

	if (checkProgramPortBool && checkProgramPortOpenOrClose) {
		checkServerRunning();
	} else {
		if (!checkProgramPortBool) {
			cout << "No se ha podido encontrar el puerto del programa, asegurese de tener ufw configurado en su servidor" <<endl;
		}
	}

	if (checkMenuServer) {
		if (checkProgramServiceBool) {
			if (countServerProcess()) {
			// Lanza el servidor
				if (getNameProgram() == nameProgramService) {
					execServer();
				} else {
					mostrarMenu();
				}
			} else {
				mostrarMenu();
			}
		} else {
			mostrarMenu();
		}
	}
}

void Menu::checkPort() {
	resetPortVariables();

	resultCommand = getResultOfCommand(ufwCommand);
	vector<string> resultCommandSplit = splitLineToLine(resultCommand);

	checkProgramPort(resultCommandSplit);	

}



void Menu::mostrarMenu() {
	

	while (true) {
		cout << "Opciones del menu:" << endl;
		cout << "1. Activar el servidor" << endl;
		cout << "2. Ver estado del servidor" << endl;
		cout << "3. Desactivar el servidor" << endl;
		// Remove opción de clave RSA --> Se pasa a usar clave AES de 256 bits
		cout << "4. Modificar clave RSA" << endl;
		cout << "5. Salir" << endl;
		cout << endl;
		cout << "Introduce el índice de la acción que desea realizar" << endl;
		cin >> userValue;

		if (userValue == "1") {
			// Activa el servidor
			checkServerRunning();
			if (!checkProgramServiceBool) {
				createServer();
			} else {
				cout << "el server ya está en funcionamiento"<<endl;
			// Error, el servidor ya está corriendo
			}
		} else if (userValue == "2") {
			checkPort();
			checkServerRunning();
			
			// Sleep para dar tiempo al servidor a ejecutar el comando 'checkServerRunning'
			std::this_thread::sleep_for(std::chrono::milliseconds(2000));

			if (checkProgramServiceBool) {
				cout << "El servidor se encuentra en funcionamiento" << endl;
			} else {
				cout << "El servidor no está en funcionamiento" << endl;
			}

			// ver el estado del servidor
			// Muestra estado de los puertos y si el servidor está funcionando
		} else if (userValue == "3") {
			
			bool checkProgramRunning = false;

			checkServerRunning();

			if (checkProgramServiceBool) {
				checkProgramRunning = true;
			} else {
				checkProgramRunning = false;
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(2000));

			// Desactivar el servidor
			execCommand(killServerProcess);

			// Retardo de dos segundos para dar tiempo al servidor a ejecutar el comando indicado
			std::this_thread::sleep_for(std::chrono::milliseconds(2000));

			checkServerRunning();

			if (!checkProgramServiceBool && checkProgramRunning) {
				cout << "El servidor ha sido detenido satisfactoriamente" << endl;
				cout << endl;

			} else if (!checkProgramServiceBool && !checkProgramRunning){
				cout << "El servidor no está en funcionamiento" << endl;
			} else {
				cout << "El servidor no se ha podido detener" << endl;
			}

		} else if (userValue == "4") {
			// Modificar la clave RSA
			generateRSAKeys();
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
				cout << "Estado del puerto del servidor: Encontrado y abierto"<< endl;
				checkProgramPortOpenOrClose = true;
				break;
			} else if (vectorParam[counterLines].find(statusPortClose) != string::npos) {
				cout << "Estado del puerto del servidor: Encontrado, pero cerrado" << endl;
				break;
			}
			break;
		}
	}

	if (!checkProgramPortBool) {
		cout << "Estado del puerto del servidor: Puerto no encontrado" << endl;
	}
}

void Menu::resetPortVariables() {
	checkProgramPortBool = false;
	checkProgramPortOpenOrClose = false;
}

void Menu::checkServerRunning() {
	string programMenuName = getNameProgram();
	string menuExec = getResultOfCommand(listAllProcessCommand + programMenuName);

	if (menuExec != "") {
		// Se encuentra el menú en ejecucción
		checkMenuServer = true;
		string serviceRunning = getResultOfCommand(listAllProcessCommand + nameProgramService);
		if (serviceRunning != "") {
		// Lo ha encontrado, es decir, el servidor ya está corriendo
			checkProgramServiceBool = true;
		} else {
			// El servidor no está corriendo
			checkProgramServiceBool = false;
		}
	}
}

void Menu::createServer() {
	string programMenuName = getNameProgram();
	execCommand(commandCopyProgram);
	bool copyProgram = searchServiceProgram();

	// Busca el servicio hasta que este ha sido copiado, no se puede ejecutar si no se ha terminado de copiar antes
	while (!copyProgram) {
		copyProgram = searchServiceProgram();
	}

	checkPort();

	if (!checkProgramPortBool) {
		cout << "El puerto del servidor no se ha encontrado, asegurese de tener ufw instalado" << endl;
	} else if(!checkProgramPortOpenOrClose){
		cout << "El puerto del servidor se encuentra cerrado, no se pudo lanzar el servidor" << endl;
	} else {
		if (execCommand(commandLaunchService)) {
			// Se espera 2 segundos para dar tiempo al lanzamiento del servidor
			std::this_thread::sleep_for(std::chrono::milliseconds(2000));
			checkServerRunning();
			if (checkProgramServiceBool) {
				cout << "El servidor se lanzó correctamente" << endl;
			} else {
				cout << "Hubo algún fallo al lanzar el servidor" << endl;
			}
		} else {
			cout << "Hubo algún fallo al lanzar el servidor" << endl;
		}
	}
}

void Menu::execServer() {
	MasterSocket t;
}

void Menu::generateRSAKeys() {
	bool checkRSAKey = false;
	string allElementsOnDirectory = getResultOfCommand(lsCommand);
	vector <string> elementsFound = splitLineToLine(allElementsOnDirectory);
	for (int i = 0; i < elementsFound.size(); i++) {
		if (elementsFound[i].find(RSAKeyName) != string::npos || elementsFound[i].find(RSAKeyName) != string::npos) {
			execCommand(removeRSAKey);
			execCommand(removeRSAKey + ".pub");
			// Ejecuta creación de clave RSA
			std::this_thread::sleep_for(std::chrono::milliseconds(2000));
			execCommand(createRSAKeys);
			std::this_thread::sleep_for(std::chrono::milliseconds(2000));

			// Comprobación de existencia de clave AQUI


			// Lectura clave RSA publica y privada
			checkRSAKey = true;
			string privateKey = getResultOfCommand(readPrivateKey);
			vector <string> linesOfPrivateKey = splitLineToLine(privateKey);
			string privateKeyData = "";
			for (int pkLines = 0; pkLines < linesOfPrivateKey.size(); pkLines++) {
				if (pkLines != 0 && pkLines != linesOfPrivateKey.size() - 1) {
					privateKeyData += linesOfPrivateKey[pkLines];
				}
			}

			std::stringstream ss;
			ss << "mysql -u root -puserpass test -e " << '"' << "UPDATE testeo SET userName = '"<< privateKeyData <<"' " << "WHERE id = '1'" << '"' << ";";
			std::string s = ss.str();
			cout << s << endl;

			// AQUI
			
			//execCommand("mysql -u root -puserpass test -e " << "UPDATE testeo ");
			break;
		}
	}
	if (!checkRSAKey) {
		execCommand(createRSAKeys);
	}

}

bool Menu::execCommand(string commandParam) {
	try {
		FILE* stream;
		const int max_buffer = 256;
		char buffer[max_buffer];
		commandParam.append(" 2>&1");

		stream = popen(commandParam.c_str(), "r");
		return true;
	} catch (...) {
		return false;
	}
}

bool Menu::countServerProcess() {
	string serviceRunning = getResultOfCommand(listAllProcessCommand + nameProgramService);
	vector <string> servicesFound = splitLineToLine(serviceRunning);
	if (servicesFound.size() == sizeOfOneServerService) {
	// Solo un proceso servidor abierto, lanza servidor
		return true;
	} else {
	// Más de un proceso servidor abierto, sale del programa
		return false;
	}
}

bool Menu::searchServiceProgram() {
	string resultSearch = getResultOfCommand(lsCommand);
	vector<string> resultSearchSplit = splitLineToLine(resultSearch);
	bool foundValue = false;

	for (int lsCounter = 0; lsCounter < resultSearchSplit.size(); lsCounter++) {
		if (resultSearchSplit[lsCounter].find(nameProgramService) != string::npos) {
			foundValue = true;
			break;
		} else {
			foundValue = false;
		}
	}
	return foundValue;
}

string Menu::getNameProgram() {
	ifstream comm("/proc/self/comm");
	string name;
	getline(comm, name);
	return name;
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
