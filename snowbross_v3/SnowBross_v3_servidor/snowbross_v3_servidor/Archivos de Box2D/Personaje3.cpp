#include "Personaje3.h"

Personaje::Personaje(b2World* mundo,  sObjeto structObj):Figura(mundo,structObj){
	
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
	this->datos.alto=RADIO_CUERPO*2;
	this->datos.ancho=RADIO_CUERPO*2;

	this->estado=EVENTO_CLIENTE_PERSONAJE_QUIETO_IZQ;
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
	castearRayosDePiernas();
	castearRayosHorizontales();

	analizarMovimientos();
	desacelerarSiEstaQuieto();

	saltarPresionado=false;
}

void Personaje::castearRayosDePiernas(){

	float distanciaAlPisoDer=castearRayo(ANCHO_PIERNAS, ALTURA_INICIAL_PIERNAS,ALTURA_FINAL_PIERNAS);
	float distanciaAlPisoIzq=castearRayo(-ANCHO_PIERNAS,ALTURA_INICIAL_PIERNAS,ALTURA_FINAL_PIERNAS);

	float distanciaAlPiso=elegirDistanciaAlPiso(distanciaAlPisoDer,distanciaAlPisoIzq);
	
	if ( distanciaAlPiso < SNOWBOY_FLOTACION_DESEADA ) {

		float distanciaAFlotacionDeseada = SNOWBOY_FLOTACION_DESEADA - distanciaAlPiso;
		this->body->SetGravityScale(0);
		this->body->SetTransform(this->body->GetWorldCenter()+b2Vec2(0,distanciaAFlotacionDeseada),0);

		//NUEVO: para probar que no salga volando
		b2Vec2 velocidad = this->body->GetLinearVelocity();
		if (velocidad.y > 0 ){
			velocidad.y = 0;
			this->body->SetLinearVelocity(velocidad);
		}

	}
	else{this->body->SetGravityScale(1);}

	setearEstado(distanciaAlPiso);

}


