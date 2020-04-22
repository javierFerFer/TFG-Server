//todos los include de la clase
#include "../Headers/Main.h"
#include "../Headers/Menu.h"
#include <sstream>
#include <thread>

#define Crypto_CipherFactory_INCLUDED;

#ifndef _COLORS_

#define _COLORS_


/* FOREGROUND */

#define RST  "\x1B[0m"

#define KRED  "\x1B[31m"

#define KGRN  "\x1B[32m"

#define KYEL  "\x1B[33m"

#define KBLU  "\x1B[34m"

#define KMAG  "\x1B[35m"

#define KCYN  "\x1B[36m"

#define KWHT  "\x1B[37m"


#define FRED(x) KRED + x + RST

#define FGRN(x) KGRN + x + RST

#define FYEL(x) KYEL + x + RST

#define FBLU(x) KBLU + x + RST

#define FMAG(x) KMAG + x + RST

#define FCYN(x) KCYN + x + RST

#define FWHT(x) KWHT + x + RST


#define BOLD(x) "\x1B[1m" x RST

#define UNDL(x) "\x1B[4m" x RST


#endif  /* _COLORS_ */

/*
Main del programa

*/

// Constructor de la clase, se ejecuta al crear un objeto de la misma
Menu::Menu(){

	checkPort();

	if (checkProgramPortBool) {
		checkServerRunning();
	} else {
		if (!checkProgramPortBool) {
			system("clear");
			std::this_thread::sleep_for(std::chrono::milliseconds(300));
			cout << FRED(errorMessagePort) <<endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(4700));
			system("clear");
			std::this_thread::sleep_for(std::chrono::milliseconds(300));
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
		system("clear");
		std::this_thread::sleep_for(std::chrono::milliseconds(300));
		cout << FYEL(banner) << endl;
		cout << endl;
		cout << "Opciones del menu:" << endl;
		cout << "1. Activar el servidor" << endl;
		cout << "2. Ver estado del servidor" << endl;
		cout << "3. Desactivar el servidor" << endl;
		cout << "4. Salir" << endl;
		cout << endl;
		cout << "Introduce el indice de la accion que desea realizar" << endl;
		cin >> userValue;

		if (userValue == "1") {
			// Activa el servidor
			checkServerRunning();
			if (!checkProgramServiceBool) {
				createServer();
			} else {
				system("clear");
				std::this_thread::sleep_for(std::chrono::milliseconds(300));
				cout << FRED(messageServerRunningNow) <<endl;
				std::this_thread::sleep_for(std::chrono::milliseconds(4700));
				system("clear");
				std::this_thread::sleep_for(std::chrono::milliseconds(300));
			// Error, el servidor ya está corriendo
			}
		} else if (userValue == "2") {
			checkPort();
			checkServerRunning();
			
			// Sleep para dar tiempo al servidor a ejecutar el comando 'checkServerRunning'
			std::this_thread::sleep_for(std::chrono::milliseconds(2000));

			if (checkProgramServiceBool) {
				system("clear");
				std::this_thread::sleep_for(std::chrono::milliseconds(300));
				cout << FGRN(serverIsRunning) << endl;
				std::this_thread::sleep_for(std::chrono::milliseconds(4700));
				system("clear");
				std::this_thread::sleep_for(std::chrono::milliseconds(300));
			} else {
				system("clear");
				std::this_thread::sleep_for(std::chrono::milliseconds(300));
				cout << FRED(serverIsNotRunning) << endl;
				std::this_thread::sleep_for(std::chrono::milliseconds(4700));
				system("clear");
				std::this_thread::sleep_for(std::chrono::milliseconds(300));
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
				system("clear");
				std::this_thread::sleep_for(std::chrono::milliseconds(300));
				cout << FGRN(serverStopSuccess) << endl;
				std::this_thread::sleep_for(std::chrono::milliseconds(4700));
				system("clear");
				std::this_thread::sleep_for(std::chrono::milliseconds(300));

			} else if (!checkProgramServiceBool && !checkProgramRunning){
				system("clear");
				std::this_thread::sleep_for(std::chrono::milliseconds(300));
				cout << FRED(serverIsNotRunning) << endl;
				std::this_thread::sleep_for(std::chrono::milliseconds(4700));
				system("clear");
				std::this_thread::sleep_for(std::chrono::milliseconds(300));
			} else {			
				system("clear");
				std::this_thread::sleep_for(std::chrono::milliseconds(300));
				cout << FRED(serverStopFail) << endl;
				std::this_thread::sleep_for(std::chrono::milliseconds(4700));
				system("clear");
				std::this_thread::sleep_for(std::chrono::milliseconds(300));
			}

		} else if (userValue == "4") {
			exit(0);
		} else {
			// Error de valor introducido
			system("clear");
			std::this_thread::sleep_for(std::chrono::milliseconds(300));
			cout << FRED(invalidCaracter) << endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(4700));
			system("clear");
			std::this_thread::sleep_for(std::chrono::milliseconds(300));
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
				system("clear");
				std::this_thread::sleep_for(std::chrono::milliseconds(300));
				cout << FGRN(statusPortOpenMessage)<< endl;
				std::this_thread::sleep_for(std::chrono::milliseconds(4700));
				system("clear");
				std::this_thread::sleep_for(std::chrono::milliseconds(300));
				checkProgramPortOpenOrClose = true;
				break;
			} else if (vectorParam[counterLines].find(statusPortClose) != string::npos) {
				system("clear");
				std::this_thread::sleep_for(std::chrono::milliseconds(300));
				cout << FYEL(statusPortClosedMessage) << endl;
				std::this_thread::sleep_for(std::chrono::milliseconds(4700));
				system("clear");
				std::this_thread::sleep_for(std::chrono::milliseconds(300));
				break;
			}
			break;
		}
	}

	if (!checkProgramPortBool) {
		system("clear");
		std::this_thread::sleep_for(std::chrono::milliseconds(300));
		cout << FRED(statusPortNotFound) << endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(4700));
		system("clear");
		std::this_thread::sleep_for(std::chrono::milliseconds(300));
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
		system("clear");
		std::this_thread::sleep_for(std::chrono::milliseconds(300));
		cout << FRED(errorMessageUFW) << endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(4700));
		system("clear");
		std::this_thread::sleep_for(std::chrono::milliseconds(300));
	} else if(!checkProgramPortOpenOrClose){
		system("clear");
		std::this_thread::sleep_for(std::chrono::milliseconds(300));
		cout << FRED(errorPortClosedMessage) << endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(4700));
		system("clear");
		std::this_thread::sleep_for(std::chrono::milliseconds(300));
	} else {
		if (execCommand(commandLaunchService)) {
			// Se espera 2 segundos para dar tiempo al lanzamiento del servidor
			std::this_thread::sleep_for(std::chrono::milliseconds(2000));
			checkServerRunning();
			if (checkProgramServiceBool) {
				system("clear");
				std::this_thread::sleep_for(std::chrono::milliseconds(300));
				cout << FGRN(exectServerSuccesfully) << endl;
				std::this_thread::sleep_for(std::chrono::milliseconds(4700));
				system("clear");
				std::this_thread::sleep_for(std::chrono::milliseconds(300));
			} else {
				system("clear");
				std::this_thread::sleep_for(std::chrono::milliseconds(300));
				cout << FRED(unknownErrorServer) << endl;
				std::this_thread::sleep_for(std::chrono::milliseconds(4700));
				system("clear");
			}
		} else {
			system("clear");
			std::this_thread::sleep_for(std::chrono::milliseconds(300));
			cout << FRED(unknownErrorServer) << endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(4700));
			system("clear");
			std::this_thread::sleep_for(std::chrono::milliseconds(300));
		}
	}
}

void Menu::execServer() {
	MasterSocket t;
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
