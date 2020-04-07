//todos los include de la clase
#include "../Headers/Main.h"
#include "../Headers/ThreadTest.h"
#include <chrono>
#include <thread>
#include <iostream>
#include <thread>
using namespace std;

ThreadTest::ThreadTest(string param) {
	nameThread = param;
}

void ThreadTest::mostrar() {
	int foo;
	if (nameThread == "hilo 1") {
		std::this_thread::sleep_for(std::chrono::milliseconds(40000));
	}
	for (size_t i = 0; i < 10; i++) {
		cout << "Soy el hilo " + nameThread << endl;
	}
}
