#pragma once
#ifndef _PersonajeVista_H_
#define _PersonajeVista_H_

#include "Vista.h"
#include <map>
#include "ControladorCliente.h"

class PersonajeVista : public Vista {
	

		
protected:
		int estadoPersonaje;  // Sirven para saber el cambio de estado.  TODO: ANALIZAR SI ESTE TRATAMIENTO DEBERIA DARSE PARA TODAS LAS FIGURAS

		std::string        spriteSheetName; // Nombre de la plantilla, es seteada por el constructor del personaje que reimplemente la clase
		SDL_Texture*       spriteTexture;   // Textura que contiene la plantilla con sprites		
		map< int,sSprite*> SpriteMap;       // Contiene las coordenadas de los sprite discriminados por el estado		
		SDL_RendererFlip   flip;            // Utilizado para saber si el sprite debe dibujarse con orientación DERECHA o IZQUIERDA										
		int                useClip;         // Utilizado para cambiar de clip cada tantos ciclos de renderizado. TODO: Analizar remplazarlo por un temporizador
		int                actualClip;      // Contiene al Clip actual


		//Esta función template,  debe redefinirse dependiendo del sprite utilizado
		virtual void inicializacionEstados()=0;
		void calcularVertices(){}

		int getEstadoPersonaje();

public:
		PersonajeVista( sEvento estadoEvento );
		~PersonajeVista();

		virtual void dibujar( SDL_Renderer* renderer );

		virtual void setEstado( sEvento estadoEvento );
		void setEstadoPersonaje( int nuevoEstado );
};


#endif
