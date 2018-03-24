#include "StdAfx.h"
#include "SnowBoy.h"


SnowBoy::SnowBoy(b2World* mundo,  sObjeto structObj):Figura(mundo,structObj){
	
    //b2BodyDef bodyDef;
	//bodyDef.type = b2_dynamicBody;
	//bodyDef.fixedRotation = true;
	//bodyDef.position.Set(personaje.x,personaje.y);
	//snowBody = mundo->CreateBody(&bodyDef);
	this->body->SetType(b2_dynamicBody);
    this->body->SetFixedRotation(true);

	crearForma();

	/*bool habilitacionIzq=true;
	bool habilitacionDer=true;
	bool habilitacionSalto=false;*/
    this->habilitacionIzq=true;
	this->habilitacionDer=true;
	this->habilitacionSalto=false;

	this->saltarPresionado=false;
	this->izqPresionado=false;
	this->derPresionado=false;

	this->datos.radio=RADIO_CUERPO;
	this->datos.alto=RADIO_CUERPO*2;
	this->datos.ancho=RADIO_CUERPO*2;

	//this->estado=QUIETO;
	this->estado=EVENTO_CLIENTE_PERSONAJE_QUIETO;


}

void SnowBoy::crearForma(){
	b2CircleShape cuerpo;
	cuerpo.m_radius=RADIO_CUERPO;
	cuerpo.m_p=b2Vec2(0,0);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &cuerpo;
	fixtureDef.density = 1;
	fixtureDef.friction = 0;
      
	this->fixtureDef = this->body->CreateFixture(&fixtureDef);
}


void SnowBoy::actualizar(){
	castearRayosDePiernas();
	castearRayosHorizontales();

	analizarMovimientos();
	desacelerarSiEstaQuieto();

	saltarPresionado=false;
}

void SnowBoy::castearRayosDePiernas(){

	float distanciaAlPisoDer=castearRayo(ANCHO_PIERNAS, ALTURA_INICIAL_PIERNAS,ALTURA_FINAL_PIERNAS);
	float distanciaAlPisoIzq=castearRayo(-ANCHO_PIERNAS,ALTURA_INICIAL_PIERNAS,ALTURA_FINAL_PIERNAS);

	float distanciaAlPiso=elegirDistanciaAlPiso(distanciaAlPisoDer,distanciaAlPisoIzq);
	
	if ( distanciaAlPiso < SNOWBOY_FLOTACION_DESEADA ) {

		float distanciaAFlotacionDeseada = SNOWBOY_FLOTACION_DESEADA - distanciaAlPiso;
		this->body->SetGravityScale(0);
		this->body->SetTransform(this->body->GetWorldCenter()+b2Vec2(0,distanciaAFlotacionDeseada),0);
		

	}
	else{this->body->SetGravityScale(1);}

	setearEstado(distanciaAlPiso);

}


float SnowBoy::castearRayo(float posX, float posYInicial, float posYFinal){
	
	b2Vec2 inicioRayo = this->body->GetWorldPoint( b2Vec2(posX,posYInicial) );
	b2Vec2 finRayo = this->body->GetWorldPoint( b2Vec2(posX,posYFinal) );


	SnowBoyRayCastCallback callback;
	this->body->GetWorld()->RayCast(&callback, inicioRayo, finRayo);

	
	float distanciaAlPiso=-1;
	if ( callback.hit ){
		distanciaAlPiso = (inicioRayo - callback.puntoDeContacto).Length();
	}
	
	return distanciaAlPiso;
}

