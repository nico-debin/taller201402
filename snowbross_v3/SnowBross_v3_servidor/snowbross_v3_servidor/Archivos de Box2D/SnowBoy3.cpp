#include "SnowBoy3.h"
#include "Enemigo3.h"


SnowBoy3::SnowBoy3(b2World* mundo,  sObjeto structObj):Personaje(mundo,structObj){
	this->enemigoBolaTocadaDer = nullptr;
	this->enemigoBolaTocadaIzq = nullptr;

	
	//Esto se puede hacer mejor con los mask y category bits
	for (b2Fixture* fixtureActual = this->body->GetFixtureList(); fixtureActual; fixtureActual = fixtureActual->GetNext()){
		b2Filter filtro = fixtureActual->GetFilterData();
		filtro.groupIndex = -1;
		fixtureActual->SetFilterData(filtro);
	}

}

	


void SnowBoy3::castearRayosHorizontales(){
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

Enemigo* SnowBoy3::analizarBolaTocada(SnowBoyRayCastCallback* callback){
	sUserData* userData = callback->userDataObjetoTocado;
	if ((userData != nullptr) && (userData->tipo == TIPO_ENEMIGO/*_BOLA_DE_NIEVE*/)){
		//Aca el rayCast sabe que está tocando un enemigo, y se le pregunta el estado para saber si es una bola
		if (((Enemigo*) userData->objeto)->getEstado() == EVENTO_CLIENTE_ENEMIGO_BOLA_DE_NIEVE){return ((Enemigo*) userData->objeto);}
		else {return nullptr;}
		
	}
	else{return nullptr;}
}


	
void SnowBoy3::disparar(){

	//Se le pasa el body para saber en qué posició crearla y también para tener el world usando body->getWorld()

	//Nieve* nieve = new Nieve(this->body, orientacionDerecha);
	/*Aca hay dos opciones, 1 es que el objeto sea estatico y se destrua en el momento, el body queda vivo
	  en el world, y despues se agrega a la lista de bodies a destruir

	  La otra es que sea un objeto dinamico, que se destructor tenga un destroyBody y que la nieve se agregue 
	  a una lista de nieve a destruir y despues se le hace delete

	  PROBLEMA:
		-Si se crea estático, entonces NUNCA se hace delete de la user data y habría que borrarla desde afuera, antes de eliminar el body. Algo que mucho no me gusta como suena
		-Si se crea dinámico se puede deletear en el destructor, pero, en vez de tener un grupo de bodies a deletear, debería haber un grupo de Nieve a deletear
			-La última opción sería tener un grupo de bodies a deletear y ahí meto todos los bodies, sólo que antes al body le pido la user data y a la user data le pido el objeto y lo deleteo

		-Otro problema: si se crea estáticamente, queda inutilizable el ptero 'this' en el userData de la Nieve
	*/

	if (this->enemigoBolaTocadaDer != nullptr && this->derPresionado){this->enemigoBolaTocadaDer->patear(true);}
	else if (this->enemigoBolaTocadaIzq != nullptr && this->izqPresionado){this->enemigoBolaTocadaIzq->patear(false);}
	else {
		Nieve nieve(this->body, orientacionDerecha);//Tiene que ser un objeto dinamico
		//Nieve::agregarNuevaNieve(&nieve);
	}

}



void SnowBoy3::atraparEnBolaDeNieve(){
	/*Habría que hacer que el setear estado de Personaje no funcione si el snowboy esta aatrapado
	  Para que se manden menos cosas podría hacerse que se mande un sólo evento del snowboy que tenga EVENTO_CLIENTE_SNOWBOY_ATRAPADO_EN_BOLA_DE_NIEVE
	  y despues hasta que no salga de ese estado que no se creen nuevos eventos para enviar.
	  Ahora lo va a hacer xq la XY del snowBoy va a cambiar y se va a tomar como un cambio de Estado)
	*/


	if (this->estado != EVENTO_CLIENTE_SNOWBOY_ATRAPADO_EN_BOLA_DE_NIEVE){
		//Podria darse el caso de que dos bolas le peguen al SB al mismo tiempo...
		this->estado = EVENTO_CLIENTE_SNOWBOY_ATRAPADO_EN_BOLA_DE_NIEVE;

		//Esto lo hago así por si despues el Snowboy tiene las placas para que pueda empujar bien
		//La destruccion de fixtures debe hacerse en el actualizar
	/*	for (b2Fixture* fixtureActual = this->body->GetFixtureList(); fixtureActual; fixtureActual = fixtureActual->GetNext()){
			this->body->DestroyFixture(fixtureActual); 
		}
	*/
	}

}

void SnowBoy3::liberarDeBolaDeNieve(b2Vec2 posicionDeLaBolaDestruida){
	this->body->SetTransform(posicionDeLaBolaDestruida,0);
	this->crearForma();
	this->estado = EVENTO_CLIENTE_PERSONAJE_QUIETO_IZQ;
	
}


void SnowBoy3::actualizar(){
	//Esto lo hago asi xq creo que el snowboy despues va  atener mas fixtures
	if (this->estado == EVENTO_CLIENTE_SNOWBOY_ATRAPADO_EN_BOLA_DE_NIEVE){
		b2Fixture* fixtureADestruir = nullptr;
		b2Fixture* fixtureActual = this->body->GetFixtureList();
		while(fixtureActual != nullptr){
			printf("%S \n", fixtureActual );
			fixtureADestruir = fixtureActual;
			fixtureActual = fixtureActual->GetNext();
			this->body->DestroyFixture(fixtureADestruir); 			
			//Si fuera con un for el problema es que al destruirla, fixtureActual queda en null y explota en el getNext()
		}		
	}
	else {Personaje::actualizar();} 
}


