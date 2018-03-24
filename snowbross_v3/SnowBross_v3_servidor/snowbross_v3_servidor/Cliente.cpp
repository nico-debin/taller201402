#include "stdafx.h"
#include "Cliente.h"
#include "JuegoControlador.h"

Cliente::Cliente(string name, int idSnowBoy, SOCKET socket){
	this->name = name;
	this->estaConectado = true;
	this->socketCliente = socket;
	this->idSnowBoy = idSnowBoy;
	this->pausarEnvioDeDatos = false;

	JuegoControlador::estadisticasJugadores->setJugador(idSnowBoy, name);
}

Cliente::~Cliente(){
	shutdown(this->socketCliente, SD_BOTH);
	closesocket(this->socketCliente);
}

void Cliente::reconectar(SOCKET newSocket){
	this->colaEventosEnviar.clear();
	shutdown(this->socketCliente, SD_BOTH);
	closesocket(this->socketCliente);
	this->estaConectado = true;
	this->socketCliente = newSocket;	
}

SOCKET Cliente::getSocket(){
	return this->socketCliente;
}

void Cliente::enqueueEventoEnviar(sEvento evento){
	//Lock::getLock()->start();
	this->colaEventosEnviar.push(evento);
	//Lock::getLock()->stop();
}

sEvento Cliente::dequeueEventoEnviar(){
	//Lock::getLock()->start();
	//sEvento eventoServidor = this->colaEventosEnviar.front();
	//this->colaEventosEnviar.pop();	
	//Lock::getLock()->stop();

	sEvento eventoServidor = this->colaEventosEnviar.pop();
	return eventoServidor;
}

bool Cliente::hayEventosParaEnviar(){
	//Lock::getLock()->start();
	bool estado = (this->colaEventosEnviar.size() > 0);
	//Lock::getLock()->stop();
	return estado;
}

string Cliente::getName(){
	return this->name;
}