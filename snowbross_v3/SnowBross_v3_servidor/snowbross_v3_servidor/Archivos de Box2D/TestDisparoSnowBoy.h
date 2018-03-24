#ifndef _TestDisparo_H_
#define _TestDisparo_H_


#include <Box2D\Box2D.h>
#include "Personaje3.h"
#include "SnowBoy3.h"
#include "Nieve3.h"
#include "DisparoContactListener3.h"

//class Utillities{
//public:
//	static std::set<b2Body*> bodiesDeNieveAEliminar;
//}
//
//
//
//
// class Nieve {
//	/*ctes:
//		NieveOffsetX
//		NieveOffsetY
//		NieveRadio
//		NieveVelocidadHorizontal
//	*/
//	private:
//		b2Body* body;
//		void Nieve::crearBody(b2Body* bodyDelSnowBoy, bool snowBoyOrientacionDerecha){
//			b2BodyDef bodyDef;			
//			bodyDef.type = b2_dynamicBody;
//						
//			b2Vec2 posicionDelSnowBoy = bodyDelSnowBoy->GetPosition();
//			//No se que nombre ponerle
//			//Despues van a pasar a ser ctes
//			float NieveOffsetX = RADIO_CUERPO + 0.2;
//			float NieveOffsetY = RADIO_CUERPO/2;
//
//			if (snowBoyOrientacionDerecha){bodyDef.position.Set(posicionDelSnowBoy.x + NieveOffsetX, posicionDelSnowBoy.y + NieveOffsetY);}
//			else{bodyDef.position.Set(posicionDelSnowBoy.x - NieveOffsetX, posicionDelSnowBoy.y + NieveOffsetY);}
//			bodyDef.angle=0;
//			this->body = bodyDelSnowBoy->GetWorld()->CreateBody(&bodyDef);			
//		}
//
//		void Nieve::crearForma(){
//			b2CircleShape circuloShape;
//			circuloShape.m_p.Set(0, 0);			
//			//Va a ser cte
//			float NieveRadio = 0.1f;
//			circuloShape.m_radius = NieveRadio;
//
//			b2FixtureDef nieveFixDef;
//			nieveFixDef.shape = &circuloShape;
//			nieveFixDef.restitution = 0;
//			
//			nieveFixDef.density = 0.005;//Para que no  pueda empujar nada
//			this->body->CreateFixture(&nieveFixDef); 
//		}
//
//		void Nieve::setearVelocidad(bool snowBoyOrientacionDerecha){
//			//Va a ser cte
//			float NieveVelocidadHorizontal = 15;
//			if (snowBoyOrientacionDerecha){this->body->SetLinearVelocity(b2Vec2(NieveVelocidadHorizontal,0));}
//			else {this->body->SetLinearVelocity(b2Vec2(-NieveVelocidadHorizontal,0));}
//
//		}
//
//	public:
//		Nieve::Nieve( b2Body* bodyDelSnowBoy, bool snowBoyOrientacionDerecha){
//			crearBody(bodyDelSnowBoy,snowBoyOrientacionDerecha);
//			crearForma();
//			setearVelocidad(snowBoyOrientacionDerecha);
//
//			//Cualquier cosa para identificarlo, puede ser el tipo, o TIPO_NIEVE		
//			
//			/*Ahora esto va ser un sUserData y se va a destruir cuando se destruyen los bodies de las nieves
//			pair<TIPO_FIGURA, Nieve*>* userData = new pair<TIPO_FIGURA,Nieve*>(TIPO_NIEVE,this);
//			this->body->SetUserData((void*)userData);	
//			*/
//			sUserData* nieveUserData = new sUserData;
//			nieveUserData->tipo = TIPO_NIEVE;
//			nieveUserData->objeto = this;
//			this->body->SetUserData((void*)nieveUserData);	
//
//
//		}
//
//		Nieve::~Nieve(){
//			//Aca voy a probar si se pierde memoria o no al destruir
//
//		}
//};
//




