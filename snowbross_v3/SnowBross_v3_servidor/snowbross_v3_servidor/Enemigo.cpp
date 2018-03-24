#include "Enemigo.h"
#include "SnowBoy.h"
#include "Dragon.h"
#include "Escenario.h"
#include "BonusVelocidad.h"
#include "BonusVida.h"

Enemigo::Enemigo (b2World* mundo,  sObjeto structObj): Personaje(mundo,structObj){
	//this->userData.tipo = TIPO_ENEMIGO;
	//this->userData.objeto = this;


	
	this->nieveAcumulada = 0;
	this->contadorSteps = 0;
	this->stepsNecesariosParaPerderNieve = 200;
	this->limiteDeNieve1 = 2;
	this->limiteDeNieve2 = 4;
	this->maximaNieveAcumulada = 11;
	this->nieveNecesariaParaFormarBola = 5;
	this->esBola = false;
	
	this->stepsNecesariosParaMorirEnBolaDeNieve = 600;

	this->tiempoAgonia = 200;

	srand(time(NULL));
	this->timerSalto.start();

	if( this->datos.id%2 == 0 ){ 
		this->moverIzquierda();
	} else {
		this->moverDerecha();
	}

		//Esto se puede hacer mejor con los mask y category bits
	//for (b2Fixture* fixtureActual = this->body->GetFixtureList(); fixtureActual; fixtureActual = fixtureActual->GetNext()){
	//	b2Filter filtro = fixtureActual->GetFilterData();
	//	//filtro.groupIndex = -2;
	//	
	//	filtro.categoryBits = BITS_ENEMIGO;
	//	filtro.maskBits = BITS_ESCENARIO|BITS_SNOWBOY | BITS_NIEVE | BITS_ENEMIGO_CON_NIEVE | BITS_BOLA_DE_NIEVE_PATEADA;

	//	fixtureActual->SetFilterData(filtro);
	//}

	this->cargarBitsDeColisionDeEnemigoNormal();

}

void Enemigo::serAtacadoConNieve(){
	if (this->nieveAcumulada < this->maximaNieveAcumulada){
		this->nieveAcumulada++;
		//printf("Nieve acumulada: %i  \n", nieveAcumulada);
	}
	this->contadorSteps = 0; //Se resetea el contador cuando lo atacan aunque se llegue a la nieve MAX
}

