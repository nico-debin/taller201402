#ifndef _estructuraDeDatos_H_
#define _estructuraDeDatos_H_

#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include "Constantes.h"

using namespace std;

//struct sPersonaje {
//    int id;
//	float x;
//	float y;
//};

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


// Struct a utilizar para enviar mediante sockets

struct sEvento {
	int id;
	int evento;
	TIPO_FIGURA tipo;
	float x,y;
	float rot; //angulo
	float ancho; // Para poligonos y circulos = radio*2 o escala*2
	float alto;  // Para poligonos y circulos = radio*2 o escala*2
	string color;

	// Solo para trapecios y paralelogramos
	float angulo_1;
	float angulo_2;

	// Se envian vertices para el dibujo particular del paralelogramo
	float verticesX[MAX_LADOS];
	float verticesY[MAX_LADOS];
};

struct sSprite{
	SDL_Rect sprites[3];
};

// Eventos enviados desde el Controlador Cliente al Servidor
struct sEventoCliente {
	int id;
	EVENTOS_RECIBIDOS_SERVIDOR evento;
};

#endif