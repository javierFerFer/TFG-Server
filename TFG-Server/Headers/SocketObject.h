#include "../Headers/Main.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <vector>
#include<iostream>
#include<iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define _BSD_SOURCE

using namespace std;

class SocketObject {
private:
	sockaddr_in clientSocket;
public:
	SocketObject(sockaddr_in address);
	sockaddr_in getClientSocket() const{
	return clientSocket;
	}

	void setClientSocket(sockaddr_in clientSocketParam) {
		this->clientSocket = clientSocketParam;
	}

};