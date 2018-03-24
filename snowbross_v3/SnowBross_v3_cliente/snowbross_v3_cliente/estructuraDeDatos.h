#ifndef _estructuraDeDatos_H_
#define _estructuraDeDatos_H_

#include <WinSock2.h>
#include <sstream>
#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include "Constantes.h"

using namespace std;

struct sObjeto {
    int id;
	TIPO_FIGURA tipo;
	float x;
	float y;

	float ancho;
	float alto;
	float radio;
	int lados;
	float escala;
	float baseMayor;
	float baseMenor;

	// Solo para trapecios y paralelogramos
	float angulo_1;
	float angulo_2;

	string color;
	float rot;
	float masa;
	bool estatico;
};

struct sEscenario {
	int alto_px;
    int ancho_px;
	float alto_ul;
    float ancho_ul;
    string imagen_fondo;
	//vector <sPersonaje> personajes;
	vector <sObjeto> objetos;
};


// Struct que el server envia al cliente para que pueda inicializar el escenario
struct sEscenarioCliente{
	int idSnowBoy;
	int alto_px;
    int ancho_px;
	char imagen_fondo[70];
};

// Struct que el server envia al cliente para que pueda obtener los nombres de los jugadores
struct sNombresClientes{
	int ids[4];
	char nombres[4][30];
};

// Struct a utilizar para enviar mediante sockets
struct sEvento {
	int id;
	int evento;
	TIPO_FIGURA tipo;
	int x,y;
	float rot; //angulo
	int ancho; // Para poligonos y circulos = radio*2 o escala*2
	int alto;  // Para poligonos y circulos = radio*2 o escala*2
	//string color;
};

struct sSprite{
	SDL_Rect sprites[3];
	short size;
};

// Eventos enviados desde el Controlador Cliente al Servidor
struct sEventoCliente {
	int id;
	EVENTOS_RECIBIDOS_SERVIDOR evento;
};
struct sEventoClienteConectar {
	char nombre_usuario[30];
};

#endif