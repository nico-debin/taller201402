#include "StdAfx.h"
#include "Fuego.h"

Fuego::Fuego(b2World* mundo,  sObjeto structObj):Proyectil( mundo, structObj){
    this->crearForma();
	this->setearVelocidad();
	
    this->datos.tipo = TIPO_FUEGO;
    this->userData.tipo = TIPO_FUEGO;
    this->tiempoAgonia = 6;
	this->tiempoFuego = 0;
	//this->matar();
}

void Fuego::crearForma(){
	b2CircleShape circuloShape;
	circuloShape.m_p.Set(0, 0);			
	//Va a ser cte
	float NieveRadio = 1.0f;
	circuloShape.m_radius = NieveRadio;

	b2FixtureDef nieveFixDef;
	nieveFixDef.shape = &circuloShape;
	nieveFixDef.restitution = 0;

	nieveFixDef.filter.categoryBits = BITS_FUEGO;
	nieveFixDef.filter.maskBits = BITS_ENEMIGO | BITS_ESCENARIO | BITS_SNOWBOY;


	nieveFixDef.density = 0.005;//Para que no  pueda empujar nada
	this->body->CreateFixture(&nieveFixDef); 

	//A definir
	this->datos.alto = NieveRadio*2;
	this->datos.ancho = NieveRadio*2;
	
	this->datos.lados = 0;
	this->datos.masa = 0.1;

}

void Fuego::setearVelocidad(){
	//Va a ser cte
	float velocidadHorizontal = 12;
    this->body->SetGravityScale(0);
	if (this->tieneOrientacionDerecha()){this->body->SetLinearVelocity(b2Vec2(velocidadHorizontal,0));}
	else {this->body->SetLinearVelocity(b2Vec2(-velocidadHorizontal,0));}

}

void Fuego::actualizar(){
	this->tiempoFuego++;
	if ( this->tiempoFuego > DRAGON_TIEMPO_VIDA_FUEGO ){
		this->matar();
	}
}
