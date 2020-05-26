/**********************************************************************************************//**
 * @file	TFG-Server\Headers\SocketObject.h.
 *
 * @brief	Declaración de la clase SocketObject.\n
 * 			Declares the socket object class.
 **************************************************************************************************/

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
#include <math.h>

#include <hpdf.h>

#include <stdio.h>
#include <wchar.h>
#include <locale.h>
#include <stdlib.h>

#include <podofo/podofo.h>

#include "../Headers/DataBaseConnect.h"



#define _BSD_SOURCE

using namespace std;

/**********************************************************************************************//**
 * @class	SocketObject
 *
 * @brief	Clase SocketObject, gestiona cada hilo de conexión al servidor.\n
 * 			SocketObject class, manages each connection thread to the server.
 *
 * @author	Javier Fernández Fernández
 * @date	22/04/2020
 **************************************************************************************************/

class SocketObject{
private:
	sockaddr_in clientSocket;
	int sendReceiveDataSocket;
	int timeOut = 0;
	int valread;
	int maxTimeOut = 180;
	char buffer[12024] = { 0 };
	string passwd;
	string ivString = "0123456789123456";
	string errMsg = "Error al crear el examen";
	bool checkTimeOut = true;
	bool checkThreadFunction = true;
	DataBaseConnect dataBaseConnection;
	Poco::Crypto::Cipher* pCipher;

public:

	vector <thread>& allSockets;

	/**********************************************************************************************//**
	 * @fn	SocketObject::SocketObject(sockaddr_in address, int sendReceiveData, vector <thread>& allSocketsParam)
	 *
	 * @brief	Constructor de la clase.\n
	 * 			Constructor of the class.
	 *
	 * @author	Javier Fernández Fernández
	 * @date	22/04/2020
	 *
	 * @param 		  	address, sockaddr_in	Estructura de socket.\n
	 * 											Structure of socket.	   	
	 * @param 		  	sendReceiveData, int	Valor del socket --> 0 = no funciona, 1 = funcionando.\n
	 * 											Value of socket --> 0 = don't work, 1 = working.
	 * @param [in,out]	allSocketsParam, vector<thread>	Hilo socket.\n
	 * 													socket thread.
	 **************************************************************************************************/

	SocketObject(sockaddr_in address, int sendReceiveData, vector <thread>& allSocketsParam) :clientSocket(address), sendReceiveDataSocket(sendReceiveData), allSockets(allSocketsParam) {

	};

	/**********************************************************************************************//**
	 * @fn	void SocketObject::launchReadThread();
	 *
	 * @brief	Inicia el hilo para procesar los datos de la conexión.\n
	 * 			Initialize the thread to process al data of connection.
	 *
	 * @author	Javier Fernández Fernández
	 * @date	22/04/2020
	 **************************************************************************************************/

	void launchReadThread();

	/**********************************************************************************************//**
	 * @fn	void SocketObject::timeOutData();
	 *
	 * @brief	Comprueba la inactividad del usuario, si el usuario pasa el límite establecido, se cierra la conexión.\n
	 * 			Check the inactivity of user, if the user pass the limit of the time, his connection will be closed.
	 *
	 * @author	Javier Fernández Fernández
	 * @date	22/04/2020
	 **************************************************************************************************/

	void timeOutData();

	/**********************************************************************************************//**
	 * @fn	void SocketObject::removeThread(thread::id id);
	 *
	 * @brief	Borra el hilo concurrente de la lista de conexiones en base a su ID.\n
	 * 			Delete concurrent thread of list with all connections based on his ID.
	 *
	 * @author	Javier Fernández Fernández
	 * @date	22/04/2020
	 *
	 * @param 	id, Thread::id	ID del hilo.\n
	 * 							ID of thread.
	 **************************************************************************************************/

	void removeThread(thread::id id);

	/**********************************************************************************************//**
	 * @fn	void SocketObject::spawn();
	 *
	 * @brief	Lanza el hilo TimeOut como hilo independiente.\n
	 * 			Throw timeOut thread.
	 *
	 * @author	Javier Fernández Fernández
	 * @date	22/04/2020
	 **************************************************************************************************/

	void spawn();

	/**********************************************************************************************//**
	 * @fn	void SocketObject::sendSigleMessage(string titleParam, string messageParam, Poco::Crypto::Cipher *cipherParam);
	 *
	 * @brief	Permite enviar un objeto JsonSingleData encriptado en AES 256-CBC.\n
	 * 			Allow to send JsonSingleData object encrypted with AES-256-CBC key.
	 *
	 * @author	Javier Fernández Fernández
	 * @date	22/04/2020
	 *
	 * @param 		  	titleParam, string  	Título del objeto.\n
	 * 											Title of the object.
	 * @param 		  	messageParam, string	Mensaje del objecto.\n
	 * 											Message of the object.
	 * @param [in,out]	cipherParam, Poco::Crypto::Cipher 	Si no es nulo, el objeto Cipher usado en la encriptación.\n
	 * 														If non-null, the cipher object used to encrypt.
	 **************************************************************************************************/

