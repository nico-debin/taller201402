#ifndef _ConexionServidor_H_
#define _ConexionServidor_H_

#include "Conexion.h"

class ConexionServidor : public Conexion{
	public:
		ConexionServidor(int port);

		bool bindSocket(int port);
		void listenSocket();
		bool aceptarCliente(SOCKET &new_socket);
};

#endif