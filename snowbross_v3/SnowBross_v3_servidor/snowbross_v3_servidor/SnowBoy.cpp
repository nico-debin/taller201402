#include "StdAfx.h"
#include "SnowBoy.h"
#include "Enemigo.h"
#include "JuegoControlador.h"

#define SNOWBOY_DURACION_INVULNERABILIDAD 600 //Nota: tiene que ser SNOWBOY_STEPS_INVULNERABILIDAD_PARPADEO * nroPar para que termine en estado normal en vez de invisible
#define SNOWBOY_STEPS_INVULNERABILIDAD_PARPADEO 20
#define SNOWBOY_DURACION_INVULNERABILIDAD_AL_SER_LIBERADO 320 

SnowBoy::SnowBoy(b2World* mundo,  sObjeto structObj):Personaje(mundo,structObj){
	this->enemigoBolaTocadaDer = nullptr;
	this->enemigoBolaTocadaIzq = nullptr;
	this->bolaCaptora = nullptr;
	
	//Esto se reemplaza por 	this->iniciarInvulnerabilidad();
/*	for (b2Fixture* fixtureActual = this->body->GetFixtureList(); fixtureActual; fixtureActual = fixtureActual->GetNext()){
		b2Filter filtro = fixtureActual->GetFilterData();
		filtro.groupIndex = -1;

		filtro.categoryBits = BITS_SNOWBOY;
		filtro.maskBits = BITS_ESCENARIO | BITS_ENEMIGO | BITS_BOLA_DE_NIEVE_QUIETA | BITS_BOLA_DE_NIEVE_PATEADA | BITS_FUEGO | BITS_BONO;// | BITS_ATAQUE_ENEMIGO;

		fixtureActual->SetFilterData(filtro);
	}
*/

	this->datos.alto=SNOWBOY_ALTO_UL;
	this->datos.ancho=SNOWBOY_ANCHO_UL;

	this->estado=EVENTO_CLIENTE_PERSONAJE_QUIETO_IZQ;
    this->tiempoAgonia = 1000;

	this->userData.tipo = TIPO_SNOWBOY;
	//this->vidas=1;

	this->iniciarInvulnerabilidad();

	this->timerRenacer = nullptr;
	this->stepsParaRenacer = 0;
	this->xInicial = structObj.x;
	this->yInicial = structObj.y;

}

int SnowBoy::getVidas(){
	return JuegoControlador::estadisticasJugadores->getVidasJugador(this->getId());
}
void SnowBoy::setVida(int vida){
	return JuegoControlador::estadisticasJugadores->setVidas(this->getId(), vida);
}

void SnowBoy::incrementarVida(){
	printf("vidas: %.3d", JuegoControlador::estadisticasJugadores->agregarVida(this->getId()));		
}

void SnowBoy::castearRayosHorizontales(){
	//Analisis derecho
	SnowBoyRayCastCallback* callbackRayoHorizontalDer = this->castearRayoHorizontal(RAYO_HORIZONTAL_POSX);
	habilitacionDer=analizarHabilitacionParaRayo(callbackRayoHorizontalDer);

	this->enemigoBolaTocadaDer = analizarBolaTocada(callbackRayoHorizontalDer);
	delete callbackRayoHorizontalDer;

	//Analisis izquierdo
	SnowBoyRayCastCallback* callbackRayoHorizontalIzq = this->castearRayoHorizontal(-RAYO_HORIZONTAL_POSX);
	habilitacionIzq=analizarHabilitacionParaRayo(callbackRayoHorizontalIzq);


	this->enemigoBolaTocadaIzq = analizarBolaTocada(callbackRayoHorizontalIzq);
	delete callbackRayoHorizontalIzq;
}


Enemigo* SnowBoy::analizarBolaTocada(SnowBoyRayCastCallback* callback){
	sUserData* userData = callback->userDataObjetoTocado;
	if ((userData != nullptr) && (userData->tipo == TIPO_DIABLO || userData->tipo == TIPO_DRAGON)){//ESTO DEBERIA REEMPLAZARSE POR esTipoEnemigo()
		//Aca el rayCast sabe que está tocando un enemigo, y se le pregunta el estado para saber si es una bola
		if (((Enemigo*) userData->objeto)->getEstado() == EVENTO_CLIENTE_ENEMIGO_BOLA_DE_NIEVE){return ((Enemigo*) userData->objeto);}
		else {return nullptr;}
		
	}
	else{return nullptr;}
}


