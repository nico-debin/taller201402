#ifndef _Vista_H_
#define _Vista_H_

#include "SDLController.h"
#include "Constantes.h"
#include "EstructuraDeDatos.h"
#include "Utilities.h"
#include "Logger.h"

class Vista {
	protected:
		int id;
		string tipo;
		short lados;
		float x;
		float y;
		float ancho;
		float alto;
		float rot; // radianes
		int color_r;
		int color_g;
		int color_b;

		float angulo_1;
		float angulo_2;
		SDL_Texture* textura;
		// Textura de la vista que representa
		SDL_Texture* texturaObjeto;

		virtual void calcularVertices(){};

	public:
		//Utilizados para crear y actualizar elementos de la vista
		Vista( sEvento estadoEvento );		

		virtual void setEstado( sEvento estadoEvento );
		void setPosition(float x, float y);

		int getId();
		float get_x();
		float get_y();
		float get_ancho();
		float get_alto();
		float get_rot();
		int get_color_r();
		int get_color_g();
		int get_color_b();

		float get_angulo1();
		float get_angulo2();
		float get_base_menor();

		virtual void dibujar(SDL_Renderer* renderer) = 0;	
		virtual void dibujarTextura(SDL_Renderer* renderer) = 0;	
		//virtual sEvento getEventoEstado() = 0;
		
		//Método utilizado para calcular el resto de las variables a partir de los datos del evento
		//virtual void actualizarEstado() = 0;

		// Dibuja el centro x,y como un pixel blanco
		void dibujar_centro(SDL_Renderer* renderer);

	private:
		//virtual void cargarEstado() = 0;

};

#endif