
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include "../Headers/MasterSocket.h"
using namespace std;

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
	Menu();

	void mostrarMenu();
	void checkPort();
	void checkProgramPort(vector <string> vectorParam);
	void resetPortVariables();
	void checkServerRunning();
	void createServer();
	void execServer();

	bool execCommand(string command);
	bool countServerProcess();
	bool searchServiceProgram();

	string getNameProgram();
	vector<string> splitLineToLine(string sentence);

	string getResultOfCommand(string cmdCommand);
};
