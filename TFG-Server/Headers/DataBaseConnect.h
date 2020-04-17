#include "../Headers/Main.h"
#include<iostream>
#include<mysql/mysql.h>
#include<stdio.h>

using namespace std;

class DataBaseConnect {

private:

	MYSQL* conn; /* pointer to connection handler */
	MYSQL_RES* res; /* holds the result set */
	MYSQL_ROW row;

	char* opt_host_name = "localhost"; /* HOST */
	char* opt_user_name = "root"; /* USERNAME */
	char* opt_password = "Luna1000"; /* PASSWORD */
	unsigned int opt_port_num = 3306; /* PORT */
	char* opt_socket_name = NULL; /* SOCKET NAME, DO NOT CHANGE */
	char* opt_db_name = "tfg_server"; /* DATABASE NAME */
	unsigned int opt_flags = 0; /* CONNECTION FLAGS, DO NOT CHANGE */

	// All tables here
	string teachersTableName = " profesorado ";
	
	// All MySQL sentences
	string selectAll = "select * from ";

public:

	DataBaseConnect();

	bool loginQuery(string emailParam, string passwdParam);

};

