#include "stdafx.h"
#include "Servidor.h"
#include "Logger.h"
//#include "Lock.h"
#include "Utilities.h"
#include "JuegoControlador.h"

// Threads SERVIDOR
DWORD WINAPI escucharClientes( LPVOID args ); // Escucha por nuevas conexiones de clientes
DWORD WINAPI getDataCliente( LPVOID args ); // Escucha por eventos enviados por el cliente
DWORD WINAPI procesarEventosParaEnviar( LPVOID args ); // Desencola mensajes de la queue general y los encola en cada cliente
DWORD WINAPI sendDataCliente( LPVOID args ); // Obtiene eventos para enviar de la queue general y se los envia al cliente


typedef struct sGetDataClientes {
	Servidor* servidor;
	string nombreUsuario;
	bool _thread_ok; // Indica si el thread ya leyo la informacion
} sGetDataClientes, *p_sGetDataClientes;

typedef struct sSendDataCliente {
	Servidor* servidor;
	Cliente* cliente;
	bool _thread_ok; // Indica si el thread ya leyo la informacion
} sSendDataCliente, *p_sSendDataCliente;


Servidor::Servidor(int puerto){ //: SalaIncompleta(Lock::getLock()){
	this->lock = new Lock();
	this->SalaIncompleta = new ConditionVariable(new Lock());
	this->pausarEnvioDeDatos = false;
	this->juegoFinalizado = false;

	Logger::getLogger()->info("Iniciando Servidor...");
    this->iniciaServidor = false;
	this->inicioOReconexion = false;
	this->cnxServidor = new ConexionServidor(puerto);

	this->idsClientesDisponibles.clear();
}

void Servidor::start(){
	Logger::getLogger()->info("Esperando clientes...\n");
	HANDLE hEscucharClientes = CreateThread( NULL, 0, escucharClientes, (LPVOID)this, 0, NULL);
	this->vectorThreads.push_back(hEscucharClientes);

	HANDLE hProcesarEventosParaEnviar = CreateThread( NULL, 0, procesarEventosParaEnviar, (LPVOID)this, 0, NULL);
	this->vectorThreads.push_back(hProcesarEventosParaEnviar);
}

Servidor::~Servidor(){
	for(unsigned int i=0 ; i < this->vectorThreads.size() ; i++){
		TerminateThread(this->vectorThreads[i], NULL);
	}
	for (map<string,Cliente*>::iterator it=this->clientes.begin(); it!=this->clientes.end(); ++it){
		delete this->clientesCV[it->second->getName()];
		delete it->second;
	}
	
	delete this->cnxServidor;
}

void Servidor::enviarEscenarioClientes(){
	Cliente* cliente;
	for (map<string,Cliente*>::iterator it=this->clientes.begin(); it!=this->clientes.end(); ++it){
		cliente = it->second;
		if( cliente->estaConectado){
			this->enviarEscenarioCliente( cliente->getSocket(), cliente->idSnowBoy );
		}
	}
}
void Servidor::enviarEscenarioCliente(SOCKET socket, int id){
	sEscenarioCliente escenarioCliente = {id, Utilities::ALTO_PX , Utilities::ANCHO_PX , 0 };
	strcpy(escenarioCliente.imagen_fondo, Utilities::IMAGEN_FONDO_NIVEL1.c_str());

	this->cnxServidor->sendData(socket,(char*) &escenarioCliente, sizeof(sEscenarioCliente));
}

void Servidor::enviarNombresJugadoresClientes(int idCliente){
	sNombresClientes nombresClientes;
	memset(&nombresClientes,0,sizeof(sNombresClientes));
	Cliente* cliente;
	short i = 0;
	for (map<string,Cliente*>::iterator it=this->clientes.begin(); it!=this->clientes.end(); ++it){
		cliente = it->second;
		nombresClientes.ids[i] = cliente->idSnowBoy;
		strcpy(nombresClientes.nombres[i], cliente->getName().c_str());
		i++;
	}

	if(this->juegoFinalizado == true){
		for (vector<string>::iterator it=this->juegoFinalizado_nombreNuevosClientes.begin(); it!=this->juegoFinalizado_nombreNuevosClientes.end(); ++it){
			cliente = this->clientes[*it];
			this->cnxServidor->sendData(cliente->getSocket(),(char*) &nombresClientes, sizeof(sNombresClientes));
		}
	}else{
		for (map<string,Cliente*>::iterator it=this->clientes.begin(); it!=this->clientes.end(); ++it){
			cliente = it->second;
			this->cnxServidor->sendData(cliente->getSocket(),(char*) &nombresClientes, sizeof(sNombresClientes));
		}
	}


	
}

