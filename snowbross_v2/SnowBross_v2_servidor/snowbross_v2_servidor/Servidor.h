#ifndef _Servidor_H_
#define _Servidor_H_

#include "estructuraDeDatos.h"
#include "Cliente.h"
#include "ConexionServidor.h"
#include "ConditionVariable.h"

#include <string>
#include <map>
//#include <queue>
#include "QueueLock.h"

using namespace std;

class Servidor{
	protected:
		ConexionServidor* cnxServidor;
		Lock* lock;

		// Conditions Variables usadas cuando se necesita saber si el cliente esta descoenctado y 
		// se debe esperar a que vuelva a conectarse
		map<string,ConditionVariable*> clientesCV; 

		vector<HANDLE> vectorThreads;

		//queue<sEvento> colaEventosEnviar;
		QueueLock<sEvento> colaEventosEnviar;
		//queue<sEventoCliente> colaEventosRecibidos;
		QueueLock<sEventoCliente> colaEventosRecibidos;
		
		//void enviarEscenarioCliente(SOCKET);
		void enviarEscenarioCliente(SOCKET socket, int id);

		sEvento dequeueEventoEnviar();
		void enqueueEventoRecibido(sEventoCliente);		
		//queue<sEventoCliente> getColaEventosRecibidos();

		int getCantidadEventosParaEnviar();

	public:
		map<string,Cliente*> clientes; 
		//ConditionVariable SalaIncompleta;
		ConditionVariable* SalaIncompleta;

        bool iniciaServidor;
		bool inicioOReconexion;

		Servidor(int puerto);
		~Servidor();

		void start();

		// Metodos utilizados por los threads
		void aceptarCliente();
		void escucharCliente(string nombreUsuario);
		void enviarEventosCliente(Cliente*);
		void enqueueMensajesAClientes();

		ConexionServidor* getConexionServidor();

        void enqueueEventoEnviar(sEvento evento);
		bool hayEventosParaEnviar();
        sEventoCliente dequeueEventoRecibido();
		bool hayEventosRecibidos();


};

#endif