void Enemigo::actualizar(){
	if (!this->esBola){Personaje::actualizar();}

	//ESTO ES PARA QUE LA BOLA DE NIEVE NO CAIGA LENTAMENTE
	if (this->estado == EVENTO_CLIENTE_ENEMIGO_BOLA_DE_NIEVE ){
		b2Vec2 vel = this->body->GetLinearVelocity();
		if (vel.y < 0){ this->body->SetLinearDamping(0); }
		else { this->body->SetLinearDamping(15);}
	}
	
	
	if (this->estado == EVENTO_CLIENTE_ENEMIGO_BOLA_DE_NIEVE_PATEADA){
		this->stepsNecesariosParaMorirEnBolaDeNieve --;
		if ( this->stepsNecesariosParaMorirEnBolaDeNieve == 0 ){
			this->matar();
		}
	}
	else{

		if (this->nieveAcumulada > 0){
			//Seteo de estado de Nieve:
			if ((this->estado != EVENTO_CLIENTE_ENEMIGO_CON_NIEVE_1) && ( 1 <= this->nieveAcumulada && this->nieveAcumulada <= limiteDeNieve1) ){ 
				this->estado = EVENTO_CLIENTE_ENEMIGO_CON_NIEVE_1;
				this->cargarBitsDeColisionDeEnemigoConNieve();
			}
			if ((this->estado != EVENTO_CLIENTE_ENEMIGO_CON_NIEVE_2) && ( (limiteDeNieve1 +1) <= this->nieveAcumulada && this->nieveAcumulada <= limiteDeNieve2) ){ 
				this->estado = EVENTO_CLIENTE_ENEMIGO_CON_NIEVE_2;
				this->cargarBitsDeColisionDeEnemigoConNieve();
			}

			//FRENAR
			if ( this->estado == EVENTO_CLIENTE_ENEMIGO_CON_NIEVE_1 || this->estado == EVENTO_CLIENTE_ENEMIGO_CON_NIEVE_2 ){
				b2Vec2 vel = this->body->GetLinearVelocity();
				vel.x=0;
				this->body->SetLinearVelocity(vel);
			}
			

			//CONTADOR DE PERDIDA DE NIEVE
			this->contadorSteps++;
			if(this->contadorSteps == stepsNecesariosParaPerderNieve){
				this->nieveAcumulada--;
				this->contadorSteps = 0;
			}

			//VER SI SE TRANSFORMA EN BOLA
			//ESTO DEBE HACERSE EN EL ACTUALIZAR, XQ SI SE BORRA LA FIXTURE EN EL ATACAR, ENTONCES PUEDE PINCHAR EN EL CONTACTLISTENER
			if (this->nieveAcumulada >= this->nieveNecesariaParaFormarBola && !this->esBola){
				//Si entra aca es xq tiene suficiente nieve y no es una bola
				this->transformarEnBolaDeNieve();
			}
			if(this->nieveAcumulada < this->nieveNecesariaParaFormarBola && this->esBola){
				this->volverALaFormaNormal();
			}
			
		}
		else {//Aca entra si nieveAcumulada == 0
			if( this->puedeSaltar() ){ 
				if( this->timerSalto.getTicks() > 5000 ){
					int random = rand() % 100;
					if( random >=75 && random <= 79){
						this->timerSalto.stop();
						this->timerSalto.start();
						this->saltar();
					}
				}
			}

			if ( this->estado == EVENTO_CLIENTE_ENEMIGO_CON_NIEVE_1 ){
				if (this->orientacionDerecha){
					this->estado = EVENTO_CLIENTE_PERSONAJE_QUIETO_DER;
					this->cargarBitsDeColisionDeEnemigoNormal();
				}
				else {
					this->estado = EVENTO_CLIENTE_PERSONAJE_QUIETO_IZQ;
					this->cargarBitsDeColisionDeEnemigoNormal();
				}
			} else if ( this->datos.tipo == TIPO_DRAGON ){
				((Dragon*)this)->analizarLanzarFuego();
			}
		}
	}
}


bool Enemigo::puedeMatar(){
	//Esto creo que no va  aser necesario xq despues se van a filtrar las colisiones
	return ( //(this->estado != EVENTO_CLIENTE_ENEMIGO_CON_NIEVE_1) &&
	 // (this->estado != EVENTO_CLIENTE_ENEMIGO_CON_NIEVE_2) &&
	  (this->estado != EVENTO_CLIENTE_ENEMIGO_BOLA_DE_NIEVE) &&
	  (this->estado != EVENTO_CLIENTE_ENEMIGO_BOLA_DE_NIEVE_PATEADA) &&
	  (this->estado != EVENTO_CLIENTE_FIGURA_MURIENDO) &&
	  (this->estado != EVENTO_CLIENTE_FIGURA_MUERTO) 
	);
}



void Enemigo::transformarEnBolaDeNieve(){

	this->esBola = true;

	b2Fixture* fixtureCuerpo = this->body->GetFixtureList();
	this->body->DestroyFixture(fixtureCuerpo);

	b2CircleShape bolaDeNieve;
	//Poner como cte
	bolaDeNieve.m_radius=SNOWBOY_ALTO_UL/2;

	//Este centra la bola de nieve en el centro del Enemigo, y=-0.5 en relacion al body
	bolaDeNieve.m_p=b2Vec2(0,-SNOWBOY_DIFERENCIA_CENTRO_CUERPO_CENTRO_FIGURA);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &bolaDeNieve;
	fixtureDef.density = 0.5625;//0.25;
	fixtureDef.friction = 1;
      
	this->body->CreateFixture(&fixtureDef);

	this->body->SetGravityScale(1);
	this->body->SetAngularDamping(5);//Cte para establecer la desaceleracion angular
	this->body->SetLinearDamping(15);//Cte para establecer la desaceleracion lineal

	//Esto deberia sacarse y cambiar a estados
	this->estado = EVENTO_CLIENTE_ENEMIGO_BOLA_DE_NIEVE;

	this->body->SetFixedRotation(false);

	//IMPORTANTE, SI NO SE VE MAL EL SPRITE, Y TIENE QUE VOLVER AL ANCHO ANTERIOR CUANDO VUELVE  A LA FORMA NORMAL
	//ADEMAS EN EL SET ESTADO DE ENEMIGO EN LA VISTA SE DEBE ACTUALIZAR EL ANCHO
	this->datos.ancho = SNOWBOY_ALTO_UL;

	this->cargarBitsDeColisionDeEnemigoBolaDeNieveQuieta();
}


