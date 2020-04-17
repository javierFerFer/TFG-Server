#include "../Headers/Main.h"
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <sstream>
#include <vector>
#include<iostream>
#include <algorithm>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>
#include <mutex>
using namespace std;


class JsonSingleData {
private:

public:
	string A_Title;
	string B_Content;

	JsonSingleData(string titleParam, string contentParam);
	JsonSingleData();
};
