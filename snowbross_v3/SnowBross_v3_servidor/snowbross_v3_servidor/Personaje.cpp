#include "StdAfx.h"
#include "Personaje.h"
#include "Escenario.h"
#include "SaltarRayCastCallback.h"
#include "Constantes.h"

Personaje::Personaje(b2World* mundo,  sObjeto structObj):Figura(mundo,structObj){
	
	this->velocidadHorizontal = VELOCIDAD_HORIZONTAL;


	this->body->SetType(b2_dynamicBody);
	this->body->SetFixedRotation(true);

	crearForma();

	this->habilitacionIzq=true;
	this->habilitacionDer=true;
	this->habilitacionSalto=false;

	this->saltarPresionado=false;
	this->izqPresionado=false;
	this->derPresionado=false;

	this->datos.radio=RADIO_CUERPO;
	this->datos.alto=SNOWBOY_ALTO_UL;
	this->datos.ancho=SNOWBOY_ANCHO_UL;

	this->estado=EVENTO_CLIENTE_PERSONAJE_QUIETO_IZQ;
	this->orientacionDerecha = false;

	this->deshabilitarSalto();
}

void Personaje::crearForma(){
	b2CircleShape cuerpo;
	cuerpo.m_radius=RADIO_CUERPO;
	cuerpo.m_p=b2Vec2(0,0);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &cuerpo;
	fixtureDef.density = 5;
	fixtureDef.friction = 0;
      

	//fixtureDef.filter.categoryBits =
	//fixtureDef.filter.categoryBits =


	this->fixtureDef = this->body->CreateFixture(&fixtureDef);
}


void Personaje::actualizar(){
	// Utilizado para detectar si es posible saltar en la inteligencia del enemigo.
	this->deshabilitarSalto();
	this->castearRayoSalto(0,0,PERSONAJE_SALTAR_UL);

	if ( (this->estado != EVENTO_CLIENTE_FIGURA_MURIENDO) && (this->estado != EVENTO_CLIENTE_FIGURA_MUERTO) && (this->estado != EVENTO_CLIENTE_SNOWBOY_VA_A_RENACER) && (this->estado != EVENTO_CLIENTE_PERSONAJE_DESCONECTADO)){
		castearRayosDePiernas();
		castearRayosHorizontales();
	
		analizarMovimientos();
		desacelerarSiEstaQuieto();
	
		saltarPresionado=false;
	}
	else{/*ACA SE LE PUEDE PONER SET GRAVITY SCALE 0 Y CAMBIARLE LOS BITS PARA QUE NO COLISIONE CON NADA*/
		castearRayosDePiernas();
	}
}

void Personaje::castearRayosDePiernas(){

	float distanciaAlPisoDer=castearRayo(ANCHO_PIERNAS, ALTURA_INICIAL_PIERNAS,ALTURA_FINAL_PIERNAS);
	float distanciaAlPisoIzq=castearRayo(-ANCHO_PIERNAS,ALTURA_INICIAL_PIERNAS,ALTURA_FINAL_PIERNAS);

	float distanciaAlPiso=elegirDistanciaAlPiso(distanciaAlPisoDer,distanciaAlPisoIzq);
	
	if ( distanciaAlPiso < SNOWBOY_FLOTACION_DESEADA ) {

		float distanciaAFlotacionDeseada = SNOWBOY_FLOTACION_DESEADA - distanciaAlPiso;
		this->body->SetGravityScale(0);
		this->body->SetTransform(this->body->GetWorldCenter()+b2Vec2(0,distanciaAFlotacionDeseada),0);

		//NUEVO: para que no salga volando
/*		if (!this->saltarPresionado && this->estado != EVENTO_CLIENTE_PERSONAJE_SALTANDO_DER && this->estado !=EVENTO_CLIENTE_PERSONAJE_SALTANDO_IZQ ){
			b2Vec2 velocidad = this->body->GetLinearVelocity();
			if (velocidad.y > 0 ){
				velocidad.y = 0;
				this->body->SetLinearVelocity(velocidad);
			}
		}
*/
	}

	else{this->body->SetGravityScale(1);}

	setearEstado(distanciaAlPiso);

	//NUEVO: para que no salga volando
	if (!this->saltarPresionado && this->estado != EVENTO_CLIENTE_PERSONAJE_SALTANDO_DER && this->estado !=EVENTO_CLIENTE_PERSONAJE_SALTANDO_IZQ ){
		b2Vec2 velocidad = this->body->GetLinearVelocity();
		if (velocidad.y > 0 ){
			velocidad.y = 0;
			this->body->SetLinearVelocity(velocidad);
		}
	}


}


