#include "stdafx.h"
#include "ConexionCliente.h"

ConexionCliente::ConexionCliente() : Conexion(){
	this->crearSocket();
	this->disableNagle();
}

bool ConexionCliente::connectServer(std::string ip, int port){
	if( this->conexionEstablecida() ){
		memset(&this->server_address, 0, sizeof(this->server_address));
		this->server_address.sin_addr.s_addr = inet_addr(ip.c_str());
		this->server_address.sin_family = AF_INET;
		this->server_address.sin_port = htons(port);
		this->conexion_establecida = (connect(this->mySocket , (struct sockaddr *)&(this->server_address) , sizeof(this->server_address)) != SOCKET_ERROR);
		return this->conexion_establecida;
	}
	return false;
}