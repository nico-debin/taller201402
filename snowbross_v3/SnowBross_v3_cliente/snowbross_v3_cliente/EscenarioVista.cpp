#include "stdafx.h"
#include "EscenarioVista.h"
#include "SDLController.h"
#include "PersonajeVista.h"
#include "NieveVista.h"
#include <vector>

#include "ControladorCliente.h"

EscenarioVista::EscenarioVista(){
	this->font_header = nullptr;
	this->crearVentana();
	this->texturaFondo = nullptr;
	this->texturaPuntaje = nullptr;
	this->texturaVidas = nullptr;
	this->camara = new Camara;

	this->statusMostrarEscenarioCompletado = false;
	this->statusMostrarEscenarioGameOver = false;
	this->statusMostrarJuegoCompletado = false;

	this->statusMostrarCartelTeclas = true;
}

EscenarioVista::~EscenarioVista(){
	this->eliminarObjetos();
	delete this->camara;

	this->controlSonido.tocar(ControlRemoto::PARAR, Temas::MUSICA);

	SDL_DestroyTexture(this->texturaVidasHeart);
	SDL_DestroyTexture(this->spriteTexture);

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
		this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED);// | SDL_RENDERER_PRESENTVSYNC);
		//this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_SOFTWARE);

		if (this->renderer == NULL){
			Logger::getLogger()->error( "SDL_CreateRenderer Error: " );
			Logger::getLogger()->error( SDL_GetError());
		}else{
			this->font_header = TTF_OpenFont("fonts/arial.ttf", 18);
			this->font_header_54 = TTF_OpenFont("fonts/arial.ttf", 54);
		}
	}


	// Creacion de sprite
	this->texturaVidasHeart = SDLController::loadTexture("imagenes/heart.png",this->renderer);
	this->spriteTexture = SDLController::loadTexture("imagenes/SnowBrosSheet1.gif",this->renderer);
	this->useClip = 1;

	this->controlSonido.tocar(ControlRemoto::REPRODUCIR, Temas::MUSICA);
}

void EscenarioVista::addObjeto(int id, Vista* vista){
	this->mapaObjeto[id] = vista;

	if (vista->getId() == Utilities::ID_SNOWBOY){
		this->idVistaSnowBoyCliente = id;
	}
}

Vista* EscenarioVista::getObjeto(int id){
	return this->mapaObjeto[id];
}

bool EscenarioVista::existeObjeto(int id){
	return ( this->mapaObjeto.find( id ) != this->mapaObjeto.end() );
}

void EscenarioVista::elminarObjeto(int id){
    //delete this->mapaObjeto[id];

	if (this->mapaObjeto[id]->getEstado().tipo == TIPO_NIEVE){
		delete ((NieveVista*) this->mapaObjeto[id]);
	}
	else{
		delete this->mapaObjeto[id];
	}

	this->mapaObjeto.erase(id);
}

void EscenarioVista::eliminarObjetos(bool eliminarSnowboys){
	
	vector<Vista*> snowboys;
	Vista* vista;
	for( map<int,Vista*>::iterator it = this->mapaObjeto.begin() ; it != this->mapaObjeto.end(); ++it){
		vista = (*it).second;
		if( eliminarSnowboys == false  &&  vista->getTipo() == SNOWBOY ){
			snowboys.push_back(vista);
			continue;
		}
		//delete vista;
	}
	this->mapaObjeto.clear();

	for( vector<Vista*>::iterator itSnowboys = snowboys.begin() ; itSnowboys != snowboys.end(); ++itSnowboys){
		vista = *itSnowboys;
		this->addObjeto( vista->getId(), vista );
	}
	
}

void EscenarioVista::dibujar(){
	// Color de fondo por defecto si no hay fondo
	SDL_SetRenderDrawColor(this->renderer, 168, 230, 255, 255);

	SDL_RenderClear(this->renderer);

	if(this->existeObjeto(this->idVistaSnowBoyCliente)){
		sEvento estadoDelSnowBoyCliente = this->getObjeto(this->idVistaSnowBoyCliente)->getEstado();
		this->camara->actualizarCamaraXY(estadoDelSnowBoyCliente);
	}

	this->dibujarFondo();
	this->dibujarObjetos();
	this->dibujarPanel();

	if( this->statusMostrarEscenarioCompletado ){
		this->dibujarEscenarioCompletado();
	} else if ( this->statusMostrarEscenarioGameOver ){
		this->dibujarEscenarioGameOver();
	} else if ( this->statusMostrarJuegoCompletado ){
		this->dibujarJuegoCompletado();
	}

	SDL_RenderPresent(this->renderer);
}

