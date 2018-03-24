#include "StdAfx.h"
#include "Nieve.h"


Nieve::Nieve(b2World* mundo,  sObjeto structObj) : Proyectil(mundo, structObj){
    this->crearForma();
	this->setearVelocidad();
	
    //Si se hace esto antes que lo de arriba, entonces las cosas de arriba van a funcionar mal
	this->datos.tipo = TIPO_NIEVE;
    this->userData.tipo = TIPO_NIEVE;
    this->tiempoAgonia = 10;
}


void Nieve::crearForma(){
	b2CircleShape circuloShape;
	circuloShape.m_p.Set(0, 0);			
	//Va a ser cte
	float NieveRadio = 0.5f;
	circuloShape.m_radius = NieveRadio;

	b2FixtureDef nieveFixDef;
	nieveFixDef.shape = &circuloShape;
	nieveFixDef.restitution = 0;

	nieveFixDef.filter.categoryBits = BITS_NIEVE;
	nieveFixDef.filter.maskBits = BITS_ESCENARIO | BITS_ENEMIGO |BITS_ENEMIGO_CON_NIEVE | BITS_BOLA_DE_NIEVE_QUIETA;


	nieveFixDef.density = 0.005;//Para que no  pueda empujar nada
	this->body->CreateFixture(&nieveFixDef); 

	//A definir
	this->datos.alto = NieveRadio*2;
	this->datos.ancho = NieveRadio*2;
	
	this->datos.lados = 0;
	this->datos.masa = 0.1;

}

void Nieve::setearVelocidad(){
	//Va a ser cte
	float velocidadHorizontal = 12;
	if (this->tieneOrientacionDerecha()){this->body->SetLinearVelocity(b2Vec2(velocidadHorizontal,0));}
	else {this->body->SetLinearVelocity(b2Vec2(-velocidadHorizontal,0));}

}

void Nieve::aplicarInercia(){
	b2Vec2 vel = this->body->GetLinearVelocity();
	if (vel.x > 0){
		vel.x = VELOCIDAD_HORIZONTAL_BONUS + 4;
	}
	else{
		vel.x = -VELOCIDAD_HORIZONTAL_BONUS - 4;
	}
	this->body->SetLinearVelocity(vel);
}