//class SnowBoyDisparo: public Figura{
//protected:		
//	//b2Body* snowBody;
//	bool habilitacionIzq;
//	bool habilitacionDer;
//	bool habilitacionSalto;
//	
//	bool saltarPresionado;
//	bool izqPresionado;
//	bool derPresionado;
//	bool orientacionDerecha;
//
//
////	Enemigo* enemigoBolaTocadaDer;
////	Enemigo* enemigoBolaTocadaIzq;
//
//
//
//	int estado;
//	b2Fixture* fixtureDef;
//	sObjeto datos;
//public:
//	SnowBoyDisparo(b2World* mundo,  sObjeto structObj):Figura(mundo,structObj){
//	
//		this->body->SetType(b2_dynamicBody);
//		this->body->SetFixedRotation(true);
//
//		crearForma();
//
//		this->habilitacionIzq=true;
//		this->habilitacionDer=true;
//		this->habilitacionSalto=false;
//
//		this->saltarPresionado=false;
//		this->izqPresionado=false;
//		this->derPresionado=false;
//
//		this->datos.radio=RADIO_CUERPO;
//		this->datos.alto=RADIO_CUERPO*2;
//		this->datos.ancho=RADIO_CUERPO*2;
//
////		this->enemigoBolaTocadaDer = nullptr;
////		this->enemigoBolaTocadaIzq = nullptr;
//
//
//		this->estado=EVENTO_CLIENTE_PERSONAJE_QUIETO_IZQ;
//	}
//
//	void SnowBoyDisparo::crearForma(){
//		b2CircleShape cuerpo;
//		cuerpo.m_radius=RADIO_CUERPO;
//		cuerpo.m_p=b2Vec2(0,0);
//
//		b2FixtureDef fixtureDef;
//		fixtureDef.shape = &cuerpo;
//		fixtureDef.density = 5;
//		fixtureDef.friction = 0;
//      
//		fixtureDef.filter.groupIndex = -1;
//
//		//fixtureDef.filter.categoryBits =
//		//fixtureDef.filter.categoryBits =
//
//
//		this->fixtureDef = this->body->CreateFixture(&fixtureDef);
//	}
//
//
//	void SnowBoyDisparo::actualizar(){
//		castearRayosDePiernas();
//		castearRayosHorizontales();
//
//		analizarMovimientos();
//		desacelerarSiEstaQuieto();
//
//		saltarPresionado=false;
//	}
//
//	void SnowBoyDisparo::castearRayosDePiernas(){
//
//		float distanciaAlPisoDer=castearRayo(ANCHO_PIERNAS, ALTURA_INICIAL_PIERNAS,ALTURA_FINAL_PIERNAS);
//		float distanciaAlPisoIzq=castearRayo(-ANCHO_PIERNAS,ALTURA_INICIAL_PIERNAS,ALTURA_FINAL_PIERNAS);
//
//		float distanciaAlPiso=elegirDistanciaAlPiso(distanciaAlPisoDer,distanciaAlPisoIzq);
//	
//		if ( distanciaAlPiso < SNOWBOY_FLOTACION_DESEADA ) {
//
//			float distanciaAFlotacionDeseada = SNOWBOY_FLOTACION_DESEADA - distanciaAlPiso;
//			this->body->SetGravityScale(0);
//			this->body->SetTransform(this->body->GetWorldCenter()+b2Vec2(0,distanciaAFlotacionDeseada),0);
//		
//
//		}
//		else{this->body->SetGravityScale(1);}
//
//		setearEstado(distanciaAlPiso);
//
//	}
//
//
//	float SnowBoyDisparo::castearRayo(float posX, float posYInicial, float posYFinal){
//	
//		b2Vec2 inicioRayo = this->body->GetWorldPoint( b2Vec2(posX,posYInicial) );
//		b2Vec2 finRayo = this->body->GetWorldPoint( b2Vec2(posX,posYFinal) );
//
//
//		SnowBoyRayCastCallback callback;
//		this->body->GetWorld()->RayCast(&callback, inicioRayo, finRayo);
//
//	
//		float distanciaAlPiso=-1;
//		if ( callback.hit ){
//			distanciaAlPiso = (inicioRayo - callback.puntoDeContacto).Length();
//		}
//	
//
//		glColor3f(1,1,1);
//		glBegin(GL_LINES);
//		glVertex2f(inicioRayo.x, inicioRayo.y);
//		glVertex2f(finRayo.x, finRayo.y);
//		glEnd();
//
//
//
//		return distanciaAlPiso;
//	}
//
//	float SnowBoyDisparo::elegirDistanciaAlPiso(float distanciaAlPisoDer, float distanciaAlPisoIzq){
//		float distanciaAlPiso;
//		if((distanciaAlPisoDer==-1)&&(distanciaAlPisoIzq==-1)){
//				//Si ninguno esta tocando algo
//				habilitacionSalto=false;
//				distanciaAlPiso=SNOWBOY_FLOTACION_DESEADA+1;
//			}
//			else{
//				//Por lo menos una pierna esta tocando algo
//				habilitacionSalto=true;
//				//Si las dos estan tocando algo:
//				if ((distanciaAlPisoDer>=0)&&(distanciaAlPisoIzq>=0)){
//					//La menor se setea como distancia al piso
//					if(distanciaAlPisoDer<distanciaAlPisoIzq){distanciaAlPiso=distanciaAlPisoDer;}
//					else{distanciaAlPiso=distanciaAlPisoIzq;}
//				}
//				else{
//					//Si la pierna der no toca nada, la izq se setea como distancia al piso
//					//Si la pierna izq no toca nada, la der se setea como distancia al piso
//					if (distanciaAlPisoDer==-1){distanciaAlPiso=distanciaAlPisoIzq;}
//					else{distanciaAlPiso=distanciaAlPisoDer;}
//				}	
//			}
//			return distanciaAlPiso;
//	}
//
//
//	void SnowBoyDisparo::setearEstado(float distanciaAlPiso){
//		
//		int estadoInicial = this->estado;
//	
//		if (distanciaAlPiso<SNOWBOY_FLOTACION_DESEADA){
//			/*if (this->estado==EVENTO_CLIENTE_PERSONAJE_SALTANDO){this->estado=EVENTO_CLIENTE_PERSONAJE_QUIETO;}
//			else*/ if(this->estado==EVENTO_CLIENTE_PERSONAJE_SALTANDO_IZQ){this->estado=EVENTO_CLIENTE_PERSONAJE_QUIETO_IZQ;}
//				else if(this->estado==EVENTO_CLIENTE_PERSONAJE_SALTANDO_DER){this->estado=EVENTO_CLIENTE_PERSONAJE_QUIETO_DER;}
//		}
//
//
//
//		//if (saltarPresionado){this->estado=EVENTO_CLIENTE_PERSONAJE_SALTANDO;}
//		if ((saltarPresionado)/*&&(izqPresionado||derPresionado)*/){
//			if /*(izqPresionado)*/(orientacionDerecha){this->estado=EVENTO_CLIENTE_PERSONAJE_SALTANDO_DER;}
//			else {this->estado=EVENTO_CLIENTE_PERSONAJE_SALTANDO_IZQ;}
//		}
//
//		/*if((this->estado==EVENTO_CLIENTE_PERSONAJE_SALTANDO || this->estado==EVENTO_CLIENTE_PERSONAJE_SALTANDO_DER) && (izqPresionado)) {this->estado=EVENTO_CLIENTE_PERSONAJE_SALTANDO_IZQ;}
//		if((this->estado==EVENTO_CLIENTE_PERSONAJE_SALTANDO || this->estado==EVENTO_CLIENTE_PERSONAJE_SALTANDO_IZQ) && (derPresionado)) {this->estado=EVENTO_CLIENTE_PERSONAJE_SALTANDO_DER;}*/
//
//		if(this->estado==EVENTO_CLIENTE_PERSONAJE_SALTANDO_DER && (!orientacionDerecha)) {this->estado=EVENTO_CLIENTE_PERSONAJE_SALTANDO_IZQ;}
//		if(this->estado==EVENTO_CLIENTE_PERSONAJE_SALTANDO_IZQ && (orientacionDerecha)) {this->estado=EVENTO_CLIENTE_PERSONAJE_SALTANDO_DER;}
//
//
//		//printf("IzqPresionado: %s, DerPresionado: %s, orientacionDerecha: %s \n", izqPresionado ? "true" : "false", derPresionado ? "true" : "false", orientacionDerecha ? "true" : "false");//izqPresionado,derPresionado,orientacionDerecha);
//		
//		if((izqPresionado||derPresionado) && (this->estado!=EVENTO_CLIENTE_PERSONAJE_SALTANDO_IZQ && this->estado!=EVENTO_CLIENTE_PERSONAJE_SALTANDO_DER)){
//			if /*(derPresionado)*/(orientacionDerecha){this->estado=EVENTO_CLIENTE_PERSONAJE_CAMINANDO_DER;}
//			else {this->estado=EVENTO_CLIENTE_PERSONAJE_CAMINANDO_IZQ;}
//		}
//
//
//		if((!saltarPresionado && !izqPresionado && !derPresionado) && ( this->estado!=EVENTO_CLIENTE_PERSONAJE_SALTANDO_IZQ && this->estado!=EVENTO_CLIENTE_PERSONAJE_SALTANDO_DER )){
//			if (this->estado==EVENTO_CLIENTE_PERSONAJE_CAMINANDO_DER){this->estado=EVENTO_CLIENTE_PERSONAJE_QUIETO_DER;}
//			else if(this->estado==EVENTO_CLIENTE_PERSONAJE_CAMINANDO_IZQ){this->estado=EVENTO_CLIENTE_PERSONAJE_QUIETO_IZQ;}
//				//else {this->estado=EVENTO_CLIENTE_PERSONAJE_QUIETO;}
//		}
//		
//		if (estadoInicial != this->estado){
//			//Debug estado
//			switch(this->estado){
//				//case EVENTO_CLIENTE_PERSONAJE_QUIETO: printf("QUIETO \n");break;
//				case EVENTO_CLIENTE_PERSONAJE_QUIETO_IZQ: printf("QUIETO_IZQ orientacion: %s \n", orientacionDerecha ? "DERECHA" : "IZQUIERDA");break;
//				case EVENTO_CLIENTE_PERSONAJE_QUIETO_DER: printf("QUIETO_DER orientacion: %s \n", orientacionDerecha ? "DERECHA" : "IZQUIERDA");break;
//				case EVENTO_CLIENTE_PERSONAJE_CAMINANDO_IZQ: printf("CAMINANDO_IZQ orientacion: %s \n", orientacionDerecha ? "DERECHA" : "IZQUIERDA");break;
//				case EVENTO_CLIENTE_PERSONAJE_CAMINANDO_DER: printf("CAMINANDO_DER orientacion: %s \n", orientacionDerecha ? "DERECHA" : "IZQUIERDA");break;
//				//case EVENTO_CLIENTE_PERSONAJE_SALTANDO: printf("SALTANDO \n");break;
//				case EVENTO_CLIENTE_PERSONAJE_SALTANDO_IZQ: printf("SALTANDO_IZQ orientacion: %s \n", orientacionDerecha ? "DERECHA" : "IZQUIERDA");break;
//				case EVENTO_CLIENTE_PERSONAJE_SALTANDO_DER: printf("SALTANDO_DER orientacion: %s \n", orientacionDerecha ? "DERECHA" : "IZQUIERDA");break;
//			}
//		}
//	}
//
//
//
//
//	SnowBoyRayCastCallback* SnowBoyDisparo::castearRayoHorizontal(float posXFinal){
//		b2Vec2 inicioRayo = this->body->GetWorldPoint( b2Vec2(0,RAYO_HORIZONTAL_POSY) );
//		b2Vec2 finRayo = this->body->GetWorldPoint( b2Vec2(posXFinal,RAYO_HORIZONTAL_POSY) );
//        
//		SnowBoyRayCastCallback* callback = new SnowBoyRayCastCallback;
//		this->body->GetWorld()->RayCast(callback, inicioRayo, finRayo);
//
//		glColor3f(1,1,1);
//		glBegin(GL_LINES);
//		glVertex2f(inicioRayo.x, inicioRayo.y);
//		glVertex2f(finRayo.x, finRayo.y);
//		glEnd();
//
//		return callback;
//	}
////EL CASTEO DE RAYOS HORIZONTALES SE SACA DE ACA
//	virtual void SnowBoyDisparo::castearRayosHorizontales(){
//	/*  AHORA SE CAMBIA PARA SEPARAR EL CASTEO DEL RAYO DEL ANALISIS DEL CALLBACK
//		habilitacionDer=analizarHabilitacionParaRayo(RAYO_HORIZONTAL_POSX);
//		if ((!habilitacionDer)&&(this->body->GetLinearVelocity().x>0)){this->frenarDerecha();}
//		habilitacionIzq=analizarHabilitacionParaRayo(-RAYO_HORIZONTAL_POSX);
//		if ((!habilitacionIzq)&&(this->body->GetLinearVelocity().x<0)){this->frenarIzquierda();}
//	*/	
//		//Analisis derecho
//		SnowBoyRayCastCallback* callbackRayoHorizontalDer = this->castearRayoHorizontal(RAYO_HORIZONTAL_POSX);
//		habilitacionDer=analizarHabilitacionParaRayo(callbackRayoHorizontalDer);
////		this->enemigoBolaTocadaDer = analizarBolaTocada(callbackRayoHorizontalDer);
//		delete callbackRayoHorizontalDer;
//
//		//Analisis izquierdo
//		SnowBoyRayCastCallback* callbackRayoHorizontalIzq = this->castearRayoHorizontal(-RAYO_HORIZONTAL_POSX);
//		habilitacionIzq=analizarHabilitacionParaRayo(callbackRayoHorizontalIzq);
////		this->enemigoBolaTocadaIzq = analizarBolaTocada(callbackRayoHorizontalIzq);
//		delete callbackRayoHorizontalIzq;
//	}
//
///*	Enemigo* SnowBoyDisparo::analizarBolaTocada(SnowBoyRayCastCallback* callback){
//		return callback->enemigoBolaTocada;
//	}
//*/
//
//	//AHORA PASA A RECIBIR UN CALLBACK
//	//bool SnowBoyDisparo::analizarHabilitacionParaRayo(float posXFinal){
//	bool SnowBoyDisparo::analizarHabilitacionParaRayo(SnowBoyRayCastCallback* callback){
//
//	//Calculo del angulo de la rampa:
//
//
//	/* AHORA EL RAYCAST SE VA  HACER APARTE Y ESTO SÓLO VA A RECIBIR UN CALLBACK PARA ANALIZARLO
//		b2Vec2 inicioRayo = this->body->GetWorldPoint( b2Vec2(0,RAYO_HORIZONTAL_POSY) );
//		b2Vec2 finRayo = this->body->GetWorldPoint( b2Vec2(posXFinal,RAYO_HORIZONTAL_POSY) );
//        
//		SnowBoyRayCastCallback callback;
//		this->body->GetWorld()->RayCast(&callback, inicioRayo, finRayo);
//	*/
//		if (( callback->hit )&&( callback->tipoDeBodyTocado==b2_staticBody )){
//				
//			b2Vec2 normalWorldPos=this->body->GetWorldPoint(callback->puntoFinalNormal);
//			float hipotenusa = (normalWorldPos-callback->puntoDeContacto).Length();
//			float adyacente = (b2Vec2(normalWorldPos.x,callback->puntoDeContacto.y)-callback->puntoDeContacto).Length();
//			float anguloRespectoAlRayo=acos(adyacente/hipotenusa);
//			//float anguloRampa=90-Utilities::radianes2grados(anguloRespectoAlRayo);
//
//			//if(anguloRampa>ANGULO_MAX_ESCALABLE){return false;}
//			if(false){}//linea agregada para que compile
//			else {return true;}
//		}
//		else {return true;}
//	
//		//return true;//Esto lo pongo para que compile
//	}
//
//	void SnowBoyDisparo::desacelerarSiEstaQuieto(){
//		//if (this->estado==EVENTO_CLIENTE_PERSONAJE_QUIETO){
//		if (this->estado==EVENTO_CLIENTE_PERSONAJE_QUIETO_DER || this->estado==EVENTO_CLIENTE_PERSONAJE_QUIETO_IZQ){
//			b2Vec2 vel=this->body->GetLinearVelocity();
//			if (vel.x>0){
//					vel.x-=SNOWBOY_CONSTANTE_DESACELERACION;
//					if (vel.x<0){vel.x=0;}
//					this->body->SetLinearVelocity(vel);
//				}
//			if (vel.x<0){
//					vel.x+=SNOWBOY_CONSTANTE_DESACELERACION;
//					if (vel.x>0){vel.x=0;}
//					this->body->SetLinearVelocity(vel);
//				}
//		}
//	}
//
//	void SnowBoyDisparo::analizarMovimientos(){
//		b2Vec2 vel=this->body->GetLinearVelocity();
//
//		if (derPresionado && izqPresionado){
//			if (orientacionDerecha){//Si el ultimo presionado fue el boton Derecho
//				vel.x=VELOCIDAD_HORIZONTAL;
//				this->body->SetLinearVelocity(vel);
//			}
//			else {//Si el ultimo presionado fue el boton Izquierdo
//				vel.x=-VELOCIDAD_HORIZONTAL;
//				this->body->SetLinearVelocity(vel);
//			}
//		}else{
//			if (derPresionado && (vel.x!=VELOCIDAD_HORIZONTAL)){
//				vel.x=VELOCIDAD_HORIZONTAL;
//				this->body->SetLinearVelocity(vel);
//				orientacionDerecha=true;
//			}
//			else if (izqPresionado && (vel.x!=-VELOCIDAD_HORIZONTAL)){
//					vel.x=-VELOCIDAD_HORIZONTAL;
//					this->body->SetLinearVelocity(vel);
//					orientacionDerecha=false;
//			}
//		}
//	}
//
//	void SnowBoyDisparo::moverDerecha(){
//		if (habilitacionDer){
//			derPresionado=true;
//			orientacionDerecha=true;
//		}
//	}
//	void SnowBoyDisparo::moverIzquierda(){
//		if (habilitacionIzq){
//			izqPresionado=true;
//			orientacionDerecha=false;	
//		}
//	}
//
//	void SnowBoyDisparo::frenarDerecha(){
//		b2Vec2 vel=this->body->GetLinearVelocity();
//		if (vel.x>0){
//			vel.x=0; 
//			this->body->SetLinearVelocity(vel);
//		}
//		derPresionado=false;
//	}
//
//	void SnowBoyDisparo::frenarIzquierda(){
//		b2Vec2 vel=this->body->GetLinearVelocity();
//		if (vel.x<0){
//			vel.x=0; 
//			this->body->SetLinearVelocity(vel);
//		}
//		izqPresionado=false;
//	}
//
//
//	void SnowBoyDisparo::saltar(){
//		if (habilitacionSalto){
//			b2Vec2 vel=this->body->GetLinearVelocity();
//			vel.y=VELOCIDAD_SALTO;
//			this->body->SetLinearVelocity(vel);
//			//this->estado=SALTANDO;
//			//this->estado=EVENTO_CLIENTE_PERSONAJE_SALTANDO;
//			saltarPresionado=true;
//		}
//	}
//
//	//ESTADO_PERSONAJE SnowBoy::getEstado(){
//	int SnowBoyDisparo::getEstado(){
//		return this->estado;
//	}
//
//
//	b2Fixture* SnowBoyDisparo::getFixture(){
//		return this->fixtureDef;
//	}
//
//
//	float SnowBoyDisparo::getY(){
//		return this->body->GetWorldPoint(b2Vec2(0,-SNOWBOY_DIFERENCIA_CENTRO_CUERPO_CENTRO_FIGURA)).y;
//	}
//
//
//	SnowBoyDisparo::~SnowBoyDisparo(void)
//	{
//	}
//
//	float SnowBoyDisparo::calcularArea(){return 1;};
//
//
//	void SnowBoyDisparo::disparar(){
//		//Se le pasa el body para saber en qué posició crearla y también para tener el world usando body->getWorld()
//		Nieve nieve(this->body, orientacionDerecha);
//		//Nieve* nieve = new Nieve(this->body, orientacionDerecha);
//		/*Aca hay dos opciones, 1 es que el objeto sea estatico y se destrua en el momento, el body queda vivo
//		  en el world, y despues se agrega a la lista de bodies a destruir
//
//		  La otra es que sea un objeto dinamico, que se destructor tenga un destroyBody y que la nieve se agregue 
//		  a una lista de nieve a destruir y despues se le hace delete
//
//		  PROBLEMA:
//			-Si se crea estático, entonces NUNCA se hace delete de la user data y habría que borrarla desde afuera, antes de eliminar el body. Algo que mucho no me gusta como suena
//			-Si se crea dinámico se puede deletear en el destructor, pero, en vez de tener un grupo de bodies a deletear, debería haber un grupo de Nieve a deletear
//				-La última opción sería tener un grupo de bodies a deletear y ahí meto todos los bodies, sólo que antes al body le pido la user data y a la user data le pido el objeto y lo deleteo
//
//			-Otro problema: si se crea estáticamente, queda inutilizable el ptero 'this' en el userData de la Nieve
//		*/
//	}
// };





































 






 
















































 /*
	-ANALIZAR COMO PASAR LA USER DATA
	-IMP: TAMBIEN HAY QUE PONERLE USER DATA AL PISO Y PAREDES
	-Ahora al poner la User Data se hace new y se pierde memoria
	-Tambien hay que ver cuando deletear la nieve

	-En la UserData puedo poner el Tipo y el puntero al objeto (this), despues si es nieve puedo
	 agregar el objeto Nieve a la lista de borrado y al borrarlo hacer delete Nieve y que el destructor de nieve
	 elimine el body del World

	-Puede disparar mientras salta?
	-Tal vez tambien haya que tener quieto izq y quieto der para saber a donde disparar

	-Falta ver el choque nieve con nieve (puede ser que se disparen la nieve entre dos jugadores
	 (esto se puede arreglar haciendo que no colisione la nieve con nieve)

	-UserData del enemigo se carga en su constructor
		-1)La user data necesita que tenga la referencia al Enemigo para poder atacarlo, con un par: TIPO_FIGURA-ENEMIGO*
		-2)La otra opcion es que tenga la user data tenga el par TIPO-ID y que se agregue el ID a una lista que se procesa despues
		-3)Se puede tener en la user data un OBJETO UserData
	-Enemigo va a tener golpear();
	-Dos formas: 1)que se lo ataque en el momento
				 2)que se lo agregue a una lista de enemigos a atacar y se lo ataque después de procesar todas las colisiones

	-Ahora voy a hacer la de pasar el par con TIPO_FIGURA-ENEMIGO* y de atacarlo en el momento
	-La user data es un puntero a un pair<TIPO_FIGURA, Enemigo*>, Enemigo va a tener ese par como variable que la va a deletear en su destructor
	-Voy a ponerle al resto un pair en la userData para poder manejarlo todo de la misma manera, osea que sea un par<TIPO-*algo*>
	-Problema: va a haber algunos que no se deleteen como la user data del piso
	
	-Pasé al Enemigo aca
	
	-Si la nieve toca las piernas de un enemigo entonces se eleva el enemigo
	-Posibles soluciones:
		1)Que el RayCast ignore a la nieve
		2)Ponerle un sensor a a la parte de abajo del enemigo
		3)Hacer que la nieve sea un sensor

	-En la prueba el enemigo se mueve con 'q' pero no se le pone frenar izquierda en el keyup

	-Se resetea el contaor de steps cuando atacan al enemigo

	-this->fixtureDef se puede poner como protected y cambiar cuando se hace bola el enemigo

	-Se agrega la transformacion en bola de nieve en el actualizar de enemigo

	-Faltaba ponerle UserData al snowboy, para que no se pise con la del Enemigo, que hereda del snowBoy, ahora la voy a poner
	 a mano en la test. Despues el movimiento va a tener que estar en  una clase Personaje, y Snowboy va a heredar de esa y se le va a poder poner userData en el constructor
	 La pongo asi ahora para que no me tire el error de que no se puede acceder a la user data cuando el SB chaoca con algo

	 -Posibles soluciones para las hitbox de las piernas
		1)Que los RayCast ignoren a los sensores
		2)Que las piernas tengan sensores a la izquierda y derecha (dos rectangulos separados)

	-Agregue el estado de bola de nieve

	Enemigo objetivos:
	a)Que reconozca el contacto con la nieve (X)
	b)Que cuente cuantas veces fue golpeado (X)
	c)Que se detenga cuando tiene nieve encima (X)
	d)Que se transforme en bola cuando le pegan X veces (X)
	e)Que se destransforme despues de Y steps (X)


	Falta:
	f) Snowboy muere cuando toca a un enemigo
	g) Snowboy que patee a la bola de nieve
	h) Bola de nieve golpea a un enemigo y lo mata
	i) Bola de nieve arrastra a los tros snowboys
		-Bola puede tener un aray de 4, que tenga pteros a los SnowBoys que arrastra


	Preguntas:
	-La userData se pasa el ptero al struct, es necesario hacerle 'new' o con dejarlo como variable de instancia alcanza?
	-Hace falta deletearlo o x ser de instancia, se borra sólo cuando se destruye el objeto?

 */

