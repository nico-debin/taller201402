#pragma warning(disable:4244) 
#include "stdafx.h"

#include "Escenario.h"
#include "PoligonoRegular.h"
#include "Rectangulo.h"
#include "Paralelogramo.h"
#include "Trapecio.h"

Escenario::Escenario(sEscenario datosEscenario){
	Utilities::ANCHO_PX = datosEscenario.ancho_px;
	Utilities::ALTO_PX = datosEscenario.alto_px;
	Utilities::ANCHO_UL = datosEscenario.ancho_ul;
	Utilities::ALTO_UL = datosEscenario.alto_ul;
	Utilities::IMAGEN_FONDO_NIVEL1 = datosEscenario.imagen_fondo;

	this->alto_px = datosEscenario.alto_px;
	this->ancho_px = datosEscenario.ancho_px;
	this->alto_ul = datosEscenario.alto_ul;
	this->ancho_ul = datosEscenario.ancho_ul;
	this->imagen_fondo = datosEscenario.imagen_fondo;

	this->figuras = new map<int,Figura*>;
	this->figurasAEliminar= new map<int,Figura*>;

    //this->personajes = new map<int,SnowBoy*>;

	this->fisica = new Fisica(this->ancho_ul, this->alto_ul);

	this->vectorEventosServidor = new vector<sEvento*>();
}

void Escenario::verificarSuperposicion(){
		
	for( map<int,Figura*>::iterator itFigura = this->figuras->begin() ; itFigura != this->figuras->end(); ++itFigura){
		Figura* figura = (*itFigura).second;
		haySuperposicion(figura, figurasAEliminar);
	}

}

void Escenario::eliminarPrimerFiguraSuperpuesta(){

	for( map<int,Figura*>::iterator itFiguraAEliminar = (this->figurasAEliminar)->begin() ; itFiguraAEliminar != (this->figurasAEliminar)->end(); ++itFiguraAEliminar){
			Figura* figuraAEliminar = (*itFiguraAEliminar).second;
			this->figuras->erase(figuraAEliminar->getId());					
	}
}

map<int,Figura*>* Escenario::getFigurasAEliminar(){
	return this->figurasAEliminar;
}

void Escenario::haySuperposicion(Figura* figura, map<int,Figura*>* figurasAEliminar){

		bool superposicion = false;
		b2Body* segundoCuerpo = this->fisica->getMundo()->GetBodyList();

		b2Body* cuerpoAEliminar = figura->getFiguraCuerpo();		
		int idCuerpoAEliminar = figura->getId();

		while((!superposicion)&&(segundoCuerpo != NULL)){
			if(segundoCuerpo != cuerpoAEliminar){
				superposicion = b2TestOverlap(figura->getFixture()->GetShape(),0,segundoCuerpo->GetFixtureList()->GetShape(),0,cuerpoAEliminar->GetTransform(),segundoCuerpo->GetTransform());
				
			}
			
			if(superposicion){
				sObjeto datosCuerpoAEliminar=figura->getDatos();
				if(!(datosCuerpoAEliminar.tipo==TIPO_SNOWBOY)){										
					(*figurasAEliminar)[idCuerpoAEliminar] = figura;
					this->fisica->getMundo()->DestroyBody(cuerpoAEliminar);	
				}
			}

			segundoCuerpo = segundoCuerpo->GetNext();
			
		}
}

Escenario::~Escenario(){
	//for( map<int,SnowBoy*>::iterator it = this->personajes->begin() ; it != this->personajes->end(); ++it){
	//	delete (*it).second;
	//}
	//delete this->personajes;

    	for( map<int,Figura*>::iterator it = this->figuras->begin() ; it != this->figuras->end(); ++it){
		delete (*it).second;
	}
	delete this->figuras;

	// Limpio el vector
	for(std::vector<sEvento*>::iterator it = vectorEventosServidor->begin(); it != vectorEventosServidor->end(); it++){
		delete (*it);
	}
	if(this->figurasAEliminar!=NULL){
		delete this->figurasAEliminar;
	}	
	delete this->vectorEventosServidor;

	delete this->fisica;

}


vector<sEvento*>* Escenario::getEventosEscenario( bool todos, map<string,Cliente*> clientes ){

	// Limpio el vector, libero memoria de los structs
	for(std::vector<sEvento*>::iterator it = this->vectorEventosServidor->begin(); it != this->vectorEventosServidor->end(); it++){
		delete (*it);
	}
	this->vectorEventosServidor->clear();

	Figura* figura;
	// Datos de las figuras
	for( map<int,Figura*>::iterator it = this->figuras->begin() ; it != this->figuras->end(); ++it){
		figura = (*it).second;

		// En caso de que el cliente se haya desconectado actualizao estado del snowboy
		if( figura->getTipo() == TIPO_SNOWBOY ){			
			for (map<string,Cliente*>::iterator it=clientes.begin(); it!=clientes.end(); ++it){
				if( it->second->idSnowBoy == figura->getId() && !it->second->estaConectado){				
					((SnowBoy*)figura)->setEstado( EVENTO_CLIENTE_PERSONAJE_DESCONECTADO );
					break;
				}
			}
		}
		//if( figura->getTipo() == TIPO_SNOWBOY || figura->cambioEstado() || todos ){
		if( figura->cambioEstado() || todos ){
			this->vectorEventosServidor->push_back( this->crearEventoFigura(figura) );
		}
	}

	return this->vectorEventosServidor;
}

