//todos los include de la clase
#include "../Headers/Main.h"
#include "../Headers/ThreadTest.h"
#include <iostream>
#include <thread>
using namespace std;

ThreadTest::ThreadTest(string param) {
	nameThread = param;
	mostrar();
}

void ThreadTest::mostrar() {
	int foo;
	for (size_t i = 0; i < 10; i++) {
		cout << "Soy el hilo " + nameThread << endl;
	}
}
