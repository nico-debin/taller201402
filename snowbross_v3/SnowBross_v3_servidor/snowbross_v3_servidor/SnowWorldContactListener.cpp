#include "StdAfx.h"
#include "SnowWorldContactListener.h"
#include "Nieve.h"
#include "JuegoControlador.h"

#define PUNTOS_POR_LEVANTAR_BONUS 50

SnowWorldContactListener::SnowWorldContactListener(JuegoControlador* juegoControlador){
	this->juegoControlador = juegoControlador;
}

void SnowWorldContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold){
    sUserData* bodyUserDataA = (sUserData*) contact->GetFixtureA()->GetBody()->GetUserData();
	sUserData* bodyUserDataB = (sUserData*) contact->GetFixtureB()->GetBody()->GetUserData();

	//Si alguno no tiene user data que no haga nada
	if ( (bodyUserDataA != nullptr) && (bodyUserDataB != nullptr) ){

		TIPO_FIGURA tipoFiguraA = bodyUserDataA->tipo;
		TIPO_FIGURA tipoFiguraB = bodyUserDataB->tipo;

		void* objetoA = bodyUserDataA->objeto;
		void* objetoB = bodyUserDataB->objeto;

        if ( this->compararTipos( TIPO_FIGURA::TIPO_RECTANGULO   , tipoFiguraA, tipoFiguraB) &&
             ( esTipoEnemigo( tipoFiguraA) || 
			   esTipoEnemigo( tipoFiguraB) ||
			   this->compararTipos( TIPO_FIGURA::TIPO_SNOWBOY, tipoFiguraA, tipoFiguraB)   ) ){
				  this->procesarContactoSuperficie(contact, tipoFiguraA, tipoFiguraB, objetoA, objetoB);
		}

		if(  esTipoEnemigo( tipoFiguraA)  && esTipoEnemigo( tipoFiguraB) ){
			this->procesarContactoDelEnemigoConEnemigoPreSolve(contact,tipoFiguraA, tipoFiguraB, objetoA, objetoB);
		}
	}
}

void SnowWorldContactListener::BeginContact(b2Contact* contact) {
    sUserData* bodyUserDataA = (sUserData*) contact->GetFixtureA()->GetBody()->GetUserData();
	sUserData* bodyUserDataB = (sUserData*) contact->GetFixtureB()->GetBody()->GetUserData();

	//Si alguno no tiene user data que no haga nada
	if ( (bodyUserDataA != nullptr) && (bodyUserDataB != nullptr) ){

		TIPO_FIGURA tipoFiguraA = bodyUserDataA->tipo;
		TIPO_FIGURA tipoFiguraB = bodyUserDataB->tipo;

		void* objetoA = bodyUserDataA->objeto;
		void* objetoB = bodyUserDataB->objeto;
		

		if ( this->compararTipos(TIPO_NIEVE, tipoFiguraA, tipoFiguraB) && !this->compararTipos(TIPO_PISO, tipoFiguraA, tipoFiguraB) ){
			this->procesarContactoDeNieve(contact, tipoFiguraA, tipoFiguraB, objetoA, objetoB);
		
		} else if( this->compararTipos(TIPO_SNOWBOY, tipoFiguraA, tipoFiguraB) && 
			       (esTipoEnemigo( tipoFiguraA) || esTipoEnemigo( tipoFiguraB) || this->compararTipos(TIPO_FUEGO, tipoFiguraA, tipoFiguraB)) ){
			this->procesarContactoDelSnowBoyConEnemigo(contact,tipoFiguraA, tipoFiguraB, objetoA, objetoB);
		//IMPORTANTE: ACA TIPO_DIABLO ESTA MAL XQ NO VA  ABARCAR AL DRAGON, DESPUES CAMBIARLO
		} else if( esTipoEnemigo( tipoFiguraA) && esTipoEnemigo( tipoFiguraB) ){
			this->procesarContactoDelEnemigoConEnemigo(contact,tipoFiguraA, tipoFiguraB, objetoA, objetoB);
		} else if( (esTipoEnemigo( tipoFiguraA) || esTipoEnemigo( tipoFiguraB)) && 
			      ( this->compararTipos(TIPO_PARED_DERECHA, tipoFiguraA, tipoFiguraB) || this->compararTipos(TIPO_PARED_IZQUIERDA, tipoFiguraA, tipoFiguraB) )){
				  this->procesarContactoDelEnemigoConPared(contact,tipoFiguraA, tipoFiguraB, objetoA, objetoB);			
		}
		else if (  this->compararTipos(TIPO_SNOWBOY, tipoFiguraA, tipoFiguraB) && 
			      (this->compararTipos(TIPO_BONUS_VIDA, tipoFiguraA, tipoFiguraB) || this->compararTipos(TIPO_BONUS_VELOCIDAD, tipoFiguraA, tipoFiguraB) ) ){
					this->procesarContactoDelSnowBoyConBonus(contact,tipoFiguraA, tipoFiguraB, objetoA, objetoB);
		}

        if( tipoFiguraA == TIPO_FUEGO ){
    		this->figurasAMatar.insert((Figura*)objetoA);
		}

        if ( tipoFiguraB == TIPO_FUEGO ){
			this->figurasAMatar.insert((Figura*)objetoB);
		}

	}
}

