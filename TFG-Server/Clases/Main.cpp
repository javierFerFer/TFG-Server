
//todos los include de la clase
#include "../Headers/Main.h"
#include "../Headers/ThreadMenu.h"
#include "../Headers/ThreadTest.h"
#include <iostream>
#include <thread>
using namespace std;


/*
Main del programa

*/
void test() {
	int foo;
	for (int i = 0; i < 100; i++) {
		cout << i << endl;
	}
	cin >> foo;
}

int main() {
	int foo;
	//ThreadMenu t;
	//cin >> foo;
	thread t(&ThreadTest::mostrar, ThreadTest("hilo 1"));
	thread test(&ThreadTest::mostrar, ThreadTest("hilo 2"));
	// Indica que los hilos van por separado
	t.detach();
	test.detach();
	cin >> foo;
	return 0;
}