float Personaje::castearRayo(float posX, float posYInicial, float posYFinal){
	
	b2Vec2 inicioRayo = this->body->GetWorldPoint( b2Vec2(posX,posYInicial) );
	b2Vec2 finRayo = this->body->GetWorldPoint( b2Vec2(posX,posYFinal) );


	SnowBoyRayCastCallback callback;
	this->body->GetWorld()->RayCast(&callback, inicioRayo, finRayo);

	
	float distanciaAlPiso=-1;
	if ( callback.hit ){
		distanciaAlPiso = (inicioRayo - callback.puntoDeContacto).Length();
	}
	

/*	glColor3f(1,1,1);
	glBegin(GL_LINES);
	glVertex2f(inicioRayo.x, inicioRayo.y);
	glVertex2f(finRayo.x, finRayo.y);
	glEnd();

	*/

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
	
	int estadoInicial = this->estado;//Esto es para debugear nada mas
	
	if (distanciaAlPiso<SNOWBOY_FLOTACION_DESEADA){
		/*if (this->estado==EVENTO_CLIENTE_PERSONAJE_SALTANDO){this->estado=EVENTO_CLIENTE_PERSONAJE_QUIETO;}
		else*/ if(this->estado==EVENTO_CLIENTE_PERSONAJE_SALTANDO_IZQ){this->estado=EVENTO_CLIENTE_PERSONAJE_QUIETO_IZQ;}
			else if(this->estado==EVENTO_CLIENTE_PERSONAJE_SALTANDO_DER){this->estado=EVENTO_CLIENTE_PERSONAJE_QUIETO_DER;}
	}



	//if (saltarPresionado){this->estado=EVENTO_CLIENTE_PERSONAJE_SALTANDO;}
	if ((saltarPresionado)/*&&(izqPresionado||derPresionado)*/){
		if /*(izqPresionado)*/(orientacionDerecha){this->estado=EVENTO_CLIENTE_PERSONAJE_SALTANDO_DER;}
		else {this->estado=EVENTO_CLIENTE_PERSONAJE_SALTANDO_IZQ;}
	}

	/*if((this->estado==EVENTO_CLIENTE_PERSONAJE_SALTANDO || this->estado==EVENTO_CLIENTE_PERSONAJE_SALTANDO_DER) && (izqPresionado)) {this->estado=EVENTO_CLIENTE_PERSONAJE_SALTANDO_IZQ;}
	if((this->estado==EVENTO_CLIENTE_PERSONAJE_SALTANDO || this->estado==EVENTO_CLIENTE_PERSONAJE_SALTANDO_IZQ) && (derPresionado)) {this->estado=EVENTO_CLIENTE_PERSONAJE_SALTANDO_DER;}*/

	if(this->estado==EVENTO_CLIENTE_PERSONAJE_SALTANDO_DER && (!orientacionDerecha)) {this->estado=EVENTO_CLIENTE_PERSONAJE_SALTANDO_IZQ;}
	if(this->estado==EVENTO_CLIENTE_PERSONAJE_SALTANDO_IZQ && (orientacionDerecha)) {this->estado=EVENTO_CLIENTE_PERSONAJE_SALTANDO_DER;}


	//printf("IzqPresionado: %s, DerPresionado: %s, orientacionDerecha: %s \n", izqPresionado ? "true" : "false", derPresionado ? "true" : "false", orientacionDerecha ? "true" : "false");//izqPresionado,derPresionado,orientacionDerecha);
		
	if((izqPresionado||derPresionado) && (this->estado!=EVENTO_CLIENTE_PERSONAJE_SALTANDO_IZQ && this->estado!=EVENTO_CLIENTE_PERSONAJE_SALTANDO_DER)){
		if /*(derPresionado)*/(orientacionDerecha){this->estado=EVENTO_CLIENTE_PERSONAJE_CAMINANDO_DER;}
		else {this->estado=EVENTO_CLIENTE_PERSONAJE_CAMINANDO_IZQ;}
	}


	if((!saltarPresionado && !izqPresionado && !derPresionado) && ( this->estado!=EVENTO_CLIENTE_PERSONAJE_SALTANDO_IZQ && this->estado!=EVENTO_CLIENTE_PERSONAJE_SALTANDO_DER )){
		if (this->estado==EVENTO_CLIENTE_PERSONAJE_CAMINANDO_DER){this->estado=EVENTO_CLIENTE_PERSONAJE_QUIETO_DER;}
		else if(this->estado==EVENTO_CLIENTE_PERSONAJE_CAMINANDO_IZQ){this->estado=EVENTO_CLIENTE_PERSONAJE_QUIETO_IZQ;}
			//else {this->estado=EVENTO_CLIENTE_PERSONAJE_QUIETO;}
	}
		
	if (estadoInicial != this->estado){
		//Debug estado
		switch(this->estado){
			//case EVENTO_CLIENTE_PERSONAJE_QUIETO: printf("QUIETO \n");break;
			case EVENTO_CLIENTE_PERSONAJE_QUIETO_IZQ: printf("QUIETO_IZQ orientacion: %s \n", orientacionDerecha ? "DERECHA" : "IZQUIERDA");break;
			case EVENTO_CLIENTE_PERSONAJE_QUIETO_DER: printf("QUIETO_DER orientacion: %s \n", orientacionDerecha ? "DERECHA" : "IZQUIERDA");break;
			case EVENTO_CLIENTE_PERSONAJE_CAMINANDO_IZQ: printf("CAMINANDO_IZQ orientacion: %s \n", orientacionDerecha ? "DERECHA" : "IZQUIERDA");break;
			case EVENTO_CLIENTE_PERSONAJE_CAMINANDO_DER: printf("CAMINANDO_DER orientacion: %s \n", orientacionDerecha ? "DERECHA" : "IZQUIERDA");break;
			//case EVENTO_CLIENTE_PERSONAJE_SALTANDO: printf("SALTANDO \n");break;
			case EVENTO_CLIENTE_PERSONAJE_SALTANDO_IZQ: printf("SALTANDO_IZQ orientacion: %s \n", orientacionDerecha ? "DERECHA" : "IZQUIERDA");break;
			case EVENTO_CLIENTE_PERSONAJE_SALTANDO_DER: printf("SALTANDO_DER orientacion: %s \n", orientacionDerecha ? "DERECHA" : "IZQUIERDA");break;
		}
	}
}