void SnowBoy::lanzar(){
	if(this->estado!=EVENTO_CLIENTE_SNOWBOY_ATRAPADO_EN_BOLA_DE_NIEVE){
		if (this->enemigoBolaTocadaDer != nullptr && this->derPresionado){this->enemigoBolaTocadaDer->patear(true);}
		else if (this->enemigoBolaTocadaIzq != nullptr && this->izqPresionado){this->enemigoBolaTocadaIzq->patear(false);}
		else {
			Personaje::lanzar();
		}
	}
}






void SnowBoy::atraparEnBolaDeNieve(Enemigo* bola){

	if (this->estado != EVENTO_CLIENTE_SNOWBOY_ATRAPADO_EN_BOLA_DE_NIEVE){
		//Podria darse el caso de que dos bolas le peguen al SB al mismo tiempo
		this->estado = EVENTO_CLIENTE_SNOWBOY_ATRAPADO_EN_BOLA_DE_NIEVE;
		this->bolaCaptora = bola;
	}

}
void SnowBoy::liberarDeBolaDeNieve(b2Vec2 posicionDeLaBolaDestruida){
	this->body->SetTransform(posicionDeLaBolaDestruida,0);
	this->crearForma();
	this->estado = EVENTO_CLIENTE_PERSONAJE_QUIETO_IZQ;
	this->bolaCaptora = nullptr;
	this->iniciarInvulnerabilidad();
	this->stepsDuracionInvulnerabilidad = SNOWBOY_DURACION_INVULNERABILIDAD_AL_SER_LIBERADO;
}

void SnowBoy::incrementarVelocidad(){
	this->velocidadHorizontal = VELOCIDAD_HORIZONTAL_BONUS;
}

void SnowBoy::saltar(){

	if (this->bolaCaptora != nullptr ){

		//Bola captora se nullea en this->liberarDeBolaDeNieve
		this->bolaCaptora->liberarSnowBoy(this); 

		b2Vec2 vel=this->body->GetLinearVelocity();
		vel.y=VELOCIDAD_SALTO;
		this->body->SetLinearVelocity(vel);
		saltarPresionado=true;
	}
	else{
		Personaje::saltar();
	}
}

void SnowBoy::matar(){
	
	JuegoControlador::estadisticasJugadores->quitarVida(this->getId());
	
	if (this->getVidas() > 0){
		//Necesito que se ejecute esto del matar() de Personaje pero no quiero que tambien se ejecute el matar() de Figura comose hace en el matar de Personaje
		this->habilitacionIzq = false;
		this->habilitacionDer = false;
		this->habilitacionSalto = false;
	
		this->saltarPresionado = false;
		this->izqPresionado = false;
		this->derPresionado = false;
		this->orientacionDerecha = false;

		if( this->timerRenacer == nullptr ){

			this->timerRenacer = new Timer();
 			this->timerRenacer->start();
			this->estado = EVENTO_CLIENTE_FIGURA_MURIENDO;			
			//Esto es para evitar problemas si la figura debe morir y teletransportarse
			this->body->SetUserData(nullptr);

			this->cargarBitsDeColisionDeSnowboyMuerto();
		}

	}
	else{
		Personaje::matar();
		this->cargarBitsDeColisionEnFixtures(BITS_SNOWBOY, BITS_ESCENARIO);
	}	
}


bool SnowBoy::estaMuerta(){

	if (this->getVidas() > 0){
		//El tiempoAgonia es el mismo para el renacer
		if( this->timerRenacer != nullptr && this->timerRenacer->getTicks() > this->tiempoAgonia ) {
			this->estado = EVENTO_CLIENTE_SNOWBOY_VA_A_RENACER;
			if ( this->stepsParaRenacer == 0 ){
				this->stepsParaRenacer = SNOWBOY_STEPS_PARA_RENACER;
			}
		}
		return false;
	}
	else{
		return Personaje::estaMuerta();
	}
}

