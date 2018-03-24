#include "StdAfx.h"
#include "Figura.h"

Figura::Figura(){
	this->datos.estatico = false;
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
}

sObjeto Figura::getDatos(){
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

Figura::~Figura(void)
{
}