bool SnowWorldContactListener::compararTipos(TIPO_FIGURA tipoDeFiguraAComparar, TIPO_FIGURA tipoFiguraA,TIPO_FIGURA tipoFiguraB){
	return (( tipoFiguraA == tipoDeFiguraAComparar ) || ( tipoFiguraB == tipoDeFiguraAComparar ));
}

void SnowWorldContactListener::procesarContactoDeNieve(b2Contact* contact, TIPO_FIGURA tipoFiguraA, TIPO_FIGURA tipoFiguraB, void* objetoA, void* objetoB ){
	if ( tipoFiguraA == TIPO_NIEVE ){
		this->figurasAMatar.insert((Figura*)objetoA);
		Nieve* nieveA = (Nieve*)objetoA;
		if( this->esTipoEnemigo(tipoFiguraB) && nieveA->puedeGolpear()  ){
			((Enemigo*)objetoB)->serAtacadoConNieve();
			nieveA->deshabilitar();
		}
        //Me fijo si la fixture B es nieve
	} else if ( tipoFiguraB == TIPO_NIEVE ){	

			this->figurasAMatar.insert((Figura*)objetoB);
			Nieve* nieveB = (Nieve*)objetoB;

			if(this->esTipoEnemigo(tipoFiguraA)  && nieveB->puedeGolpear()){
				((Enemigo*)objetoA)->serAtacadoConNieve();
				nieveB->deshabilitar();
			}						
    }
}

void SnowWorldContactListener::procesarContactoSuperficie(b2Contact* contact, TIPO_FIGURA tipoFiguraA, TIPO_FIGURA tipoFiguraB, void* objetoA, void* objetoB ){
    b2Vec2 positionA, positionB;
    positionA = ((Figura*)objetoA)->body->GetPosition();
    positionB = ((Figura*)objetoB)->body->GetPosition();

    if ( tipoFiguraA == TIPO_FIGURA::TIPO_RECTANGULO ){
        if ( positionA.y > positionB.y){
            contact->SetEnabled(false);
        }
    } else {
        if ( positionA.y < positionB.y){
            contact->SetEnabled(false);
        }   
    }
}


bool SnowWorldContactListener::esTipoEnemigo(TIPO_FIGURA tipo){
	return (( tipo == TIPO_DIABLO || tipo == TIPO_DRAGON ));// || ( tipo == TIPO_ENEMIGO_BOLA_DE_NIEVE ));
}


