#include "StdAfx.h"
#include "Circulo.h"


b2Fixture* Circulo::getFixture(){
	return this->fixtureDef;
}

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


