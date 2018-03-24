#include "StdAfx.h"
#include "NieveVista.h"


NieveVista::NieveVista(sEvento estadoEvento): ProyectilVista(estadoEvento){
	this->rutaTextura = "imagenes/SnowBrosSheet2.gif";
	this->iniciarClips();
	
	/*
 	controlSonido.tocar(ControlRemoto::REPRODUCIR, Temas::MUSICA);
	
    controlSonido.tocar(ControlRemoto::PAUSA, Temas::MUSICA);
	
	controlSonido.tocar(ControlRemoto::REPRODUCIR, Temas::BOMBA1);
	
	controlSonido.tocar(ControlRemoto::REPRODUCIR, Temas::MUSICA);
	controlSonido.tocar(ControlRemoto::PAUSA, Temas::MUSICA);
	controlSonido.tocar(ControlRemoto::REPRODUCIR, Temas::CLICK);		
	controlSonido.tocar(ControlRemoto::PARAR, Temas::MUSICA);
	*/
	this->controlSonido.tocar(ControlRemoto::REPRODUCIR, Temas::BOMBA1);
	//printf("Entro en el constructor \n");
}

void NieveVista::iniciarClips(){
	int iW = 9;
	int iH = 16;

	this->clipIzq.x = 95;
	this->clipIzq.y = 2102;
	this->clipIzq.w = iW;
	this->clipIzq.h = iH;

	this->clipDer.x = 317;
	this->clipDer.y = 2102;
	this->clipDer.w = iW;
	this->clipDer.h = iH;

}

NieveVista::~NieveVista(){
	//printf("Entro en el destructor \n");
	//this->controlSonido.tocar(ControlRemoto::SINSONIDO, Temas::BOMBA1);
}