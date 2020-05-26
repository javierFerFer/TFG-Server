/**********************************************************************************************//**
 * @file	TFG-Server\Headers\Menu.h.
 *
 * @brief	Declaración de la clase menu.\n
 * 			Declaration on menu class.
 **************************************************************************************************/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include "../Headers/MasterSocket.h"
using namespace std;

/**********************************************************************************************//**
 * @class	Menu
 *
 * @brief	Menu del panel de control del servidor.\n
 * 			Menu of admin panel in server.
 *
 * @author	Javier Fernández Fernández
 * @date	22/04/2020
 **************************************************************************************************/

class Menu {

private:
	const string port = "12345";
	const string RSAKeyName = "test";
	const string RSAKeyNamePub = "test.pub";
	const string removeRSAKey = "rm -r test";
	const string createRSAKeys = "ssh-keygen -b 2048 -t rsa -f " + RSAKeyName + " -q -N ''";
	const string readPrivateKey = "cat " + RSAKeyName;
	const string readPublicKey = "nano " + RSAKeyNamePub;
	const string statusPortOpen = "ALLOW";
	const string statusPortClose = "DENY";
	const string ufwCommand = "ufw status";
	const string lsCommand = "ls";
	const string nameProgramService = "TFG-back.out";
	const string listAllProcessCommand = "pgrep ";
	const string callAsService = "&";
	const string clearConsoleCommand = "clear";
	const string killServerProcess = "killall -s SIGKILL " + nameProgramService;
	const string commandCopyProgram = "cp " + getNameProgram() + " " + nameProgramService;
	const string commandLaunchService = "./" + nameProgramService + callAsService;

	const string banner = R"(
   _____       ______        _____                                                           
  / ____|     |  ____|      / ____|                                                          
 | (___       | |__        | |                                                               
  \___ \      |  __|       | |                                                               
  ____) |  _  | |____   _  | |____   _                                                        
 |_____/  (_) |______| (_)  \_____| (_)                                                       
                                                                                             
                                                                                             
             _____    __  __   _____   _   _     _____               _   _   ______   _      
     /\     |  __ \  |  \/  | |_   _| | \ | |   |  __ \      /\     | \ | | |  ____| | |     
    /  \    | |  | | | \  / |   | |   |  \| |   | |__) |    /  \    |  \| | | |__    | |     
   / /\ \   | |  | | | |\/| |   | |   | . ` |   |  ___/    / /\ \   | . ` | |  __|   | |     
  / ____ \  | |__| | | |  | |  _| |_  | |\  |   | |       / ____ \  | |\  | | |____  | |____ 
 /_/    \_\ |_____/  |_|  |_| |_____| |_| \_|   |_|      /_/    \_\ |_| \_| |______| |______|

)";

	const string errorMessagePort = "No se ha podido encontrar el puerto del programa, asegurese de tener ufw configurado en su servidor";
	const string statusPortOpenMessage = "Estado del puerto del servidor: Encontrado y abierto";
	const string statusPortClosedMessage = "Estado del puerto del servidor: Encontrado, pero cerrado";
	const string errorPortClosedMessage = "El puerto del servidor se encuentra cerrado, no se pudo lanzar el servidor";
	const string statusPortNotFound = "Estado del puerto del servidor: Puerto no encontrado";
	const string errorMessageUFW = "El puerto del servidor no se ha encontrado, asegurese de tener ufw instalado";
	const string exectServerSuccesfully = "El servidor se lanzo correctamente";
	const string unknownErrorServer = "Hubo algun fallo al lanzar el servidor";
	const string messageServerRunningNow = "el servidor ya esta en funcionamiento";
	const string serverIsRunning = "El servidor se encuentra en funcionamiento";
	const string serverIsNotRunning = "El servidor no esta en funcionamiento";
	const string serverStopSuccess = "El servidor ha sido detenido satisfactoriamente";
	const string serverStopFail = "El servidor no se ha podido detener";
	const string invalidCaracter = "Ha introducido un valor invalido";
	const string serverNotClosed = "El servidor no ha sido cerrado";

	const int sizeOfOneServerService = 1;

	string userValue;
	string resultCommand;

	bool checkProgramPortBool = false;
	bool checkProgramPortOpenOrClose = false;
	bool checkProgramServiceBool = false;
	bool checkMenuServer = false;

