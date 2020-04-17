#include "../Headers/Main.h"
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sstream>
#include <vector>
#include <iostream>
#include <algorithm>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <Poco/Foundation.h>
#include <Poco/Crypto/Cipher.h>
#include <Poco/Crypto/CipherKey.h>
#include <Poco/Crypto/CipherFactory.h>
#include <openssl/crypto.h>
#include <thread>
#include <mutex>



#define _BSD_SOURCE

using namespace std;

class SocketObject{
private:
	sockaddr_in clientSocket;
	int sendReceiveDataSocket;
	int timeOut = 0;
	int valread;
	int maxTimeOut = 18;
	char buffer[1024] = { 0 };
	string passwd;
	string ivString = "0123456789123456";
	bool checkTimeOut = true;

public:
	vector <thread>& allSockets;
	// Constructor modificado
	SocketObject(sockaddr_in address, int sendReceiveData, vector <thread>& allSocketsParam) :clientSocket(address), sendReceiveDataSocket(sendReceiveData), allSockets(allSocketsParam) {

	};


	void launchReadThread();
	void timeOutData();
	void removeThread(thread::id id);
	void spawn();
	void sendMessage(string titleParam, string messageParam, Poco::Crypto::Cipher *cipherParam);

	string generatePasswd();
};