void EscenarioVista::dibujarFondo(){

	this->texturaFondo = SDLController::loadTexture(Utilities::IMAGEN_FONDO_NIVEL1, this->renderer);
		
	if (this->texturaFondo==nullptr){
		//Entrar aca significaria que no se pudo cargar la textura de IMAGEN_FONDO_NIVEL1
		Logger::getLogger()->error("Se utiliza la imagen de fondo por default: "+(string)IMAGEN_FONDO_DEFAULT);
		this->texturaFondo = SDLController::loadTexture(IMAGEN_FONDO_DEFAULT, this->renderer);
	}
	SDL_QueryTexture(this->texturaFondo,NULL,NULL,&this->anchoTexturaFondo, &this->altoTexturaFondo);
	

	SDL_Rect* clip = this->camara->getRectangulo(this->anchoTexturaFondo,this->altoTexturaFondo);
	SDLController::renderTexture(this->texturaFondo, this->renderer, NULL, clip);
	delete clip;
}

void EscenarioVista::dibujarPanel(){
	const int cantJugadores = 4;
	const int hearthWidth = 18;
	SDL_Color color = {0,0,0,0};
	SDL_Texture* textoPuntaje = nullptr;
	int anchoPuntaje, altoPuntaje;
	
	vector<estadisticaJugador> estadisticas = ControladorCliente::estadisticasJugadores->getAllEstadisticas();
	for(int i=0 ; i<estadisticas.size() ; i++){

		estadisticaJugador estadistica = estadisticas[i];

		int x = Utilities::redondeo(((float)i/cantJugadores)*Utilities::ANCHO_PX);

		// Muestro barra de vidas
		for(int j=0 ; j<estadistica.vidas ; j++){
			SDLController::renderTexture(this->texturaVidasHeart, this->renderer, x+(j*hearthWidth), 0);
		}

		// Muestro nombre y puntaje de cada jugador
		stringstream mensaje;
		mensaje << estadistica.nombre << ": " << estadistica.puntaje;

		textoPuntaje = SDLController::renderText(mensaje.str(), this->font_header, color, this->renderer);
		SDL_QueryTexture(textoPuntaje, NULL, NULL, &anchoPuntaje, &altoPuntaje);
		
		SDLController::renderTexture(textoPuntaje, this->renderer, x, 18, anchoPuntaje, altoPuntaje);
		SDL_DestroyTexture(textoPuntaje);
	}

	
}

void EscenarioVista::dibujarObjetos(){
	Vista* vista;
	PersonajeVista* personajeVista;
	vector<PersonajeVista*> vectorPersonajes;

	for( map<int,Vista*>::iterator it = this->mapaObjeto.begin() ; it != this->mapaObjeto.end(); ++it){
		vista = (*it).second;

		// Si es un personaje, lo dibujo al final de todo asi aparece por sobre todas las demas figuras
		personajeVista = dynamic_cast<PersonajeVista*>(vista);
		if(personajeVista == 0){
			this->camara->aplicarZoomScrollYDibujar(vista, renderer);
		}else{
			vectorPersonajes.push_back(personajeVista);
		}
	}

	// Dibujo los personajes
	for( vector<PersonajeVista*>::iterator it_p = vectorPersonajes.begin() ; it_p != vectorPersonajes.end(); ++it_p){
		personajeVista = *it_p;
		this->camara->aplicarZoomScrollYDibujar(personajeVista, renderer);
	}
	
	//Esto actualiza los zooms por los que paso y si cambio o no el zoom
	this->camara->actualizar();
}

