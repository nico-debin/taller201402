#ifndef _Conexion_H_
#define _Conexion_H_

#include <winsock2.h>
#include <string>

class Conexion {
	protected:
		WSADATA wsa;
		SOCKET mySocket;
		bool conexion_establecida;

		SOCKADDR_IN server_address;
	public:
		Conexion();
		virtual ~Conexion();

		bool conexionEstablecida();
		int getLastErrorCode();
		void getLastErrorMsg(std::string &str);

		bool crearSocket();
		SOCKET getSocket();

		// Deshabilita algoritmo de Nagle
		void disableNagle();

		bool sendData(SOCKET, const char*, int max_chars);
		bool getData(SOCKET, char* response, int max_chars);

		void close();
};

#endif