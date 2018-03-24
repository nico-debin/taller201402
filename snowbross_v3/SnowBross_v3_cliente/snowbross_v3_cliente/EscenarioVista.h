#ifndef _EscenarioVista_H_
#define _EscenarioVista_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "Vista.h"
#include "Logger.h"
#include "Constantes.h"
#include <map>
#include "estructuraDeDatos.h"
#include "Utilities.h"
#include <string>
#include "Camara.h"
#include "ControlSonido.h"

class ControladorCliente;

class EscenarioVista {

	private:
		// Test
		SDL_Texture* spriteTexture;
		int useClip;

		SDL_Window* window;
	    SDL_Renderer* renderer;

		// Textura del fondo principal del escenario
		SDL_Texture* texturaFondo;
		int anchoTexturaFondo, altoTexturaFondo;
		SDL_Texture* texturaPuntaje;
		SDL_Texture* texturaVidas;
		SDL_Texture* texturaVidasHeart;
		TTF_Font *font_header;
		TTF_Font *font_header_54;

		//Contiene todos los objetos dibujables dentro del escenario
		map<int,Vista*> mapaObjeto;
		int idVistaSnowBoyCliente;

		void crearVentana();
		void dibujarFondo();
		void dibujarPanel();
		void dibujarObjetos();
		void dibujarEscenarioCompletado();
		void dibujarEscenarioGameOver();
		void dibujarJuegoCompletado();
		void dibujarCartelReinicio();

		Camara* camara;
		ControlSonido controlSonido;

	public:
		EscenarioVista();
		~EscenarioVista();

		void dibujar();
		bool dibujarZoomIn();
		bool dibujarZoomOut();

		void addObjeto(int id, Vista* vista);
		bool existeObjeto(int id);
		Vista* getObjeto(int id);
		void elminarObjeto(int id);
		void eliminarObjetos(bool eliminarSnowboys = false);

		bool statusMostrarEscenarioCompletado;
		bool statusMostrarEscenarioGameOver;
		bool statusMostrarJuegoCompletado;

		bool statusMostrarCartelTeclas;
};

#endif