void SnowWorldContactListener::procesarContactoDelSnowBoyConEnemigo(b2Contact* contact,TIPO_FIGURA tipoFiguraA, TIPO_FIGURA tipoFiguraB, void* objetoA, void* objetoB ){
	if ( tipoFiguraA == TIPO_SNOWBOY ){		
		if ( this->esTipoEnemigo(tipoFiguraB) || tipoFiguraB == TIPO_FUEGO ){
			//Esto creo que no va  aser necesario xq despues se van a filtrar las colisiones
			if( ((Enemigo*)objetoB)->puedeMatar() || tipoFiguraB == TIPO_FUEGO ){
				this->figurasAMatar.insert((Figura*)objetoA);
			}
			else if ( ((Enemigo*)objetoB)->getEstado() == EVENTO_CLIENTE_ENEMIGO_BOLA_DE_NIEVE_PATEADA ){
				((Enemigo*)objetoB)->atraparSnowBoyEnBolaDeNieve((SnowBoy*)objetoA);
				printf("El snowBoy es atrapado \n");
				contact->SetEnabled(false);				
			}
		
		}
		else if ( tipoFiguraB == TIPO_BONUS_VIDA ){
				if( ((Bonus*)objetoB)->getEstado() ==  EVENTO_CLIENTE_BONUS_HABILITADO){
					contact->SetEnabled(false);				
				}		
		}
		else if ( tipoFiguraB == TIPO_BONUS_VELOCIDAD ){
				if( ((Bonus*)objetoB)->getEstado() ==  EVENTO_CLIENTE_BONUS_HABILITADO ){
					contact->SetEnabled(false);				
				}		
		}
	
	}
	else if ( tipoFiguraB == TIPO_SNOWBOY ){		
		if ( this->esTipoEnemigo(tipoFiguraA)  || tipoFiguraA == TIPO_FUEGO ){
			//Esto creo que no va  aser necesario xq despues se van a filtrar las colisiones
			if( ((Enemigo*)objetoA)->puedeMatar()  || tipoFiguraA == TIPO_FUEGO ){
				this->figurasAMatar.insert((Figura*)objetoB);
			}
			else if ( ((Enemigo*)objetoA)->getEstado() == EVENTO_CLIENTE_ENEMIGO_BOLA_DE_NIEVE_PATEADA ){
				((Enemigo*)objetoA)->atraparSnowBoyEnBolaDeNieve((SnowBoy*)objetoB);
				printf("El snowBoy es atrapado \n");
				contact->SetEnabled(false);
		
			}
		}
		else if ( tipoFiguraA == TIPO_BONUS_VIDA ){
				if( ((Bonus*)objetoA)->getEstado() == /*TIPO_BONUS_HABILITADO */ EVENTO_CLIENTE_BONUS_HABILITADO){
					contact->SetEnabled(false);					
				}		
		}
		else if ( tipoFiguraA == TIPO_BONUS_VELOCIDAD ){
				if( ((Bonus*)objetoA)->getEstado() == /*TIPO_BONUS_HABILITADO */ EVENTO_CLIENTE_BONUS_HABILITADO){
					contact->SetEnabled(false);				
				}		
		}
	
	}

}

