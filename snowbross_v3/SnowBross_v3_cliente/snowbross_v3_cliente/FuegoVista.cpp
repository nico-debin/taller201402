#include "StdAfx.h"
#include "FuegoVista.h"


FuegoVista::FuegoVista(sEvento estadoEvento): ProyectilVista(estadoEvento){
	this->rutaTextura = "imagenes/SnowBrosSheet2.gif";
	this->iniciarClips();	
}

void FuegoVista::iniciarClips(){
	int iW = 26;
	int iH = 20;

	this->clipIzq.x = 113;
	this->clipIzq.y = 165;
	this->clipIzq.w = iW;
	this->clipIzq.h = iH;

	this->clipDer.x = 280;
	this->clipDer.y = 164;
	this->clipDer.w = iW;
	this->clipDer.h = iH;

}