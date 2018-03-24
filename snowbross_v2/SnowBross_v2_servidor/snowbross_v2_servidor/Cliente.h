#ifndef _Cliente_H_
#define _Cliente_H_

#include <WinSock2.h>
#include "Lock.h"
#include <string>
//#include <queue>
#include "QueueLock.h"
#include "estructuraDeDatos.h"

using namespace std;

class Cliente{
	protected:
		SOCKET socketCliente;
		//queue<sEvento> colaEventosEnviar;
	    QueueLock<sEvento> colaEventosEnviar;
		string name;

	public:
		int idSnowBoy;

		bool estaConectado;

		Cliente(string name, int idSnowBoy, SOCKET);
		~Cliente();

		void reconectar(SOCKET);
		SOCKET getSocket();
		
		void    enqueueEventoEnviar(sEvento);
		sEvento dequeueEventoEnviar();
		bool    hayEventosParaEnviar();
		
		string getName();

};

#endif