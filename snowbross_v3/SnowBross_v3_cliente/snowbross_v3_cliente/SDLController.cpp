#include "stdafx.h"
#include "SDLController.h"
#include "Logger.h"
#include <iostream>

std::map<std::string,SDL_Texture*> SDLController::texturasCargadas;

SDL_Texture* SDLController::getTexturaCargada(std::string path){
	if( SDLController::texturasCargadas.find(path) == SDLController::texturasCargadas.end() ){
		// Textura no existe
		return nullptr;
	}
	return SDLController::texturasCargadas[path];
}

SDL_Texture* SDLController::loadTexture(const std::string &file, SDL_Renderer *renderer){
	SDL_Texture* texture = SDLController::getTexturaCargada(file);
	if (texture == nullptr){
		// Si no existe la textura en el mapa, levanto la textura de disco y luego la guardo.
		texture = IMG_LoadTexture(renderer, file.c_str());
		if (texture == nullptr){
			SDLController::logSDLError("SDL_Image: IMG_Init");
		}else{
			SDLController::texturasCargadas[file] = texture;
		}
	}
	return texture;
}

void SDLController::renderTexture(SDL_Texture *tex, SDL_Renderer *renderer, int x, int y, int w, int h){
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	dst.w = w;
	dst.h = h;
	SDL_RenderCopy(renderer, tex, NULL, &dst);
}

void SDLController::renderTexture(SDL_Texture *tex, SDL_Renderer *renderer, SDL_Rect dst, SDL_Rect *clip ){
	SDL_RenderCopy(renderer, tex, clip, &dst);
}


void SDLController::renderTexture(SDL_Texture *tex, SDL_Renderer *renderer, SDL_Rect *dst, SDL_Rect *clip ){
	SDL_RenderCopy(renderer, tex, clip, dst);
}

void SDLController::renderTexture(SDL_Texture *tex, SDL_Renderer *renderer, int x, int y, SDL_Rect *clip ){
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	if (clip != nullptr){
		dst.w = clip->w;
		dst.h = clip->h;
	}
	else {
		SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
	}
	SDLController::renderTexture(tex, renderer, dst, clip);
}

SDL_Texture* SDLController::renderText(const std::string &message, const std::string &fontFile, SDL_Color color, int fontSize, SDL_Renderer *renderer){
	TTF_Font *font = TTF_OpenFont(fontFile.c_str(), fontSize);
	SDL_Texture* textura = SDLController::renderText(message, font, color, renderer);
	TTF_CloseFont(font);
	return textura;
}

SDL_Texture* SDLController::renderText(const std::string &message, TTF_Font *font, SDL_Color color, SDL_Renderer *renderer){
	
	if (font == nullptr){
		SDLController::logSDLError("TTF_OpenFont");
		return nullptr;
	}	

	SDL_Surface *surf = TTF_RenderText_Solid(font, message.c_str(), color);
	if (surf == nullptr){
		TTF_CloseFont(font);
		SDLController::logSDLError("TTF_RenderText");
		return nullptr;
	}
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);
	if (texture == nullptr){
		SDLController::logSDLError("CreateTexture");
	}

	SDL_FreeSurface(surf);

	return texture;
}



void SDLController::logSDLError(std::string mensaje){
	Logger::getLogger()->error("Error SDL: "+mensaje + SDL_GetError());
	
}