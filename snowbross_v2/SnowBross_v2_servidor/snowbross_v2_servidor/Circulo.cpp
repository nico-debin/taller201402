#include "StdAfx.h"
#include "Circulo.h"


Circulo::Circulo(b2World* mundo, float x,float y, float radio, string color,float rotacion, float masa, bool estatico):Figura(mundo,x,y,color,rotacion,estatico){

	this->datos.ancho = radio*2;
	this->datos.alto = this->datos.ancho;

	b2CircleShape circuloShape;
	circuloShape.m_p.Set(0, 0); //posicion relativa al body
	circuloShape.m_radius = radio;
	this->datos.radio = radio;

	b2FixtureDef circuloFixDef;
	circuloFixDef.shape = &circuloShape; 
	circuloFixDef.restitution = RESTITUCION; //para establecer cuánto rebota
	circuloFixDef.friction = FRICCION;
	circuloFixDef.density=calcularDensidad();
	
	body->CreateFixture(&circuloFixDef); //agrega la fixture al body
}

Circulo::Circulo(float x,float y, float radio, string color,float rotacion, float masa, bool estatico):Figura(){

	this->datos.ancho = radio*2;
	this->datos.alto = this->datos.ancho;

	b2CircleShape circuloShape;
	circuloShape.m_p.Set(0, 0); //posicion relativa al body
	circuloShape.m_radius = radio;
	this->datos.radio = radio;

	b2FixtureDef circuloFixDef;
	circuloFixDef.shape = &circuloShape; 
	circuloFixDef.restitution = RESTITUCION; //para establecer cuánto rebota
	circuloFixDef.friction = FRICCION;
	circuloFixDef.density=calcularDensidad();
	
	body->CreateFixture(&circuloFixDef); //agrega la fixture al body
}

b2Fixture* Circulo::getFixture(){
	return this->fixtureDef;
}

Circulo::Circulo():Figura(){}

Circulo::Circulo(b2World* mundo,sObjeto structObj):Figura(mundo,structObj){

	this->datos.ancho = structObj.radio*2;
	this->datos.alto = this->datos.ancho;

	mundo->SetContactListener(this);
	b2CircleShape circuloShape;
	circuloShape.m_p.Set(0, 0); 
	circuloShape.m_radius = this->datos.radio;

	b2FixtureDef circuloFixDef;
	circuloFixDef.shape = &circuloShape; 
	circuloFixDef.restitution = RESTITUCION; //para establecer cuánto rebota
	circuloFixDef.friction = FRICCION;
	circuloFixDef.density = calcularDensidad();
	this->fixtureDef= body->CreateFixture(&circuloFixDef);
}


float Circulo::calcularArea(){
	return (b2_pi*this->datos.radio*this->datos.radio);
}

float Circulo::getRadio(){
	return this->datos.radio;
}


