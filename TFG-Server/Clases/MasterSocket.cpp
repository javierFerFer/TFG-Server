
//todos los include de la clase
#include "../Headers/MasterSocket.h"

using namespace std;

#define PORT 12345

/*
Main del programa

*/

MasterSocket::MasterSocket() {
		int server_fd, new_socket, valread;
		struct sockaddr_in address;
		int opt = 1;
		int addrlen = sizeof(address);
		char buffer[1024] = { 0 };
		char* hello = "Hola, vengo desde el servidor";

		// Creating socket file descriptor
		if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
			perror("socket failed");
			exit(EXIT_FAILURE);
		}

		// Forcefully attaching socket to the port 8080
		if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
			&opt, sizeof(opt))) {
			perror("setsockopt");
			exit(EXIT_FAILURE);
		}
		address.sin_family = AF_INET;
		address.sin_addr.s_addr = INADDR_ANY;
		address.sin_port = htons(PORT);

		// Forcefully attaching socket to the port 8080
		if (bind(server_fd, (struct sockaddr*) & address,
			sizeof(address)) < 0) {
			perror("bind failed");
			exit(EXIT_FAILURE);
		}

		// 10 = numero de conexiones a la espera de ser procesadas (Agregar numero alto)
		if (listen(server_fd, 10) < 0) {
			perror("listen");
			exit(EXIT_FAILURE);
		}

		while (true) {
		if ((new_socket = accept(server_fd, (struct sockaddr*) & address,
			(socklen_t*)&addrlen)) < 0) {
			perror("accept");
			exit(EXIT_FAILURE);
		}

		// Objeto hilo conexión cliente
		//vector <thread>* temporalList = getAllSockets();
		thread threadSocketObject(&SocketObject::launchReadThread, SocketObject(address, new_socket, allSockets));

		// Asigna la ejecución del hilo como uno independiente al programa en sí
		//cout << threadSocketObject.get_id() << endl;
		
		//threadSocketObject.detach();
		//printf("Got connection from: %s:%d\n", inet_ntoa(address.sin_addr), address.sin_port);
		mutex mtx;
		unique_lock<mutex> lock(mtx);
		
		allSockets.push_back(move(threadSocketObject));
		cout << allSockets.size()<<endl;
		
		
		
		//printf("Got connection from: %s:%d\n", inet_ntoa(allSockets[i].getClientSocket().sin_addr), allSockets[i].getClientSocket().sin_port);
	}
	

		// Tanto la lectura como escritura de datos en sockets en c++ funciona con bytes
		//valread = read(new_socket, buffer, 1024);
		//printf("%s\n", buffer);
		//send(new_socket, hello, strlen(hello), 0);

		// Test para que no se cierre la consola
		//int answerUserNumber;
		//cout << "Introduce el nombre del perro" << endl;
		//cin >> answerUserNumber;
	
}
