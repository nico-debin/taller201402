// snowbross_v3_cliente.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <SDL2/SDL.h>

#include "Logger.h"
#include "EscenarioVista.h"
#include "ControladorCliente.h"
#include "Constantes.h"

#include "Logger.h"


void pedirInfoServer(string &ip, int &port, string &username){
	string input;

	ip = "127.0.0.1";
	cout << "Ingrese IP [" << ip << "]: ";
	getline(cin, input);
	if(input != ""){
		ip = input;
	}

	stringstream puerto_str;
	int aux = SERVIDOR_PUERTO_DEFAULT;
	puerto_str << aux ;
	cout << "Ingrese Puerto [" << puerto_str.str() << "]: ";
	getline(cin, input);
	if(input != ""){
		puerto_str = stringstream(input);
	}
	puerto_str >> port;

	username = "nico";
	cout << "Ingrese Username [" << username << "]: ";
	getline(cin, input);
	if(input != ""){
		username = input;
	}

}


int main(int argc, char *argv[]){

	//TestThreads2::test();
	Logger::getLogger()->setLevel(INFO_LOG);
	
	string ip, username;
	int port;

	pedirInfoServer(ip, port, username);
	ControladorCliente controladorCliente(username, ip, port);
	controladorCliente.iniciar();
	
	return 0;
}