void SnowWorldContactListener::procesarContactoDelEnemigoConEnemigoPreSolve(b2Contact* contact,TIPO_FIGURA tipoFiguraA, TIPO_FIGURA tipoFiguraB, void* objetoA, void* objetoB ){

			Enemigo* enemigoA = (Enemigo*) objetoA;
			Enemigo* enemigoB = (Enemigo*) objetoB;
			
            if ( ( enemigoA->getEstado() == EVENTO_CLIENTE_ENEMIGO_BOLA_DE_NIEVE_PATEADA ||
				   enemigoA->getEstado() == EVENTO_CLIENTE_SNOWBOY_ATRAPADO_EN_BOLA_DE_NIEVE   ) && 
				 ( enemigoB->getEstado() != EVENTO_CLIENTE_ENEMIGO_BOLA_DE_NIEVE ||
				   enemigoB->getEstado() != EVENTO_CLIENTE_SNOWBOY_ATRAPADO_EN_BOLA_DE_NIEVE   )    ){					
			//	enemigoB->matarConBolaDeNievePateada();
 				this->figurasAMatar.insert((Figura*)enemigoB);
				contact->SetEnabled(false);
            // Si una bola pateada choca a una sin patear, esta otra se patea
			} else if ( enemigoA->getEstado() == EVENTO_CLIENTE_ENEMIGO_BOLA_DE_NIEVE_PATEADA && 
				        enemigoB->getEstado() == EVENTO_CLIENTE_ENEMIGO_BOLA_DE_NIEVE   				   ){
                enemigoB->patear(true);
            } else if ( ( enemigoB->getEstado() == EVENTO_CLIENTE_ENEMIGO_BOLA_DE_NIEVE_PATEADA ||
				          enemigoB->getEstado() == EVENTO_CLIENTE_SNOWBOY_ATRAPADO_EN_BOLA_DE_NIEVE ) && 
				        ( enemigoA->getEstado() != EVENTO_CLIENTE_ENEMIGO_BOLA_DE_NIEVE ||
						  enemigoA->getEstado() != EVENTO_CLIENTE_SNOWBOY_ATRAPADO_EN_BOLA_DE_NIEVE )    ){
   				//enemigoA->matarConBolaDeNievePateada();
				this->figurasAMatar.insert((Figura*)enemigoA);
				contact->SetEnabled(false);
             // Si una bola pateada choca a una sin patear, esta otra se patea
            } else if ( enemigoB->getEstado() == EVENTO_CLIENTE_ENEMIGO_BOLA_DE_NIEVE_PATEADA &&  
				enemigoA->getEstado() == EVENTO_CLIENTE_ENEMIGO_BOLA_DE_NIEVE            ){
                enemigoA->patear(true);
			} /*else if (  enemigoA->getEstado() == EVENTO_CLIENTE_PERSONAJE_CAMINANDO_DER && ( enemigoB->getEstado() == EVENTO_CLIENTE_ENEMIGO_CON_NIEVE_1 || enemigoB->getEstado() == EVENTO_CLIENTE_ENEMIGO_CON_NIEVE_2 )){
				((Enemigo*)objetoA)->frenarDerecha();
				((Enemigo*)objetoA)->moverIzquierda();
			} else if (  enemigoA->getEstado() == EVENTO_CLIENTE_PERSONAJE_CAMINANDO_IZQ && ( enemigoB->getEstado() == EVENTO_CLIENTE_ENEMIGO_CON_NIEVE_1 || enemigoB->getEstado() == EVENTO_CLIENTE_ENEMIGO_CON_NIEVE_2 )){
				((Enemigo*)objetoA)->frenarIzquierda();			
				((Enemigo*)objetoA)->moverDerecha();
			} else if (  enemigoB->getEstado() == EVENTO_CLIENTE_PERSONAJE_CAMINANDO_DER && ( enemigoA->getEstado() == EVENTO_CLIENTE_ENEMIGO_CON_NIEVE_1 || enemigoA->getEstado() == EVENTO_CLIENTE_ENEMIGO_CON_NIEVE_2 )){
				((Enemigo*)objetoB)->frenarDerecha();
				((Enemigo*)objetoB)->moverIzquierda();
			} else if (  enemigoB->getEstado() == EVENTO_CLIENTE_PERSONAJE_CAMINANDO_IZQ && ( enemigoA->getEstado() == EVENTO_CLIENTE_ENEMIGO_CON_NIEVE_1 || enemigoA->getEstado() == EVENTO_CLIENTE_ENEMIGO_CON_NIEVE_2 )){
				((Enemigo*)objetoB)->frenarIzquierda();			
				((Enemigo*)objetoB)->moverDerecha();
			}*/
}

