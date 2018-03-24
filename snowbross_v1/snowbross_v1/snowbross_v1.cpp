// snowbross_v1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <SDL2/SDL.h>

#include "Logger.h"
#include "Escenario.h"
#include "EscenarioVista.h"
#include "ControladorCliente.h"

#include "TestCirculoVista.h"
#include "TestFigurasVista.h"
#include "TestFigurasVistaAndFisica.h"
#include "TestFigurasVistaAndFisica2.h"
#include "TestFigurasCreacion.h"
#include "TestMovimiento.h"
#include "TestJson.h"
#include "TestPersonajeVista.h"

#include "JuegoControlador.h"


int main(int argc, char *argv[])	
{
	//TestCirculoVista test1;
	//test1.testDibujarCirculo();
	
	//TestFigurasVista::test();
	//TestFigurasVistaAndFisica::test();
	//TestPersonajeVista::test();
	//TestFigurasVistaAndFisica2::test();

	//TestJson::test();
	//TestFigurasCreacion test2;
	//test2.test_CreacionDeCirculoYRectangulo();

	//TestMovimiento testMov;
	//testMov.testParalelogramo();
	
	JuegoControlador juego;
	juego.iniciar( string(argv[1]) );

	//system("pause");
	return 0;
}