void EscenarioVista::dibujarEscenarioCompletado(){
	// Fondo oscuro
	short vertices_x[4] = {0,Utilities::ANCHO_PX,Utilities::ANCHO_PX,0};
	short vertices_y[4] = {0,0,Utilities::ALTO_PX,Utilities::ALTO_PX};
	filledPolygonRGBA(renderer,vertices_x,vertices_y,4,0,0,0,200);

	// Texto
	SDL_Color colorTexto = {255,255,255,0};
	SDL_Texture* texturaTexto = nullptr;
	int anchoTexto, altoTexto;

	stringstream mensaje;
	mensaje << "Nivel Superado!";

	texturaTexto = SDLController::renderText(mensaje.str(), this->font_header_54, colorTexto, this->renderer);
	SDL_QueryTexture(texturaTexto, NULL, NULL, &anchoTexto, &altoTexto);

	int x_texto = Utilities::redondeo( (Utilities::ANCHO_PX - anchoTexto)/2 );
	int y_texto = Utilities::redondeo( (Utilities::ALTO_PX*0.25f) - (altoTexto/2) );

	// Dibujo Recuadro
	int expansion = 50;
	short vertices_recuadro_x[4] = {x_texto-expansion, x_texto+anchoTexto+expansion, x_texto+anchoTexto+expansion, x_texto-expansion };
	short vertices_recuadro_y[4] = {y_texto-expansion, y_texto-expansion, y_texto+altoTexto+expansion, y_texto+altoTexto+expansion };
	filledPolygonRGBA(renderer,vertices_recuadro_x,vertices_recuadro_y,4,128,128,128,255);

	// Dibujo Texto
	SDLController::renderTexture(texturaTexto, this->renderer, x_texto, y_texto, anchoTexto, altoTexto);
	SDL_DestroyTexture(texturaTexto);
}

void EscenarioVista::dibujarEscenarioGameOver(){
	// Fondo rojo
	short vertices_x[4] = {0,Utilities::ANCHO_PX,Utilities::ANCHO_PX,0};
	short vertices_y[4] = {0,0,Utilities::ALTO_PX,Utilities::ALTO_PX};
	filledPolygonRGBA(renderer,vertices_x,vertices_y,4,255,0,0,200);

	// Texto
	SDL_Color colorTexto = {255,255,255,0};
	SDL_Texture* texturaTexto = nullptr;
	int anchoTexto, altoTexto;

	stringstream mensaje;
	mensaje << "GAME OVER";

	texturaTexto = SDLController::renderText(mensaje.str(), this->font_header_54, colorTexto, this->renderer);
	SDL_QueryTexture(texturaTexto, NULL, NULL, &anchoTexto, &altoTexto);

	int x_texto = Utilities::redondeo( (Utilities::ANCHO_PX - anchoTexto)/2 );
	int y_texto = Utilities::redondeo( (Utilities::ALTO_PX*0.25f) - (altoTexto/2) );

	// Dibujo Recuadro
	int expansion = 50;
	short vertices_recuadro_x[4] = {x_texto-expansion, x_texto+anchoTexto+expansion, x_texto+anchoTexto+expansion, x_texto-expansion };
	short vertices_recuadro_y[4] = {y_texto-expansion, y_texto-expansion, y_texto+altoTexto+expansion, y_texto+altoTexto+expansion };
	filledPolygonRGBA(renderer,vertices_recuadro_x,vertices_recuadro_y,4,128,128,128,255);

	// Dibujo Texto
	SDLController::renderTexture(texturaTexto, this->renderer, x_texto, y_texto, anchoTexto, altoTexto);
	SDL_DestroyTexture(texturaTexto);

	this->dibujarCartelReinicio();
}

