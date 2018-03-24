#include "stdafx.h"
#include "EstadisticasJugadores.h"
#include "Logger.h"
#include "Escenario.h"

bool EstadisticasJugadores::existeJugador(int id){
	return ( this->estadisticas.find(id) != this->estadisticas.end() );
}

void EstadisticasJugadores::setJugador(int id, string nombre, int vidas, int puntaje){
	estadisticaJugador jugador;
	jugador.idJugador = id;
	jugador.nombre = nombre;
	jugador.vidas = vidas;
	jugador.puntaje = puntaje;
	this->estadisticas[id] = jugador;
}

vector<estadisticaJugador> EstadisticasJugadores::getAllEstadisticas(){
	vector<estadisticaJugador> vec;
	for (map<int,estadisticaJugador>::iterator it=this->estadisticas.begin(); it!=this->estadisticas.end(); ++it){
		vec.push_back(it->second);
	}
	return vec;
}

estadisticaJugador EstadisticasJugadores::getEstadisticaJugador(int id){
	return this->estadisticas[id];
}

string EstadisticasJugadores::getNombreJugador(int id){
	return this->estadisticas[id].nombre;
}

int EstadisticasJugadores::getVidasJugador(int id){
	return this->estadisticas[id].vidas;
}

void EstadisticasJugadores::setVidas(int id, int vidas){
	estadisticaJugador jugador = this->estadisticas[id];
	jugador.vidas = vidas;
	this->estadisticas[id] = jugador;
}

int EstadisticasJugadores::agregarVida(int id){
	estadisticaJugador jugador = this->estadisticas[id];
	jugador.vidas += 1;
	this->estadisticas[id] = jugador;
	return jugador.vidas;
}

int EstadisticasJugadores::quitarVida(int id){
	estadisticaJugador jugador = this->estadisticas[id];
	jugador.vidas -= 1;
	this->estadisticas[id] = jugador;
	return jugador.vidas;
}


int EstadisticasJugadores::getPuntosJugador(int id){
	return this->estadisticas[id].puntaje;
}

void EstadisticasJugadores::setPuntos(int id, int puntos){
	estadisticaJugador jugador = this->estadisticas[id];
	jugador.puntaje = puntos;
	this->estadisticas[id] = jugador;
}

int EstadisticasJugadores::agregarPuntos(int id, int puntos){
	estadisticaJugador jugador = this->estadisticas[id];
	jugador.puntaje += puntos;
	this->estadisticas[id] = jugador;
	return jugador.puntaje;
}

int EstadisticasJugadores::quitarPuntos(int id, int puntos){
	estadisticaJugador jugador = this->estadisticas[id];
	jugador.puntaje -= puntos;
	if(jugador.puntaje < 0) jugador.puntaje = 0;
	this->estadisticas[id] = jugador;
	return jugador.puntaje;
}

void EstadisticasJugadores::reiniciarEstadisticas(){
	estadisticaJugador estadistica;
	for (map<int,estadisticaJugador>::iterator it=this->estadisticas.begin(); it!=this->estadisticas.end(); ++it){
		estadistica = it->second;
		it->second.puntaje = 0;
		it->second.vidas = MAX_VIDAS;
		//estadistica.vidas = MAX_VIDAS;
		//estadistica.puntaje = 0;
		map<int,Figura*>* figuras = Escenario::getFiguras();
		SnowBoy* snowBoy = (SnowBoy*)figuras->at( estadistica.idJugador );
		//snowBoy->setEstado(SNOWBOY_STEPS_PARA_RENACER);
		snowBoy->renacer();	
		//// Si el snowboy fue eliminado lo vuelvo a crear
		//if ( figuras->find( estadistica.idJugador ) == figuras->end() ){

		//	Escenario::agregarFigura(figura);
		//}
	}
}

bool EstadisticasJugadores::isGameOver(){
	for (map<int,estadisticaJugador>::iterator it=this->estadisticas.begin(); it!=this->estadisticas.end(); ++it){
		estadisticaJugador estadistica = (estadisticaJugador)it->second;
		if ( estadistica.vidas > 0 ) {
			return false;
		}
	}
	return true;
}
