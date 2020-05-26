/**********************************************************************************************//**
 * @file	TFG-Server\Headers\MasterSocket.h.
 *
 * @brief	Declaración de la clase MasterSocket.\n
 * 			Declaration of MasterSocket class, this class takes care of all connections.
 **************************************************************************************************/

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
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>


#define _BSD_SOURCE

using namespace std;

/**********************************************************************************************//**
 * @class	MasterSocket
 *
 * @brief	Clase masterSocket, encargada de la gestión de las conexiones entrantes.\n
 * 			MasterSocket class, this class takes care of all connections.
 *
 * @author	Javier Fernández Fernández
 * @date	22/04/2020
 **************************************************************************************************/

class MasterSocket {
	private:

	public:

		/**********************************************************************************************//**
		 * @fn	MasterSocket::MasterSocket();
		 *
		 * @brief	Constructor por defecto de la clase.\n
		 * 			Default constructor.
		 *
		 * @author	Javier Fernández Fernández
		 * @date	22/04/2020
		 **************************************************************************************************/

		MasterSocket();

		vector <thread> allSockets;

};
