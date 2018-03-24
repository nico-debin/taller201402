#include "stdafx.h"
#include "Cliente.h"
#include "Utilities.h"
#include "ControladorCliente.h"

//TODO: Solo utilizado para test, eliminar
bool test_clientExit = false;

//Threads CLIENTE
DWORD WINAPI getDataServidor( LPVOID args );
DWORD WINAPI sendDataServidor( LPVOID args );

Cliente::Cliente( string username, string ipServidor, int puertoServidor ){
	this->username = username;
	this->ipServidor = ipServidor;
	this->puertoServidor = puertoServidor;
	this->esperarNuevoEscenario = false;
	
	this->cnxCliente =  new ConexionCliente;
	this->hayEventosParaEnviar = new ConditionVariable();
	this->hayEventosRecibidos = new ConditionVariable();
}

Cliente::~Cliente(){
	for(int i=0 ; i < this->vectorThreads.size() ; i++){
		TerminateThread(this->vectorThreads[i], NULL);
	}
	delete this->cnxCliente;
	delete this->hayEventosParaEnviar;
	delete this->hayEventosRecibidos;
}

bool Cliente::conectarseConServidor(){
	stringstream info;
	int intentos = 1;
	while( intentos <= this->MAX_INTENTOS_CONEXION ){

		this->cnxCliente->connectServer(this->ipServidor, this->puertoServidor);

		if( cnxCliente->conexionEstablecida() != false ){
			break;
		}

		stringstream info;
		info << "[" << intentos << "]: No se pudo conectar al servidor...\n";
		Logger::getLogger()->error(info.str());
		intentos++;
	}

	return  cnxCliente->conexionEstablecida();
}

bool Cliente::loginServidor(){
	sEventoClienteConectar* eventoClienteConectar = new sEventoClienteConectar;
	strncpy(eventoClienteConectar->nombre_usuario, this->username.c_str(), 30);

	this->cnxCliente->sendData( cnxCliente->getSocket(), (char*)eventoClienteConectar, sizeof(sEventoClienteConectar));
	delete eventoClienteConectar;

	sEvento* evento;
	char* datosRecibidosEvento = new char[sizeof(sEvento)];
    memset(datosRecibidosEvento,'\0',sizeof(sEvento));

	if( this->cnxCliente->getData( this->cnxCliente->getSocket(), datosRecibidosEvento, sizeof(sEvento) ) == true ){
		evento = (sEvento*)datosRecibidosEvento;
		this->enqueueEventoRecibido(*evento);
	}
	
	// Recibo la informacion del escenario
	sEscenarioCliente* escenarioCliente;
	char* datosRecibidosEscenario = new char[sizeof(sEscenarioCliente)];
    memset(datosRecibidosEscenario,'\0',sizeof(sEscenarioCliente));

	bool estado = true;
	if( ( evento->evento == EVENTO_CLIENTE_RECONEXION_ACEPTADA || evento->evento == EVENTO_CLIENTE_CONEXION_ACEPTADA ) &&
		this->cnxCliente->getData( this->cnxCliente->getSocket(), datosRecibidosEscenario, sizeof(sEscenarioCliente)) == true ){

		escenarioCliente = (sEscenarioCliente*)datosRecibidosEscenario;

		Utilities::ALTO_PX  = escenarioCliente->alto_px;
		Utilities::ANCHO_PX = escenarioCliente->ancho_px;
		Utilities::IMAGEN_FONDO_NIVEL1 = escenarioCliente->imagen_fondo;
		Utilities::ID_SNOWBOY = escenarioCliente->idSnowBoy;

		delete[] datosRecibidosEscenario;

		Logger::getLogger()->info("Esperando que se terminen de unir todos los jugadores...");

		// Recibo nombre de los jugadoes
		char* datosRecibidosNombres = new char[sizeof(sNombresClientes)];
		memset(datosRecibidosNombres,'\0',sizeof(sNombresClientes));
		sNombresClientes* nombresClientes;
		//memset(&nombresClientes,0,sizeof(sNombresClientes));
		if( this->cnxCliente->getData( this->cnxCliente->getSocket(), datosRecibidosNombres, sizeof(sNombresClientes)) == true ){
			nombresClientes = (sNombresClientes*)datosRecibidosNombres;
			int idJugador;
			for(short i = 0 ; i < 4 ; i++){
				idJugador = nombresClientes->ids[i];
				if( idJugador > 0 ){
					ControladorCliente::estadisticasJugadores->setJugador(idJugador, nombresClientes->nombres[i]);
				}
			}
		}
		delete[] datosRecibidosNombres;

	}else{
		// No se acepto la conexion.
		estado = false;

		switch(evento->evento){
			case EVENTO_CLIENTE_CONEXION_RECHAZADA_NOMBRE_EXISTENTE:
				Logger::getLogger()->info("CONEXION RECHAZADA NOMBRE EXISTENTE");
				break;
			case EVENTO_CLIENTE_CONEXION_RECHAZADA_SALA_LLENA:
				Logger::getLogger()->info("CONEXION RECHAZADA SALA LLENA");
				break;
		}

	}
	delete[] datosRecibidosEvento;

	return estado;
}

void Cliente::iniciar(){
	DWORD IdThread; 

	// Escucho mensajes del servidor
	HANDLE hGetDataServidor = CreateThread( NULL, 0, getDataServidor, (LPVOID)this, 0, NULL);
	this->vectorThreads.push_back(hGetDataServidor);

	// Envio mensajes al servidor
	HANDLE hSendDataServidor = CreateThread( NULL, 0, sendDataServidor, (LPVOID)this, 0, NULL);
	this->vectorThreads.push_back(hSendDataServidor);
}