float SnowBoy::elegirDistanciaAlPiso(float distanciaAlPisoDer, float distanciaAlPisoIzq){
	float distanciaAlPiso;
	if((distanciaAlPisoDer==-1)&&(distanciaAlPisoIzq==-1)){
			//Si ninguno esta tocando algo
			habilitacionSalto=false;
			distanciaAlPiso=SNOWBOY_FLOTACION_DESEADA+1;
		}
		else{
			//Por lo menos una pierna esta tocando algo
			habilitacionSalto=true;
			//Si las dos estan tocando algo:
			if ((distanciaAlPisoDer>=0)&&(distanciaAlPisoIzq>=0)){
				//La menor se setea como distancia al piso
				if(distanciaAlPisoDer<distanciaAlPisoIzq){distanciaAlPiso=distanciaAlPisoDer;}
				else{distanciaAlPiso=distanciaAlPisoIzq;}
			}
			else{
				//Si la pierna der no toca nada, la izq se setea como distancia al piso
				//Si la pierna izq no toca nada, la der se setea como distancia al piso
				if (distanciaAlPisoDer==-1){distanciaAlPiso=distanciaAlPisoIzq;}
				else{distanciaAlPiso=distanciaAlPisoDer;}
			}	
		}
		return distanciaAlPiso;
}


void SnowBoy::setearEstado(float distanciaAlPiso){

	//if ((this->estado==SALTANDO)&&(distanciaAlPiso<SNOWBOY_FLOTACION_DESEADA)){this->estado=QUIETO;}
	if ((this->estado==EVENTO_CLIENTE_PERSONAJE_SALTANDO)&&(distanciaAlPiso<SNOWBOY_FLOTACION_DESEADA)){this->estado=EVENTO_CLIENTE_PERSONAJE_QUIETO;}
	//if (saltarPresionado){this->estado=SALTANDO;}
	if (saltarPresionado){this->estado=EVENTO_CLIENTE_PERSONAJE_SALTANDO;}
	//if ((saltarPresionado)&&(izqPresionado||derPresionado)){this->estado=SALTANDO;}
	if ((saltarPresionado)&&(izqPresionado||derPresionado)){this->estado=EVENTO_CLIENTE_PERSONAJE_SALTANDO;}
	//if((izqPresionado||derPresionado) && (this->estado!=SALTANDO)){this->estado=CAMINANDO;}
	if((izqPresionado||derPresionado) && (this->estado!=EVENTO_CLIENTE_PERSONAJE_SALTANDO)){this->estado=EVENTO_CLIENTE_PERSONAJE_CAMINANDO;}
	//if((!saltarPresionado && !izqPresionado && !derPresionado) && (this->estado!=SALTANDO)){this->estado=QUIETO;}
	if((!saltarPresionado && !izqPresionado && !derPresionado) && (this->estado!=EVENTO_CLIENTE_PERSONAJE_SALTANDO)){this->estado=EVENTO_CLIENTE_PERSONAJE_QUIETO;}

}



void SnowBoy::castearRayosHorizontales(){

	habilitacionDer=analizarHabilitacionParaRayo(RAYO_HORIZONTAL_POSX);
	if ((!habilitacionDer)&&(this->body->GetLinearVelocity().x>0)){this->frenarDerecha();}
	habilitacionIzq=analizarHabilitacionParaRayo(-RAYO_HORIZONTAL_POSX);
	if ((!habilitacionIzq)&&(this->body->GetLinearVelocity().x<0)){this->frenarIzquierda();}

}

bool SnowBoy::analizarHabilitacionParaRayo(float posXFinal){
	
//Calculo del angulo de la rampa:

	b2Vec2 inicioRayo = this->body->GetWorldPoint( b2Vec2(0,RAYO_HORIZONTAL_POSY) );
	b2Vec2 finRayo = this->body->GetWorldPoint( b2Vec2(posXFinal,RAYO_HORIZONTAL_POSY) );
        
    SnowBoyRayCastCallback callback;
	this->body->GetWorld()->RayCast(&callback, inicioRayo, finRayo);

	if (( callback.hit )&&( callback.tipoDeBodyTocado==b2_staticBody )){
				
		b2Vec2 normalWorldPos=this->body->GetWorldPoint(callback.puntoFinalNormal);
		float hipotenusa = (normalWorldPos-callback.puntoDeContacto).Length();
		float adyacente = (b2Vec2(normalWorldPos.x,callback.puntoDeContacto.y)-callback.puntoDeContacto).Length();
		float anguloRespectoAlRayo=acos(adyacente/hipotenusa);
		float anguloRampa=90-Utilities::radianes2grados(anguloRespectoAlRayo);

		if(anguloRampa>ANGULO_MAX_ESCALABLE){return false;}
		else {return true;}
	}
	else {return true;}

}