void Escenario::agregarFigura(Figura* figura){
	(*this->figuras)[figura->getId()] = figura;
}

//void Escenario::agregarPersonaje(SnowBoy* personaje){
//	(*this->personajes)[personaje->getId()] = personaje;
//}

sEvento* Escenario::crearEventoFigura(Figura* figura ){
	sEvento* evento		= new sEvento;
	memset(evento,0,sizeof(sEvento));

	sObjeto datos = figura->getDatos();

	evento->id			= figura->getId();
	evento->alto		= Utilities::unidadLogica2Pixel( figura->getAlto() , Utilities::ALTO_PX, Utilities::ALTO_UL);
	evento->ancho		= Utilities::unidadLogica2Pixel( figura->getAncho(), Utilities::ANCHO_PX, Utilities::ANCHO_UL);
	
    if( figura->getTipo() == TIPO_SNOWBOY ){
		evento->evento = figura->getEstado();
    } else {
		evento->evento = EVENTO_CLIENTE_FIGURA_ACTUALIZAR;
	}
    evento->x = Utilities::unidadLogica2Pixel( figura->getX(), Utilities::ANCHO_PX, Utilities::ANCHO_UL);
    evento->y = Utilities::euclid2sdl_ejeY( Utilities::unidadLogica2Pixel(figura->getY() , Utilities::ALTO_PX, Utilities::ALTO_UL));

	evento->rot			= figura->getAngulo();

	//evento->color		= figura->getDatos().color;

	//evento->angulo_1	= figura->getDatos().angulo_1;
	//evento->angulo_2	= figura->getDatos().angulo_2;

	evento->tipo = figura->getTipo();
	
	//float auxVerticesX[MAX_LADOS]={0,0,0,0,0,0};
	//float auxVerticesY[MAX_LADOS]={0,0,0,0,0,0};
	
	if( figura->getTipo() == TIPO_POLIGONO ){
		//((PoligonoRegular*)figura)->getVertices(evento->verticesX, evento->verticesY);
		//((PoligonoRegular*)figura)->getVertices(auxVerticesX, auxVerticesY);
		switch(figura->getDatos().lados){
			case 3:
				evento->tipo = TIPO_TRIANGULO;
				break;
			case 4:
				evento->tipo = TIPO_CUADRADO;
				break;
			case 5:
				evento->tipo = TIPO_PENTAGONO;
				break;
			case 6:
				evento->tipo = TIPO_HEXAGONO;
				break;
		}
	}

	//if( figura->getTipo() == TIPO_RECTANGULO ){
	//	((Rectangulo*)figura)->getVertices(auxVerticesX, auxVerticesY);
	//}

	//if( figura->getTipo() == TIPO_PARALELOGRAMO ){
	//	((Paralelogramo*)figura)->getVertices(auxVerticesX, auxVerticesY);
	//}

	//if( figura->getTipo() == TIPO_TRAPECIO ){
	//	((Trapecio*)figura)->getVertices(auxVerticesX, auxVerticesY);
	//}

	//if( figura->getTipo() == TIPO_POLIGONO || figura->getTipo() == TIPO_PARALELOGRAMO || figura->getTipo() == TIPO_TRAPECIO || figura->getTipo() == TIPO_RECTANGULO || figura->getTipo() == TIPO_CUADRADO ){
	//	for( short i = 0; i < figura->getDatos().lados ; i++ ){
	//		evento->verticesX[i] = Utilities::unidadLogica2Pixel( auxVerticesX[i], Utilities::ANCHO_PX, Utilities::ANCHO_UL);
	//		evento->verticesY[i] = Utilities::euclid2sdl_ejeY( Utilities::unidadLogica2Pixel(auxVerticesY[i], Utilities::ALTO_PX, Utilities::ALTO_UL) );
	//	}
	//}

	if( figura->getTipo() == TIPO_SNOWBOY ){
		std::stringstream str;
		str << "Snowboy Mensaje Enviado. x: " <<  evento->x << ". y: " <<  evento->y;
		Logger::getLogger()->debug(str.str());
	}
	return evento;
}

int Escenario::getAltoPixel(){
	return this->alto_px;
}

int Escenario::getAnchoPixel(){
	return this->ancho_px;
}

float Escenario::getAltoLogico(){
	return this->alto_ul;
}

float Escenario::getAnchoLogico(){
	return this->ancho_ul; 
}

Fisica* Escenario::getFisica(){
	return this->fisica;
}

string Escenario::getPathImagenFondo(){
	return this->imagen_fondo;
}

map<int,Figura*>* Escenario::getFiguras(){
	return this->figuras;
}