void Cliente::enviarMensajesAServidor(){	
	sEventoCliente evento;
	//int count = this->getCantidadEventosParaEnviar();
	//while(count > 0){
	this->hayEventosParaEnviar->sleep();
	while( !test_clientExit && this->colaEventosEnviar.size() > 0 ){
		evento = this->dequeueEventoParaEnviar();

		std::stringstream str;
		str << "Mensaje Enviado. Id: " << evento.id << ". Evento: " << evento.evento;
		Logger::getLogger()->debug(str.str());
		this->cnxCliente->sendData(this->cnxCliente->getSocket(), (char*)&evento, sizeof(sEventoCliente));
		Sleep(1);
		//count--;
	}

}

void Cliente::recibirMensajesDeServidor(){
	string msg_received;
	std::stringstream stream;
	char* datosRecibidos;
	bool lecturaSocket;

	if( test_clientExit == true ){
		return;	
	}

	if( this->esperarNuevoEscenario ){
		const unsigned int sizeof_sEscenarioCliente = sizeof(sEscenarioCliente);
		sEscenarioCliente* escenarioCliente;
		datosRecibidos = new char[sizeof_sEscenarioCliente];
		memset(datosRecibidos, '\0', sizeof_sEscenarioCliente);
		
		lecturaSocket = this->cnxCliente->getData( this->cnxCliente->getSocket(), datosRecibidos, sizeof_sEscenarioCliente );
		if( lecturaSocket == true ){
			escenarioCliente = (sEscenarioCliente*)datosRecibidos;
			Utilities::ALTO_PX  = escenarioCliente->alto_px;
			Utilities::ANCHO_PX = escenarioCliente->ancho_px;
			Utilities::IMAGEN_FONDO_NIVEL1 = escenarioCliente->imagen_fondo;
			Utilities::ID_SNOWBOY = escenarioCliente->idSnowBoy;
		}

		this->esperarNuevoEscenario = false;
	}else{
		const unsigned int sizeof_sEvento = sizeof(sEvento);
		sEvento* evento;
		datosRecibidos = new char[sizeof_sEvento];
		memset(datosRecibidos, '\0', sizeof_sEvento);

		lecturaSocket = this->cnxCliente->getData( this->cnxCliente->getSocket(), datosRecibidos, sizeof_sEvento );
		if( lecturaSocket == true ){
			evento = (sEvento*)datosRecibidos;
			this->enqueueEventoRecibido(*evento);

			// Esto lo pongo aca por una cuestion de sino el socket se puede volver a poner a escuchar antes de que procese el evento
			if( evento->evento == EVENTO_ESCENARIO_NIVEL_COMPLETADO ){
				this->esperarNuevoEscenario = true;
			}
		}
	}

	if( lecturaSocket == false ){

		int error_code = this->cnxCliente->getLastErrorCode();
		if( error_code == WSAECONNRESET ){

			Logger::getLogger()->info("SERVIDOR DESCONECTADO");

			test_clientExit = true;
		}else{
			std::stringstream str;
			str << "Error inesperado. Error code: " << error_code;
			
			string str_error;
			this->cnxCliente->getLastErrorMsg(str_error);
			str << ". " << str_error.c_str();
			Logger::getLogger()->error(str.str());

			test_clientExit = true;	
		}

		sEvento evento;
		evento.evento = EVENTO_CLIENTE_JUEGO_SALIR;
		this->enqueueEventoRecibido(evento);
	}

	delete[] datosRecibidos;
}

void Cliente::enqueueEventoEnviar(sEventoCliente evento) {
    //Lock::getLock()->start();
    this->colaEventosEnviar.push(evento);
	this->hayEventosParaEnviar->wakeAll();
    //Lock::getLock()->stop();
}

sEventoCliente Cliente::dequeueEventoParaEnviar(){
    //Lock::getLock()->start();
	//sEventoCliente evento = this->colaEventosEnviar.front();
	//this->colaEventosEnviar.pop();
    //Lock::getLock()->stop();
	sEventoCliente evento =this->colaEventosEnviar.pop();
    return evento;
}

int Cliente::getCantidadEventosParaEnviar(){
	//Lock::getLock()->start();
	int count = this->colaEventosEnviar.size();
	//Lock::getLock()->stop();

	return count;
}

void Cliente::enqueueEventoRecibido(sEvento evento){
	//Lock::getLock()->start();
	this->colaEventosRecibidos.push(evento);
	this->hayEventosRecibidos->wakeAll();
	//Lock::getLock()->stop();
}

sEvento Cliente::dequeueEventoRecibido() {
    //Lock::getLock()->start();
	//sEvento evento = this->colaEventosRecibidos.front();
	sEvento evento = this->colaEventosRecibidos.pop();
    //Lock::getLock()->stop();

    return evento;
}

int Cliente::getCantidadEventosRecibidos(){
	//Lock::getLock()->start();
	int count = this->colaEventosRecibidos.size();
	//Lock::getLock()->stop();

	return count;
}

DWORD WINAPI getDataServidor( LPVOID args ){
	Cliente* cliente =  (Cliente*)args;
	Logger::getLogger()->debug("Obteniendo datos del servidor");
	//while(true){
	while( !test_clientExit ){
		Sleep(2);
		cliente->recibirMensajesDeServidor();
	}
	return 0;
}

DWORD WINAPI sendDataServidor( LPVOID args ){
	Cliente* cliente =  (Cliente*)args;
	//while(true){
	while( !test_clientExit ){
//		Sleep(2);
		cliente->enviarMensajesAServidor();
	}
	return 0;
}