void SnowBoy::desacelerarSiEstaQuieto(){
	if (this->estado==EVENTO_CLIENTE_PERSONAJE_QUIETO){
		b2Vec2 vel=this->body->GetLinearVelocity();
		if (vel.x>0){
				vel.x-=SNOWBOY_CONSTANTE_DESACELERACION;
				if (vel.x<0){vel.x=0;}
				this->body->SetLinearVelocity(vel);
			}
		if (vel.x<0){
				vel.x+=SNOWBOY_CONSTANTE_DESACELERACION;
				if (vel.x>0){vel.x=0;}
				this->body->SetLinearVelocity(vel);
			}
	}
}

void SnowBoy::analizarMovimientos(){
	b2Vec2 vel=this->body->GetLinearVelocity();

	if (derPresionado && izqPresionado){
		if (ultimoBotonPresionado){//Si el ultimo presionado fue el boton Derecho
			vel.x=VELOCIDAD_HORIZONTAL;
			this->body->SetLinearVelocity(vel);
		}
		else {//Si el ultimo presionado fue el boton Izquierdo
			vel.x=-VELOCIDAD_HORIZONTAL;
			this->body->SetLinearVelocity(vel);
		}
	}else{
		if (derPresionado && (vel.x!=VELOCIDAD_HORIZONTAL)){
			vel.x=VELOCIDAD_HORIZONTAL;
			this->body->SetLinearVelocity(vel);
		}
		else if (izqPresionado && (vel.x!=-VELOCIDAD_HORIZONTAL)){
				vel.x=-VELOCIDAD_HORIZONTAL;
				this->body->SetLinearVelocity(vel);
		}
	}
}

void SnowBoy::moverDerecha(){
	if (habilitacionDer){
		derPresionado=true;
		ultimoBotonPresionado=true;
	}
}
void SnowBoy::moverIzquierda(){
	if (habilitacionIzq){
		izqPresionado=true;
		ultimoBotonPresionado=false;	
	}
}

void SnowBoy::frenarDerecha(){
	b2Vec2 vel=this->body->GetLinearVelocity();
	if (vel.x>0){
		vel.x=0; 
		this->body->SetLinearVelocity(vel);
	}
	derPresionado=false;
}

void SnowBoy::frenarIzquierda(){
	b2Vec2 vel=this->body->GetLinearVelocity();
	if (vel.x<0){
		vel.x=0; 
		this->body->SetLinearVelocity(vel);
	}
	izqPresionado=false;
}


void SnowBoy::saltar(){
	if (habilitacionSalto){
		b2Vec2 vel=this->body->GetLinearVelocity();
		vel.y=VELOCIDAD_SALTO;
		this->body->SetLinearVelocity(vel);
		//this->estado=SALTANDO;
		this->estado=EVENTO_CLIENTE_PERSONAJE_SALTANDO;
		saltarPresionado=true;
	}
}

//ESTADO_PERSONAJE SnowBoy::getEstado(){
int SnowBoy::getEstado(){
	return this->estado;
}


b2Fixture* SnowBoy::getFixture(){
	return this->fixtureDef;
}


float SnowBoy::getY(){
	return this->body->GetWorldPoint(b2Vec2(0,-SNOWBOY_DIFERENCIA_CENTRO_CUERPO_CENTRO_FIGURA)).y;
}


SnowBoy::~SnowBoy(void)
{
}
