#pragma warning( disable : 4996 )

#include "stdafx.h"
#include "Conexion.h"
#include <sstream>

Conexion::Conexion(){
	this->conexion_establecida = ( WSAStartup( MAKEWORD(2,2), &(this->wsa) ) == 0 );
}
Conexion::~Conexion(){
	if(this->conexion_establecida){
		close();
		WSACleanup();
		this->conexion_establecida = false;
	}
}

bool Conexion::conexionEstablecida(){
	return this->conexion_establecida;
}

int Conexion::getLastErrorCode(){
	return WSAGetLastError();
}

void Conexion::getLastErrorMsg(std::string &str){
	char* s = NULL;
	FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, 
				   NULL, WSAGetLastError(),
				   MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				   (LPTSTR )&s, 0, NULL);
	//fprintf(stderr, "%S\n", s);
	str = s;
	LocalFree(s);
}

bool Conexion::crearSocket(){
	if( this->conexionEstablecida() ){
		this->conexion_establecida = ((this->mySocket = socket(AF_INET , SOCK_STREAM , 0 )) != INVALID_SOCKET);
		return this->conexion_establecida;
	}
	return false;
}

SOCKET Conexion::getSocket(){
	return this->mySocket;
}

// Deshabilita algoritmo de Nagle
// http://en.wikipedia.org/wiki/Nagle%27s_algorithm#Interactions_with_real-time_systems
void Conexion::disableNagle(){
    char value = 1;
    setsockopt( this->mySocket, IPPROTO_TCP, TCP_NODELAY, &value, sizeof( value ) );
}

bool Conexion::sendData(SOCKET socket, const char* bytes, int max_chars){
	unsigned int bytes_send = 0;
	char* buffer = new char[max_chars + 1];
	do{
		for(int i=0 ; i <max_chars - bytes_send ; i++){
			buffer[i] = bytes[bytes_send + i];
		}

		//strncpy(buffer, bytes + bytes_send, max_chars - bytes_send );
		bytes_send = send(socket ,buffer, max_chars - bytes_send , 0);
	}while( bytes_send != SOCKET_ERROR && bytes_send > 0 && bytes_send < max_chars);

	delete[] buffer;

	return ( bytes_send > 0);
}

bool Conexion::getData(SOCKET socket, char* response, int max_chars){
	char* bytesLeidos = new char[max_chars + 1];
	int bytes_received      = 0;
	unsigned int bytesCount = 0;

	do{	
		bytes_received = recv(socket, bytesLeidos, max_chars, 0);

		if(bytes_received == SOCKET_ERROR){
			delete[] bytesLeidos;
			return false;
		}
		//strncpy(response + bytesCount, bytesLeidos, bytes_received );
		for(int i=0 ; i < bytes_received ; i++){
			response[bytesCount + i] = bytesLeidos[i];
		}


		bytesCount += bytes_received;
	}while( bytes_received > 0 && bytesCount < max_chars);

	delete[] bytesLeidos;
	return true;
}

void Conexion::close(){
	closesocket(this->mySocket);
}