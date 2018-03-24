#include "StdAfx.h"
#include "Figura.h"
#include "SnowBoy.h"
#include "Logger.h"

Figura::Figura(b2World* mundo,sObjeto structObj){
	this->datos = structObj;
	this->datos.rot = Utilities::grados2radianes(this->datos.rot);

	b2BodyDef bodyDef;
	if(!this->datos.estatico){
		bodyDef.type = b2_dynamicBody;
	}
	bodyDef.position.Set(this->datos.x, this->datos.y);//posicion en el mundo
	bodyDef.angle = this->datos.rot;


	while (mundo->IsLocked()){Sleep(1);}
	this->body = mundo->CreateBody(&bodyDef);

		//Estado anterior inicial
	this->datosEstadoAnterior.alto=0;
	this->datosEstadoAnterior.ancho=0;
	this->datosEstadoAnterior.angulo_1=0;
	this->datosEstadoAnterior.angulo_2=0;
	this->datosEstadoAnterior.baseMayor=0;
	this->datosEstadoAnterior.baseMenor=0;
	this->datosEstadoAnterior.color="";
	this->datosEstadoAnterior.escala=0;
	this->datosEstadoAnterior.estatico=false;
	this->datosEstadoAnterior.id=-1;
	this->datosEstadoAnterior.lados=0;
	this->datosEstadoAnterior.masa=0;
	this->datosEstadoAnterior.radio=0;
	this->datosEstadoAnterior.rot=0;
	this->datosEstadoAnterior.tipo=TIPO_NULL;
	this->datosEstadoAnterior.x=-1;
	this->datosEstadoAnterior.y=-1;	
	this->estado = -1;
	this->estadoAnterior = -2;

	this->timerAgonia = nullptr;
	this->estaMuerto = false;

	this->userData.tipo = this->datos.tipo;
	this->userData.objeto = this;
	this->body->SetUserData((void*)&this->userData);
}

Figura::~Figura(void){
	this->body->GetWorld()->DestroyBody(this->body);
	delete this->timerAgonia;
}

sObjeto Figura::getDatos(){	
	this->datos.rot = this->getAngulo();
	this->datos.x   = this->getX();
	if( this->getTipo() == TIPO_SNOWBOY || this->getTipo() == TIPO_DIABLO || this->getTipo() == TIPO_DRAGON ){
		//this->estado  = ((SnowBoy*)this)->getEstado();
		//this->datos.y = ((SnowBoy*)this)->getY();
		this->estado  = ((Personaje*)this)->getEstado();
		this->datos.y = ((Personaje*)this)->getY();
	} else {
		this->datos.y   = this->getY();
	}
	return this->datos;
}

TIPO_FIGURA Figura::getTipo(){
	return this->datos.tipo;
}

int Figura::getId(){
	return this->datos.id;
}

bool Figura::esEstatico(){
	return this->datos.estatico;
}

float Figura::gradosARadianes(float grados){
	return (grados*b2_pi/180.0f);
}

float32 Figura::getX(){
	return body->GetPosition().x;
}

float32 Figura::getY(){
	return body->GetPosition().y;
}

float Figura::getAncho(){
	return this->datos.ancho;
}

float Figura::getAlto(){
	return this->datos.alto;
}

float32 Figura::getAngulo(){
	return this->body->GetAngle();
}

float Figura::calcularDensidad(){
	return(this->datos.masa/calcularArea());
}

void Figura::setFiguraCuerpo(b2Body* figuraCuerpo){
	this->body = figuraCuerpo;
}

b2Body* Figura::getFiguraCuerpo(){
	return this->body;
}


void Figura::setEstadoAnterior( sObjeto datosEstadoAnterior ){
	this->datosEstadoAnterior = datosEstadoAnterior;
}

