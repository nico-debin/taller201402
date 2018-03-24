#include "StdAfx.h"
#include "Fisica.h"


Fisica::Fisica(float anchoUl,float altoUl):gravedad(GRAVEDAD_X,GRAVEDAD_Y){	
	this->mundo = new b2World(gravedad);
	this->iniciarPisoYPared(anchoUl, altoUl);
}

b2World* Fisica::getMundo(){
	return mundo;
}

void Fisica::paso(){	
	
	mundo->Step(PASO_TIEMPO, ITERACION_VELOCIDAD, ITERACION_POSICION);		
}

void Fisica::iniciarPisoYPared(float anchoUl,float altoUl){
	
	b2Vec2 vs[4];
	
	vs[0].Set(anchoUl, altoUl);
	vs[1].Set(anchoUl, 0.0f);
	vs[2].Set(0.0f, 0.0f);
	vs[3].Set(0.0f, altoUl);
	b2ChainShape contorno;
	contorno.CreateChain(vs, 4); 
	
	b2BodyDef contornoDef;
	contornoDef.position.Set(0.0f,0);
	contornoDef.type=b2_staticBody;
	b2Body* piso = this->mundo->CreateBody(&contornoDef);
	
	b2FixtureDef contornoDefFixDef;
	contornoDefFixDef.shape=&contorno;
	contornoDefFixDef.restitution=0;
	piso->CreateFixture(&contornoDefFixDef);
}

Fisica::~Fisica(void)
{
	if( this->mundo !=NULL){
		delete  this->mundo;
	}
	
}