void SnowBoy::renacer(){
	this->body->SetUserData((void*)&this->userData);

	this->velocidadHorizontal = VELOCIDAD_HORIZONTAL;
	
	this->habilitacionIzq=true;
	this->habilitacionDer=true;
	this->habilitacionSalto=false;

	this->saltarPresionado=false;
	this->izqPresionado=false;
	this->derPresionado=false;

	this->deshabilitarSalto();

	this->enemigoBolaTocadaDer = nullptr;
	this->enemigoBolaTocadaIzq = nullptr;
	this->bolaCaptora = nullptr;

	this->estado=EVENTO_CLIENTE_PERSONAJE_QUIETO_IZQ;
	this->orientacionDerecha = false;

	this->iniciarInvulnerabilidad();


	this->moverAlInicio();
	
	delete this->timerRenacer;
	this->timerRenacer = nullptr;
}


void SnowBoy::moverAlInicio(){
	b2Vec2 posicionInicial;
	posicionInicial.x = this->xInicial;
	posicionInicial.y = this->yInicial;
	this->body->SetTransform(posicionInicial,0);
}


















void SnowBoy::actualizar(){

	if (this->stepsParaRenacer > 0 ){
		this->stepsParaRenacer--;
		if (this->stepsParaRenacer == 0){
			this->renacer();
		}
	}
	else{
		this->analizarInvulnerabilidad();
		//Esto lo hago asi xq creo que el snowboy despues va  atener mas fixtures
		//tambien se podria simular sin borrar las fixtures poniendole como maskBits 0000 y despues cambiarselos cuando se libera
		if (this->estado == EVENTO_CLIENTE_SNOWBOY_ATRAPADO_EN_BOLA_DE_NIEVE){
			b2Fixture* fixtureADestruir = nullptr;
			b2Fixture* fixtureActual = this->body->GetFixtureList();
			while(fixtureActual != nullptr){
				fixtureADestruir = fixtureActual;
				fixtureActual = fixtureActual->GetNext();
				this->body->DestroyFixture(fixtureADestruir); 			
			}		
		}
		else {Personaje::actualizar();} 
	}
}


Figura* SnowBoy::crearProyectil(sObjeto structDelProyectil){
	Nieve* nieve = new Nieve(this->body->GetWorld(), structDelProyectil);
	if (this->velocidadHorizontal == VELOCIDAD_HORIZONTAL_BONUS){
		b2Vec2 vel = this->body->GetLinearVelocity();
		if (vel.x != 0){
			nieve->aplicarInercia();
		}
	}

	return nieve;
}

void SnowBoy::inmortal(){
	this->cargarBitsDeColisionDeSnowboyInvencible();
}

void SnowBoy::cargarBitsDeColisionDeSnowboyInvencible(){
	this->cargarBitsDeColisionEnFixtures(BITS_SNOWBOY,BITS_ESCENARIO | BITS_BOLA_DE_NIEVE_QUIETA );
}
void SnowBoy::cargarBitsDeColisionDeSnowboyNormal(){
	this->cargarBitsDeColisionEnFixtures(BITS_SNOWBOY,BITS_ESCENARIO | BITS_ENEMIGO | BITS_BOLA_DE_NIEVE_QUIETA | BITS_BOLA_DE_NIEVE_PATEADA | BITS_FUEGO | BITS_BONO);
}

void SnowBoy::cargarBitsDeColisionDeSnowboyMuerto(){
	this->cargarBitsDeColisionEnFixtures(BITS_SNOWBOY,BITS_ESCENARIO);
}


void SnowBoy::iniciarInvulnerabilidad(){
	this->stepsDuracionInvulnerabilidad = SNOWBOY_DURACION_INVULNERABILIDAD;
	this->cargarBitsDeColisionDeSnowboyInvencible();
	this->devolverEstadoInvulnerable = true;	
	this->cambioEstadoDeInvulnerabilidad = true;
	this->stepsParaCambiarDeEstadoADevolver = SNOWBOY_STEPS_INVULNERABILIDAD_PARPADEO;
}

void SnowBoy::terminarInvulnerabilidad(){
	this->stepsDuracionInvulnerabilidad = 0;
	this->cargarBitsDeColisionDeSnowboyNormal();
	this->devolverEstadoInvulnerable = false;
	this->cambioEstadoDeInvulnerabilidad = false;
}