class DisparoSnowBoy : public Test
{
public:
	//SnowBoyDisparo* snowBoy;
	SnowBoy3* snowBoy;

	
	//pair<TIPO_FIGURA,SnowBoyDisparo*>* userDataSnowBoy;
	sUserData* userDataSnowBoy;

	DisparoContactListener* disparoListener;
	Enemigo* enemigo;
	Enemigo* enemigo2;
	
    DisparoSnowBoy()
    {
		
		sObjeto obj;
		obj.x=0;
		obj.y=1;
		obj.estatico=false;
		obj.rot=0;
		//snowBoy=new SnowBoyDisparo(m_world,obj);
		snowBoy=new SnowBoy3(m_world,obj);
		/*
		userDataSnowBoy = new pair<TIPO_FIGURA,SnowBoyDisparo*>(TIPO_SNOWBOY, snowBoy);
		snowBoy->getBody()->SetUserData(userDataSnowBoy);
		*/

		userDataSnowBoy = new sUserData;
		userDataSnowBoy->tipo = TIPO_SNOWBOY;
		userDataSnowBoy->objeto = snowBoy;
		snowBoy->getBody()->SetUserData(userDataSnowBoy);

		obj.x=2;
		enemigo=new Enemigo(m_world,obj);

		obj.x=3;
		enemigo2=new Enemigo(m_world,obj);


		disparoListener = new DisparoContactListener();

//Piso
		{
			
			//a static floor to drop things on
			b2Vec2 v1(-200.0f, 0.0f);
			b2Vec2 v2(200.0f, 0.0f);
			b2EdgeShape edge;
			edge.Set(v1, v2);

			b2BodyDef pisoDef;
			pisoDef.position.Set(0.0f,0);
			pisoDef.type=b2_staticBody;
			b2Body* piso=m_world->CreateBody(&pisoDef);

			b2FixtureDef pisoFixDef;
			pisoFixDef.shape=&edge;
			pisoFixDef.restitution=0;
			pisoFixDef.friction=1;
			piso->CreateFixture(&pisoFixDef);

			//NO SE DELETEA
			//pair<TIPO_FIGURA, b2Body*>* userData = new pair<TIPO_FIGURA,b2Body*>(TIPO_NULL,nullptr);

			//NO SE DELETEA
			sUserData peru = {TIPO_NULL, nullptr};
			sUserData* userData = new sUserData;
			userData->tipo = TIPO_NULL;
			userData->objeto = nullptr;
			piso->SetUserData((void*)userData);
		}

		
		{	sObjeto obj2;
			obj2.x=25;
			obj2.y=5;
			obj2.radio = 2;

			obj2.color="Rojo";
			obj2.rot=0;
			obj2.masa=1;
			obj2.estatico=true;
			obj2.alto=10;
			obj2.ancho=5;

			Rectangulo rect(m_world,obj2);

			rect.getBody()->SetUserData(nullptr);

			obj2.x = -25;
			Rectangulo rect2(m_world,obj2);
			rect2.getBody()->SetUserData(nullptr);

		}


		m_world->SetContactListener(disparoListener);

    }

//Step
    void Step(Settings* settings)
    {
		
		
        Test::Step(settings);
		
		if (enemigo != nullptr){
			if (enemigo->estaMuerto()){
				delete enemigo;
				enemigo = nullptr;
			}
			else{enemigo->actualizar();}
		}


		if (enemigo2 != nullptr){
			if (enemigo2->estaMuerto()){
				delete enemigo2;
				enemigo2 = nullptr;
			}
			else{enemigo2->actualizar();}
		}
		
		


		snowBoy->actualizar();
		disparoListener->eliminarBodies();
		
/*		if (this->snowBoy->getEstado() == EVENTO_CLIENTE_SNOWBOY_ATRAPADO_EN_BOLA_DE_NIEVE){
			//for (b2Fixture* fixtureActual = snowBoy->getBody()->GetFixtureList(); fixtureActual; fixtureActual = fixtureActual->GetNext()){
				b2Fixture* fixtureActual = snowBoy->getBody()->GetFixtureList();
				if (fixtureActual != nullptr){snowBoy->getBody()->DestroyFixture(fixtureActual);}
			//}
		}
*/


		//El end es el elemento desp del ultimo body, seria como un end of set
		//IMP el getSiguiente se hace asi: ++bodyActual, NO bodyActual++
		//for(set<b2Body*>::iterator bodyActual = bodiesDeNieveAEliminar.begin(); bodyActual != bodiesDeNieveAEliminar.end(); ++bodyActual){

		//	//Para acceder a lo que tiene el iterador hay que DESREFERENCIARLO, seria como un cursor
		//	//m_world->DestroyBody(*bodyActual);

		//	//otra forma:
		//	delete (*bodyActual)->GetUserData();
		//	(*bodyActual)->GetWorld()->DestroyBody(*bodyActual);
		//}
		////MUY IMPORTANTE
		//bodiesDeNieveAEliminar.clear();




        //show some useful info
        m_debugDraw.DrawString(5, m_textLine, "'j' para disparar");
        m_textLine += 15;
		m_debugDraw.DrawString(5, m_textLine, "'q' para que el enemigo se mueva a la izquierda (no frena)");
        m_textLine += 15;
		m_debugDraw.DrawString(5, m_textLine, "'t' para atacar al enemigo");
        m_textLine += 15;

    }

