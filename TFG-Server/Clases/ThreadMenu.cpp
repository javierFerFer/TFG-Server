//todos los include de la clase
#include "../Headers/Main.h"
#include "../Headers/ThreadMenu.h"
#include <iostream>
using namespace std;


/*
Main del programa

*/

// Constructor de la clase, se ejecuta al crear un objeto de la misma
ThreadMenu::ThreadMenu(){
	mostrarMenu();
}

void ThreadMenu::mostrarMenu() {
		cout << "Opciones del menu:" << endl;
		cout << "1. Activar el servidor" << endl;
		cout << "2. Ver estado del servidor" << endl;
		cout << "3. Desactivar el servidor" << endl;
		cout << "4. Modificar clave RSA" << endl;
		cout << "5. Salir" << endl;
		cout << endl;
		cout << "Introduce el índice de la acción que desea realizar" << endl;
		cin >> userValue;
		
		if (userValue == "1") {
			// Activa el servidor
		} else if (userValue == "2") {
			//
		} else if (userValue == "3") {
		
		} else if (userValue == "4") {
		
		} else if(userValue == "5"){
		
		} else {
		
		}
}