void EscenarioVista::dibujarJuegoCompletado(){
	// Fondo verde
	short vertices_x[4] = {0,Utilities::ANCHO_PX,Utilities::ANCHO_PX,0};
	short vertices_y[4] = {0,0,Utilities::ALTO_PX,Utilities::ALTO_PX};
	filledPolygonRGBA(renderer,vertices_x,vertices_y,4,0,255,0,200);

	// Texto
	SDL_Color colorTexto = {255,255,255,0};
	SDL_Texture* texturaTexto = nullptr;
	int anchoTexto, altoTexto;

	stringstream mensaje;
	mensaje << "Haz ganado el Juego!";

	texturaTexto = SDLController::renderText(mensaje.str(), this->font_header_54, colorTexto, this->renderer);
	SDL_QueryTexture(texturaTexto, NULL, NULL, &anchoTexto, &altoTexto);

	int x_texto = Utilities::redondeo( (Utilities::ANCHO_PX - anchoTexto)/2 );
	int y_texto = Utilities::redondeo( (Utilities::ALTO_PX*0.25f) - (altoTexto/2) );

	// Dibujo Recuadro
	int expansion = 50;
	short vertices_recuadro_x[4] = {x_texto-expansion, x_texto+anchoTexto+expansion, x_texto+anchoTexto+expansion, x_texto-expansion };
	short vertices_recuadro_y[4] = {y_texto-expansion, y_texto-expansion, y_texto+altoTexto+expansion, y_texto+altoTexto+expansion };
	filledPolygonRGBA(renderer,vertices_recuadro_x,vertices_recuadro_y,4,128,128,128,255);

	// Dibujo Texto
	SDLController::renderTexture(texturaTexto, this->renderer, x_texto, y_texto, anchoTexto, altoTexto);
	SDL_DestroyTexture(texturaTexto);

	this->dibujarCartelReinicio();
}

void EscenarioVista::dibujarCartelReinicio(){

	if(this->statusMostrarCartelTeclas){
		// Texto
		SDL_Color colorTexto = {255,255,255,0};
		SDL_Texture* texturaTexto = nullptr;
		int anchoTexto, altoTexto;

		stringstream mensaje;
		mensaje << "Presiona [R] para reiniciar o [Q] para salir";

		texturaTexto = SDLController::renderText(mensaje.str(), this->font_header, colorTexto, this->renderer);
		SDL_QueryTexture(texturaTexto, NULL, NULL, &anchoTexto, &altoTexto);

		int x_texto = Utilities::redondeo( (Utilities::ANCHO_PX - anchoTexto)/2 );
		int y_texto = Utilities::redondeo( (Utilities::ALTO_PX*0.75f) - (altoTexto/2) );

		// Dibujo Recuadro
		int expansion = 50;
		short vertices_recuadro_x[4] = {x_texto-expansion, x_texto+anchoTexto+expansion, x_texto+anchoTexto+expansion, x_texto-expansion };
		short vertices_recuadro_y[4] = {y_texto-expansion, y_texto-expansion, y_texto+altoTexto+expansion, y_texto+altoTexto+expansion };
		filledPolygonRGBA(renderer,vertices_recuadro_x,vertices_recuadro_y,4,128,128,128,255);

		// Dibujo Texto
		SDLController::renderTexture(texturaTexto, this->renderer, x_texto, y_texto, anchoTexto, altoTexto);
		SDL_DestroyTexture(texturaTexto);
	}else{
		// Texto
		SDL_Color colorTexto = {255,255,255,0};
		SDL_Texture* texturaTexto = nullptr;
		int anchoTexto, altoTexto;

		stringstream mensaje;
		mensaje << "Esperando a los demas jugadores...";

		texturaTexto = SDLController::renderText(mensaje.str(), this->font_header, colorTexto, this->renderer);
		SDL_QueryTexture(texturaTexto, NULL, NULL, &anchoTexto, &altoTexto);

		int x_texto = Utilities::redondeo( (Utilities::ANCHO_PX - anchoTexto)/2 );
		int y_texto = Utilities::redondeo( (Utilities::ALTO_PX*0.75f) - (altoTexto/2) );

		// Dibujo Recuadro
		int expansion = 50;
		short vertices_recuadro_x[4] = {x_texto-expansion, x_texto+anchoTexto+expansion, x_texto+anchoTexto+expansion, x_texto-expansion };
		short vertices_recuadro_y[4] = {y_texto-expansion, y_texto-expansion, y_texto+altoTexto+expansion, y_texto+altoTexto+expansion };
		filledPolygonRGBA(renderer,vertices_recuadro_x,vertices_recuadro_y,4,128,128,128,255);

		// Dibujo Texto
		SDLController::renderTexture(texturaTexto, this->renderer, x_texto, y_texto, anchoTexto, altoTexto);
		SDL_DestroyTexture(texturaTexto);
	}
}

bool EscenarioVista::dibujarZoomIn(){
	return this->camara->zoomIn();
}

bool EscenarioVista::dibujarZoomOut(){
	return this->camara->zoomOut();
}


