#include "StdAfx.h"
#include "TestMovimiento.h"
#include "stdafx.h"
#include <stdio.h>
#include <windows.h>

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
#include "Paralelogramo.h"

TestMovimiento::TestMovimiento(void)
{
}

void TestMovimiento::testParalelogramo(){
	/******************* INIT PRUEBA INTREGRACION BOX2D *********************/
	b2Vec2 gravedad = b2Vec2(0.0f, -9.8f); 
	b2World mundo(gravedad);
	

	//-----  Inicio Creacion de hexagono  -----
	/*
	b2BodyDef hexagonoBodyDef;	
	hexagonoBodyDef.type = b2_dynamicBody;
	hexagonoBodyDef.position.Set(200, 250);//posicion en el mundo
	hexagonoBodyDef.angle=Utilities::grados2radianes(10);
	b2Body* hexagonoBody = mundo.CreateBody(&hexagonoBodyDef);

	b2Vec2 vertices[6];
	vertices[0].Set( 3830, 5350 );
	vertices[1].Set( 4210, 5320 );
	vertices[2].Set( 4380, 4980);
	vertices[3].Set( 4170, 4670);
	vertices[4].Set( 3800, 4690);
	vertices[5].Set( 3640, 5010);
	
	b2PolygonShape* hexagonoShape = new b2PolygonShape;
	hexagonoShape->Set( vertices, 6  ); //pass array to the shape

	b2FixtureDef hexagonoFixDef;
	hexagonoFixDef.shape=hexagonoShape;
	hexagonoFixDef.restitution=0;
	hexagonoBody->CreateFixture(&hexagonoFixDef);
	*/
	//------  Fin Creacion del hexagono  -----

	//-----  Inicio Creacion de cuadrado-----
	/*
	b2BodyDef cuadradoBodyDef;	
	cuadradoBodyDef.type = b2_dynamicBody;
	cuadradoBodyDef.position.Set(100, 700);//posicion en el mundo
	cuadradoBodyDef.angle=Utilities::grados2radianes(10);
	b2Body* cuadradoBody = mundo.CreateBody(&cuadradoBodyDef);

	b2Vec2 verticesCuadrado[4];
	verticesCuadrado[0].Set( 10, 50 );
	verticesCuadrado[1].Set( 35, 45 );
	verticesCuadrado[2].Set( 40, 65);
	verticesCuadrado[3].Set( 15, 70);

	b2PolygonShape* cuadradoShape = new b2PolygonShape;
	cuadradoShape->Set( verticesCuadrado, 4  ); //pass array to the shape

	b2FixtureDef cuadradoFixDef;
	cuadradoFixDef.shape=cuadradoShape;
	cuadradoFixDef.restitution=0;
	cuadradoBody->CreateFixture(&cuadradoFixDef);
	*/
	//------  Fin Creacion del cuadrado  -----

		
	Circulo circuloFisico2(&mundo, 400, 950, 30.0f, "Rojo", 0, 1, false);
	Rectangulo rectFisico(&mundo, 400, 100, 300.0f,100.0f, "Rojo", 0,1, true);
	Paralelogramo paralelogramoFisico(&mundo, 400, 470,120.0f,50.0f,Utilities::grados2radianes(55),"Rojo", 0,1, true);

	//a static floor to drop things on
	b2Vec2 v1(-1000.0f, 0.0f);
	b2Vec2 v2(1000.0f, 0.0f);
	b2EdgeShape edge;
	edge.Set(v1, v2);

	b2BodyDef pisoDef;
	pisoDef.position.Set(0.0f,0);
	pisoDef.type=b2_staticBody;
	b2Body* piso=mundo.CreateBody(&pisoDef);

	b2FixtureDef pisoFixDef;
	pisoFixDef.shape=&edge;
	pisoFixDef.restitution=0;
	piso->CreateFixture(&pisoFixDef);
	
	

	float32 timeStep = 1.0f / 30.0f;
	int32 velocityIterations = 8;
	int32 positionIterations = 3;
	/******************* END PRUEBA INTREGRACION BOX2D *********************/
	
	//Se utilizan variables globlales para estos datos del escenario
	Utilities::ANCHO_PX = 800;
	Utilities::ALTO_PX = 600;
	Utilities::ANCHO_UL = 80;
	Utilities::ALTO_UL = 60;
	Utilities::IMAGEN_FONDO_NIVEL1 = "./imagenes/fondo.01.800x600.png";

	int id_actual = 2;
	
	EscenarioVista escenarioVista;


	/*********************** CIRCULO ****************************/
	//Inicializo el evento de creación del circulo
	sEvento eventoCreacionCirculo;
	eventoCreacionCirculo.id     = id_actual++; // Este valor debe ser unico y definido por el servidor
	//eventoCreacionCirculo.evento = EVENTO_CREAR_CIRCULO;
	eventoCreacionCirculo.x      = 385; // Centro de la pantalla en pixeles
	eventoCreacionCirculo.y      = Utilities::euclid2sdl_ejeY(600); // Centro de la pantalla en pixeles
	eventoCreacionCirculo.rot    = 0; // Utilizado para mover el punto que permite visualizar que gira
	eventoCreacionCirculo.alto   = 60; 
	eventoCreacionCirculo.ancho  = 60; 
	eventoCreacionCirculo.color  = "#FF0000"; // notacion #rrggbb

	//eventoCreacionCirculo.x, Utilities::sdl2euclid_ejeY(eventoCreacionCirculo.y)
	
	CirculoVista circuloVista(eventoCreacionCirculo);
	Circulo circuloFisico1(&mundo, eventoCreacionCirculo.x, Utilities::sdl2euclid_ejeY(eventoCreacionCirculo.y), 30.0f, "Rojo", 0,1000, false);

	//Agrego al escenarioVista el elemento con su id unico
	escenarioVista.addObjeto(circuloVista.getId(), &circuloVista);


	/*********************** RECTANGULO 1 ****************************/
	//Inicializo el evento de creación de un rectangulo
	sEvento eventoCreacionRectangulo;
	eventoCreacionRectangulo.id     = 20;//id_actual++; // Este valor debe ser unico y definido por el servidor
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


	/*********************** CIRCULO 2 ****************************/
	//Inicializo el evento de creación del circulo
	sEvento eventoCreacionCirculo2;
	eventoCreacionCirculo2.id     = id_actual++; // Este valor debe ser unico y definido por el servidor
	//eventoCreacionCirculo2.evento = EVENTO_CREAR_CIRCULO;
	eventoCreacionCirculo2.x      = 200; // Centro de la pantalla en pixeles
	eventoCreacionCirculo2.y      = 300; // Centro de la pantalla en pixeles
	eventoCreacionCirculo2.rot    = 0; // Utilizado para mover el punto que permite visualizar que gira
	eventoCreacionCirculo2.alto   = 2; 
	eventoCreacionCirculo2.ancho  = 2; 
	eventoCreacionCirculo2.color  = "#FFFFFF"; // notacion #rrggbb

	CirculoVista circuloVista2(eventoCreacionCirculo2);

	//Agrego al escenarioVista el elemento con su id unico
	escenarioVista.addObjeto(circuloVista2.getId(), &circuloVista2);


	/*********************** RECTANGULO 2 ****************************/
	//Inicializo el evento de creación de un rectangulo
	sEvento eventoCreacionRectangulo2;
	eventoCreacionRectangulo2.id     = 3;//id_actual++; // Este valor debe ser unico y definido por el servidor
	//eventoCreacionRectangulo2.evento = EVENTO_CREAR_RECTANGULO;
	eventoCreacionRectangulo2.x      = 300; // Centro de la pantalla en pixeles
	eventoCreacionRectangulo2.y      = 300; // Centro de la pantalla en pixeles
	eventoCreacionRectangulo2.rot    = 0; // Utilizado para mover el punto que permite visualizar que gira
	/*eventoCreacionRectangulo2.x      = cuadradoBody->GetPosition().x;
	eventoCreacionRectangulo2.y      = cuadradoBody->GetPosition().y;
	eventoCreacionRectangulo2.rot    = cuadradoBody->GetAngle();
	*/
	eventoCreacionRectangulo2.alto   = 120; 
	eventoCreacionRectangulo2.ancho  = 140; 
	eventoCreacionRectangulo2.color  = "#F00F00"; // notacion #rrggbb

	RectanguloVista rectanguloVista2(eventoCreacionRectangulo2);

	// Defino los vertices del rectangulo como si vinieran de box2d
	float vertices_ul_x[4], vertices_ul_y[4];
	vertices_ul_x[0] = 1;
	vertices_ul_y[0] = 5;

	vertices_ul_x[1] = 3.5;
	vertices_ul_y[1] = 4.5;

	vertices_ul_x[2] = 4;
	vertices_ul_y[2] = 6.5;

	vertices_ul_x[3] = 1.5;
	vertices_ul_y[3] = 7;

	rectanguloVista2.setVertices(vertices_ul_x,vertices_ul_y);

	//Agrego al escenarioVista el elemento con su id unico
	escenarioVista.addObjeto(rectanguloVista2.getId(), &rectanguloVista2);

	/*********************** TRIANGULO ****************************/
	//Inicializo el evento de creación de un triangulo
	sEvento eventoCreacionTriangulo;
	eventoCreacionTriangulo.id     = 106;//id_actual++; // Este valor debe ser unico y definido por el servidor
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


	/*********************** CIRCULO 3 ****************************/
	//Inicializo el evento de creación del circulo
	sEvento eventoCreacionCirculo3;
	eventoCreacionCirculo3.id     = id_actual++; // Este valor debe ser unico y definido por el servidor
	//eventoCreacionCirculo3.evento = EVENTO_CREAR_CIRCULO;
	eventoCreacionCirculo3.x      = 500; // Centro de la pantalla en pixeles
	eventoCreacionCirculo3.y      = 500; // Centro de la pantalla en pixeles
	eventoCreacionCirculo3.rot    = 0; // Utilizado para mover el punto que permite visualizar que gira
	eventoCreacionCirculo3.alto   = 2; 
	eventoCreacionCirculo3.ancho  = 2; 
	eventoCreacionCirculo3.color  = "#FFFFFF"; // notacion #rrggbb

	CirculoVista circuloVista3(eventoCreacionCirculo3);

	//Agrego al escenarioVista el elemento con su id unico
	escenarioVista.addObjeto(circuloVista3.getId(), &circuloVista3);

	/*********************** PENTAGONO ****************************/
	//Inicializo el evento de creación de un pentagono
	sEvento eventoCreacionPentagono;
	eventoCreacionPentagono.id     = 80;//id_actual++; // Este valor debe ser unico y definido por el servidor
	//eventoCreacionPentagono.evento = EVENTO_CREAR_PENTAGONO;
	eventoCreacionPentagono.x      = 500; // Centro de la pantalla en pixeles
	eventoCreacionPentagono.y      = 200; // Centro de la pantalla en pixeles
	eventoCreacionPentagono.rot    = Utilities::grados2radianes(0); // Utilizado para mover el punto que permite visualizar que gira
	eventoCreacionPentagono.alto   = 80;
	eventoCreacionPentagono.ancho  = 80;
	eventoCreacionPentagono.color  = "#AA00AA"; // notacion #rrggbb

	PentagonoVista pentagonoVista(eventoCreacionPentagono);

	//Agrego al escenarioVista el elemento con su id unico
	escenarioVista.addObjeto(pentagonoVista.getId(), &pentagonoVista);


	/*********************** CIRCULO 4 ****************************/
	//Inicializo el evento de creación del circulo
	sEvento eventoCreacionCirculo4;
	eventoCreacionCirculo4.id     = id_actual++; // Este valor debe ser unico y definido por el servidor
	//eventoCreacionCirculo4.evento = EVENTO_CREAR_CIRCULO;
	eventoCreacionCirculo4.x      = 500; // Centro de la pantalla en pixeles
	eventoCreacionCirculo4.y      = 200; // Centro de la pantalla en pixeles
	eventoCreacionCirculo4.rot    = 0; // Utilizado para mover el punto que permite visualizar que gira
	eventoCreacionCirculo4.alto   = 2; 
	eventoCreacionCirculo4.ancho  = 2; 
	eventoCreacionCirculo4.color  = "#FFFFFF"; // notacion #rrggbb

	CirculoVista circuloVista4(eventoCreacionCirculo4);

	//Agrego al escenarioVista el elemento con su id unico
	escenarioVista.addObjeto(circuloVista4.getId(), &circuloVista4);

	/*********************** HEXAGONO ****************************/
	//Inicializo el evento de creación de un hexagono
	sEvento eventoCreacionHexagono;
	eventoCreacionHexagono.id     = 10;//id_actual++; // Este valor debe ser unico y definido por el servidor
	//eventoCreacionHexagono.evento = EVENTO_CREAR_HEXAGONO;
	eventoCreacionHexagono.x      = 100; // Centro de la pantalla en pixeles
	eventoCreacionHexagono.y      = 500; // Centro de la pantalla en pixeles	
	eventoCreacionHexagono.rot    = Utilities::grados2radianes(10); // Utilizado para mover el punto que permite visualizar que gira		
	/*eventoCreacionHexagono.x      = hexagonoBody->GetPosition().x;
	eventoCreacionHexagono.y      = Utilities::ALTO_PX - hexagonoBody->GetPosition().y; 
	eventoCreacionHexagono.rot    = hexagonoBody->GetAngle();
	*/
	eventoCreacionHexagono.alto   = 70;
	eventoCreacionHexagono.ancho  = 70;
	eventoCreacionHexagono.color  = "#666644"; // notacion #rrggbb

	HexagonoVista hexagonoVista(eventoCreacionHexagono);

	//Agrego al escenarioVista el elemento con su id unico
	escenarioVista.addObjeto(hexagonoVista.getId(), &hexagonoVista);


	/*********************** CIRCULO 5 ****************************/
	//Inicializo el evento de creación del circulo
	sEvento eventoCreacionCirculo5;
	eventoCreacionCirculo5.id     = id_actual++; // Este valor debe ser unico y definido por el servidor
	//eventoCreacionCirculo5.evento = EVENTO_CREAR_CIRCULO;
	eventoCreacionCirculo5.x      = 200; // Centro de la pantalla en pixeles
	eventoCreacionCirculo5.y      = 500; // Centro de la pantalla en pixeles
	eventoCreacionCirculo5.rot    = 0; // Utilizado para mover el punto que permite visualizar que gira
	eventoCreacionCirculo5.alto   = 2; 
	eventoCreacionCirculo5.ancho  = 2; 
	eventoCreacionCirculo5.color  = "#FFFFFF"; // notacion #rrggbb

	CirculoVista circuloVista5(eventoCreacionCirculo5);

	//Agrego al escenarioVista el elemento con su id unico
	escenarioVista.addObjeto(circuloVista5.getId(), &circuloVista5);

	/*********************** PARALELOGRAMO ****************************/
	//Inicializo el evento de creación de un paralelogramo
	sEvento eventoCreacionParalelogramo;
	eventoCreacionParalelogramo.id     = 12;//id_actual++; // Este valor debe ser unico y definido por el servidor
	//eventoCreacionParalelogramo.evento = EVENTO_CREAR_PARALELOGRAMO;
	eventoCreacionParalelogramo.x      = 400; // Centro de la pantalla en pixeles
	eventoCreacionParalelogramo.y      = 130; // Centro de la pantalla en pixeles
	eventoCreacionParalelogramo.rot    = Utilities::grados2radianes(0); // Utilizado para mover el punto que permite visualizar que gira
	eventoCreacionParalelogramo.alto   = 50;
	eventoCreacionParalelogramo.ancho  = 120;
	eventoCreacionParalelogramo.color  = "#666644"; // notacion #rrggbb
	eventoCreacionParalelogramo.angulo_1  = Utilities::grados2radianes(55);

	ParalelogramoVista paralelogramoVista(eventoCreacionParalelogramo);

	//Agrego al escenarioVista el elemento con su id unico
	escenarioVista.addObjeto(paralelogramoVista.getId(), &paralelogramoVista);


	/*********************** CIRCULO 6 ****************************/
	//Inicializo el evento de creación del circulo
	sEvento eventoCreacionCirculo6;
	eventoCreacionCirculo6.id     = id_actual++; // Este valor debe ser unico y definido por el servidor
	//eventoCreacionCirculo6.evento = EVENTO_CREAR_CIRCULO;
	eventoCreacionCirculo6.x      = 200; // Centro de la pantalla en pixeles
	eventoCreacionCirculo6.y      = 130; // Centro de la pantalla en pixeles
	eventoCreacionCirculo6.rot    = 0; // Utilizado para mover el punto que permite visualizar que gira
	eventoCreacionCirculo6.alto   = 2; 
	eventoCreacionCirculo6.ancho  = 2; 
	eventoCreacionCirculo6.color  = "#FFFFFF"; // notacion #rrggbb

	CirculoVista circuloVista6(eventoCreacionCirculo6);

	//Agrego al escenarioVista el elemento con su id unico
	escenarioVista.addObjeto(circuloVista6.getId(), &circuloVista6);

	/*********************** TRAPECIO ****************************/
	//Inicializo el evento de creación de un Trapecio
	sEvento eventoCreacionTrapecio;
	eventoCreacionTrapecio.id     = 14;//id_actual++; // Este valor debe ser unico y definido por el servidor
	//eventoCreacionTrapecio.evento = EVENTO_CREAR_TRAPECIO;
	eventoCreacionTrapecio.x      = 650; // Centro de la pantalla en pixeles
	eventoCreacionTrapecio.y      = 450; // Centro de la pantalla en pixeles
	eventoCreacionTrapecio.rot    = Utilities::grados2radianes(0); // Utilizado para mover el punto que permite visualizar que gira
	eventoCreacionTrapecio.alto   = 90;//50;
	eventoCreacionTrapecio.ancho  = 200;//120;
	eventoCreacionTrapecio.color  = "#666644"; // notacion #rrggbb
	eventoCreacionTrapecio.angulo_1  = Utilities::grados2radianes(85);//65);
	eventoCreacionTrapecio.angulo_2  = Utilities::grados2radianes(50);

	TrapecioVista trapecioVista(eventoCreacionTrapecio);

	//Agrego al escenarioVista el elemento con su id unico
	escenarioVista.addObjeto(trapecioVista.getId(), &trapecioVista);


	/*********************** CIRCULO 7 ****************************/
	//Inicializo el evento de creación del circulo
	sEvento eventoCreacionCirculo7;
	eventoCreacionCirculo7.id     = id_actual++; // Este valor debe ser unico y definido por el servidor
	///eventoCreacionCirculo7.evento = EVENTO_CREAR_CIRCULO;
	eventoCreacionCirculo7.x      = 650; // Centro de la pantalla en pixeles
	eventoCreacionCirculo7.y      = 450; // Centro de la pantalla en pixeles
	eventoCreacionCirculo7.rot    = 0; // Utilizado para mover el punto que permite visualizar que gira
	eventoCreacionCirculo7.alto   = 2; 
	eventoCreacionCirculo7.ancho  = 2; 
	eventoCreacionCirculo7.color  = "#FFFFFF"; // notacion #rrggbb

	CirculoVista circuloVista7(eventoCreacionCirculo7);

	//Agrego al escenarioVista el elemento con su id unico
	escenarioVista.addObjeto(circuloVista7.getId(), &circuloVista7);


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

			/******************* INIT PRUEBA INTREGRACION BOX2D *********************/
			mundo.Step(timeStep, velocityIterations, positionIterations);

			//printf("Circulo X: %4.2f Y:%4.2f\n", circuloFisico.getX(), circuloFisico.getY());
			//printf("Rectang X: %4.2f Y:%4.2f\n\n", rectFisico.getX(), rectFisico.getY());

			/******************* END PRUEBA INTREGRACION BOX2D *********************/

			// Obtengo Rectangulo Cuadrado 2
			/*RectanguloVista* rectangulo = dynamic_cast<RectanguloVista*>(escenarioVista.getObjeto(20));
			
			nuevoEstadoRectangulo.evento = EVENTO_ACTUALIZAR_FIGURA;
			nuevoEstadoRectangulo.id     = rectangulo->getId();
			nuevoEstadoRectangulo.x      = cuadradoBody->GetPosition().x;
			nuevoEstadoRectangulo.y      =  Utilities::ALTO_PX  - cuadradoBody->GetPosition().y;
			nuevoEstadoRectangulo.rot    = cuadradoBody->GetAngle();
			nuevoEstadoRectangulo.alto   = rectFisico.getAltura();
			nuevoEstadoRectangulo.ancho  = rectFisico.getBase();
			
			nuevoEstadoRectangulo.color  = "#00F00F"; // notacion #rrggbb

			rectangulo->setEstado(nuevoEstadoRectangulo);
			*/
			// Obtengo el circulo
			CirculoVista* circulo = dynamic_cast<CirculoVista*>(escenarioVista.getObjeto(2));
			
			//nuevoEstadoCirculo.evento = EVENTO_ACTUALIZAR_FIGURA;
			nuevoEstadoCirculo.id     = circulo->getId();
			/*nuevoEstadoCirculo.x      = circulo->get_x();
			nuevoEstadoCirculo.y      = circulo->get_y();
			nuevoEstadoCirculo.rot    = circulo->get_rot() + Utilities::grados2radianes(5);
			nuevoEstadoCirculo.alto   = circulo->get_alto();
			nuevoEstadoCirculo.ancho  = circulo->get_ancho();
			*/
			nuevoEstadoCirculo.x      = circuloFisico1.getX();
			nuevoEstadoCirculo.y      = Utilities::ALTO_PX  - circuloFisico1.getY();
			nuevoEstadoCirculo.rot    = circuloFisico1.getAngulo();
			nuevoEstadoCirculo.alto   = circuloFisico1.getRadio()*2;
			nuevoEstadoCirculo.ancho  = circuloFisico1.getRadio()*2;
			

			nuevoEstadoCirculo.color  = "#FF0000"; // notacion #rrggbb

			circulo->setEstado(nuevoEstadoCirculo);

			// Obtengo el circulo
			CirculoVista* circulo2 = dynamic_cast<CirculoVista*>(escenarioVista.getObjeto(7));
			
			//nuevoEstadoCirculo.evento = EVENTO_ACTUALIZAR_FIGURA;
			nuevoEstadoCirculo.id     = circulo2->getId();
			nuevoEstadoCirculo.x      = circuloFisico2.getX();
			nuevoEstadoCirculo.y      = Utilities::ALTO_PX  - circuloFisico2.getY();
			nuevoEstadoCirculo.rot    = circuloFisico2.getAngulo();
			nuevoEstadoCirculo.alto   = circuloFisico2.getRadio()*2;
			nuevoEstadoCirculo.ancho  = circuloFisico2.getRadio()*2;


			nuevoEstadoCirculo.color  = "#FF0000"; // notacion #rrggbb

			circulo2->setEstado(nuevoEstadoCirculo);
			
			// Obtengo el rectangulo
			RectanguloVista* rectangulo1 = dynamic_cast<RectanguloVista*>(escenarioVista.getObjeto(3));

			
			//nuevoEstadoRectangulo.evento = EVENTO_ACTUALIZAR_FIGURA;
			nuevoEstadoRectangulo.id     = rectangulo1->getId();
			/*nuevoEstadoRectangulo.x      = rectangulo->get_x();
			nuevoEstadoRectangulo.y      = rectangulo->get_y();
			nuevoEstadoRectangulo.rot    = rectangulo->get_rot() + Utilities::grados2radianes(5);
			nuevoEstadoRectangulo.alto   = rectangulo->get_alto();
			nuevoEstadoRectangulo.ancho  = rectangulo->get_ancho();
			*/			
			nuevoEstadoRectangulo.x      = rectFisico.getX();
			nuevoEstadoRectangulo.y      =  Utilities::ALTO_PX  -rectFisico.getY();
			nuevoEstadoRectangulo.rot    = rectFisico.getAngulo();
			nuevoEstadoRectangulo.alto   = rectFisico.getAltura();
			nuevoEstadoRectangulo.ancho  = rectFisico.getBase();
			
			nuevoEstadoRectangulo.color  = "#00F00F"; // notacion #rrggbb

			rectangulo1->setEstado(nuevoEstadoRectangulo);


			// Obtengo el triangulo
			TrianguloVista* triangulo = dynamic_cast<TrianguloVista*>(escenarioVista.getObjeto(106));

			
			//nuevoEstadoTriangulo.evento = EVENTO_ACTUALIZAR_FIGURA;
			nuevoEstadoTriangulo.id     = triangulo->getId();
			nuevoEstadoTriangulo.x      = triangulo->get_x();
			nuevoEstadoTriangulo.y      = triangulo->get_y();
			nuevoEstadoTriangulo.rot    = triangulo->get_rot() - Utilities::grados2radianes(10);
			nuevoEstadoTriangulo.alto   = triangulo->get_alto();
			nuevoEstadoTriangulo.ancho  = triangulo->get_ancho();
			nuevoEstadoTriangulo.color  = "#00F00F"; // notacion #rrggbb

			triangulo->setEstado(nuevoEstadoTriangulo);


			// Obtengo el Pentagono
			PentagonoVista* pentagono = dynamic_cast<PentagonoVista*>(escenarioVista.getObjeto(80));

			
			//nuevoEstadoPentagono.evento = EVENTO_ACTUALIZAR_FIGURA;
			nuevoEstadoPentagono.id     = pentagono->getId();
			nuevoEstadoPentagono.x      = pentagono->get_x();
			nuevoEstadoPentagono.y      = pentagono->get_y();
			nuevoEstadoPentagono.rot    = pentagono->get_rot() - Utilities::grados2radianes(10);
			nuevoEstadoPentagono.alto   = pentagono->get_alto();
			nuevoEstadoPentagono.ancho  = pentagono->get_ancho();
			nuevoEstadoPentagono.color  = "#AA00AA"; // notacion #rrggbb

			pentagono->setEstado(nuevoEstadoPentagono);


			// Obtengo el Hexagono
			HexagonoVista* hexagono = dynamic_cast<HexagonoVista*>(escenarioVista.getObjeto(10));

			
			//nuevoEstadoHexagono.evento = EVENTO_ACTUALIZAR_FIGURA;
			nuevoEstadoHexagono.id     = hexagono->getId();
			nuevoEstadoHexagono.x      = hexagono->get_x();
			nuevoEstadoHexagono.y      = hexagono->get_y();
			nuevoEstadoHexagono.rot    = hexagono->get_rot() - Utilities::grados2radianes(10);

			/*nuevoEstadoHexagono.x      = hexagonoBody->GetPosition().x;
			nuevoEstadoHexagono.y      = Utilities::ALTO_PX - hexagonoBody->GetPosition().y;
			nuevoEstadoHexagono.rot    = hexagonoBody->GetAngle();
			*/
			nuevoEstadoHexagono.alto   = hexagono->get_alto();
			nuevoEstadoHexagono.ancho  = hexagono->get_ancho();
			nuevoEstadoHexagono.color  = "#666644"; // notacion #rrggbb

			hexagono->setEstado(nuevoEstadoHexagono);


			// Obtengo el Paralelogramo
			ParalelogramoVista* paralelogramo = dynamic_cast<ParalelogramoVista*>(escenarioVista.getObjeto(12));

			
			//nuevoEstadoParalelogramo.evento = EVENTO_ACTUALIZAR_FIGURA;
			nuevoEstadoParalelogramo.id     = paralelogramo->getId();
			/*
			nuevoEstadoParalelogramo.x      = paralelogramo->get_x();
			nuevoEstadoParalelogramo.y      = paralelogramo->get_y();
			nuevoEstadoParalelogramo.rot    = paralelogramo->get_rot() - Utilities::grados2radianes(7);
			nuevoEstadoParalelogramo.alto   = paralelogramo->get_alto();
			nuevoEstadoParalelogramo.ancho  = paralelogramo->get_ancho();
			nuevoEstadoParalelogramo.angulo_1  = paralelogramo->get_angulo1();
			*/
			nuevoEstadoParalelogramo.x      = paralelogramoFisico.getX();
			nuevoEstadoParalelogramo.y      =  Utilities::ALTO_PX  - paralelogramoFisico.getY();
			nuevoEstadoParalelogramo.rot    = paralelogramoFisico.getAngulo(); 
			nuevoEstadoParalelogramo.alto   = paralelogramoFisico.getAltura();
			nuevoEstadoParalelogramo.ancho  = paralelogramoFisico.getBase();
			nuevoEstadoParalelogramo.angulo_1 = paralelogramoFisico.getAnguloInclinacion() - Utilities::grados2radianes(7);
			nuevoEstadoParalelogramo.color  = "#666644"; // notacion #rrggbb

			paralelogramo->setEstado(nuevoEstadoParalelogramo);


			// Obtengo el Trapecio
			TrapecioVista* trapecio = dynamic_cast<TrapecioVista*>(escenarioVista.getObjeto(14));

			//nuevoEstadoTrapecio.evento = EVENTO_ACTUALIZAR_FIGURA;
			nuevoEstadoTrapecio.id     = trapecio->getId();
			nuevoEstadoTrapecio.x      = trapecio->get_x();
			nuevoEstadoTrapecio.y      = trapecio->get_y();
			nuevoEstadoTrapecio.rot    = trapecio->get_rot() - Utilities::grados2radianes(7);
			nuevoEstadoTrapecio.alto   = trapecio->get_alto();
			nuevoEstadoTrapecio.ancho  = trapecio->get_ancho();
			nuevoEstadoTrapecio.angulo_1  = trapecio->get_angulo1();
			nuevoEstadoTrapecio.angulo_2  = trapecio->get_angulo2();
			nuevoEstadoTrapecio.color  = "#666644"; // notacion #rrggbb

			trapecio->setEstado(nuevoEstadoTrapecio);


			escenarioVista.dibujar();
			Sleep(1);
		}

	}
	SDL_Quit(); 
}

TestMovimiento::~TestMovimiento(void)
{
}
