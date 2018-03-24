#ifndef _Servidor_H_
#define _Servidor_H_

#include "estructuraDeDatos.h"
#include "Cliente.h"
#include "ConexionServidor.h"
#include "ConditionVariable.h"

#include <string>
#include <map>
#include <vector>
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
		void enviarNombresJugadoresClientes(int idCliente = -1);

		

		sEvento dequeueEventoEnviar();
		void enqueueEventoRecibido(sEventoCliente);		
		//queue<sEventoCliente> getColaEventosRecibidos();

		int getCantidadEventosParaEnviar();

		vector<int> idsClientesDisponibles;
		int getNextIdCliente();

		bool juegoFinalizado;

	public:
		map<string,Cliente*> clientes; 
		//ConditionVariable SalaIncompleta;
		ConditionVariable* SalaIncompleta;

        bool iniciaServidor;
		bool inicioOReconexion;
		bool pausarEnvioDeDatos;

		void setEstadoJuegoFinalizado(bool);
		vector<string> juegoFinalizado_nombreNuevosClientes;

		Servidor(int puerto);
		~Servidor();

		void start();

		int getCantidadClientesDesconectados();
		int eliminarClientesDesconectados();

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

		void encolarEventosEstadisticas();

		void enviarEscenarioClientes();

		bool envioDeDatosPausado();
		void quitarPausaEnvioDeDatos();
};

#endif