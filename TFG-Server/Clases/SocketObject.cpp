#include "../Headers/SocketObject.h"

using namespace std;

SocketObject::SocketObject(sockaddr_in sockaddrParam) {
	clientSocket = sockaddrParam;
	cout << "hola" << endl;
}
