#include "StdAfx.h"
#include "Bonus.h"

Bonus::Bonus(b2World* mundo,  sObjeto structObj):Figura(mundo,structObj){

    this->body->SetType(b2_staticBody);

   	
	b2PolygonShape rectanguloShape;
	rectanguloShape.SetAsBox(this->datos.ancho/2 , this->datos.alto/2);
	
	b2FixtureDef rectanguloFixDef;
	rectanguloFixDef.shape = &rectanguloShape; 
	rectanguloFixDef.friction = FRICCION;
	rectanguloFixDef.isSensor = true;

	rectanguloFixDef.filter.categoryBits = BITS_BONO;
	rectanguloFixDef.filter.maskBits = BITS_SNOWBOY;

	//rectanguloFixDef.restitution = RESTITUCION; //para establecer cuánto rebota
	rectanguloFixDef.density=calcularDensidad();
	this->fixtureDef=body->CreateFixture(&rectanguloFixDef);

	this->datos.lados = LADOS_CUADRADO;

    this->tiempoAgonia = 6;
	this->tiempoEsperaSinLevantar = 0;
//    this->matar();

	//this->estado = TIPO_BONUS_HABILITADO;
	this->estado = EVENTO_CLIENTE_BONUS_HABILITADO;

	this->habilitacion = true;
}

void Bonus::actualizar(){
	this->tiempoEsperaSinLevantar++;
	if ( this->tiempoEsperaSinLevantar > 1000 ){
		this->matar();
	}
}

void Bonus::deshabilitar(){
	this->habilitacion = false;
	this->estado = EVENTO_CLIENTE_BONUS_DESHABILITADO;
}

bool Bonus::estaHabilitado(){
	return this->habilitacion;
}