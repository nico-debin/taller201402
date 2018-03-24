#include "Enemigo3.h"
#include "SnowBoy3.h"

Enemigo::Enemigo(b2World* mundo,  sObjeto structObj):Personaje(mundo,structObj){
		this->userData.tipo = TIPO_ENEMIGO;
		this->userData.objeto = this;

		//Esta bien hacerlo asi?
		this->body->SetUserData((void*)&userData);

		this->nieveAcumulada = 0;
		this->contadorSteps = 0;
		this->stepsNecesariosParaPerderNieve = 60;
		this->maximaNieveAcumulada = 20;
		this->nieveNecesariaParaFormarBola = 5;//15;
		this->esBola = false;

		this->stepsNecesariosParaMorirEnBolaDeNieve = 600;
		
			
		/*b2PolygonShape sensorDePiernas;
		b2Vec2 vertices[4];
		vertices[0]=b2Vec2(RADIO_CUERPO,0);
		vertices[1]=b2Vec2(-RADIO_CUERPO,0);
		vertices[2]=b2Vec2(-RADIO_CUERPO,-SNOWBOY_FLOTACION_DESEADA);
		vertices[3]=b2Vec2(RADIO_CUERPO,-SNOWBOY_FLOTACION_DESEADA);
		sensorDePiernas.Set(vertices,4);
		//cuerpo.SetAsBox(RADIO_CUERPO,RADIO_CUERPO);

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &sensorDePiernas;
		fixtureDef.density = 1;
		fixtureDef.friction = 0;
		fixtureDef.restitution = 0;
		fixtureDef.isSensor = true;
      
		this->body->CreateFixture(&fixtureDef);*/

		/*-Posibles soluciones para las hitbox de las piernas
			1)Que los RayCast ignoren a los sensores
			2)Que las piernas tengan sensores a la izquierda y derecha (dos rectangulos separados)
		*/

	/*	b2PolygonShape sensorDePiernaDer;
		b2Vec2 vertices[4];
		vertices[0]=b2Vec2(RADIO_CUERPO,0);
		vertices[1]=b2Vec2(-RADIO_CUERPO,0);
		vertices[2]=b2Vec2(-RADIO_CUERPO,-SNOWBOY_FLOTACION_DESEADA);
		vertices[3]=b2Vec2(RADIO_CUERPO,-SNOWBOY_FLOTACION_DESEADA);
		sensorDePiernaDer.Set(vertices,4);

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &sensorDePiernaDer;
		fixtureDef.density = 1;
		fixtureDef.friction = 0;
		fixtureDef.restitution = 0;
		fixtureDef.isSensor = true;
      
		this->body->CreateFixture(&fixtureDef);
	*/	



}

void Enemigo::atacar(){
	if (this->nieveAcumulada < this->maximaNieveAcumulada){
		this->nieveAcumulada++;
		printf("Nieve acumulada: %i  \n", nieveAcumulada);
	}
	this->contadorSteps = 0; //Se resetea el contador cuando lo atacan aunque se llegue a la nieve MAX
}