float Personaje::castearRayo(float posX, float posYInicial, float posYFinal){
	
	b2Vec2 inicioRayo = this->body->GetWorldPoint( b2Vec2(posX,posYInicial) );
	b2Vec2 finRayo = this->body->GetWorldPoint( b2Vec2(posX,posYFinal) );


	SnowBoyRayCastCallback callback(&this->userData);
	this->body->GetWorld()->RayCast(&callback, inicioRayo, finRayo);

	
	float distanciaAlPiso=-1;
	if ( callback.hit ){
		distanciaAlPiso = (inicioRayo - callback.puntoDeContacto).Length();
	}

	return distanciaAlPiso;
}

float Personaje::elegirDistanciaAlPiso(float distanciaAlPisoDer, float distanciaAlPisoIzq){
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


void Personaje::setearEstado(float distanciaAlPiso){
    
	if (this->estado==EVENTO_CLIENTE_FIGURA_MURIENDO || this->estado==EVENTO_CLIENTE_FIGURA_MUERTO || this->estado==EVENTO_CLIENTE_SNOWBOY_VA_A_RENACER){
        return;
    }

	if (distanciaAlPiso<SNOWBOY_FLOTACION_DESEADA){
		if(this->estado==EVENTO_CLIENTE_PERSONAJE_SALTANDO_IZQ){this->estado=EVENTO_CLIENTE_PERSONAJE_QUIETO_IZQ;}
		else if(this->estado==EVENTO_CLIENTE_PERSONAJE_SALTANDO_DER){this->estado=EVENTO_CLIENTE_PERSONAJE_QUIETO_DER;}
	}


	if ((saltarPresionado)){
		if (orientacionDerecha){this->estado=EVENTO_CLIENTE_PERSONAJE_SALTANDO_DER;}
		else {this->estado=EVENTO_CLIENTE_PERSONAJE_SALTANDO_IZQ;}
	}

	if(this->estado==EVENTO_CLIENTE_PERSONAJE_SALTANDO_DER && (!orientacionDerecha)) {this->estado=EVENTO_CLIENTE_PERSONAJE_SALTANDO_IZQ;}
	if(this->estado==EVENTO_CLIENTE_PERSONAJE_SALTANDO_IZQ && (orientacionDerecha)) {this->estado=EVENTO_CLIENTE_PERSONAJE_SALTANDO_DER;}
		
	if((izqPresionado||derPresionado) && (this->estado!=EVENTO_CLIENTE_PERSONAJE_SALTANDO_IZQ && this->estado!=EVENTO_CLIENTE_PERSONAJE_SALTANDO_DER)){
		if (orientacionDerecha){this->estado=EVENTO_CLIENTE_PERSONAJE_CAMINANDO_DER;}
		else {this->estado=EVENTO_CLIENTE_PERSONAJE_CAMINANDO_IZQ;}
	}


	if((!saltarPresionado && !izqPresionado && !derPresionado) && ( this->estado!=EVENTO_CLIENTE_PERSONAJE_SALTANDO_IZQ && this->estado!=EVENTO_CLIENTE_PERSONAJE_SALTANDO_DER )){
		if (this->estado==EVENTO_CLIENTE_PERSONAJE_CAMINANDO_DER){this->estado=EVENTO_CLIENTE_PERSONAJE_QUIETO_DER;}
		else if(this->estado==EVENTO_CLIENTE_PERSONAJE_CAMINANDO_IZQ){this->estado=EVENTO_CLIENTE_PERSONAJE_QUIETO_IZQ;}
	}
		
}




SnowBoyRayCastCallback* Personaje::castearRayoHorizontal(float posXFinal){
	b2Vec2 inicioRayo = this->body->GetWorldPoint( b2Vec2(0,RAYO_HORIZONTAL_POSY) );
	b2Vec2 finRayo = this->body->GetWorldPoint( b2Vec2(posXFinal,RAYO_HORIZONTAL_POSY) );
        
	SnowBoyRayCastCallback* callback = new SnowBoyRayCastCallback(&this->userData);
	this->body->GetWorld()->RayCast(callback, inicioRayo, finRayo);

	return callback;
}

//EL CASTEO DE RAYOS HORIZONTALES SE SACA DE ACA
void Personaje::castearRayosHorizontales(){

	SnowBoyRayCastCallback* callbackRayoHorizontalDer = this->castearRayoHorizontal(RAYO_HORIZONTAL_POSX);
	habilitacionDer=analizarHabilitacionParaRayo(callbackRayoHorizontalDer);
	delete callbackRayoHorizontalDer;

	SnowBoyRayCastCallback* callbackRayoHorizontalIzq = this->castearRayoHorizontal(-RAYO_HORIZONTAL_POSX);
	habilitacionIzq=analizarHabilitacionParaRayo(callbackRayoHorizontalIzq);
	delete callbackRayoHorizontalIzq;
}

bool Personaje::analizarHabilitacionParaRayo(SnowBoyRayCastCallback* callback){

//Calculo del angulo de la rampa:

	if (( callback->hit )&&( callback->tipoDeBodyTocado==b2_staticBody )){
				
		b2Vec2 normalWorldPos=this->body->GetWorldPoint(callback->puntoFinalNormal);
		float hipotenusa = (normalWorldPos-callback->puntoDeContacto).Length();
		float adyacente = (b2Vec2(normalWorldPos.x,callback->puntoDeContacto.y)-callback->puntoDeContacto).Length();
		float anguloRespectoAlRayo=acos(adyacente/hipotenusa);
		float anguloRampa=90-Utilities::radianes2grados(anguloRespectoAlRayo);

		if(anguloRampa>ANGULO_MAX_ESCALABLE){return false;}
		else {return true;}
	}
	else {return true;}
}

void Personaje::desacelerarSiEstaQuieto(){
	if (this->estado==EVENTO_CLIENTE_PERSONAJE_QUIETO_DER || this->estado==EVENTO_CLIENTE_PERSONAJE_QUIETO_IZQ  || this->estado==EVENTO_CLIENTE_PERSONAJE_CORRIENDO_QUIETO_DER || this->estado==EVENTO_CLIENTE_PERSONAJE_CORRIENDO_QUIETO_IZQ){
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

void Personaje::analizarMovimientos(){
	b2Vec2 vel=this->body->GetLinearVelocity();

	if (derPresionado && izqPresionado){
		if (orientacionDerecha){//Si el ultimo presionado fue el boton Derecho
			vel.x=this->velocidadHorizontal;
			this->body->SetLinearVelocity(vel);
		}
		else {//Si el ultimo presionado fue el boton Izquierdo
			vel.x=-this->velocidadHorizontal;
			this->body->SetLinearVelocity(vel);
		}
	}else{
		if (derPresionado && (vel.x!=this->velocidadHorizontal)){
			vel.x=this->velocidadHorizontal;
			this->body->SetLinearVelocity(vel);
			orientacionDerecha=true;
		}
		else if (izqPresionado && (vel.x!=-this->velocidadHorizontal)){
				vel.x=-this->velocidadHorizontal;
				this->body->SetLinearVelocity(vel);
				orientacionDerecha=false;
		}
	}
}

void Personaje::moverDerecha(){
	if (habilitacionDer){
		derPresionado=true;
		orientacionDerecha=true;
	}
}
void Personaje::moverIzquierda(){
	if (habilitacionIzq){
		izqPresionado=true;
		orientacionDerecha=false;	
	}
}

void Personaje::frenarDerecha(){
	b2Vec2 vel=this->body->GetLinearVelocity();
	if (vel.x>0){
		vel.x=0; 
		this->body->SetLinearVelocity(vel);
	}
	derPresionado=false;
}

void Personaje::frenarIzquierda(){
	b2Vec2 vel=this->body->GetLinearVelocity();
	if (vel.x<0){
		vel.x=0; 
		this->body->SetLinearVelocity(vel);
	}
	izqPresionado=false;
}


void Personaje::saltar(){
	if (habilitacionSalto){
		b2Vec2 vel=this->body->GetLinearVelocity();
		vel.y=VELOCIDAD_SALTO;
		this->body->SetLinearVelocity(vel);
		saltarPresionado=true;
	}
}

//int Personaje::getEstado(){
//	return this->estado;
//}


b2Fixture* Personaje::getFixture(){
	return this->fixtureDef;
}


float Personaje::getY(){
	return this->body->GetWorldPoint(b2Vec2(0,-SNOWBOY_DIFERENCIA_CENTRO_CUERPO_CENTRO_FIGURA)).y;
}



void Personaje::matar(){
	this->habilitacionIzq = false;
	this->habilitacionDer = false;
	this->habilitacionSalto = false;
	
	this->saltarPresionado = false;
	this->izqPresionado = false;
	this->derPresionado = false;
	this->orientacionDerecha = false;

	Figura::matar();
}



Personaje::~Personaje(void)
{
}

void Personaje::lanzar(){
	if (this->estaViva()){
		sObjeto structDelProyectil;
	
		structDelProyectil.x = this->body->GetPosition().x;
		structDelProyectil.y = this->body->GetPosition().y;
	
		//Esto es necesario para saber si la velocidad del proyectil debe ser positiva o negativa (DER o IZQ)
		//Despues adentro del constructor del proyectil se le cambia el TIPO, x ej en Nieve se cambia a TIPO_NIEVE
		if (this->orientacionDerecha){structDelProyectil.tipo = TIPO_PROYECTIL_DER;} 
		else {structDelProyectil.tipo = TIPO_PROYECTIL_IZQ;}

		Figura* proyectil = this->crearProyectil(structDelProyectil);
		Escenario::agregarFigura(proyectil);//El ID lo debe cargar el constructor del proyectil
	}
}

void Personaje::castearRayoSalto(float posX, float posYInicial, float posYFinal){
	
	b2Vec2 inicioRayo = this->body->GetWorldPoint( b2Vec2(posX,posYInicial) );
	b2Vec2 finRayo = this->body->GetWorldPoint( b2Vec2(posX,posYFinal) );

	SaltarRayCastCallback callback(&this->userData);
	this->body->GetWorld()->RayCast(&callback, inicioRayo, finRayo);
}

void Personaje::habilitarSalto(){
	this->saltoHabilitado = true;
}

void Personaje::deshabilitarSalto(){
	this->saltoHabilitado = false;
}

bool Personaje::puedeSaltar(){
	return this->saltoHabilitado;
}



void Personaje::cargarBitsDeColisionEnFixtures(uint16 categoryBits, uint16 maskBits){
	for (b2Fixture* fixtureActual = this->body->GetFixtureList(); fixtureActual; fixtureActual = fixtureActual->GetNext()){
		b2Filter filtro = fixtureActual->GetFilterData();
		
		filtro.categoryBits = categoryBits;
		filtro.maskBits = maskBits;

		fixtureActual->SetFilterData(filtro);
	}
}