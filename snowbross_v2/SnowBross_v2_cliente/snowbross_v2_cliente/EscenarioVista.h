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
		TTF_Font *font_header;

		//Contiene todos los objetos dibujables dentro del escenario
		map<int,Vista*> mapaObjeto;
		int idVistaSnowBoyCliente;

		void crearVentana();
		void dibujarFondo();
		void dibujarPanel();
		void dibujarObjetos();
		void eliminarObjetos();

		Camara* camara;

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

};

#endif
