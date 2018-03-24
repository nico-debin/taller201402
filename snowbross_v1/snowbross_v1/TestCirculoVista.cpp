#include "stdafx.h"

#include "TestCirculoVista.h"

void TestCirculoVista::testDibujarCirculo(){
	//Se utilizan variables globlales para estos datos del escenario
	Utilities::ANCHO_PX = 800;
	Utilities::ALTO_PX = 600;
	Utilities::IMAGEN_FONDO_NIVEL1 = "./imagenes/fondo.01.800x600.png";

	EscenarioVista escenarioVista;

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
	escenarioVista.addObjeto(circuloVista.getId(),&circuloVista);

	ControladorCliente controladorCliente(&escenarioVista);

	SDL_Event evento;
	bool quit = false;
	while (!quit){
		
		while(SDL_PollEvent(&evento)){

			quit = controladorCliente.procesar(evento);

			if(quit== true) break;
		}
	}
	SDL_Quit(); 

}