void Servidor::aceptarCliente(){
	SOCKET new_socket;
	string response;
	
	// VER ESTO!
	sEventoClienteConectar* eventoClienteConectar;
	char* datosRecibidos = new char[sizeof(sEventoClienteConectar)];
    memset(datosRecibidos,'\0',sizeof(sEventoClienteConectar));

	sEvento* evento = new sEvento;
	ZeroMemory(evento, sizeof(sEvento));

	sGetDataClientes* getDataClientes = new sGetDataClientes;
	getDataClientes->_thread_ok = false;
	getDataClientes->servidor = this;

	sSendDataCliente* sendDataClienteInfo = new sSendDataCliente;
	sendDataClienteInfo->_thread_ok = false;
	sendDataClienteInfo->servidor = this;

	Logger::getLogger()->debug("Esperando nuevo cliente... ");
	if( this->cnxServidor->aceptarCliente(new_socket) ){
		
		// Se recibe el nombre de usuario
		this->cnxServidor->getData(new_socket,datosRecibidos, sizeof(sEventoClienteConectar));
		eventoClienteConectar =(sEventoClienteConectar*)datosRecibidos;
		//Lock::getLock()->start();
		this->lock->start();

		// Verifico si existe el nombre de usuario
		if ( this->clientes.find(eventoClienteConectar->nombre_usuario) != this->clientes.end() ) {
			// El usuario ya existe
				
			//if( this->clientes[eventoClienteConectar->nombre_usuario]->estaConectado ){
				// ERROR
				evento->evento = EVENTO_CLIENTE_CONEXION_RECHAZADA_NOMBRE_EXISTENTE;
				Logger::getLogger()->info("NUEVA CONEXION RECHAZADA NOMBRE EXISTENTE: " + string(eventoClienteConectar->nombre_usuario));
			//}else{
			//	// RECONECTAR
			//	evento->evento = EVENTO_CLIENTE_RECONEXION_ACEPTADA;
			//	Logger::getLogger()->info("RECONEXION ACEPTADA: " + string(eventoClienteConectar->nombre_usuario));
			//	this->clientes[eventoClienteConectar->nombre_usuario]->reconectar(new_socket);
			//	this->clientesCV[eventoClienteConectar->nombre_usuario]->wakeAll();
			//	// Se utiliza para poder enviar todos las figuras a los clientes cuando hay una reconexion o cuando recien si inicializa
			//	// el juego
			//	this->inicioOReconexion = true;
			//}

		} else {
			if( this->clientes.size() == Utilities::MAX_JUGADORES ){
				// ERROR
				evento->evento = EVENTO_CLIENTE_CONEXION_RECHAZADA_SALA_LLENA;
				Logger::getLogger()->info("CONEXION RECHAZADA SALA LLENA: " + string(eventoClienteConectar->nombre_usuario));
			}else{
				// Primera conexion
				evento->evento = EVENTO_CLIENTE_CONEXION_ACEPTADA;
				Logger::getLogger()->info("CONEXION ACEPTADA: " + string(eventoClienteConectar->nombre_usuario));
				this->clientes[eventoClienteConectar->nombre_usuario] = new Cliente(eventoClienteConectar->nombre_usuario, this->getNextIdCliente(), new_socket);
				//this->clientesCV[eventoClienteConectar->nombre_usuario] = new ConditionVariable(Lock::getLock());
				this->clientesCV[eventoClienteConectar->nombre_usuario] = new ConditionVariable(new Lock());

				if(this->juegoFinalizado){
					this->clientes[eventoClienteConectar->nombre_usuario]->pausarEnvioDeDatos = true;
					this->juegoFinalizado_nombreNuevosClientes.push_back(eventoClienteConectar->nombre_usuario);
				}

                if ( this->clientes.size() == Utilities::MAX_JUGADORES ) {
                    // La primera vez que queda completa la sala ya permite iniciar el negocio del servior
					
                    this->iniciaServidor = true;
					this->inicioOReconexion = true;

					if(this->juegoFinalizado == false){
						this->SalaIncompleta->wakeAll();
					}
                }                                
			}
		}
		//Lock::getLock()->stop();
		this->lock->stop();

		this->cnxServidor->sendData(new_socket,(char*) evento, sizeof(sEvento)) ;
		if( (evento->evento == EVENTO_CLIENTE_CONEXION_ACEPTADA) || (evento->evento == EVENTO_CLIENTE_RECONEXION_ACEPTADA) ){
			this->enviarEscenarioCliente(new_socket, this->clientes[eventoClienteConectar->nombre_usuario]->idSnowBoy);

			if ( this->clientes.size() == Utilities::MAX_JUGADORES ) {
				this->enviarNombresJugadoresClientes();

				if(this->juegoFinalizado == true){
					// A los nuevos clientes le mando el evento de nuevo nivel
					sEvento eventoJuegoCompletado;
					memset(&eventoJuegoCompletado,0,sizeof(sEvento));
					eventoJuegoCompletado.evento = EVENTO_ESCENARIO_NIVEL_COMPLETADO;

					Cliente* cliente;
					for (vector<string>::iterator it=this->juegoFinalizado_nombreNuevosClientes.begin(); it!=this->juegoFinalizado_nombreNuevosClientes.end(); ++it){
						cliente = this->clientes[*it];
						this->cnxServidor->sendData(cliente->getSocket(),(char*) &eventoJuegoCompletado, sizeof(sEvento));
					}

					this->SalaIncompleta->wakeAll();
				}
			}
		}

		if( evento->evento == EVENTO_CLIENTE_CONEXION_ACEPTADA ){
			// Se levanta un thread para escuchar mensajes del nuevo cliente
			getDataClientes->nombreUsuario = eventoClienteConectar->nombre_usuario;
			HANDLE hGetDataCliente = CreateThread( NULL, 0, getDataCliente, (LPVOID)getDataClientes, 0, NULL);
			this->vectorThreads.push_back(hGetDataCliente);

			// Se levanta un thread para enviar mensajes al nuevo cliente
			sendDataClienteInfo->cliente = this->clientes[eventoClienteConectar->nombre_usuario];
			HANDLE hSendDataCliente = CreateThread( NULL, 0, sendDataCliente, (LPVOID)sendDataClienteInfo, 0, NULL);
			this->vectorThreads.push_back(hSendDataCliente);
		}
	}

	delete datosRecibidos;
}