void Enemigo::volverALaFormaNormal(){
	this->esBola = false;
	//Borro la fixture de la bola de nieve y creo la misma que antes de ser bola
	b2Fixture* fixtureCuerpo = this->body->GetFixtureList();
				
	this->body->DestroyFixture(fixtureCuerpo);
	this->crearForma();

	this->body->SetTransform(this->body->GetPosition(),0);
	this->body->SetAngularVelocity(0);
	this->body->SetFixedRotation(true);
	this->body->SetLinearDamping(0);

	//IMPORTANTE
	this->datos.ancho = SNOWBOY_ANCHO_UL;

	//No se si sea necesario xq en el proximo estep se carga lo mismo arriba
	this->cargarBitsDeColisionDeEnemigoConNieve();
}


void Enemigo::patear(bool patearALaDerecha){	
	this->body->SetLinearDamping(0);
	//Va a ser cte
	float velocidadPateada = 100;

	this->body->GetFixtureList()->SetRestitution(1);
	if (patearALaDerecha){this->body->SetLinearVelocity( b2Vec2(velocidadPateada,0));}
	else{this->body->SetLinearVelocity( b2Vec2(-velocidadPateada,0));}

	//NUEVO
	this->estado = EVENTO_CLIENTE_ENEMIGO_BOLA_DE_NIEVE_PATEADA;

	this->cargarBitsDeColisionDeEnemigoBolaDeNievePateada();
}

void Enemigo::crearBonus(){

	sObjeto structBonus;
	
	structBonus.x = this->body->GetPosition().x;
	structBonus.y = this->body->GetPosition().y;
	
	////Esto es necesario para saber si la velocidad del proyectil debe ser positiva o negativa (DER o IZQ)
	////Despues adentro del constructor del proyectil se le cambia el TIPO, x ej en Nieve se cambia a TIPO_NIEVE
	//if (this->orientacionDerecha){structDelProyectil.tipo = TIPO_PROYECTIL_DER;} 
	//else {structDelProyectil.tipo = TIPO_PROYECTIL_IZQ;}

	//Figura* proyectil = this->crearProyectil(structDelProyectil);
	//Escenario::agregarFigura(proyectil);//El ID lo debe cargar el constructor del proyectil
}


void Enemigo::atraparSnowBoyEnBolaDeNieve(SnowBoy* snowBoyAtrapado){
	snowBoyAtrapado->atraparEnBolaDeNieve(this);
	this->snowBoysAtrapados.insert(snowBoyAtrapado);
}

Enemigo::~Enemigo(){
	this->liberarSnowBoysAtrapados();		
	this->crearBono();
}

void Enemigo::crearBono(){	
		
	b2World* mundo = this->body->GetWorld();

	Figura* figura = nullptr;
	sObjeto structObj = inicializoFigura();
	structObj.id = Utilities::getNewID();
	//funcion random para crear los dif bonos va de 20 a 23
	// pero solo crea cuando es 22 y 23 por el "ocasionalmente cuando muera enemigo"
	//int tipoBono = rand() % 3 + 21;

	//Va de 1 a 100
	int tipoBono = rand() % 100 + 1;
	//if (tipoBono == TIPO_BONUS_VELOCIDAD)
	if (tipoBono <=50)//Para que siempre se cree un bono o el otro y poder probarlo
		figura = new BonusVelocidad(mundo, structObj);	
	//else if (tipoBono == TIPO_BONUS_VIDA)
	else if (tipoBono >50)
		figura = new BonusVida(mundo, structObj);		
	
	Escenario::agregarFigura(figura);
}

