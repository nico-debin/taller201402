#ifndef _Cliente_H_
#define _Cliente_H_

#include "ConexionCliente.h"
#include "Lock.h"
#include "estructuraDeDatos.h"
#include "Logger.h"

#include <string>
#include <sstream>
#include <queue>
#include "QueueLock.h"
#include "ConditionVariable.h"

using namespace std;

class Cliente{
	private:
		string username;
		string ipServidor;
		int puertoServidor;	

		vector<HANDLE> vectorThreads;

		QueueLock<sEventoCliente> colaEventosEnviar;
		QueueLock<sEvento> colaEventosRecibidos;

		ConditionVariable* hayEventosParaEnviar;

	public:

		ConexionCliente* cnxCliente;
		static const int MAX_INTENTOS_CONEXION = 3;

		Cliente( string username, string ip, int puerto );	
	
		bool conectarseConServidor();	
		bool loginServidor();
		void iniciar();

		void enviarMensajesAServidor();
		void recibirMensajesDeServidor();

		void enqueueEventoEnviar(sEventoCliente);
		sEventoCliente dequeueEventoParaEnviar();
		int getCantidadEventosParaEnviar();

		void enqueueEventoRecibido(sEvento);
		sEvento dequeueEventoRecibido();
		int getCantidadEventosRecibidos();
		ConditionVariable* hayEventosRecibidos;

		~Cliente();
};

#endif