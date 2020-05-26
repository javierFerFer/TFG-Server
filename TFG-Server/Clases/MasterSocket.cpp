
#include "../Headers/MasterSocket.h"

using namespace std;

#define PORT 12345

MasterSocket::MasterSocket() {
		int server_fd, new_socket, valread;
		struct sockaddr_in address;
		int opt = 1;
		int addrlen = sizeof(address);
		char buffer[1024] = { 0 };

		// Creación del socket
		if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
			perror("socket failed");
			exit(EXIT_FAILURE);
		}

		// Se fuerza al socket a usar el puerto definido
		if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
			&opt, sizeof(opt))) {
			perror("setsockopt");
			exit(EXIT_FAILURE);
		}
		address.sin_family = AF_INET;
		address.sin_addr.s_addr = INADDR_ANY;
		address.sin_port = htons(PORT);

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
		thread threadSocketObject(&SocketObject::launchReadThread, SocketObject(address, new_socket, allSockets));

		mutex mtx;
		unique_lock<mutex> lock(mtx);
		allSockets.push_back(move(threadSocketObject));
	}
}
