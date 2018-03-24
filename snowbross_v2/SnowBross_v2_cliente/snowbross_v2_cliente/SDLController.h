#pragma once
#ifndef _SDLCONTROLLER_H_
#define _SDLCONTROLLER_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL2_rotozoom.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>

class SDLController {
	private:
		static void logSDLError(std::string);
	public:
		/**
		* Carga una imagen en una textura en el renderer
		* Loads an image into a texture on the rendering device
		* @param file La imagen que se desea cargar
		* @param renderer El renderer donde cargar la textura
		* @return la textura cargada, o nullptr si algo salió mal.
		*/
		static SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *renderer);

		/**
		* Dibuja una SDL_Texture en un SDL_Renderer en la posicion x,y, con el ancho y alto deseado
		* @param tex La textura que queremos dibujar
		* @param renderer El renderer donde queremos dibujar la textura
		* @param x Coordenada x donde queremos dibujar
		* @param y Coordenada y donde queremos dibujar
		* @param w El ancho de la textura a dibujar
		* @param h El alto de la textura a dibujar
		*/
		static void renderTexture(SDL_Texture *tex, SDL_Renderer *renderer, int x, int y, int w, int h);

		/**
		* Dibuja una SDL_Texture en un SDL_Renderer en algun rectangulo destino, tomando una 
		* porcion (clip) de la textura (si se desea).
		* @param tex La textura que queremos dibujar
		* @param renderer El renderer donde queremos dibujar la textura
		* @param dst El rectangulo de destino donde se renderizara la textura
		* @param clip sub-seccion de la textura a dibujar (clipping rect)
		*		default nullptr dibuja toda la textura
		*/
		static void renderTexture(SDL_Texture *tex, SDL_Renderer *renderer, SDL_Rect dst, SDL_Rect *clip = nullptr);
		static void renderTexture(SDL_Texture *tex, SDL_Renderer *renderer, SDL_Rect *dst, SDL_Rect *clip );

		/**
		* Dibuja una SDL_Texture en un SDL_Renderer en la posicion x,y, preservando el ancho y alto de la textura
		* y tomando una porcion (clip) de la textura (si se desea).
		* Si se pasa un clip, se usará su ancho y alto en lugar del de la textura.
		* @param tex La textura que queremos dibujar
		* @param renderer El renderer donde queremos dibujar la textura
		* @param x Coordenada x donde queremos dibujar
		* @param y Coordenada y donde queremos dibujar
		* @param clip sub-seccion de la textura a dibujar (clipping rect)
		*		default nullptr dibuja toda la textura
		*/
		static void renderTexture(SDL_Texture *tex, SDL_Renderer *renderer, int x, int y, SDL_Rect *clip = nullptr);

		/**
		* Muestra el mensaje de texto que deseamos mostrar a una textura para dibujar
		* @param message El mensaje que queremos mostrar
		* @param fontFile El font que queremos usar para mostrar el texto
		* @param color Color del texto
		* @param fontSize Tamaño del font
		* @param renderer El renderer donde cargar la textura
		* @return SDL_Texture conteniendo el mensaje renderizado, o nullptr si algo salió mal.
		*/
		static SDL_Texture* renderText(const std::string &message, const std::string &fontFile, SDL_Color color, int fontSize, SDL_Renderer *renderer);
		static SDL_Texture* renderText(const std::string &message, TTF_Font *font, SDL_Color color, SDL_Renderer *renderer);
};

#endif /*_SDLCONTROLLER_H_*/