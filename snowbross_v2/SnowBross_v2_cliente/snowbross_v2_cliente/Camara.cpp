#include "StdAfx.h"
#include "Camara.h"
#include "PersonajeVista.h"

/*#define SNOWBOY_PROPORCION_SCROLL_X_EN_PANTALLA 0.25f //Al scrollear el SnowBoy va a estar en 1/2 de la pantalla desde la izq. Si fuera 0.25f estaria en 1/4 de la pantalla desde la izq
#define SNOWBOY_PROPORCION_SCROLL_Y_EN_PANTALLA 0.75f //Al scrollear el SnowBoy va a estar en 3/4 de la pantalla desde arriba. Si fuera .025f estaria en 1/4 de la pantalla desde arriba   
#define ZOOM_INICIAL 2.0f
#define ZOOM_MAXIMO 4.1f
#define ZOOM_VARIACION 0.1f*/

Camara::Camara(void){
	this->x = 0;
	this->y = 0;
	this->zoom = ZOOM_INICIAL;
	this->cambioElZoom = true;
	this->actualizarAnchoAltoEscenarioPX();
}


void Camara::actualizarCamaraXY(sEvento &estadoSnowBoy){



	//this->x=estadoSnowBoy.x*this->zoom-Utilities::ANCHO_PX*SNOWBOY_PROPORCION_SCROLL_X_EN_PANTALLA;

	
	int diferenciaXSnowboyConCentroPantalla = estadoSnowBoy.x * this->zoom - (this->x +Utilities::ANCHO_PX/2);
	int deadZoneX = Utilities::ANCHO_PX/4;//Tamanio de la camara  en que el SnowBoy puede moverse sin  que se mueva la camara
	if (diferenciaXSnowboyConCentroPantalla > deadZoneX ){
		this->x += diferenciaXSnowboyConCentroPantalla - deadZoneX;		
	}
	else if (diferenciaXSnowboyConCentroPantalla < -deadZoneX ){
		this->x += diferenciaXSnowboyConCentroPantalla + deadZoneX;
	}
	if (this->x < 0){
		this->x = 0;
		//SI SE PASA DEL FONDO DE LA PANTALLA
	}
	if (this->x > this->xMax){
		this->x = this->xMax;
	}


	int ySnowBoyZoomeada = this->aplicarZoomCoordenadaY(estadoSnowBoy.y);

	int diferenciaYSnowboyConCentroPantalla = /*estadoSnowBoy.y * this->zoom*/ ySnowBoyZoomeada - (this->y +Utilities::ALTO_PX/2);
	int deadZoneY = Utilities::ALTO_PX/4;
	if (diferenciaYSnowboyConCentroPantalla > deadZoneY ){
		this->y += diferenciaYSnowboyConCentroPantalla - deadZoneY;
	}
	else if (diferenciaYSnowboyConCentroPantalla < -deadZoneY ){
		this->y += diferenciaYSnowboyConCentroPantalla + deadZoneY;
	}
	
//	this->y = ySnowBoyZoomeada-Utilities::ALTO_PX*SNOWBOY_PROPORCION_SCROLL_Y_EN_PANTALLA;
	if (this->y > 0){
		this->y = 0;
	}	
	if (this->y < this->yMin){
		this->y = this->yMin;
	}
}


Camara::~Camara(void){

}


SDL_Rect* Camara::getRectangulo(int anchoImagen, int altoImagen){

	SDL_Rect* clip=new SDL_Rect;
	
	float relacionCamaraImagenX=anchoImagen/this->anchoEscenarioPX;
	float relacionCamaraImagenY=altoImagen/this->altoEscenarioPX;


	//SE OBTIENEN X e Y EN COORDENADAS DE LA IMAGEN
	clip->x=this->x*relacionCamaraImagenX;
	//PRIMERO NIEGO Y LE SUMO LA PARTE NEGATIVA DEL ESCENARIO AL Y DE LA CAMARA PARA QUE TODO QUEDE ARRIBA DE CERO
	//DESPUES SACO ESE RESULTADO EN COORDENADAS DE LA IMAGEN
	clip->y=(-(this->yMin)+this->y)*relacionCamaraImagenY;


	//Obtengo la xMax en coordenadas de la imagen y eso lo resto al ancho de la imagen para saber el tamanio del clip
	clip->w= anchoImagen-this->xMax*relacionCamaraImagenX;
	//Similar al del ancho pero para el alto
	clip->h=altoImagen+this->yMin*relacionCamaraImagenY;
	
	return clip;
}


