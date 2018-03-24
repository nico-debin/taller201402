#include "StdAfx.h"
#include "Proyectil.h"

Proyectil::Proyectil(b2World* mundo,  sObjeto structObj) : Figura(mundo, structObj){

	//Esto modifica los datos y la posicion del body
	this->modificarBody();
	//this->crearForma();
	//this->setearVelocidad();
	//Esto es para saber si va a la izq o der
	this->setearEstado();
	
	

	//Este ID no lo pone el parser
	this->datos.id = Utilities::getNewID();
	
	//TODO: TENER EN CUENTA....Si se hace esto antes que lo de arriba, entonces las cosas de arriba van a funcionar mal
	//this->datos.tipo = TIPO_NIEVE;

	

/* Como no hay colisiones ahora no lo necesita, pero lo dejo para despues
	sUserData* nieveUserData = new sUserData;
	nieveUserData->tipo = TIPO_NIEVE;
	nieveUserData->objeto = this;
	this->body->SetUserData((void*)nieveUserData);
*/	

	//this->userData.tipo = TIPO_NIEVE;
	//this->tiempoAgonia = 10;

	this->habilitacionParaGolpear = true;

}

void Proyectil::modificarBody(){
	//Despues van a pasar a ser ctes
	float NieveOffsetX = RADIO_CUERPO + 0.5;
	float NieveOffsetY = RADIO_CUERPO/2;

	b2Vec2 nuevaPosicion;
	nuevaPosicion.y = this->getY() + NieveOffsetY;
	//Cuando entra aca lo que hay en this->x/y es la posicion del SB
	if (this->tieneOrientacionDerecha() ){
		nuevaPosicion.x = this->getX() + NieveOffsetX;
		this->body->SetTransform(nuevaPosicion,0);
	}
	else{
		nuevaPosicion.x = this->getX() - NieveOffsetX;
		this->body->SetTransform(nuevaPosicion,0);
	}
	
	this->datos.estatico = false; 
	this->datos.rot = 0;
	this->body->SetType(b2BodyType::b2_dynamicBody);
	this->body->SetFixedRotation(true);
}

void Proyectil::setearEstado(){
	if (this->tieneOrientacionDerecha()){this->estado = EVENTO_CLIENTE_PROYECTIL_DER;}
	else {this->estado = EVENTO_CLIENTE_PROYECTIL_IZQ;}
}


bool Proyectil::tieneOrientacionDerecha(){
	if (this->getTipo() == TIPO_PROYECTIL_DER ) {return true;}
	else { return false;}
}


b2Fixture* Proyectil::getFixture(){
	return this->fixtureDef;
}



bool Proyectil::puedeGolpear(){
	return this->habilitacionParaGolpear;
}
void Proyectil::deshabilitar(){
	this->habilitacionParaGolpear = false;
}