void Servidor::setEstadoJuegoFinalizado(bool newStatus){
	if(newStatus == true){
		this->juegoFinalizado_nombreNuevosClientes.clear();
	}
	this->juegoFinalizado = newStatus;
}

int Servidor::getNextIdCliente(){
	int id;
	if(this->idsClientesDisponibles.size() > 0){
		id = this->idsClientesDisponibles.back();
		this->idsClientesDisponibles.pop_back();
	}else{
		id = this->clientes.size()+1;
	}
	return id;
}

int Servidor::eliminarClientesDesconectados(){
	Cliente* cliente;
	vector<string> nombresParaEliminar;
	int cantClientesEliminados = 0;
	for (map<string,Cliente*>::iterator it= this->clientes.begin(); it!= this->clientes.end(); ++it){
		cliente = it->second;
		if( cliente->estaConectado == false ){
			// No puedo eliminarlos porque sino se rompen los threads.
			//delete this->clientesCV[it->second->getName()];
			//delete it->second;

			this->idsClientesDisponibles.push_back(cliente->idSnowBoy);

			// Solo lo quito del vector. Va a haber perdida de memoria al destruir el servidor.
			nombresParaEliminar.push_back(cliente->getName());
		}
	}
	cantClientesEliminados = nombresParaEliminar.size();
	for (vector<string>::iterator itNombre= nombresParaEliminar.begin(); itNombre!= nombresParaEliminar.end(); ++itNombre){
		this->clientes.erase( *itNombre );
	}

	return cantClientesEliminados;
}

int Servidor::getCantidadClientesDesconectados(){
	int count = 0;
	for (map<string,Cliente*>::iterator it= this->clientes.begin(); it!= this->clientes.end(); ++it){
		if( it->second->estaConectado == false ){
			count++;
		}
	}
	return count;
}

