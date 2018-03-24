#include "DisparoContactListener3.h"

	
void DisparoContactListener::BeginContact(b2Contact* contact) {

	//Me fijo si alguno es nieve (la comparacion desp puede ser una funcion
	//Si es nieve lo agrego a la lista de borrado
	//borrarlo desp en el step


    //Me fijo si la fixture A es nieve
//   void* bodyUserDataA = contact->GetFixtureA()->GetBody()->GetUserData();
//	void* bodyUserDataB = contact->GetFixtureB()->GetBody()->GetUserData();

    sUserData* bodyUserDataA = (sUserData*) contact->GetFixtureA()->GetBody()->GetUserData();
	sUserData* bodyUserDataB = (sUserData*) contact->GetFixtureB()->GetBody()->GetUserData();

	//Si alguno no tiene user data que no haga nada
	if ( (bodyUserDataA != nullptr) && (bodyUserDataB != nullptr) ){

/*		TIPO_FIGURA tipoFiguraA = ((sUserData*)bodyUserDataA)->tipo;
		TIPO_FIGURA tipoFiguraB = ((sUserData*)bodyUserDataB)->tipo;

		void* objetoA = ((sUserData*)bodyUserDataA)->objeto;
		void* objetoB = ((sUserData*)bodyUserDataB)->objeto;
*/

		TIPO_FIGURA tipoFiguraA = bodyUserDataA->tipo;
		TIPO_FIGURA tipoFiguraB = bodyUserDataB->tipo;

		void* objetoA = bodyUserDataA->objeto;
		void* objetoB = bodyUserDataB->objeto;


		//Si era alguna era nieve devulve true, si no, false
		/*if (!this->analizarSiEsTipoNieve(contact, tipoFiguraA, tipoFiguraB, bodyUserDataA, bodyUserDataB)){
			//Si NO es tipo nieve, analiza otro caso
		}*/
		if (this->compararTipos(TIPO_NIEVE, tipoFiguraA, tipoFiguraB)){procesarContactoDeNieve(contact, tipoFiguraA, tipoFiguraB, objetoA, objetoB);}
		else if(this->compararTipos(TIPO_SNOWBOY, tipoFiguraA, tipoFiguraB)){procesarContactoDelSnowBoy(tipoFiguraA, tipoFiguraB, objetoA, objetoB);}
		else if(this->compararTipos(TIPO_ENEMIGO, tipoFiguraA, tipoFiguraB)){procesarContactoDelEnemigo(tipoFiguraA, tipoFiguraB, objetoA, objetoB);}
	}
}

void DisparoContactListener::EndContact(b2Contact* contact) {

}


bool DisparoContactListener::compararTipos(TIPO_FIGURA tipoDeFiguraAComparar, TIPO_FIGURA tipoFiguraA,TIPO_FIGURA tipoFiguraB){
	return (( tipoFiguraA == tipoDeFiguraAComparar ) || ( tipoFiguraB == tipoDeFiguraAComparar ));
}

void DisparoContactListener::procesarContactoDeNieve(b2Contact* contact, TIPO_FIGURA tipoFiguraA, TIPO_FIGURA tipoFiguraB, void* objetoA, void* objetoB ){
	if ( tipoFiguraA == TIPO_FIGURA::TIPO_NIEVE ){
		this->bodiesDeNieveAEliminar.insert(contact->GetFixtureA()->GetBody());

		if(this->esTipoEnemigo(tipoFiguraB)){
			//((Enemigo*)((sUserData*)bodyUserDataB)->objeto)->atacar();
			((Enemigo*)objetoB)->atacar();
		}			
	}
	else{	//Me fijo si la fixture B es nieve
		if ( tipoFiguraB == TIPO_NIEVE ){	

			this->bodiesDeNieveAEliminar.insert(contact->GetFixtureB()->GetBody());	
			if(this->esTipoEnemigo(tipoFiguraA)){
				//((Enemigo*)((sUserData*)bodyUserDataA)->objeto)->atacar();
				((Enemigo*)objetoA)->atacar();
			}				
		}
	}
}

