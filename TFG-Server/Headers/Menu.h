
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Menu {

private:
	const string port = "12345";
	const string statusPortOpen = "ALLOW";
	const string statusPortClose = "DENY";
	const string ufwCommand = "ufw status";
	const string clearConsoleCommand = "clear";

	string userValue;
	string resultCommand;

	bool checkMenu = true;
	bool checkProgramPortBool = false;
	bool checkProgramServiceBool = false;

public:
	Menu();

	void mostrarMenu();
	void checkPort();
	vector<string> splitLineToLine(string sentence);
	void checkProgramPort(vector <string> vectorParam);

	string getResultOfCommand(string cmdCommand);
};