void SnowWorldContactListener::procesarContactoDelEnemigoConEnemigo(b2Contact* contact,TIPO_FIGURA tipoFiguraA, TIPO_FIGURA tipoFiguraB, void* objetoA, void* objetoB ){

			Enemigo* enemigoA = (Enemigo*) objetoA;
			Enemigo* enemigoB = (Enemigo*) objetoB;
			
            if ( ( enemigoA->getEstado() == EVENTO_CLIENTE_ENEMIGO_BOLA_DE_NIEVE_PATEADA ||
				   enemigoA->getEstado() == EVENTO_CLIENTE_SNOWBOY_ATRAPADO_EN_BOLA_DE_NIEVE   ) && 
				 ( enemigoB->getEstado() != EVENTO_CLIENTE_ENEMIGO_BOLA_DE_NIEVE ||
				   enemigoB->getEstado() != EVENTO_CLIENTE_SNOWBOY_ATRAPADO_EN_BOLA_DE_NIEVE   )    ){					
			//	enemigoB->matarConBolaDeNievePateada();
 				this->figurasAMatar.insert((Figura*)enemigoB);
				contact->SetEnabled(false);
            // Si una bola pateada choca a una sin patear, esta otra se patea
			} else if ( enemigoA->getEstado() == EVENTO_CLIENTE_ENEMIGO_BOLA_DE_NIEVE_PATEADA && 
				        enemigoB->getEstado() == EVENTO_CLIENTE_ENEMIGO_BOLA_DE_NIEVE   				   ){
                enemigoB->patear(true);
            } else if ( ( enemigoB->getEstado() == EVENTO_CLIENTE_ENEMIGO_BOLA_DE_NIEVE_PATEADA ||
				          enemigoB->getEstado() == EVENTO_CLIENTE_SNOWBOY_ATRAPADO_EN_BOLA_DE_NIEVE ) && 
				        ( enemigoA->getEstado() != EVENTO_CLIENTE_ENEMIGO_BOLA_DE_NIEVE ||
						  enemigoA->getEstado() != EVENTO_CLIENTE_SNOWBOY_ATRAPADO_EN_BOLA_DE_NIEVE )    ){
   				//enemigoA->matarConBolaDeNievePateada();
				this->figurasAMatar.insert((Figura*)enemigoA);
				contact->SetEnabled(false);
             // Si una bola pateada choca a una sin patear, esta otra se patea
            } else if ( enemigoB->getEstado() == EVENTO_CLIENTE_ENEMIGO_BOLA_DE_NIEVE_PATEADA &&  
				enemigoA->getEstado() == EVENTO_CLIENTE_ENEMIGO_BOLA_DE_NIEVE            ){
                enemigoA->patear(true);
			} else if (  enemigoA->getEstado() == EVENTO_CLIENTE_PERSONAJE_CAMINANDO_DER && ( enemigoB->getEstado() == EVENTO_CLIENTE_ENEMIGO_CON_NIEVE_1 || enemigoB->getEstado() == EVENTO_CLIENTE_ENEMIGO_CON_NIEVE_2 )){
				((Enemigo*)objetoA)->frenarDerecha();
				((Enemigo*)objetoA)->moverIzquierda();
			} else if (  enemigoA->getEstado() == EVENTO_CLIENTE_PERSONAJE_CAMINANDO_IZQ && ( enemigoB->getEstado() == EVENTO_CLIENTE_ENEMIGO_CON_NIEVE_1 || enemigoB->getEstado() == EVENTO_CLIENTE_ENEMIGO_CON_NIEVE_2 )){
				((Enemigo*)objetoA)->frenarIzquierda();			
				((Enemigo*)objetoA)->moverDerecha();
			} else if (  enemigoB->getEstado() == EVENTO_CLIENTE_PERSONAJE_CAMINANDO_DER && ( enemigoA->getEstado() == EVENTO_CLIENTE_ENEMIGO_CON_NIEVE_1 || enemigoA->getEstado() == EVENTO_CLIENTE_ENEMIGO_CON_NIEVE_2 )){
				((Enemigo*)objetoB)->frenarDerecha();
				((Enemigo*)objetoB)->moverIzquierda();
			} else if (  enemigoB->getEstado() == EVENTO_CLIENTE_PERSONAJE_CAMINANDO_IZQ && ( enemigoA->getEstado() == EVENTO_CLIENTE_ENEMIGO_CON_NIEVE_1 || enemigoA->getEstado() == EVENTO_CLIENTE_ENEMIGO_CON_NIEVE_2 )){
				((Enemigo*)objetoB)->frenarIzquierda();			
				((Enemigo*)objetoB)->moverDerecha();
			}
}

