#include "StdAfx.h"
#include "Figura.h"
#include "SnowBoy.h"

Figura::Figura(){
	this->datos.estatico = false;
	
	//Estado anterior inicial
	this->datosEstadoAnterior.alto=0;
	this->datosEstadoAnterior.ancho=0;
	this->datosEstadoAnterior.angulo_1=0;
	this->datosEstadoAnterior.angulo_2=0;
	this->datosEstadoAnterior.baseMayor=0;
	this->datosEstadoAnterior.baseMenor=0;
	this->datosEstadoAnterior.color="";
	this->datosEstadoAnterior.escala=0;
	this->datosEstadoAnterior.estatico=false;
	this->datosEstadoAnterior.id=-1;
	this->datosEstadoAnterior.lados=0;
	this->datosEstadoAnterior.masa=0;
	this->datosEstadoAnterior.radio=0;
	this->datosEstadoAnterior.rot=0;
	this->datosEstadoAnterior.tipo=TIPO_NULL;
	this->datosEstadoAnterior.x=-1;
	this->datosEstadoAnterior.y=-1;	
    this->estado = -1;
	this->estadoAnterior = -2;
}

Figura::Figura(b2World* mundo, float x,float y, string color,float rotacion, bool estatico){

	b2BodyDef bodyDef;
	if(!estatico){
		bodyDef.type = b2_dynamicBody;
	}    
	bodyDef.position.Set(x, y);//posicion en el mundo
	bodyDef.angle=gradosARadianes(rotacion);
	this->body = mundo->CreateBody(&bodyDef);
	this->datos.color = color;

		//Estado anterior inicial
	this->datosEstadoAnterior.alto=0;
	this->datosEstadoAnterior.ancho=0;
	this->datosEstadoAnterior.angulo_1=0;
	this->datosEstadoAnterior.angulo_2=0;
	this->datosEstadoAnterior.baseMayor=0;
	this->datosEstadoAnterior.baseMenor=0;
	this->datosEstadoAnterior.color="";
	this->datosEstadoAnterior.escala=0;
	this->datosEstadoAnterior.estatico=false;
	this->datosEstadoAnterior.id=-1;
	this->datosEstadoAnterior.lados=0;
	this->datosEstadoAnterior.masa=0;
	this->datosEstadoAnterior.radio=0;
	this->datosEstadoAnterior.rot=0;
	this->datosEstadoAnterior.tipo=TIPO_NULL;
	this->datosEstadoAnterior.x=-1;
	this->datosEstadoAnterior.y=-1;	
    this->estado = -1;
	this->estadoAnterior = -2;
}

Figura::Figura(b2World* mundo,sObjeto structObj){
	this->datos = structObj;
	this->datos.rot = Utilities::grados2radianes(this->datos.rot);

	b2BodyDef bodyDef;
	if(!this->datos.estatico){
		bodyDef.type = b2_dynamicBody;
	}
	bodyDef.position.Set(this->datos.x, this->datos.y);//posicion en el mundo
	bodyDef.angle = this->datos.rot;
	this->body = mundo->CreateBody(&bodyDef);

		//Estado anterior inicial
	this->datosEstadoAnterior.alto=0;
	this->datosEstadoAnterior.ancho=0;
	this->datosEstadoAnterior.angulo_1=0;
	this->datosEstadoAnterior.angulo_2=0;
	this->datosEstadoAnterior.baseMayor=0;
	this->datosEstadoAnterior.baseMenor=0;
	this->datosEstadoAnterior.color="";
	this->datosEstadoAnterior.escala=0;
	this->datosEstadoAnterior.estatico=false;
	this->datosEstadoAnterior.id=-1;
	this->datosEstadoAnterior.lados=0;
	this->datosEstadoAnterior.masa=0;
	this->datosEstadoAnterior.radio=0;
	this->datosEstadoAnterior.rot=0;
	this->datosEstadoAnterior.tipo=TIPO_NULL;
	this->datosEstadoAnterior.x=-1;
	this->datosEstadoAnterior.y=-1;	
	this->estado = -1;
	this->estadoAnterior = -2;
}

