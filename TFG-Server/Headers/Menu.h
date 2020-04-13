
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
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
	void generateRSAKeys();

	bool execCommand(string command);
	bool countServerProcess();
	bool searchServiceProgram();

	string getNameProgram();
	vector<string> splitLineToLine(string sentence);

	string getResultOfCommand(string cmdCommand);
};
