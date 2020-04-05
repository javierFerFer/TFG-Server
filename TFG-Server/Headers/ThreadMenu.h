#include <string>
using namespace std;

class ThreadMenu {

private:
	string userValue;
	bool checkMenu = true;

public:
	ThreadMenu();

	void mostrarMenu();
	string getResultOfCommand(string cmdCommand);
	void splitLineToLine(string sentence);
};
