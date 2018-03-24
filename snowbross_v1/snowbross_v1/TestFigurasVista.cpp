#include "stdafx.h"
#include <stdio.h>
#include <windows.h>

#include "TestFigurasVista.h"

#include "Utilities.h"
#include "EscenarioVista.h"
#include "ControladorCliente.h"
#include "CirculoVista.h"
#include "TrianguloVista.h"
#include "RectanguloVista.h"
#include "PentagonoVista.h"
#include "HexagonoVista.h"
#include "ParalelogramoVista.h"
#include "TrapecioVista.h"

void TestFigurasVista::test(){
	//Se utilizan variables globlales para estos datos del escenario
	Utilities::ANCHO_PX = 800;
	Utilities::ALTO_PX = 600;
	Utilities::ANCHO_UL = 80;
	Utilities::ALTO_UL = 60;
	Utilities::IMAGEN_FONDO_NIVEL1 = "./imagenes/fondo.01.800x600.png";

	int id_actual = 1;
	
	EscenarioVista escenarioVista;


	/*********************** CIRCULO ****************************/
	//Inicializo el evento de creación del circulo
	sEvento eventoCreacionCirculo;
	eventoCreacionCirculo.id     = 2; // Este valor debe ser unico y definido por el servidor
	//eventoCreacionCirculo.evento = EVENTO_CREAR_CIRCULO;
	eventoCreacionCirculo.x      = 400; // Centro de la pantalla en pixeles
	eventoCreacionCirculo.y      = 300; // Centro de la pantalla en pixeles
	eventoCreacionCirculo.rot    = 0; // Utilizado para mover el punto que permite visualizar que gira
	eventoCreacionCirculo.alto   = 60; 
	eventoCreacionCirculo.ancho  = 60; 
	eventoCreacionCirculo.color  = "#FF0000"; // notacion #rrggbb

	CirculoVista circuloVista(eventoCreacionCirculo);

	//Agrego al escenarioVista el elemento con su id unico
	escenarioVista.addObjeto(circuloVista.getId(), &circuloVista);


	/*********************** RECTANGULO 1 ****************************/
	//Inicializo el evento de creación de un rectangulo
	sEvento eventoCreacionRectangulo;
	eventoCreacionRectangulo.id     = 3; // Este valor debe ser unico y definido por el servidor
	//eventoCreacionRectangulo.evento = EVENTO_CREAR_RECTANGULO;
	eventoCreacionRectangulo.x      = 200; 
	eventoCreacionRectangulo.y      = 300; 
	eventoCreacionRectangulo.rot    = Utilities::grados2radianes(0); // Utilizado para mover el punto que permite visualizar que gira
	eventoCreacionRectangulo.alto   = 60; 
	eventoCreacionRectangulo.ancho  = 120; 
	eventoCreacionRectangulo.color  = "#00F00F"; // notacion #rrggbb

	RectanguloVista rectanguloVista(eventoCreacionRectangulo);

	//Agrego al escenarioVista el elemento con su id unico
	escenarioVista.addObjeto(rectanguloVista.getId(), &rectanguloVista);

	/*********************** TRIANGULO ****************************/
	//Inicializo el evento de creación de un triangulo
	sEvento eventoCreacionTriangulo;
	eventoCreacionTriangulo.id     = 4; // Este valor debe ser unico y definido por el servidor
	//eventoCreacionTriangulo.evento = EVENTO_CREAR_TRIANGULO;
	eventoCreacionTriangulo.x      = 500; // Centro de la pantalla en pixeles
	eventoCreacionTriangulo.y      = 500; // Centro de la pantalla en pixeles
	eventoCreacionTriangulo.rot    = Utilities::grados2radianes(0); // Utilizado para mover el punto que permite visualizar que gira
	eventoCreacionTriangulo.alto   = 80;
	eventoCreacionTriangulo.ancho  = 80;
	eventoCreacionTriangulo.color  = "#00F00F"; // notacion #rrggbb

	TrianguloVista trianguloVista(eventoCreacionTriangulo);

	//Agrego al escenarioVista el elemento con su id unico
	escenarioVista.addObjeto(trianguloVista.getId(), &trianguloVista);


	/*********************** PENTAGONO ****************************/
	//Inicializo el evento de creación de un pentagono
	sEvento eventoCreacionPentagono;
	eventoCreacionPentagono.id     = 5; // Este valor debe ser unico y definido por el servidor
	//eventoCreacionPentagono.evento = EVENTO_CREAR_PENTAGONO;
	eventoCreacionPentagono.x      = 500; // Centro de la pantalla en pixeles
	eventoCreacionPentagono.y      = 200; // Centro de la pantalla en pixeles
	eventoCreacionPentagono.rot    = Utilities::grados2radianes(0); // Utilizado para mover el punto que permite visualizar que gira
	eventoCreacionPentagono.alto   = 80;
	eventoCreacionPentagono.ancho  = 80;
	eventoCreacionPentagono.color  = "#AA00AA"; // notacion #rrggbb

	//PentagonoVista pentagonoVista(eventoCreacionPentagono);

	//Agrego al escenarioVista el elemento con su id unico
	//escenarioVista.addObjeto(pentagonoVista.getId(), &pentagonoVista);

	/*********************** HEXAGONO ****************************/
	//Inicializo el evento de creación de un hexagono
	sEvento eventoCreacionHexagono;
	eventoCreacionHexagono.id     = 6; // Este valor debe ser unico y definido por el servidor
	//eventoCreacionHexagono.evento = EVENTO_CREAR_HEXAGONO;
	eventoCreacionHexagono.x      = 200; // Centro de la pantalla en pixeles
	eventoCreacionHexagono.y      = 500; // Centro de la pantalla en pixeles
	eventoCreacionHexagono.rot    = Utilities::grados2radianes(10); // Utilizado para mover el punto que permite visualizar que gira
	eventoCreacionHexagono.alto   = 70;
	eventoCreacionHexagono.ancho  = 70;
	eventoCreacionHexagono.color  = "#666644"; // notacion #rrggbb

	//HexagonoVista hexagonoVista(eventoCreacionHexagono);

	//Agrego al escenarioVista el elemento con su id unico
	//escenarioVista.addObjeto(hexagonoVista.getId(), &hexagonoVista);

	/*********************** PARALELOGRAMO ****************************/
	//Inicializo el evento de creación de un paralelogramo
	sEvento eventoCreacionParalelogramo;
	eventoCreacionParalelogramo.id     = 7; // Este valor debe ser unico y definido por el servidor
	//eventoCreacionParalelogramo.evento = EVENTO_CREAR_PARALELOGRAMO;
	eventoCreacionParalelogramo.x      = 200; // Centro de la pantalla en pixeles
	eventoCreacionParalelogramo.y      = 130; // Centro de la pantalla en pixeles
	eventoCreacionParalelogramo.rot    = Utilities::grados2radianes(0); // Utilizado para mover el punto que permite visualizar que gira
	eventoCreacionParalelogramo.alto   = 50;
	eventoCreacionParalelogramo.ancho  = 120;
	eventoCreacionParalelogramo.color  = "#666644"; // notacion #rrggbb
	eventoCreacionParalelogramo.angulo_1  = Utilities::grados2radianes(55);

	ParalelogramoVista paralelogramoVista(eventoCreacionParalelogramo);

	//Agrego al escenarioVista el elemento con su id unico
	escenarioVista.addObjeto(paralelogramoVista.getId(), &paralelogramoVista);

	/*********************** TRAPECIO ****************************/
	//Inicializo el evento de creación de un Trapecio
	sEvento eventoCreacionTrapecio;
	eventoCreacionTrapecio.id     = 8; // Este valor debe ser unico y definido por el servidor
	//eventoCreacionTrapecio.evento = EVENTO_CREAR_TRAPECIO;
	eventoCreacionTrapecio.x      = 650; // Centro de la pantalla en pixeles
	eventoCreacionTrapecio.y      = 450; // Centro de la pantalla en pixeles
	eventoCreacionTrapecio.rot    = Utilities::grados2radianes(0); // Utilizado para mover el punto que permite visualizar que gira
	eventoCreacionTrapecio.alto   = 50;
	eventoCreacionTrapecio.ancho  = 120;
	eventoCreacionTrapecio.color  = "#FF8A00"; // notacion #rrggbb
	eventoCreacionTrapecio.angulo_1  = Utilities::grados2radianes(90);
	eventoCreacionTrapecio.angulo_2  = Utilities::grados2radianes(115);

	TrapecioVista trapecioVista(eventoCreacionTrapecio);

	//Agrego al escenarioVista el elemento con su id unico
	escenarioVista.addObjeto(trapecioVista.getId(), &trapecioVista);


	ControladorCliente controladorCliente(&escenarioVista);

	sEvento nuevoEstadoCirculo;
	sEvento nuevoEstadoRectangulo;
	sEvento nuevoEstadoTriangulo;
	sEvento nuevoEstadoHexagono;
	sEvento nuevoEstadoPentagono;
	sEvento nuevoEstadoParalelogramo;
	sEvento nuevoEstadoTrapecio;

	SDL_Event evento;
	bool quit = false;
	while (!quit){
		if(SDL_PollEvent(&evento)){
			quit = controladorCliente.procesar(evento);
		}else{
			escenarioVista.dibujar();
			Sleep(30);

			// Obtengo el circulo
			CirculoVista* circulo = dynamic_cast<CirculoVista*>(escenarioVista.getObjeto(2));

			
			//nuevoEstadoCirculo.evento = EVENTO_ACTUALIZAR_FIGURA;
			nuevoEstadoCirculo.id     = circulo->getId();
			nuevoEstadoCirculo.x      = circulo->get_x();
			nuevoEstadoCirculo.y      = circulo->get_y();
			nuevoEstadoCirculo.rot    = circulo->get_rot() + Utilities::grados2radianes(5);
			nuevoEstadoCirculo.alto   = circulo->get_alto();
			nuevoEstadoCirculo.ancho  = circulo->get_ancho();
			nuevoEstadoCirculo.color  = "#FF0000"; // notacion #rrggbb

			circulo->setEstado(nuevoEstadoCirculo);


			// Obtengo el rectangulo
			RectanguloVista* rectangulo = dynamic_cast<RectanguloVista*>(escenarioVista.getObjeto(3));

			
			//nuevoEstadoRectangulo.evento = EVENTO_ACTUALIZAR_FIGURA;
			nuevoEstadoRectangulo.id     = rectangulo->getId();
			nuevoEstadoRectangulo.x      = rectangulo->get_x();
			nuevoEstadoRectangulo.y      = rectangulo->get_y();
			nuevoEstadoRectangulo.rot    = rectangulo->get_rot() + Utilities::grados2radianes(5);
			nuevoEstadoRectangulo.alto   = rectangulo->get_alto();
			nuevoEstadoRectangulo.ancho  = rectangulo->get_ancho();
			nuevoEstadoRectangulo.color  = "#00F00F"; // notacion #rrggbb

			rectangulo->setEstado(nuevoEstadoRectangulo);


			// Obtengo el triangulo
			TrianguloVista* triangulo = dynamic_cast<TrianguloVista*>(escenarioVista.getObjeto(4));

			
			//nuevoEstadoTriangulo.evento = EVENTO_ACTUALIZAR_FIGURA;
			nuevoEstadoTriangulo.id     = triangulo->getId();
			nuevoEstadoTriangulo.x      = triangulo->get_x();
			nuevoEstadoTriangulo.y      = triangulo->get_y();
			nuevoEstadoTriangulo.rot    = triangulo->get_rot() - Utilities::grados2radianes(40);
			nuevoEstadoTriangulo.alto   = triangulo->get_alto();
			nuevoEstadoTriangulo.ancho  = triangulo->get_ancho();
			nuevoEstadoTriangulo.color  = "#00F00F"; // notacion #rrggbb

			triangulo->setEstado(nuevoEstadoTriangulo);


			// Obtengo el Pentagono
			/*
			PentagonoVista* pentagono = dynamic_cast<PentagonoVista*>(escenarioVista.getObjeto(5));

			
			nuevoEstadoPentagono.evento = EVENTO_ACTUALIZAR_FIGURA;
			nuevoEstadoPentagono.id     = pentagono->getId();
			nuevoEstadoPentagono.x      = pentagono->get_x();
			nuevoEstadoPentagono.y      = pentagono->get_y();
			nuevoEstadoPentagono.rot    = pentagono->get_rot() - Utilities::grados2radianes(10);
			nuevoEstadoPentagono.alto   = pentagono->get_alto();
			nuevoEstadoPentagono.ancho  = pentagono->get_ancho();
			nuevoEstadoPentagono.color  = "#AA00AA"; // notacion #rrggbb

			pentagono->setEstado(nuevoEstadoPentagono);
			*/


			// Obtengo el 
			/*
			HexagonoVista* hexagono = dynamic_cast<HexagonoVista*>(escenarioVista.getObjeto(6));

			
			nuevoEstadoHexagono.evento = EVENTO_ACTUALIZAR_FIGURA;
			nuevoEstadoHexagono.id     = hexagono->getId();
			nuevoEstadoHexagono.x      = hexagono->get_x();
			nuevoEstadoHexagono.y      = hexagono->get_y();
			nuevoEstadoHexagono.rot    = hexagono->get_rot() - Utilities::grados2radianes(10);
			nuevoEstadoHexagono.alto   = hexagono->get_alto();
			nuevoEstadoHexagono.ancho  = hexagono->get_ancho();
			nuevoEstadoHexagono.color  = "#666644"; // notacion #rrggbb

			hexagono->setEstado(nuevoEstadoHexagono);
			*/


			// Obtengo el Paralelogramo
			ParalelogramoVista* paralelogramo = dynamic_cast<ParalelogramoVista*>(escenarioVista.getObjeto(7));

			
			//nuevoEstadoParalelogramo.evento = EVENTO_ACTUALIZAR_FIGURA;
			nuevoEstadoParalelogramo.id     = paralelogramo->getId();
			nuevoEstadoParalelogramo.x      = paralelogramo->get_x();
			nuevoEstadoParalelogramo.y      = paralelogramo->get_y();
			nuevoEstadoParalelogramo.rot    = paralelogramo->get_rot() - Utilities::grados2radianes(7);
			nuevoEstadoParalelogramo.alto   = paralelogramo->get_alto();
			nuevoEstadoParalelogramo.ancho  = paralelogramo->get_ancho();
			nuevoEstadoParalelogramo.angulo_1  = paralelogramo->get_angulo1();
			nuevoEstadoParalelogramo.color  = "#666644"; // notacion #rrggbb

			paralelogramo->setEstado(nuevoEstadoParalelogramo);


			// Obtengo el Trapecio
			TrapecioVista* trapecio = dynamic_cast<TrapecioVista*>(escenarioVista.getObjeto(8));

			//nuevoEstadoTrapecio.evento = EVENTO_ACTUALIZAR_FIGURA;
			nuevoEstadoTrapecio.id     = trapecio->getId();
			nuevoEstadoTrapecio.x      = trapecio->get_x();
			nuevoEstadoTrapecio.y      = trapecio->get_y();
			nuevoEstadoTrapecio.rot    = trapecio->get_rot() - Utilities::grados2radianes(7);
			nuevoEstadoTrapecio.alto   = trapecio->get_alto();
			nuevoEstadoTrapecio.ancho  = trapecio->get_ancho();
			nuevoEstadoTrapecio.angulo_1  = trapecio->get_angulo1();
			nuevoEstadoTrapecio.angulo_2  = trapecio->get_angulo2();
			nuevoEstadoTrapecio.color  = "#FF8A00"; // notacion #rrggbb

			trapecio->setEstado(nuevoEstadoTrapecio);



		}

	}
	SDL_Quit(); 

}