SnowBoyRayCastCallback* Personaje::castearRayoHorizontal(float posXFinal){
	b2Vec2 inicioRayo = this->body->GetWorldPoint( b2Vec2(0,RAYO_HORIZONTAL_POSY) );
	b2Vec2 finRayo = this->body->GetWorldPoint( b2Vec2(posXFinal,RAYO_HORIZONTAL_POSY) );
        
	SnowBoyRayCastCallback* callback = new SnowBoyRayCastCallback;
	this->body->GetWorld()->RayCast(callback, inicioRayo, finRayo);

/*	glColor3f(1,1,1);
	glBegin(GL_LINES);
	glVertex2f(inicioRayo.x, inicioRayo.y);
	glVertex2f(finRayo.x, finRayo.y);
	glEnd();
*/
	return callback;
}
//EL CASTEO DE RAYOS HORIZONTALES SE SACA DE ACA
void Personaje::castearRayosHorizontales(){
/*  AHORA SE CAMBIA PARA SEPARAR EL CASTEO DEL RAYO DEL ANALISIS DEL CALLBACK
	habilitacionDer=analizarHabilitacionParaRayo(RAYO_HORIZONTAL_POSX);
	if ((!habilitacionDer)&&(this->body->GetLinearVelocity().x>0)){this->frenarDerecha();}
	habilitacionIzq=analizarHabilitacionParaRayo(-RAYO_HORIZONTAL_POSX);
	if ((!habilitacionIzq)&&(this->body->GetLinearVelocity().x<0)){this->frenarIzquierda();}
*/	
	//Analisis derecho
	SnowBoyRayCastCallback* callbackRayoHorizontalDer = this->castearRayoHorizontal(RAYO_HORIZONTAL_POSX);
	habilitacionDer=analizarHabilitacionParaRayo(callbackRayoHorizontalDer);
//		this->enemigoBolaTocadaDer = analizarBolaTocada(callbackRayoHorizontalDer);
	delete callbackRayoHorizontalDer;

	//Analisis izquierdo
	SnowBoyRayCastCallback* callbackRayoHorizontalIzq = this->castearRayoHorizontal(-RAYO_HORIZONTAL_POSX);
	habilitacionIzq=analizarHabilitacionParaRayo(callbackRayoHorizontalIzq);
//		this->enemigoBolaTocadaIzq = analizarBolaTocada(callbackRayoHorizontalIzq);
	delete callbackRayoHorizontalIzq;
}

/*	Enemigo* SnowBoyDisparo::analizarBolaTocada(SnowBoyRayCastCallback* callback){
	return callback->enemigoBolaTocada;
}
*/

//AHORA PASA A RECIBIR UN CALLBACK
//bool SnowBoyDisparo::analizarHabilitacionParaRayo(float posXFinal){
bool Personaje::analizarHabilitacionParaRayo(SnowBoyRayCastCallback* callback){

//Calculo del angulo de la rampa:


/* AHORA EL RAYCAST SE VA  HACER APARTE Y ESTO SÓLO VA A RECIBIR UN CALLBACK PARA ANALIZARLO
	b2Vec2 inicioRayo = this->body->GetWorldPoint( b2Vec2(0,RAYO_HORIZONTAL_POSY) );
	b2Vec2 finRayo = this->body->GetWorldPoint( b2Vec2(posXFinal,RAYO_HORIZONTAL_POSY) );
        
	SnowBoyRayCastCallback callback;
	this->body->GetWorld()->RayCast(&callback, inicioRayo, finRayo);
*/
	if (( callback->hit )&&( callback->tipoDeBodyTocado==b2_staticBody )){
				
		b2Vec2 normalWorldPos=this->body->GetWorldPoint(callback->puntoFinalNormal);
		float hipotenusa = (normalWorldPos-callback->puntoDeContacto).Length();
		float adyacente = (b2Vec2(normalWorldPos.x,callback->puntoDeContacto.y)-callback->puntoDeContacto).Length();
		float anguloRespectoAlRayo=acos(adyacente/hipotenusa);
		//float anguloRampa=90-Utilities::radianes2grados(anguloRespectoAlRayo);

		//if(anguloRampa>ANGULO_MAX_ESCALABLE){return false;}
		if(false){}//linea agregada para que compile
		else {return true;}
	}
	else {return true;}
	
	//return true;//Esto lo pongo para que compile
}

void Personaje::desacelerarSiEstaQuieto(){
	//if (this->estado==EVENTO_CLIENTE_PERSONAJE_QUIETO){
	if (this->estado==EVENTO_CLIENTE_PERSONAJE_QUIETO_DER || this->estado==EVENTO_CLIENTE_PERSONAJE_QUIETO_IZQ){
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
			orientacionDerecha=true;
		}
		else if (izqPresionado && (vel.x!=-VELOCIDAD_HORIZONTAL)){
				vel.x=-VELOCIDAD_HORIZONTAL;
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
		//this->estado=SALTANDO;
		//this->estado=EVENTO_CLIENTE_PERSONAJE_SALTANDO;
		saltarPresionado=true;
	}
}

//ESTADO_PERSONAJE SnowBoy::getEstado(){
int Personaje::getEstado(){
	return this->estado;
}


b2Fixture* Personaje::getFixture(){
	return this->fixtureDef;
}


float Personaje::getY(){
	return this->body->GetWorldPoint(b2Vec2(0,-SNOWBOY_DIFERENCIA_CENTRO_CUERPO_CENTRO_FIGURA)).y;
}


Personaje::~Personaje(void)
{
}

float Personaje::calcularArea(){return 1;};


