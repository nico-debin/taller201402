#include "stdafx.h"
#include "ConexionServidor.h"
#include "Logger.h"
#include <sstream>

ConexionServidor::ConexionServidor(int port) : Conexion(){ 
	this->crearSocket(); 
	this->disableNagle();
	this->bindSocket(port);
	this->listenSocket();
}

bool ConexionServidor::bindSocket(int port){
	memset(&this->server_address, 0, sizeof(this->server_address));
	this->server_address.sin_family = AF_INET;
    this->server_address.sin_addr.s_addr = INADDR_ANY;
    this->server_address.sin_port = htons( port );
	return ( bind(this->mySocket, (SOCKADDR*)&this->server_address , sizeof(this->server_address)) != SOCKET_ERROR);
}

void ConexionServidor::listenSocket(){
	listen(this->mySocket , 3);
}

bool ConexionServidor::aceptarCliente(SOCKET &new_socket){
	SOCKADDR_IN client;

	int c = sizeof(SOCKADDR_IN);
    new_socket = accept(this->mySocket , (struct sockaddr *)&client, &c);

	if(new_socket != INVALID_SOCKET){
		std::stringstream str;
		str << "Nuevo cliente: socket fd is " << new_socket << " , ip is " << inet_ntoa(client.sin_addr) << " , port : " << ntohs(client.sin_port) << endl;
		Logger::getLogger()->debug( str.str() );
	}

	return (new_socket != INVALID_SOCKET);
}