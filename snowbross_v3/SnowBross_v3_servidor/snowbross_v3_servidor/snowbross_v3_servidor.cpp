// snowbross_v3_servidor.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <SDL2/SDL.h>

#define _WINSOCKAPI_    // frentar windows.h incluyendo winsock.h
#include <windows.h>
#include <winsock2.h>

#include "JuegoControlador.h"
#include "Logger.h"




int main(int argc, char *argv[])	
{
	//TestJson::test();
	//TestFigurasCreacion test2;
	//test2.test_CreacionDeCirculoYRectangulo();
	Logger::getLogger()->setLevel(INFO_LOG);

	JuegoControlador juego;

    stringstream puerto_str;
    puerto_str = stringstream(argv[2]);
    int puerto = 0;
    puerto_str >> puerto;

	juego.iniciar( string(argv[1]), puerto );

	//system("pause");
	return 0;
}