void Enemigo::actualizar(){
	if (!this->esBola){Personaje::actualizar();}
	
	if (this->estado == EVENTO_CLIENTE_ENEMIGO_BOLA_DE_NIEVE_PATEADA){
		this->stepsNecesariosParaMorirEnBolaDeNieve --;
		if ( this->stepsNecesariosParaMorirEnBolaDeNieve == 0 ){
			this->estado = EVENTO_CLIENTE_ENEMIGO_MUERTO_POR_BOLA_DE_NIEVE_DESTRUIDA;
			//Además habría que hacer algo para que se destruya la bola
			printf("Enemigo muerto \n\n\n\n");
			//Ahora aca se puede hacer que el estado sea muerto, despues antes de actualizar se le pregunta el estado al Enemigo, si esta muerto se lo deletea y en el destructor se libera a los snowboys
		}

	}
	else{
		if (this->nieveAcumulada > 0){				
			//FRENAR
			/*
			b2Vec2 vel = this->body->GetLinearVelocity();
			vel.x=0;
			this->body->SetLinearVelocity(vel);
			*/

			//CONTADOR DE PERDIDA DE NIEVE
			this->contadorSteps++;
			if(this->contadorSteps == stepsNecesariosParaPerderNieve){
				this->nieveAcumulada--;
				this->contadorSteps = 0;
				printf("Nieve acumulada: %i  \n", nieveAcumulada);
			}

			//VER SI SE TRANSFORMA EN BOLA
			//ESTO DEBE HACERSE EN EL ACTUALIZAR, XQ SI SE BORRA LA FIXTURE EN EL ATACAR, ENTONCES PUEDE PINCHAR EN EL CONTACTLISTENER
			if (this->nieveAcumulada == this->nieveNecesariaParaFormarBola && !this->esBola){
				//Si entra aca es xq tiene suficiente nieve y no es una bola
				this->transformarEnBolaDeNieve();

			}
			if(this->nieveAcumulada < this->nieveNecesariaParaFormarBola && this->esBola){
				this->volverAlEstadoNormal();
			}
			
		}
	}
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
	fixtureDef.density = 0.25;
	fixtureDef.friction = 1;
      
	this->body->CreateFixture(&fixtureDef);

	this->body->SetGravityScale(1);
	this->body->SetAngularDamping(5);//Cte para establecer la desaceleracion angular
	this->body->SetLinearDamping(15);//Cte para establecer la desaceleracion lineal


	//Esto deberia sacarse y cambiar a estados
	//((sUserData*)this->body->GetUserData())->tipo = TIPO_ENEMIGO_BOLA_DE_NIEVE;
	this->estado = EVENTO_CLIENTE_ENEMIGO_BOLA_DE_NIEVE;

	this->body->SetFixedRotation(false);
}

void Enemigo::volverAlEstadoNormal(){
this->esBola = false;
	//Borro la fixture de la bola de nieve y creo la misma que antes
	b2Fixture* fixtureCuerpo = this->body->GetFixtureList();
				
	this->body->DestroyFixture(fixtureCuerpo);
	this->crearForma();

	this->body->SetTransform(this->body->GetPosition(),0);
	this->body->SetAngularVelocity(0);
	this->body->SetFixedRotation(true);
	this->body->SetLinearDamping(0);
	//Esto deberia sacarse y cambiar a estados
	//((sUserData*)this->body->GetUserData())->tipo = TIPO_ENEMIGO;
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
}


Enemigo::~Enemigo(){
	//Es estatica, de instancia, se destruye sola?
	//delete this->userData;
	
	
	this->liberarSnowBoysAtrapados();
	//El enemigo se saca del world, esto podria ir en ~Figura
	this->body->GetWorld()->DestroyBody(this->body);

	

}


void Enemigo::liberarSnowBoysAtrapados(){
	for (set<SnowBoy3*>::iterator snowBoyALiberar = this->snowBoysAtrapados.begin(); snowBoyALiberar != this->snowBoysAtrapados.end(); ++snowBoyALiberar){
		(*snowBoyALiberar)->liberarDeBolaDeNieve(this->body->GetPosition());
	}
	this->snowBoysAtrapados.clear();

}



void Enemigo::setearEstado(){
	//Si fue pateada que se le setee ese estado
	//Estado_bola_pateada puede setearse sólo en patear( asi: this->estado = BOLA_PATEADA
	/* podria hacerse algo como:
		if(this->estado != Bola_pateada){ Personaje::setearEstdo();}		
	*/
}

void Enemigo::atraparSnowBoyEnBolaDeNieve(SnowBoy3* snowBoyAtrapado){
	snowBoyAtrapado->atraparEnBolaDeNieve();
	this->snowBoysAtrapados.insert(snowBoyAtrapado);
}

bool Enemigo::estaMuerto(){
	return ( (this->estado == EVENTO_CLIENTE_ENEMIGO_MUERTO_POR_BOLA_DE_NIEVE_DESTRUIDA) || (this->estado == EVENTO_CLIENTE_ENEMIGO_MUERTO_POR_GOLPE_DE_BOLA_DE_NIEVE) );
}

void Enemigo::matarConBolaDeNievePateada(){
	this->estado = EVENTO_CLIENTE_ENEMIGO_MUERTO_POR_GOLPE_DE_BOLA_DE_NIEVE;


}