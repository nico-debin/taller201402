#include "stdafx.h"
#include "EscenarioVista.h"
#include "SDLController.h"
#include "PersonajeVista.h"
#include "CirculoVista.h"
#include <vector>
EscenarioVista::EscenarioVista(){
	this->font_header = nullptr;
	this->crearVentana();
	this->texturaFondo = nullptr;
	this->texturaPuntaje = nullptr;
	this->texturaVidas = nullptr;
}

EscenarioVista::~EscenarioVista(){
	this->eliminarObjetos();
	TTF_CloseFont(this->font_header);
	SDL_DestroyRenderer(this->renderer);
	SDL_DestroyWindow(this->window);
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

void EscenarioVista::crearVentana(){
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0){
		Logger::getLogger()->error( "SDL_Init Error: " );
		Logger::getLogger()->error( SDL_GetError());
		SDL_Quit();
    }

	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG){
		Logger::getLogger()->error( "IMG_Init Error");
		Logger::getLogger()->error( SDL_GetError());
		SDL_Quit();
	}

	if (TTF_Init() != 0){
		Logger::getLogger()->error( "TTF_Init: " );
		Logger::getLogger()->error( SDL_GetError());
		SDL_Quit();
	}

	// Creacion de la ventana
	this->window = SDL_CreateWindow(NOMBRE_JUEGO, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Utilities::ANCHO_PX, Utilities::ALTO_PX, SDL_WINDOW_SHOWN);

	if (this->window == NULL){
		Logger::getLogger()->error( "SDL_CreateWindow Error: " );
		Logger::getLogger()->error( SDL_GetError());
	}else{
		// Creacion del renderer
		//this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_SOFTWARE);

		if (this->renderer == NULL){
			Logger::getLogger()->error( "SDL_CreateRenderer Error: " );
			Logger::getLogger()->error( SDL_GetError());
		}else{
			this->font_header = TTF_OpenFont("fonts/justabit.ttf", 50);
		}
	}


	// Creacion de sprite
	this->spriteTexture = SDLController::loadTexture("imagenes/SnowBrosSheet1.gif",this->renderer);
	this->useClip = 1;
}

void EscenarioVista::addObjeto(int id, Vista* vista)
{
	this->mapaObjeto[id] = vista;
}

Vista* EscenarioVista::getObjeto(int id)
{
	return this->mapaObjeto[id];
}

bool EscenarioVista::existeObjeto(int id){
	return ( this->mapaObjeto.find( id ) != this->mapaObjeto.end() );
}

void EscenarioVista::elminarObjeto(int id)
{
	this->mapaObjeto.erase(id);
}

void EscenarioVista::dibujar(){
	// Color de fondo por defecto si no hay fondo
	SDL_SetRenderDrawColor(this->renderer, 168, 230, 255, 255);

	SDL_RenderClear(this->renderer);

	this->dibujarFondo();
	this->dibujarPanel();
	this->dibujarObjetos();
	

	//SDL_Color color = { 255, 255, 255, 255 };
	//SDL_Texture *texturaTexto = SDLController::renderText("Usar las flechas para mover el circulo.", this->font_header,color, this->renderer);
	//SDLController::renderTexture(texturaTexto, this->renderer, 0, 0);
	//SDL_DestroyTexture(texturaTexto);

	SDL_RenderPresent(this->renderer);
}

