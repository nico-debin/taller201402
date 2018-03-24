#include "Nieve3.h"
		
void Nieve::crearBody(b2Body* bodyDelSnowBoy, bool snowBoyOrientacionDerecha){
	b2BodyDef bodyDef;			
	bodyDef.type = b2_dynamicBody;
						
	b2Vec2 posicionDelSnowBoy = bodyDelSnowBoy->GetPosition();
	//No se que nombre ponerle
	//Despues van a pasar a ser ctes
	float NieveOffsetX = RADIO_CUERPO + 0.2;
	float NieveOffsetY = RADIO_CUERPO/2;

	if (snowBoyOrientacionDerecha){bodyDef.position.Set(posicionDelSnowBoy.x + NieveOffsetX, posicionDelSnowBoy.y + NieveOffsetY);}
	else{bodyDef.position.Set(posicionDelSnowBoy.x - NieveOffsetX, posicionDelSnowBoy.y + NieveOffsetY);}
	bodyDef.angle=0;
	this->body = bodyDelSnowBoy->GetWorld()->CreateBody(&bodyDef);			
}

void Nieve::crearForma(){
	b2CircleShape circuloShape;
	circuloShape.m_p.Set(0, 0);			
	//Va a ser cte
	float NieveRadio = 0.1f;
	circuloShape.m_radius = NieveRadio;

	b2FixtureDef nieveFixDef;
	nieveFixDef.shape = &circuloShape;
	nieveFixDef.restitution = 0;
			
	nieveFixDef.density = 0.005;//Para que no  pueda empujar nada
	this->body->CreateFixture(&nieveFixDef); 
}

void Nieve::setearVelocidad(bool snowBoyOrientacionDerecha){
	//Va a ser cte
	float NieveVelocidadHorizontal = 15;
	if (snowBoyOrientacionDerecha){this->body->SetLinearVelocity(b2Vec2(NieveVelocidadHorizontal,0));}
	else {this->body->SetLinearVelocity(b2Vec2(-NieveVelocidadHorizontal,0));}

}


Nieve::Nieve( b2Body* bodyDelSnowBoy, bool snowBoyOrientacionDerecha){
	//this->datos.id = Utillities::getNewID();
	crearBody(bodyDelSnowBoy,snowBoyOrientacionDerecha);
	crearForma();
	setearVelocidad(snowBoyOrientacionDerecha);

	//Cualquier cosa para identificarlo, puede ser el tipo, o TIPO_NIEVE		
			
	/*Ahora esto va ser un sUserData y se va a destruir cuando se destruyen los bodies de las nieves
	pair<TIPO_FIGURA, Nieve*>* userData = new pair<TIPO_FIGURA,Nieve*>(TIPO_NIEVE,this);
	this->body->SetUserData((void*)userData);	
	*/
	sUserData* nieveUserData = new sUserData;
	nieveUserData->tipo = TIPO_NIEVE;
	nieveUserData->objeto = this;
	this->body->SetUserData((void*)nieveUserData);	


}

Nieve::~Nieve(){
	//Aca voy a probar si se pierde memoria o no al destruir

}