void SnowWorldContactListener::procesarContactoDelEnemigoConPared(b2Contact* contact,TIPO_FIGURA tipoFiguraA, TIPO_FIGURA tipoFiguraB, void* objetoA, void* objetoB ){
	if ( tipoFiguraA == TIPO_PARED_DERECHA ){		
			if ( ((Enemigo*)objetoB)->getEstado() != EVENTO_CLIENTE_ENEMIGO_BOLA_DE_NIEVE_PATEADA ){
				((Enemigo*)objetoB)->frenarDerecha();
				((Enemigo*)objetoB)->moverIzquierda();
			}
	} else if (  tipoFiguraA  == TIPO_PARED_IZQUIERDA){		
			if ( ((Enemigo*)objetoB)->getEstado() != EVENTO_CLIENTE_ENEMIGO_BOLA_DE_NIEVE_PATEADA ){
				((Enemigo*)objetoB)->frenarIzquierda();
				((Enemigo*)objetoB)->moverDerecha();
			}
	} else if(  tipoFiguraB == TIPO_PARED_DERECHA ){
			if ( ((Enemigo*)objetoA)->getEstado() != EVENTO_CLIENTE_ENEMIGO_BOLA_DE_NIEVE_PATEADA ){
				((Enemigo*)objetoA)->frenarDerecha();			
				((Enemigo*)objetoA)->moverIzquierda();
			}
	} else {
			if ( ((Enemigo*)objetoA)->getEstado() != EVENTO_CLIENTE_ENEMIGO_BOLA_DE_NIEVE_PATEADA ){
				((Enemigo*)objetoA)->frenarIzquierda();			
				((Enemigo*)objetoA)->moverDerecha();
			}
	}
}


void SnowWorldContactListener::procesarContactoDelSnowBoyConBonus(b2Contact* contact,TIPO_FIGURA tipoFiguraA, TIPO_FIGURA tipoFiguraB, void* objetoA, void* objetoB ){
	if ( tipoFiguraA == TIPO_SNOWBOY ){

		if ( ((Bonus*)objetoB)->estaHabilitado() ){
			if ( tipoFiguraB == TIPO_BONUS_VIDA ){
				((SnowBoy*)objetoA)->incrementarVida();
				((Bonus*)objetoB)->deshabilitar();
				contact->SetEnabled(false);	
			}
			else if ( tipoFiguraB == TIPO_BONUS_VELOCIDAD ){
				((SnowBoy*)objetoA)->incrementarVelocidad();
				((Bonus*)objetoB)->deshabilitar();
				contact->SetEnabled(false);
			}
			this->juegoControlador->estadisticasJugadores->agregarPuntos( ((SnowBoy*)objetoA)->getId(), PUNTOS_POR_LEVANTAR_BONUS );
			this->juegoControlador->encolarEventosEstadisticas();
			this->figurasAMatar.insert((Figura*)objetoB);
		}
	}
	else if ( tipoFiguraB == TIPO_SNOWBOY ){

		if ( ((Bonus*)objetoA)->estaHabilitado() ){

			if ( tipoFiguraA == TIPO_BONUS_VIDA ){
				((SnowBoy*)objetoB)->incrementarVida();
				((Bonus*)objetoA)->deshabilitar();
				contact->SetEnabled(false);	
			}
			else if ( tipoFiguraA == TIPO_BONUS_VELOCIDAD ){
				((SnowBoy*)objetoB)->incrementarVelocidad();
				((Bonus*)objetoA)->deshabilitar();
				contact->SetEnabled(false);				
			}
			this->juegoControlador->estadisticasJugadores->agregarPuntos( ((SnowBoy*)objetoB)->getId(), PUNTOS_POR_LEVANTAR_BONUS );
			this->juegoControlador->encolarEventosEstadisticas();
			this->figurasAMatar.insert((Figura*)objetoA);
		}
	}
}