public:

	/**********************************************************************************************//**
	 * @fn	Menu::Menu();
	 *
	 * @brief	Constructor por defecto de la clase.\n
	 * 			Default constructor of the class.
	 *
	 * @author	Javier Fernández Fernández
	 * @date	22/04/2020
	 **************************************************************************************************/

	Menu();

	/**********************************************************************************************//**
	 * @fn	void Menu::mostrarMenu();
	 *
	 * @brief	Muestra el menú de opciones al administrador.\n
	 * 			Show all menu options.
	 *
	 * @author	Javier Fernández Fernández
	 * @date	22/04/2020
	 **************************************************************************************************/

	void mostrarMenu();

	/**********************************************************************************************//**
	 * @fn	void Menu::checkPort();
	 *
	 * @brief	Comprueba si el puerto que usa el programa está abierto.\n
	 * 			Check if port that server use is open or closed.
	 *
	 * @author	Javier Fernández Fernández
	 * @date	22/04/2020
	 **************************************************************************************************/

	void checkPort();

	/**********************************************************************************************//**
	 * @fn	void Menu::checkProgramPort(vector <string> vectorParam);
	 *
	 * @brief	Comprueba si el puerto que usa el programa está abierto.\n
	 * 			Check if port that server use is open or closed.
	 *
	 * @author	Javier Fernández Fernández
	 * @date	22/04/2020
	 *
	 * @param 	vectorParam, vector<string> Vector con las líneas obtenidas al ejecutar el comando que obtiene la lista de puertos del servidor.\n
	 * 										List with all lines when program execute a command to get a list with all ports in this server.
	 **************************************************************************************************/

	void checkProgramPort(vector <string> vectorParam);

	/**********************************************************************************************//**
	 * @fn	void Menu::resetPortVariables();
	 *
	 * @brief	Resetea las variables de comprobación de puertos.\n
	 * 			Reset all check port variables.
	 *
	 * @author	Javier Fernández Fernández
	 * @date	22/04/2020
	 **************************************************************************************************/

	void resetPortVariables();

	/**********************************************************************************************//**
	 * @fn	void Menu::checkServerRunning();
	 *
	 * @brief	Comprueba si el servidor está funcionando.\n
	 * 			Check if server is running.
	 *
	 * @author	Javier Fernández Fernández
	 * @date	22/04/2020
	 **************************************************************************************************/

	void checkServerRunning();

	/**********************************************************************************************//**
	 * @fn	void Menu::createServer();
	 *
	 * @brief	Crea el servidor.\n
	 * 			Create the server.
	 *
	 * @author	Javier Fernández Fernández
	 * @date	22/04/2020
	 **************************************************************************************************/

	void createServer();

	/**********************************************************************************************//**
	 * @fn	void Menu::execServer();
	 *
	 * @brief	Ejecuta el servidor.\n
	 * 			Execute the server.
	 *
	 * @author	Javier Fernández Fernández
	 * @date	22/04/2020
	 **************************************************************************************************/

	void execServer();

	/**********************************************************************************************//**
	 * @fn	bool Menu::execCommand(string command);
	 *
	 * @brief	Permite ejecutar comandos de consola.\n
	 * 			Allow to excecute command lines.
	 *
	 * @author	Javier Fernández Fernández
	 * @date	22/04/2020
	 *
	 * @param 	command, string Comando a ejecutar.\n
	 * 							Command to execute.
	 *
	 * @returns	True si lo ejecuta correctamente, en caso contrario false.\n
	 * 			True if execute successfully, in the opposite case, false.
	 **************************************************************************************************/

	bool execCommand(string command);

	/**********************************************************************************************//**
	 * @fn	bool Menu::countServerProcess();
	 *
	 * @brief	Cuenta el número de servicios del sistema.\n
	 * 			Count server process.
	 *
	 * @author	Javier Fernández Fernández
	 * @date	22/04/2020
	 *
	 * @returns	True si lo ejecuta correctamente, en caso contrario false.\n
	 * 			True if execute successfully, in the opposite case, false.
	 **************************************************************************************************/

	bool countServerProcess();

	/**********************************************************************************************//**
	 * @fn	bool Menu::searchServiceProgram();
	 *
	 * @brief	Busca el servicio del servidor en los procesos del sistema.\n
	 * 			Search server(this program) process in all process of the server.
	 *
	 * @author	Javier Fernández Fernández
	 * @date	22/04/2020
	 *
	 * @returns	True si lo encuentra, false si no.\n
	 * 			True if found, false if not.
	 **************************************************************************************************/

	bool searchServiceProgram();

	/**********************************************************************************************//**
	 * @fn	string Menu::getNameProgram();
	 *
	 * @brief	Obtiene el nombre del proceso de ejecución del programa.\n
	 * 			Get the name of process of this program.
	 *
	 * @author	Javier Fernández Fernández
	 * @date	22/04/2020
	 *
	 * @returns	string, El nombre del proceso.\n
	 * 					The name of process.
	 **************************************************************************************************/

	string getNameProgram();

	/**********************************************************************************************//**
	 * @fn	vector<string> Menu::splitLineToLine(string sentence);
	 *
	 * @brief	Genera un listado de linneas obtenidas al ejecutar un comando.\n
	 * 			Generate a list of lines obtained by executing a command.
	 *
	 * @author	Javier Fernández Fernández
	 * @date	22/04/2020
	 *
	 * @param 	sentence, string Resultado de ejecutar un comando.\n
	 * 							 Result to execute a command.
	 *
	 * @returns	vector<string>, listado de líneas obtenidas.\n
	 * 							List of lines.
	 **************************************************************************************************/

	vector<string> splitLineToLine(string sentence);

	/**********************************************************************************************//**
	 * @fn	string Menu::getResultOfCommand(string cmdCommand);
	 *
	 * @brief	Obtiene el resultado de ejecutar un comando.\n
	 * 			Get result of execute a command.
	 *
	 * @author	Javier Fernández Fernández
	 * @date	22/04/2020
	 *
	 * @param 	cmdCommand, string	Comando.\n
	 * 								Command.
	 *
	 * @returns string, El resultado de la ejecución del comando.\n
	 * 					The result of execute this command.
	 **************************************************************************************************/

	string getResultOfCommand(string cmdCommand);
};
