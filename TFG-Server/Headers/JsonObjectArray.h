/**********************************************************************************************//**
 * @file	TFG-Server\Headers\JsonObjectArray.h.
 *
 * @brief	Declaración de la clase JsonObjectArray.\n
 * 			Declares the JSON object array class.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * @class	JsonObjectArray
 *
 * @brief	Modelo de datos JSON con título y lista de elementos.\n
 * 			Model of JSON data with title and list with some datas.
 *
 * @author	Javier Fernández Fernández
 * @date	22/04/2020
 **************************************************************************************************/

class JsonObjectArray {

private:
	
public:
	string A_Title;
	vector <string> B_Content;

	/**********************************************************************************************//**
	 * @fn	JsonObjectArray::JsonObjectArray(string titleParam, vector<string> contentParam);
	 *
	 * @brief	Constructor de la clase.\n
	 * 			Constructor of the class.
	 *
	 * @author	Javier Fernández Fernández
	 * @date	22/04/2020
	 *
	 * @param 	titleParam, string  	Título del objeto.\n
	 * 									Title of the object.
	 * @param 	contentParam, vector<string>	Contenido de la lista.\n
	 * 											All data of list.
	 **************************************************************************************************/

	JsonObjectArray(string titleParam, vector<string> contentParam);

	/**********************************************************************************************//**
	 * @fn	JsonObjectArray::JsonObjectArray();
	 *
	 * @brief	Constructor por defecto de la clase.\n
	 * 			Default constructor.
	 *
	 * @author	Javier Fernández Fernández
	 * @date	22/04/2020
	 **************************************************************************************************/

	JsonObjectArray();
};