sObjeto Figura::getDatos(){	
	this->datos.rot = this->getAngulo();
	this->datos.x   = this->getX();
	if( this->getTipo() == TIPO_SNOWBOY ){
		this->estado  = ((SnowBoy*)this)->getEstado();
		this->datos.y = ((SnowBoy*)this)->getY();
	} else {
		this->datos.y   = this->getY();
	}
	return this->datos;
}

TIPO_FIGURA Figura::getTipo(){
	return this->datos.tipo;
}

int Figura::getId(){
	return this->datos.id;
}

bool Figura::esEstatico(){
	return this->datos.estatico;
}

float Figura::gradosARadianes(float grados){
	return (grados*b2_pi/180.0f);
}

float32 Figura::getX(){
	return body->GetPosition().x;
}

float32 Figura::getY(){
	return body->GetPosition().y;
}

float Figura::getAncho(){
	return this->datos.ancho;
}

float Figura::getAlto(){
	return this->datos.alto;
}

float32 Figura::getAngulo(){
	return this->body->GetAngle();
}

float Figura::calcularDensidad(){
	return(this->datos.masa/calcularArea());
}

void Figura::setFiguraCuerpo(b2Body* figuraCuerpo){
	this->body = figuraCuerpo;
}

b2Body* Figura::getFiguraCuerpo(){
	return this->body;
}

// DEBUG
void Figura::dibujar(SDL_Renderer *renderer){
	int rgb[3];
	rgb[0] = 0;
	rgb[1] = 0;
	rgb[2] = 0;

	Utilities::DrawBox2DBody(renderer, this->body, rgb[0], rgb[1], rgb[2], 0, rgb[0], rgb[1], rgb[2], 255, false);
}

void Figura::setEstadoAnterior( sObjeto datosEstadoAnterior ){
	this->datosEstadoAnterior = datosEstadoAnterior;
}

bool Figura::cambioEstado(){	
	// Actualizo estado con datos de box2d
    this->actualizarEstado();

	// Presicion de 1 pixel
	float presicionMovimiento =  Utilities::pixel2unidadLogica(4, Utilities::ALTO_PX, Utilities::ALTO_UL);
	float presicionMovimientoNegativa = -1*presicionMovimiento;

	// 1 grado = 0.02 radianes
	float presicionAnguloMovimiento = Utilities::grados2radianes(10);
	float presicionAnguloMovimientoNegativa = -1*presicionAnguloMovimiento;

	float deltaX = this->datos.x   - this->datosEstadoAnterior.x;
	float deltay = this->datos.y   - this->datosEstadoAnterior.y;
	float deltaAngulo = this->datos.rot - this->datosEstadoAnterior.rot;
	
	bool cambioEstado = this->estado != this->estadoAnterior;
	bool cambioX = !( presicionMovimientoNegativa < deltaX && deltaX < presicionMovimiento );
	bool cambioY = !( presicionMovimientoNegativa < deltay && deltay < presicionMovimiento );
	bool cambioAngulo = !( presicionAnguloMovimientoNegativa < deltaAngulo && deltaAngulo < presicionAnguloMovimiento );

	return cambioEstado ||
		   cambioX ||
		   cambioY ||
		   cambioAngulo ; 
}

//Utilizada para guardar el estado anterior
void Figura::actualizarEstadoAnterior(){
	this->datosEstadoAnterior = this->datos;	    
	this->estadoAnterior = this->estado;
}

//Utilizada para actualizar el estado
void Figura::actualizarEstado(){
	this->getDatos();
}

void Figura::setEstado( int estado ){
	this->estado = estado;
}

int Figura::getEstado(){
	return this->estado;
}

Figura::~Figura(void)
{
}