bool Camara::zoomIn(){
	this->cambioElZoom = false;
	if (this->zoom < ZOOM_MAXIMO){
		this->cambioElZoom = true;
		this->zoom+=ZOOM_VARIACION ;
		this->actualizarAnchoAltoEscenarioPX();
	}
	return this->cambioElZoom;
}

bool Camara::zoomOut(){
	this->cambioElZoom = true;
	this->zoom-=ZOOM_VARIACION;
	this->verificarLimites();
	return this->cambioElZoom;
}

void Camara::verificarLimites(){
	//Para ver las nuevas dimensiones
	this->actualizarAnchoAltoEscenarioPX();

	//PARA CONTROLAR QUE NO SE VAYA POR ANCHO
	if (this->anchoEscenarioPX + 0.001 < Utilities::ANCHO_PX){
		//Esto es: si el ancho del escenario en PX es menor al ancho de la pantalla
		this->zoom+=0.1f;
		this->cambioElZoom = false;
		this->actualizarAnchoAltoEscenarioPX();
	}
	else{
		//PARA CONTROLAR QUE NO SE VAYA POR ALTO
		if (this->altoEscenarioPX + 0.001< Utilities::ALTO_PX){
			//Si el alto del escenario en PX es menor al alto de la pantalla
			this->zoom+=0.1f;
			this->cambioElZoom = false;
			this->actualizarAnchoAltoEscenarioPX();
		}
	}

}

void Camara::actualizarAnchoAltoEscenarioPX(){

	this->anchoEscenarioPX = Utilities::ANCHO_PX *this->zoom;
	this->altoEscenarioPX = Utilities::ALTO_PX *this->zoom;


	this->xMax = this->anchoEscenarioPX-Utilities::ANCHO_PX;
	this->yMin = -(this->altoEscenarioPX-Utilities::ALTO_PX);
	//El yMin es cuanto se va por arriba el escenario, o sea, la parte negativa del escenario
}

void Camara::aplicarZoomScrollYDibujar(Vista* vista, SDL_Renderer* renderer){

	sEvento estadoAModificar = vista->getEstado();
	
	if( this->zoomUsados.find(this->zoom) == zoomUsados.end() ){
		this->aplicarZoom(estadoAModificar); 	
		//zoomUsados.insert(this->zoom);//Esto tiene que ir a otro lugar
		this->hayNuevoZoom = false;
		vista->setAnchoAltoZoom(estadoAModificar.ancho,estadoAModificar.alto,this->zoom);
	}
	

	this->aplicarScroll(estadoAModificar);
	if (this->cambioElZoom){
		vista->setZoomAUsar(this->zoom);
		//this->cambioElZoom = false;//Esto va en otro lugar
	}
	
	vista->setXYDeDibujo(estadoAModificar.x,estadoAModificar.y);
	vista->dibujar(renderer);
}


void Camara::aplicarZoom(sEvento &estadoAModificar){

	estadoAModificar.alto *= this->zoom;	
	estadoAModificar.ancho *= this->zoom;

/*	estadoAModificar.x *= this->zoom;
	estadoAModificar.y = this->aplicarZoomCoordenadaY(estadoAModificar.y);
*/
}


int Camara::aplicarZoomCoordenadaY(int yAModificar){
	int yOriginal = Utilities::sdl2euclid_ejeY(yAModificar);
	yOriginal *= this->zoom;  //Zoomeo la Y original
	return Utilities::euclid2sdl_ejeY(yOriginal);
}


float Camara::getZoom(){
	return this->zoom;
}

void Camara::aplicarScroll(sEvento &estadoEvento){
	estadoEvento.x *= this->zoom;
	estadoEvento.y = this->aplicarZoomCoordenadaY(estadoEvento.y);

	estadoEvento.x -= this->x;
	estadoEvento.y -= this->y;
}

void Camara::actualizar(){

	if (this->hayNuevoZoom){
		this->zoomUsados.insert(this->zoom);
		this->hayNuevoZoom = false;
	}
	if(this->cambioElZoom){this->cambioElZoom = false;}

}