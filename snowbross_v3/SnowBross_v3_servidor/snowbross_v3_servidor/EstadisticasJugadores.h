#ifndef _EstadisticasJugadores_H_
#define _EstadisticasJugadores_H_

#include <vector>
#include <map>
#include "Constantes.h"
#include "SnowWorldContactListener.h"

using namespace std;

struct estadisticaJugador {
	int idJugador;
	string nombre;
	int vidas;
	int puntaje;
};

class EstadisticasJugadores{
	private:
		map<int,estadisticaJugador> estadisticas;
	public:

		bool existeJugador(int id);
		void setJugador(int id, string nombre, int vidas = MAX_VIDAS, int puntaje = 0);
		vector<estadisticaJugador> getAllEstadisticas();
		estadisticaJugador getEstadisticaJugador(int id);
		string getNombreJugador(int id);

		int getVidasJugador(int id);
		void setVidas(int id, int vidas);
		int agregarVida(int id);
		int quitarVida(int id);

		int getPuntosJugador(int id);
		void setPuntos(int id, int puntos);
		int agregarPuntos(int id, int puntos);
		int quitarPuntos(int id, int puntos);

		void reiniciarEstadisticas();

		bool isGameOver();
};

#endif