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
	
	//b2Vec2 vs[4];
	b2Vec2 vsParedIzquierda[2];
	b2Vec2 vsParedDerecha[2];
	b2Vec2 vsPiso[2];
	b2Vec2 vsTecho[2];
	
	// Pared derecha
	vsParedDerecha[0].Set(anchoUl, altoUl*2);
	//vsParedDerecha[1].Set(anchoUl, 0.0f);
	vsParedDerecha[1].Set(anchoUl, -altoUl);

	// Piso
	vsPiso[0].Set(anchoUl, 0.0f);
	vsPiso[1].Set(0.0f, 0.0f);
	
	// Pared Izquierda
	//vsParedIzquierda[0].Set(0.0f, 0.0f);
	vsParedIzquierda[0].Set(0.0f, -altoUl);
	vsParedIzquierda[1].Set(0.0f, altoUl*2);

	// Techo
	vsTecho[0].Set(0.0f, altoUl*2);
	vsTecho[1].Set(anchoUl, altoUl*2);


	// Pared Derecha
	b2ChainShape contornoParedDerecha;
	contornoParedDerecha.CreateChain(vsParedDerecha, 2); 
	
	b2BodyDef contornoParedDerechaDef;
	contornoParedDerechaDef.position.Set(0.0f,0);
	contornoParedDerechaDef.type=b2_staticBody;
	b2Body* paredDerecha = this->mundo->CreateBody(&contornoParedDerechaDef);
	
	b2FixtureDef contornoParedDerechaDefFixDef;
	contornoParedDerechaDefFixDef.shape=&contornoParedDerecha;
	contornoParedDerechaDefFixDef.restitution=0;
	contornoParedDerechaDefFixDef.friction = 1;
	paredDerecha->CreateFixture(&contornoParedDerechaDefFixDef);
	
	this->userDataParedDerecha.tipo = TIPO_PARED_DERECHA;
	this->userDataParedDerecha.objeto = nullptr;
	paredDerecha->SetUserData((void*)&this->userDataParedDerecha);


	// Pared Izquierda
	b2ChainShape contornoParedIzquierda;
	contornoParedIzquierda.CreateChain(vsParedIzquierda, 2); 
	
	b2BodyDef contornoParedIzquierdaDef;
	contornoParedIzquierdaDef.position.Set(0.0f,0);
	contornoParedIzquierdaDef.type=b2_staticBody;
	b2Body* paredIzquierda = this->mundo->CreateBody(&contornoParedIzquierdaDef);
	
	b2FixtureDef contornoParedIzquierdaDefFixDef;
	contornoParedIzquierdaDefFixDef.shape=&contornoParedIzquierda;
	contornoParedIzquierdaDefFixDef.restitution=0;
	contornoParedIzquierdaDefFixDef.friction = 1;
	paredIzquierda->CreateFixture(&contornoParedIzquierdaDefFixDef);

	this->userDataParedIzquierda.tipo = TIPO_PARED_IZQUIERDA;
	this->userDataParedIzquierda.objeto = nullptr;
	paredIzquierda->SetUserData((void*)&this->userDataParedIzquierda);

	// Piso
	b2ChainShape contornoPiso;
	contornoPiso.CreateChain(vsPiso, 2); 
	
	b2BodyDef contornoPisoDef;
	contornoPisoDef.position.Set(0.0f,0);
	contornoPisoDef.type=b2_staticBody;
	b2Body* piso = this->mundo->CreateBody(&contornoPisoDef);

	b2FixtureDef contornoPisoDefFixDef;
	contornoPisoDefFixDef.shape=&contornoPiso;
	contornoPisoDefFixDef.restitution=0;
	contornoPisoDefFixDef.friction = 1;

	contornoPisoDefFixDef.isSensor = true;

	piso->CreateFixture(&contornoPisoDefFixDef);

	this->userDataPiso.tipo = TIPO_PISO;
	this->userDataPiso.objeto = nullptr;
	piso->SetUserData((void*)&this->userDataPiso);

	
	// Techo	

	b2ChainShape contornoTecho;
	contornoTecho.CreateChain(vsTecho, 2); 
	
	b2BodyDef contornoTechoDef;
	contornoTechoDef.position.Set(0.0f,0);
	contornoTechoDef.type=b2_staticBody;
	b2Body* techo = this->mundo->CreateBody(&contornoTechoDef);
	
	b2FixtureDef contornoTechoDefFixDef;
	contornoTechoDefFixDef.shape=&contornoTecho;
	contornoTechoDefFixDef.restitution=0;
	contornoTechoDefFixDef.friction = 1;
	techo->CreateFixture(&contornoTechoDefFixDef);

	this->userDataTecho.tipo = TIPO_TECHO;
	this->userDataTecho.objeto = nullptr;
	techo->SetUserData((void*)&this->userDataTecho);



}

Fisica::~Fisica(void)
{
	if( this->mundo !=NULL){
		delete  this->mundo;
	}
	
}