bool Figura::cambioEstado(){	
	// Actualizo estado con datos de box2d
    this->actualizarEstado();

	// Presicion de 1 pixel
	float presicionMovimiento =  Utilities::pixel2unidadLogica(4, Utilities::ALTO_PX, Utilities::ALTO_UL);
	float presicionMovimientoNegativa = -1*presicionMovimiento;

	// 1 grado = 0.02 radianes
	float presicionAnguloMovimiento = Utilities::grados2radianes(10);
	float presicionAnguloMovimientoNegativa = -1*presicionAnguloMovimiento;

	float deltaX = this->datos.x   - this->datosEstadoAnterior.x;
	float deltay = this->datos.y   - this->datosEstadoAnterior.y;
	float deltaAngulo = this->datos.rot - this->datosEstadoAnterior.rot;
	
	bool cambioEstado = this->estado != this->estadoAnterior;
	bool cambioX = !( presicionMovimientoNegativa < deltaX && deltaX < presicionMovimiento );
	bool cambioY = !( presicionMovimientoNegativa < deltay && deltay < presicionMovimiento );
	bool cambioAngulo = !( presicionAnguloMovimientoNegativa < deltaAngulo && deltaAngulo < presicionAnguloMovimiento );
    if ( this->datos.tipo == TIPO_SNOWBOY && ( this->estado == EVENTO_CLIENTE_FIGURA_MURIENDO || this->estado == EVENTO_CLIENTE_FIGURA_MUERTO )){
        stringstream stream;
        stream << "Estado Actual: "<< this->estado << " Estado Anterior: "<< this->estadoAnterior;
		Logger::getLogger()->debug(stream.str());
    }

	return cambioEstado ||
		   cambioX ||
		   cambioY ||
		   cambioAngulo ; 
}

//Utilizada para guardar el estado anterior
void Figura::actualizarEstadoAnterior(){
	this->datosEstadoAnterior = this->datos;	    
	this->estadoAnterior = this->estado;
}

//Utilizada para actualizar el estado
void Figura::actualizarEstado(){
	this->getDatos();
}

void Figura::setEstado( int estado ){
	this->estado = estado;
}

int Figura::getEstado(){
	return this->estado;
}

void Figura::matar(){
	if( this->timerAgonia == nullptr ){
   		stringstream stream;
        stream << "Muriendo Figura. Id: "<< this->datos.id <<" Estado Actual: " << this->estado;
		Logger::getLogger()->debug(stream.str());

		this->timerAgonia = new Timer();
 		this->timerAgonia->start();
		if ( this->datos.tipo != TIPO_FUEGO && this->datos.tipo != TIPO_BONUS_VIDA && this->datos.tipo != TIPO_BONUS_VELOCIDAD) this->estado = EVENTO_CLIENTE_FIGURA_MURIENDO;
		//this->estado = EVENTO_CLIENTE_FIGURA_MURIENDO;

		//Esto es para evitar problemas si la figura debe morir y teletransportarse
		this->body->SetUserData(nullptr);
	}
}

bool Figura::estaMuerta(){
	if( this->estaMuerto || this->timerAgonia != nullptr && this->timerAgonia->getTicks() > ( this->tiempoAgonia + 5000 ) ) {
		this->estado = EVENTO_CLIENTE_FIGURA_MUERTO;
		return true;
	} else if( this->timerAgonia != nullptr && this->timerAgonia->getTicks() > this->tiempoAgonia ) {
   		stringstream stream;
        stream << "Muerta Figura. Id: "<< this->datos.id <<" Estado Actual: " << this->estado;
		Logger::getLogger()->debug(stream.str());				
		this->estado = EVENTO_CLIENTE_FIGURA_MUERTO;
	}

	return false;
}

void  Figura::muerto(){
	this->estaMuerto = true;
}

bool Figura::estaViva(){
	return ( (this->estado != EVENTO_CLIENTE_FIGURA_MURIENDO) && (this->estado != EVENTO_CLIENTE_FIGURA_MUERTO) && (this->estado != EVENTO_CLIENTE_SNOWBOY_VA_A_RENACER ));
}