sObjeto Enemigo::inicializoFigura(){
	sObjeto structObj;
	// x y iniciales luego reemplazar con las (x,y) del enemigo
	structObj.x = this->getX();
	structObj.y = this->getY();
	structObj.ancho=3;
	structObj.alto=3;
	structObj.color = "#00FF00";
	structObj.rot =  0;
	structObj.masa = 0;
	structObj.estatico = true;
	
	return structObj;
}






void Enemigo::liberarSnowBoysAtrapados(){
	for (set<SnowBoy*>::iterator snowBoyALiberar = this->snowBoysAtrapados.begin(); snowBoyALiberar != this->snowBoysAtrapados.end(); ++snowBoyALiberar){
		(*snowBoyALiberar)->liberarDeBolaDeNieve(this->body->GetPosition());
	}
	this->snowBoysAtrapados.clear();
}

void Enemigo::liberarSnowBoy(SnowBoy* snowBoyAtrapado){	
	snowBoyAtrapado->liberarDeBolaDeNieve(this->body->GetPosition());
	this->snowBoysAtrapados.erase(snowBoyAtrapado);
}


/*
void Enemigo::cargarBitsDeColisionEnFixtures(uint16 categoryBits, uint16 maskBits){
	for (b2Fixture* fixtureActual = this->body->GetFixtureList(); fixtureActual; fixtureActual = fixtureActual->GetNext()){
		b2Filter filtro = fixtureActual->GetFilterData();
		//filtro.groupIndex = -2;
		
		filtro.categoryBits = categoryBits;
		filtro.maskBits = maskBits;

		fixtureActual->SetFilterData(filtro);
	}
}
*/

void Enemigo::cargarBitsDeColisionDeEnemigoNormal(){
	this->cargarBitsDeColisionEnFixtures(BITS_ENEMIGO, BITS_ESCENARIO|BITS_SNOWBOY | BITS_NIEVE | BITS_ENEMIGO_CON_NIEVE | BITS_BOLA_DE_NIEVE_PATEADA);
}
void Enemigo::cargarBitsDeColisionDeEnemigoConNieve(){
	this->cargarBitsDeColisionEnFixtures(BITS_ENEMIGO_CON_NIEVE, BITS_ESCENARIO | BITS_FUEGO |BITS_NIEVE | BITS_ENEMIGO | BITS_BOLA_DE_NIEVE_PATEADA);
}
void Enemigo::cargarBitsDeColisionDeEnemigoBolaDeNieveQuieta(){
	this->cargarBitsDeColisionEnFixtures(BITS_BOLA_DE_NIEVE_QUIETA, BITS_ESCENARIO | BITS_SNOWBOY | BITS_FUEGO | BITS_NIEVE | BITS_BOLA_DE_NIEVE_QUIETA | BITS_BOLA_DE_NIEVE_PATEADA);

}
void Enemigo::cargarBitsDeColisionDeEnemigoBolaDeNievePateada(){
	this->cargarBitsDeColisionEnFixtures(BITS_BOLA_DE_NIEVE_PATEADA, BITS_ESCENARIO | BITS_SNOWBOY | BITS_FUEGO | BITS_NIEVE | BITS_ENEMIGO | BITS_ENEMIGO_CON_NIEVE | BITS_BOLA_DE_NIEVE_QUIETA);
}


void Enemigo::setearEstado(float distanciaAlPiso){
	if (this->estado != EVENTO_CLIENTE_ENEMIGO_CON_NIEVE_1 && this->estado != EVENTO_CLIENTE_ENEMIGO_CON_NIEVE_2 ){
		Personaje::setearEstado(distanciaAlPiso);
    }
}