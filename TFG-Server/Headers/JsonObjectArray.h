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


class JsonObjectArray {
private:
	
public:
	string A_Title;
	vector <string> B_Content;

	JsonObjectArray(string titleParam, vector<string> contentParam);
	JsonObjectArray();
};