void Servidor::escucharCliente(string nombreUsuario){
	if ( this->clientes.find(nombreUsuario) == this->clientes.end() ) {
		// El usuario ya no existe mas.
		cout << "Cliente " << nombreUsuario << " fue eliminado" << endl;
		// Como no puedo matar este thread, lo pongo a dormir.
		ConditionVariable cv(new Lock());
		cv.sleep();
	}
	Cliente* cliente = this->clientes[nombreUsuario];

	while(cliente->estaConectado == false){
		this->clientesCV[nombreUsuario]->sleep();
	}

	sEventoCliente* eventoCliente;
	char* datosRecibidos = new char[sizeof(sEventoCliente)];
    memset(datosRecibidos,'\0',sizeof(sEventoCliente));

	if( this->cnxServidor->getData(cliente->getSocket(),datosRecibidos,  sizeof(sEventoCliente)) == true ){

		eventoCliente = (sEventoCliente*)datosRecibidos;
		std::stringstream str;
		str << "Mensaje recibido de " << nombreUsuario << ". Id: " << eventoCliente->id << " Evento: " << eventoCliente->evento;
		Logger::getLogger()->debug(str.str());

		this->enqueueEventoRecibido(*eventoCliente);
	}else{
		int error_code = cnxServidor->getLastErrorCode();
		
		//if( error_code == WSAECONNRESET ){
		if( error_code != WSAECONNRESET ){
			
		//	std::stringstream str;
		//	str << "CLIENTE " << cliente->getName() << " DESCONECTADO ";
		//	Logger::getLogger()->info(str.str());

		//	cliente->estaConectado = false;
		//}else{
			std::stringstream str;
			str << "Error inesperado. Error code: " << error_code;
			
			string str_error;
			cnxServidor->getLastErrorMsg(str_error);
			str << ". " << str_error.c_str();
			Logger::getLogger()->error(str.str());
		}
		std::stringstream str;
		str << "CLIENTE " << cliente->getName() << " DESCONECTADO ";
		Logger::getLogger()->info(str.str());

		cliente->estaConectado = false;

	}

	delete[] datosRecibidos;
}

void Servidor::enviarEventosCliente(Cliente* cliente){

	if(cliente->pausarEnvioDeDatos) return;

	this->lock->start();
	// Comienza la ejeucion cuando el servidor inició
	while( !this->iniciaServidor ){
		this->lock->stop();
		this->SalaIncompleta->sleep();
		this->lock->start();
	}
	this->lock->stop();

	sEvento eventoServidor;

	while( cliente->estaConectado && cliente->hayEventosParaEnviar() ){
		if(cliente->pausarEnvioDeDatos) return;
		eventoServidor = cliente->dequeueEventoEnviar();

		std::stringstream str;
		str << "Mensaje Enviado. Id: " << eventoServidor.id << ". Evento: " << eventoServidor.evento;
		Logger::getLogger()->debug(str.str());

		this->cnxServidor->sendData(cliente->getSocket(), (char*)&eventoServidor, sizeof(sEvento));

		// Lo pongo en pausa para que no envie otra evento del tipo sEvento (para poder mandar el escenario)
		if( eventoServidor.evento == EVENTO_ESCENARIO_NIVEL_COMPLETADO || eventoServidor.evento == EVENTO_ESCENARIO_JUEGO_COMPLETADO || eventoServidor.evento == EVENTO_ESCENARIO_GAME_OVER ){
			cliente->pausarEnvioDeDatos = true;
		}
	}
	
}


void Servidor::enqueueMensajesAClientes(){
	//Lock::getLock()->start();
	this->lock->start();
	// Comienza la ejeucion cuando el servidor inició
	while( !this->iniciaServidor ){
		//Lock::getLock()->stop();
		this->lock->stop();
		this->SalaIncompleta->sleep();

		//Lock::getLock()->start();
		this->lock->start();
	}
	//Lock::getLock()->stop();
	this->lock->stop();

	int count = this->getCantidadEventosParaEnviar();
	sEvento eventoServidor;
	Cliente* cliente;

	while(count > 0){	

		eventoServidor = this->dequeueEventoEnviar();

		for (map<string,Cliente*>::iterator it=this->clientes.begin(); it!=this->clientes.end(); ++it){
			cliente = it->second;
			if(cliente->estaConectado){
				cliente->enqueueEventoEnviar(eventoServidor);
			}
		}
		count--;
	}
}

void Servidor::enqueueEventoEnviar(sEvento eventoServidor) {
    //Lock::getLock()->start();
    this->colaEventosEnviar.push(eventoServidor);
    //Lock::getLock()->stop();
}

sEvento Servidor::dequeueEventoEnviar() {
	//Lock::getLock()->start();
	//sEvento eventoServidor = this->colaEventosEnviar.front();
	//this->colaEventosEnviar.pop();
	//Lock::getLock()->stop();
	sEvento eventoServidor = this->colaEventosEnviar.pop();
	return eventoServidor;
}

void Servidor::enqueueEventoRecibido(sEventoCliente e){
    //Lock::getLock()->start();
    this->colaEventosRecibidos.push(e);
    //Lock::getLock()->stop();
}