void SnowBoy::analizarInvulnerabilidad(){
	if (this->stepsDuracionInvulnerabilidad > 0 ){
		stepsDuracionInvulnerabilidad--;
		if (this->stepsDuracionInvulnerabilidad == 0 ){
			this->terminarInvulnerabilidad();
		}
		else{
			if(this->devolverEstadoInvulnerable){
				if(this->stepsParaCambiarDeEstadoADevolver > 0){				
					this->stepsParaCambiarDeEstadoADevolver--;
					if(this->stepsParaCambiarDeEstadoADevolver == 0){
						this->cambioEstadoDeInvulnerabilidad = true;
						this->stepsParaCambiarDeEstadoADevolver = SNOWBOY_STEPS_INVULNERABILIDAD_PARPADEO;
						this->devolverEstadoInvulnerable = false;
					}
					else{
						this->cambioEstadoDeInvulnerabilidad = false;
					}
				}
			}
			else{
				if(this->stepsParaCambiarDeEstadoADevolver > 0){				
					this->stepsParaCambiarDeEstadoADevolver--;
					if(this->stepsParaCambiarDeEstadoADevolver == 0){
						this->cambioEstadoDeInvulnerabilidad = true;
						this->stepsParaCambiarDeEstadoADevolver = SNOWBOY_STEPS_INVULNERABILIDAD_PARPADEO;
						this->devolverEstadoInvulnerable = true;
					}
					else{
						this->cambioEstadoDeInvulnerabilidad = false;
					}
				}												
			}
		}
	}
}


bool SnowBoy::getDevolverEstadoInvulnerable(){
	return this->devolverEstadoInvulnerable;
}
bool SnowBoy::getCambioEstadoDeInvulnerabilidad(){
	return this->cambioEstadoDeInvulnerabilidad;
}



void SnowBoy::setearEstado(float distanciaAlPiso){

	//Esto es para que el seteo de estado del SB corriendo no afecte al seteo de los otros estados
	if ( this->estado == EVENTO_CLIENTE_PERSONAJE_CORRIENDO_DER ){
		this->estado = EVENTO_CLIENTE_PERSONAJE_CAMINANDO_DER;
	}
	else if ( this->estado == EVENTO_CLIENTE_PERSONAJE_CORRIENDO_IZQ ){
		this->estado = EVENTO_CLIENTE_PERSONAJE_CAMINANDO_IZQ;
	}
	else if ( this->estado == EVENTO_CLIENTE_PERSONAJE_CORRIENDO_QUIETO_DER){
		this->estado = EVENTO_CLIENTE_PERSONAJE_QUIETO_DER ;
	}
	else if ( this->estado == EVENTO_CLIENTE_PERSONAJE_CORRIENDO_QUIETO_IZQ){
		this->estado = EVENTO_CLIENTE_PERSONAJE_QUIETO_IZQ ;
	}		
	
	Personaje::setearEstado(distanciaAlPiso);

	if (this->velocidadHorizontal == VELOCIDAD_HORIZONTAL_BONUS){	
		if ( this->estado == EVENTO_CLIENTE_PERSONAJE_CAMINANDO_DER ){
			this->estado = EVENTO_CLIENTE_PERSONAJE_CORRIENDO_DER;
		}
		else if ( this->estado == EVENTO_CLIENTE_PERSONAJE_CAMINANDO_IZQ ){
			this->estado = EVENTO_CLIENTE_PERSONAJE_CORRIENDO_IZQ;
		}
		else if ( this->estado == EVENTO_CLIENTE_PERSONAJE_QUIETO_DER ){
			this->estado = EVENTO_CLIENTE_PERSONAJE_CORRIENDO_QUIETO_DER;
		}
		else if ( this->estado == EVENTO_CLIENTE_PERSONAJE_QUIETO_IZQ ){
			this->estado = EVENTO_CLIENTE_PERSONAJE_CORRIENDO_QUIETO_IZQ;
		}		
	}


}


SnowBoy::~SnowBoy(){
	if (this->timerRenacer != nullptr){
		delete this->timerRenacer;
	}
	this->timerRenacer = nullptr;
};