	void sendSigleMessage(string titleParam, string messageParam, Poco::Crypto::Cipher *cipherParam);

	/**********************************************************************************************//**
	 * @fn	void SocketObject::sendMoreSingleDataMessage(string titleParam, vector<string>messageParam, Poco::Crypto::Cipher* cipherParam);
	 *
	 * @brief	Permite enviar un objeto JsonObjectArray encriptado en AES 256-CBC.\n
	 * 			Allow to send JsonObjectArray object encrypted with AES-256-CBC key.
	 *
	 * @author	Javier Fernández Fernández
	 * @date	22/04/2020
	 *
	 * @param 		  	titleParam, string 	Título del objeto.\n
	 * 										Title of the object.
	 * @param 		  	parameter2, vector<string> 	lista de datos del objeto.\n
	 * 												list of data of the object.
	 * @param [in,out]	cipherParam, Poco::Crypto::Cipher	Si no es nulo, el objeto Cipher usado en la encriptación.\n
	 * 														If non-null, the cipher object used to encrypt.
	 **************************************************************************************************/

	void sendMoreSingleDataMessage(string titleParam, vector<string>messageParam, Poco::Crypto::Cipher* cipherParam);

	/**********************************************************************************************//**
	 * @fn	void SocketObject::generateNormalExam(vector <string> allQuestions);
	 *
	 * @brief	Permite la generación de examenes de tipo normal.\n
	 * 			Allow generate normal exams.
	 *
	 * @author	Javier Fernández Fernández
	 * @date	22/04/2020
	 *
	 * @param 	allQuestions, vector<string>	Todas las preguntas.\n
	 * 											All normal questions.
	 **************************************************************************************************/

	void generateNormalExam(vector <string> allQuestions);

	/**********************************************************************************************//**
	 * @fn	void SocketObject::generateTestExam(vector <string> allQuestions, bool generateWithNotAnswer);
	 *
	 * @brief	Permite generar exámenes de tipo test.\n
	 * 			Allow to generate test exams.
	 *
	 * @author	Javier Fernández Fernández
	 * @date	22/04/2020
	 *
	 * @param 	allQuestions, vector<string>		 	Todas las preguntas.\n
	 * 													All questions.
	 * @param 	generateWithNotAnswer, bool	True si se genera el examen con las respuestas de las preguntas, false en caso contrario.\n
	 * 										True if the exam contains the answer of the questions, false in the opposite case.
	 **************************************************************************************************/

	void generateTestExam(vector <string> allQuestions, bool generateWithNotAnswer);

	/**********************************************************************************************//**
	 * @fn	string SocketObject::generatePasswd();
	 *
	 * @brief	Genera la clase AES-256-CBC de 32 dígitos usada en la comunicación cliente-servidor.\n
	 * 			Generate AES-256-CBC key with 32 digits that is used in the client-server communication
	 *
	 * @author	Javier Fernández Fernández
	 * @date	22/04/2020
	 *
	 * @returns	string, la contraseña.\n
	 * 					The password.
	 **************************************************************************************************/

	string generatePasswd();

	/**********************************************************************************************//**
	 * @fn	bool SocketObject::getResultOfCommands(string consoleCommand);
	 *
	 * @brief	Obtiene el resultado de ejecutar un comando.\n
	 * 			Get result of execute a command.
	 *
	 * @author	Javier Fernández Fernández
	 * @date	22/04/2020
	 *
	 * @param 	cmdCommand, string	Comando.\n
	 * 								Command.
	 *
	 * @returns string, El resultado de la ejecución del comando.\n
	 * 					The result of execute this command.
	 **************************************************************************************************/

	bool getResultOfCommands(string consoleCommand);

	/**********************************************************************************************//**
	 * @fn	vector<string> SocketObject::splitLineToLine(string resultOfCommand);
	 *
	 * @brief	Genera un listado de linneas obtenidas al ejecutar un comando.\n
	 * 			Generate a list of lines obtained by executing a command.
	 *
	 * @author	Javier Fernández Fernández
	 * @date	22/04/2020
	 *
	 * @param 	sentence, string Resultado de ejecutar un comando.\n
	 * 							 Result to execute a command.
	 *
	 * @returns	vector<string>, listado de líneas obtenidas.\n
	 * 							List of lines.
	 **************************************************************************************************/

	vector<string> splitLineToLine(string resultOfCommand);
};