sEventoCliente Servidor::dequeueEventoRecibido() {
    //Lock::getLock()->start();
	//sEventoCliente eventoCliente = this->colaEventosRecibidos.front();
	//this->colaEventosRecibidos.pop();
    //Lock::getLock()->stop();
	sEventoCliente eventoCliente = this->colaEventosRecibidos.pop();
    return eventoCliente;
}


bool Servidor::hayEventosRecibidos(){
	//Lock::getLock()->start();
	this->lock->start();
	bool estado = (this->colaEventosRecibidos.size() > 0);
	//Lock::getLock()->stop();
	this->lock->stop();

	return estado;
}

bool Servidor::hayEventosParaEnviar(){
	//Lock::getLock()->start();
	this->lock->start();
	bool estado = (this->colaEventosEnviar.size() > 0);
	//Lock::getLock()->stop();
	this->lock->stop();

	return estado;
}

int Servidor::getCantidadEventosParaEnviar(){
	//Lock::getLock()->start();
	this->lock->start();
	int cantidadEventos = this->colaEventosEnviar.size();
	//Lock::getLock()->stop();
	this->lock->stop();
	return cantidadEventos;
}

void Servidor::encolarEventosEstadisticas(){
	sEvento nuevoPuntaje;
	memset(&nuevoPuntaje,0,sizeof(sEvento));
	nuevoPuntaje.evento = EVENTO_CLIENTE_SNOWBOY_PUNTAJE_Y_VIDAS;
	vector<estadisticaJugador> estadisticas = JuegoControlador::estadisticasJugadores->getAllEstadisticas();
	for(int i=0 ; i<estadisticas.size() ; i++){
		estadisticaJugador estadistica = estadisticas[i];
		nuevoPuntaje.id = estadistica.idJugador;

		// En X van las vidas
		nuevoPuntaje.x = estadistica.vidas;

		// En Y van los puntos
		nuevoPuntaje.y = estadistica.puntaje;

		this->enqueueEventoEnviar(nuevoPuntaje);
	}
}

// Se fija si todos los clientes estan pausados
bool Servidor::envioDeDatosPausado(){
	Cliente* cliente;
	for (map<string,Cliente*>::iterator it=this->clientes.begin(); it!=this->clientes.end(); ++it){
		cliente = it->second;
		if(cliente->estaConectado  && cliente->pausarEnvioDeDatos == false){
			return false;
		}
	}
	return true;
}

void Servidor::quitarPausaEnvioDeDatos(){
	Cliente* cliente;
	for (map<string,Cliente*>::iterator it=this->clientes.begin(); it!=this->clientes.end(); ++it){
		cliente = it->second;
		cliente->pausarEnvioDeDatos = false;
	}
}

/************* THREADS SERVIDOR ****************/
DWORD WINAPI escucharClientes( LPVOID p ){
	// Agrega nuevos clientes al servidor
	Servidor* servidor = (Servidor*)p;

	while(true){
		Sleep(1);
		servidor->aceptarCliente();
	}
	return 0;
}

DWORD WINAPI getDataCliente( LPVOID p ){
	// Obtiene mensajes de cada cliente
	//Lock::getLock()->start();
	sGetDataClientes* parameters = (sGetDataClientes*)p;
	Servidor* servidor   = parameters->servidor;
	string nombreUsuario = parameters->nombreUsuario;
	//parameters->_thread_ok = true;
	delete parameters;
	//Lock::getLock()->stop();

	while(true){
		Sleep(1);
		servidor->escucharCliente(nombreUsuario);
	}
	return 0;
}

DWORD WINAPI procesarEventosParaEnviar( LPVOID p ){
	// Encola nuevos mensajes en la queue general
	Servidor* servidor = (Servidor*)p;
	while(true){
		Sleep(1);
		servidor->enqueueMensajesAClientes();
	}
	return 0;
}

DWORD WINAPI sendDataCliente( LPVOID p ){
	// Obtiene eventos para enviar de la queue general y se los envia al cliente
	//Lock::getLock()->start();
	sSendDataCliente* sendDataCliente = (sSendDataCliente*)p;
	Servidor* servidor = sendDataCliente->servidor;
	Cliente* cliente = sendDataCliente->cliente;
	sendDataCliente->_thread_ok = true;
	delete sendDataCliente;
	//Lock::getLock()->stop();
	while(true){
		servidor->enviarEventosCliente(cliente);
		Sleep(1);
	}
	return 0;
}