void EscenarioVista::dibujarPanel(){
	string messagePuntaje ="PUNTAJE: 250";
	string messageVidas ="VIDAS: 2";
	SDL_Color color;
	color.a=0;
	color.b=0;
	color.g=0;
	color.r=0;
	SDL_Texture* textoPuntaje = NULL; 
	SDL_Texture* textoVidas = NULL; 
	
	if(this->texturaPuntaje == nullptr){
		this->texturaPuntaje = SDLController::loadTexture("imagenes/FondoVidas.png",this->renderer);
		
		if (this->texturaPuntaje==nullptr){
			Logger::getLogger()->error("Se utiliza la imagen de fondo por default: "+(string)IMAGEN_FONDO_DEFAULT);
			this->texturaPuntaje = SDLController::loadTexture(IMAGEN_FONDO_DEFAULT, this->renderer);
		}
	}
	SDLController::renderTexture(this->texturaPuntaje, this->renderer, 0, 0, Utilities::ANCHO_PX*0.2f, Utilities::ALTO_PX*0.1f);
	SDLController::renderTexture(this->texturaPuntaje, this->renderer, Utilities::ANCHO_PX*0.2f, 0, Utilities::ANCHO_PX*0.2f, Utilities::ALTO_PX*0.1f);
	textoPuntaje= SDLController::renderText(messagePuntaje, "fonts/justabit.ttf", color, 14, this->renderer);
	textoVidas= SDLController::renderText(messageVidas, "fonts/justabit.ttf", color, 14, this->renderer);
	SDLController::renderTexture(textoPuntaje, this->renderer, 15, 5, Utilities::ANCHO_PX*0.1f, Utilities::ALTO_PX*0.07f);
	SDLController::renderTexture(textoVidas, this->renderer,Utilities::ANCHO_PX*0.2f+15, 5, Utilities::ANCHO_PX*0.1f, Utilities::ALTO_PX*0.07f);
	SDL_DestroyTexture(textoVidas);
	SDL_DestroyTexture(textoPuntaje);
	
	
}

void EscenarioVista::dibujarFondo(){
	if(this->texturaFondo == nullptr){
		this->texturaFondo = SDLController::loadTexture(Utilities::IMAGEN_FONDO_NIVEL1, this->renderer);
		
		if (this->texturaFondo==nullptr){
			//Entrar aca significaria que no se pudo cargar la textura de IMAGEN_FONDO_NIVEL1
			Logger::getLogger()->error("Se utiliza la imagen de fondo por default: "+(string)IMAGEN_FONDO_DEFAULT);
			this->texturaFondo = SDLController::loadTexture(IMAGEN_FONDO_DEFAULT, this->renderer);
		}
	}
	SDLController::renderTexture(this->texturaFondo, this->renderer, 0, 0, Utilities::ANCHO_PX, Utilities::ALTO_PX);
}

void EscenarioVista::dibujarObjetos(){
	Vista* vista;
	PersonajeVista* personajeVista;
	CirculoVista* circuloVista;
	vector<PersonajeVista*> vectorPersonajes;
	for( map<int,Vista*>::iterator it = this->mapaObjeto.begin() ; it != this->mapaObjeto.end(); ++it){
		vista = (*it).second;

		// Si es un personaje, lo dibujo al final de todo asi aparece por sobre todas las demas figuras
		personajeVista = dynamic_cast<PersonajeVista*>(vista);
		circuloVista = dynamic_cast<CirculoVista*>(vista);
		if( circuloVista!= 0){
			vista->dibujarTextura( renderer );	
		}else{
			if(personajeVista == 0){
				vista->dibujar( renderer );	
			}else{
				vectorPersonajes.push_back(personajeVista);
			}
		}
		
		// Debug para dibujar con vertices directos de box2d
		//if ( this->mapaObjetoFisicos.find( (*it).second->getId() ) != this->mapaObjetoFisicos.end() ) {
		//	this->mapaObjetoFisicos[ (*it).second->getId() ]->dibujar(renderer);
		//}
	}

	// Dibujo los personajes
	for( vector<PersonajeVista*>::iterator it_p = vectorPersonajes.begin() ; it_p != vectorPersonajes.end(); ++it_p){
		personajeVista = *it_p;
		personajeVista->dibujar( renderer );	
	}
}

void EscenarioVista::eliminarObjetos(){
	Vista* vista;
	for( map<int,Vista*>::iterator it = this->mapaObjeto.begin() ; it != this->mapaObjeto.end(); ++it){
		vista = (*it).second;
		//delete vista;
	}
}