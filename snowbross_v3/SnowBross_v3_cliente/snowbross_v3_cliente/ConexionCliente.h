#ifndef _ConexionCliente_H_
#define _ConexionCliente_H_

#include "Conexion.h"
#include <string>
class ConexionCliente : public Conexion{
	public:
		ConexionCliente();
		bool connectServer(std::string ip, int port);
};

#endif