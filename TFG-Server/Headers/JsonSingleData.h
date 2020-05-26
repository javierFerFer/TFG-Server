/**********************************************************************************************//**
 * @file	TFG-Server\Headers\JsonSingleData.h.
 *
 * @brief	Declaración de la clase JsonSingleData.\n
 * 			Declaration of JsonSingleData class.
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
 * @class	JsonSingleData
 *
 * @brief	Clase modelo de objeto JSON con un título y un solo mensaje.\n
 * 			Model class of JSON object with one title and one data.
 *
 * @author	Javier Fernández Fernández
 * @date	22/04/2020
 **************************************************************************************************/

class JsonSingleData {
private:

public:
	string A_Title;
	string B_Content;

	/**********************************************************************************************//**
	 * @fn	JsonSingleData::JsonSingleData(string titleParam, string contentParam);
	 *
	 * @brief	Constructor de la clase modelo.\n
	 * 			Constructor of model class.
	 *
	 * @author	Javier Fernández Fernández
	 * @date	22/04/2020
	 *
	 * @param 	titleParam, string  	Título.\n
	 * 									Title.
	 * @param 	contentParam, string	Contenido del mensaje.\n
	 * 									Content of message.
	 **************************************************************************************************/

	JsonSingleData(string titleParam, string contentParam);

	/**********************************************************************************************//**
	 * @fn	JsonSingleData::JsonSingleData();
	 *
	 * @brief	Constructor por defecto de la clase.\n
	 * 			Default constructor.
	 *
	 * @author	Javier Fernández Fernández
	 * @date	22/04/2020
	 **************************************************************************************************/

	JsonSingleData();
};