	void Keyboard(unsigned char key)
	{	
	    
		switch (key)
		{

		case 'a' : 
			snowBoy->moverIzquierda();			
			break;
		case 'd' :
			snowBoy->moverDerecha();
			break;
		case 'w' : 
			snowBoy->saltar();
			break;
		case 'j':		
			snowBoy->disparar();
			break;
		case 'q' : 
			enemigo->moverIzquierda();
			break;
		case 't' : 
			enemigo->atacar();
			break;
		}
	}


    void KeyboardUp(unsigned char key)
    {
        switch (key) {
		case 'a' : snowBoy->frenarIzquierda();break;
		case 'd' : snowBoy->frenarDerecha();break;
        default: Test::Keyboard(key);
        }
    }





    static Test* Create()
    {
        return new DisparoSnowBoy;
    }

	~DisparoSnowBoy() {
	//OBSERVACION MUY IMPORTANTE: CUANDO SE DELETEAN LAS COSAS, LOS SNOWBOYS SE DEBEN DELETEAR AL FINAL PORQUE SI SE DELETEA UN ENEMIGO
	//SE LIBERAN LOS SNOWBOYS Y PUEDE PASAR QUE AL LLAMAR A snowboyALiberar->liberar(), SI ESE SB SE DELETEO ANTES, EXPLOTA
		
		if (enemigo != nullptr){delete enemigo;}
		if (enemigo2 != nullptr){delete enemigo2;}
		delete disparoListener;
		delete snowBoy;
		delete userDataSnowBoy;
	
	}


};





#endif