void DisparoContactListener::procesarContactoDelSnowBoy(TIPO_FIGURA tipoFiguraA, TIPO_FIGURA tipoFiguraB, void* objetoA, void* objetoB ){
	if ( tipoFiguraA == TIPO_SNOWBOY ){		
		printf("Un Snowboy fue tocado \n");

		if ( tipoFiguraB == TIPO_ENEMIGO ){
			printf("El snowboy choco con un enemigo entonces ahora hay que analizar el estado del Enemigo \n");
			if ( ((Enemigo*)objetoB)->getEstado() == EVENTO_CLIENTE_ENEMIGO_BOLA_DE_NIEVE_PATEADA ){
			((Enemigo*)objetoB)->atraparSnowBoyEnBolaDeNieve((SnowBoy3*)objetoA);
				printf("El snowBoy es atrapado \n");

			}
		}
	}
	else if ( tipoFiguraB == TIPO_SNOWBOY ){		
		printf("Un Snowboy fue tocado \n");

		if ( tipoFiguraA == TIPO_ENEMIGO ){
			printf("El snowboy choco con un enemigo entonces ahora hay que analizar el estado del Enemigo \n");
			if ( ((Enemigo*)objetoA)->getEstado() == EVENTO_CLIENTE_ENEMIGO_BOLA_DE_NIEVE_PATEADA ){
			((Enemigo*)objetoA)->atraparSnowBoyEnBolaDeNieve((SnowBoy3*)objetoB);
				printf("El snowBoy es atrapado \n");

			}
		}
	}
}


void DisparoContactListener::procesarContactoDelEnemigo(TIPO_FIGURA tipoFiguraA, TIPO_FIGURA tipoFiguraB, void* objetoA, void* objetoB ){
		//Aca se entra SOLO si alguno es tipo Enemigo, haciendo esto veo si los dos son TIPO_ENEMIGO y me ahorro una comparacion
		if ( tipoFiguraA == tipoFiguraB ){

			Enemigo* enemigoA = (Enemigo*) objetoA;
			Enemigo* enemigoB = (Enemigo*) objetoB;
			
			if (enemigoA->getEstado() == EVENTO_CLIENTE_ENEMIGO_BOLA_DE_NIEVE_PATEADA){
				enemigoB->matarConBolaDeNievePateada();
			}
			else if (enemigoB->getEstado() == EVENTO_CLIENTE_ENEMIGO_BOLA_DE_NIEVE_PATEADA){
				enemigoA->matarConBolaDeNievePateada();
			}			
		}
}




bool DisparoContactListener::esTipoEnemigo(TIPO_FIGURA tipo){
	return (( tipo == TIPO_ENEMIGO ));// || ( tipo == TIPO_ENEMIGO_BOLA_DE_NIEVE ));
}


void DisparoContactListener::eliminarBodies(){
	//El end es el elemento desp del ultimo body, seria como un end of set
	//IMP el getSiguiente se hace asi: ++bodyActual, NO bodyActual++
	for(set<b2Body*>::iterator bodyActual = bodiesDeNieveAEliminar.begin(); bodyActual != bodiesDeNieveAEliminar.end(); ++bodyActual){

		//Para acceder a lo que tiene el iterador hay que DESREFERENCIARLO, seria como un cursor
		//otra forma:
		//Tal vez sea mejor que la userData de la Nieve se elimine haciendo delete nieve y esa sea una lista de Nieve
		//Además la Nieve debería hacer el DestroyBody
		delete (*bodyActual)->GetUserData();
		(*bodyActual)->GetWorld()->DestroyBody(*bodyActual);
	}
	//MUY IMPORTANTE
	bodiesDeNieveAEliminar.clear();

}