void SnowWorldContactListener::matarFiguras(){

	for(set<Figura*>::iterator figuraActual = this->figurasAMatar.begin(); figuraActual != this->figurasAMatar.end(); ++figuraActual){
		
/*		if(  (*figuraActual)->getDatos().tipo == TIPO_DIABLO ||  (*figuraActual)->getDatos().tipo == TIPO_DRAGON ){
			//creo el bono aca porque necesito las posicion de la figura a morir
			figuraBono = FabricaDeFiguras::crearBono((Escenario::getFisica())->getMundo(),(*figuraActual)->getDatos().x, (*figuraActual)->getDatos().y);
		}
*/
		(*figuraActual)->matar();

		if( (*figuraActual)->getDatos().tipo == TIPO_SNOWBOY ){
			this->juegoControlador->encolarEventosEstadisticas();
		}
	}
	//MUY IMPORTANTE
	this->figurasAMatar.clear();
}











void SnowWorldContactListener::EndContact(b2Contact* contact) {

	sUserData* bodyUserDataA = (sUserData*) contact->GetFixtureA()->GetBody()->GetUserData();
	sUserData* bodyUserDataB = (sUserData*) contact->GetFixtureB()->GetBody()->GetUserData();

	if ( (bodyUserDataA != nullptr) && (bodyUserDataB != nullptr) ){

		TIPO_FIGURA tipoFiguraA = bodyUserDataA->tipo;
		TIPO_FIGURA tipoFiguraB = bodyUserDataB->tipo;

		/*	LOS OBJETOS NO SON NECESARIOS, SÓLO LOS BODIES
			void* objetoA = bodyUserDataA->objeto;
			void* objetoB = bodyUserDataB->objeto;
		*/

		b2Body* bodyA = contact->GetFixtureA()->GetBody();
		b2Body* bodyB = contact->GetFixtureB()->GetBody();

		if (this->compararTipos(TIPO_PISO, tipoFiguraA, tipoFiguraB)){

			b2Vec2 posicionA = bodyA->GetPosition();
			b2Vec2 posicionB = bodyB->GetPosition();

			if ( tipoFiguraA == TIPO_PISO ){		
				//A es el piso, entonces me fijo si, al terminar el contacto, el bodyB esta por debajo del piso
				//TAL VEZ TAMBIEN ME TENGA QUE FIJAR SI LA VELOCIDAD DEL CUERPO ES MENOR A CERO, XQ NO ABARCARÍA EL CASO EN QUE UN CUERPO ESTÉ SUBIENDO MIENTRAS ROTA ( ES UN CASO POSIBLE TEÓRICAMENTE, PERO EN EL JUEGO VA A SER MUY RARO QUE PASE)
				if ( posicionA.y > posicionB.y ){					
					this->bodiesATeletransportar.insert(bodyB);
				}
			}
			else if ( tipoFiguraB == TIPO_PISO ){		
				//ACA SE COMPARA SI EL PISO QUEDO ARRIBA
				if ( posicionB.y > posicionA.y ){
					this->bodiesATeletransportar.insert(bodyA);
				}
			}
		
		}
	}
}




void SnowWorldContactListener::teletransportarBodies(){

	for(set<b2Body*>::iterator bodyActual = this->bodiesATeletransportar.begin(); bodyActual != this->bodiesATeletransportar.end(); ++bodyActual){

		b2Vec2 posicion = (*bodyActual)->GetPosition();

		b2Vec2 posicionTeletransportada;
		posicionTeletransportada.x = posicion.x;
		posicionTeletransportada.y = Utilities::ALTO_UL+2;
		(*bodyActual)->SetTransform(posicionTeletransportada,0);
		
	}
	//MUY IMPORTANTE
	this->bodiesATeletransportar.clear();
}
