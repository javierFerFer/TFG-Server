#include "../Headers/Main.h"
#include "../Headers/SocketObject.h"
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

class MasterSocket {
	private:
		vector <SocketObject> allSockets;
	public:
		MasterSocket();
};
