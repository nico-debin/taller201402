#include "stdafx.h"
#include <stdio.h>
#include <windows.h>

#include "TestFigurasVistaAndFisica2.h"

#include "Utilities.h"
#include "ControladorCliente.h"


#include "CirculoVista.h"
#include "TrianguloVista.h"
#include "RectanguloVista.h"
#include "PentagonoVista.h"
#include "HexagonoVista.h"
#include "ParalelogramoVista.h"
#include "TrapecioVista.h"

int TestFigurasVistaAndFisica2::id_actual = 1;



void TestFigurasVistaAndFisica2::test(){

	int id_actual = 2;

	b2World* mundo = TestFigurasVistaAndFisica2::iniciarMundoFisico();
	EscenarioVista* escenarioVista = TestFigurasVistaAndFisica2::iniciarEscenario();

	TestFigurasVistaAndFisica2::crearFiguras(escenarioVista, mundo);
	
	TestFigurasVistaAndFisica2::jugar(escenarioVista, mundo);
	delete mundo;
	delete escenarioVista;
}

b2World* TestFigurasVistaAndFisica2::iniciarMundoFisico(){
	b2Vec2 gravedad = b2Vec2(0.0f, -9.8f); 
	b2World* mundo = new b2World(gravedad);

	//a static floor to drop things on
	b2Vec2 v1(-1000.0f, 0.0f);
	b2Vec2 v2(1000.0f, 0.0f);
	b2EdgeShape edge;
	edge.Set(v1, v2);

	b2BodyDef pisoDef;
	pisoDef.position.Set(0.0f,0);
	pisoDef.type=b2_staticBody;
	b2Body* piso = mundo->CreateBody(&pisoDef);

	b2FixtureDef pisoFixDef;
	pisoFixDef.shape=&edge;
	pisoFixDef.restitution=0;
	piso->CreateFixture(&pisoFixDef);

	return mundo;
}

EscenarioVista* TestFigurasVistaAndFisica2::iniciarEscenario(){
	//Se utilizan variables globlales para estos datos del escenario
	Utilities::ANCHO_PX = 800;
	Utilities::ALTO_PX = 600;
	Utilities::ANCHO_UL = 80;
	Utilities::ALTO_UL = 60;
	Utilities::IMAGEN_FONDO_NIVEL1 = "./imagenes/fondo.01.800x600.png";
	
	return new EscenarioVista();
}

void TestFigurasVistaAndFisica2::jugar(EscenarioVista* escenarioVista, b2World* mundo){
	ControladorCliente controladorCliente(escenarioVista);

	float32 timeStep = 1.0f / 30.0f;
	int32 velocityIterations = 8;
	int32 positionIterations = 3;

	SDL_Event evento;
	bool quit = false;
	while (!quit){
		escenarioVista->dibujar();
		//system("pause");
		Sleep(1);
		if(SDL_PollEvent(&evento)){
			quit = controladorCliente.procesar(evento);
		}else{
			mundo->Step(timeStep, velocityIterations, positionIterations);				
			TestFigurasVistaAndFisica2::actualizarFiguras(escenarioVista,mundo);
			escenarioVista->dibujar();
		}
	} 
}

void TestFigurasVistaAndFisica2::crearFiguras(EscenarioVista* escenarioVista, b2World* mundo){
	
	/*********************** CIRCULO ****************************/
	//Inicializo el evento de creación del circulo

	float radio = 30;
	Circulo* circuloFisico = new Circulo(mundo, 385, 600, radio, "Rojo", 0, 10, false);

	sEvento eventoCreacionCirculo;
	eventoCreacionCirculo.id     = 2; // Este valor debe ser unico y definido por el servidor
	//eventoCreacionCirculo.evento = EVENTO_CREAR_CIRCULO;
	eventoCreacionCirculo.x      = circuloFisico->getX();
	eventoCreacionCirculo.y      = Utilities::euclid2sdl_ejeY( circuloFisico->getY() );
	eventoCreacionCirculo.alto   = circuloFisico->getRadio()*2;
	eventoCreacionCirculo.ancho   = circuloFisico->getRadio()*2;
	//eventoCreacionCirculo.x      = Utilities::unidadLogica2Pixel( circuloFisico->getX(), Utilities::ANCHO_PX, Utilities::ANCHO_UL);
	//eventoCreacionCirculo.y      = Utilities::euclid2sdl_ejeY( Utilities::unidadLogica2Pixel(circuloFisico->getY() , Utilities::ALTO_PX, Utilities::ALTO_UL));
	//eventoCreacionCirculo.alto   = Utilities::unidadLogica2Pixel(circuloFisico->getRadio()*2 , Utilities::ALTO_PX, Utilities::ALTO_UL);
	//eventoCreacionCirculo.ancho  = Utilities::unidadLogica2Pixel(circuloFisico->getRadio()*2, Utilities::ANCHO_PX, Utilities::ANCHO_UL);
	eventoCreacionCirculo.rot    = circuloFisico->getAngulo();
	eventoCreacionCirculo.color  = "#FF0000"; // notacion #rrggbb

	CirculoVista* circuloVista = new CirculoVista(eventoCreacionCirculo);

	//Agrego al escenarioVista el elemento con su id unico
	escenarioVista->addObjeto(circuloVista->getId(), circuloVista);
	escenarioVista->mapaObjetoFisicos[ circuloVista->getId() ]= circuloFisico;

	/*********************** RECTANGULO ****************************/
	//Inicializo el evento de creación de un rectangulo

	Rectangulo* rectanguloFisico = new Rectangulo(mundo, 400, 100, 300.0f,100.0f, "Rojo", 0, 1, true);

	sEvento eventoCreacionRectangulo;
	eventoCreacionRectangulo.id     = 3;// Este valor debe ser unico y definido por el servidor
	//eventoCreacionRectangulo.evento = EVENTO_CREAR_RECTANGULO;
	eventoCreacionRectangulo.x      = rectanguloFisico->getX();
	eventoCreacionRectangulo.y      = Utilities::euclid2sdl_ejeY( rectanguloFisico->getY() );
	eventoCreacionRectangulo.alto   = rectanguloFisico->getAltura();
	eventoCreacionRectangulo.ancho  = rectanguloFisico->getBase();

	//eventoCreacionRectangulo.x      = Utilities::unidadLogica2Pixel( rectanguloFisico->getX(), Utilities::ANCHO_PX, Utilities::ANCHO_UL);
	//eventoCreacionRectangulo.y      = Utilities::euclid2sdl_ejeY( Utilities::unidadLogica2Pixel(rectanguloFisico->getY() , Utilities::ALTO_PX, Utilities::ALTO_UL));
	//eventoCreacionRectangulo.alto   = Utilities::unidadLogica2Pixel(rectanguloFisico->getAltura() , Utilities::ALTO_PX, Utilities::ALTO_UL);
	//eventoCreacionRectangulo.ancho  = Utilities::unidadLogica2Pixel( rectanguloFisico->getBase(), Utilities::ANCHO_PX, Utilities::ANCHO_UL);
	
	eventoCreacionRectangulo.rot    = rectanguloFisico->getAngulo();
	eventoCreacionRectangulo.color  = "#00F00F"; // notacion #rrggbb
	

	RectanguloVista* rectanguloVista = new RectanguloVista(eventoCreacionRectangulo);

	escenarioVista->addObjeto(rectanguloVista->getId(), rectanguloVista);
	escenarioVista->mapaObjetoFisicos[ rectanguloVista->getId() ]= rectanguloFisico;
	

	/*********************** PARALELOGRAMO ****************************/
	//Inicializo el evento de creación de un paralelogramo
	sObjeto structParalelogramo;

	structParalelogramo.id	= 7;
	structParalelogramo.tipo= TIPO_PARALELOGRAMO;
	structParalelogramo.x	= 320.0;// + 5;
	structParalelogramo.y	= 500;

	structParalelogramo.ancho	= 80;
	structParalelogramo.alto	= 40;
	structParalelogramo.baseMayor = structParalelogramo.ancho;
	structParalelogramo.lados	= LADOS_PARALELOGRAMO;
	structParalelogramo.escala	= structParalelogramo.ancho/2;

	structParalelogramo.color	= "#FFAABB";
	structParalelogramo.rot		= Utilities::grados2radianes(10);
	structParalelogramo.angulo_1  = Utilities::grados2radianes(55);
	structParalelogramo.masa		= 10;
	structParalelogramo.estatico = false;

	Paralelogramo* paralelogramoFisico = new Paralelogramo(mundo,structParalelogramo);

	sEvento eventoCreacionParalelogramo;
	eventoCreacionParalelogramo.id     = structParalelogramo.id; // Este valor debe ser unico y definido por el servidor
	//eventoCreacionParalelogramo.evento = EVENTO_CREAR_PARALELOGRAMO;

	eventoCreacionParalelogramo.x      = structParalelogramo.x;
	eventoCreacionParalelogramo.y	   = Utilities::euclid2sdl_ejeY( structParalelogramo.y );
	eventoCreacionParalelogramo.alto   = structParalelogramo.alto;
	eventoCreacionParalelogramo.ancho  = structParalelogramo.ancho;

	//eventoCreacionParalelogramo.x      = Utilities::unidadLogica2Pixel( structParalelogramo.x, Utilities::ANCHO_PX, Utilities::ANCHO_UL); // Centro de la pantalla en pixeles
	//eventoCreacionParalelogramo.y      = Utilities::euclid2sdl_ejeY( Utilities::unidadLogica2Pixel(structParalelogramo.y , Utilities::ALTO_PX, Utilities::ALTO_UL)); // Centro de la pantalla en pixeles
	//eventoCreacionParalelogramo.alto   = Utilities::unidadLogica2Pixel( structParalelogramo.alto, Utilities::ALTO_PX, Utilities::ALTO_UL);
	//eventoCreacionParalelogramo.ancho  = Utilities::unidadLogica2Pixel( structParalelogramo.ancho, Utilities::ANCHO_PX, Utilities::ANCHO_UL);

	eventoCreacionParalelogramo.rot    = structParalelogramo.rot; // Utilizado para mover el punto que permite visualizar que gira
	eventoCreacionParalelogramo.color  = "#FFAABB"; // notacion #rrggbb
	eventoCreacionParalelogramo.angulo_1  = Utilities::grados2radianes(55);

	ParalelogramoVista* paralelogramoVista = new ParalelogramoVista(eventoCreacionParalelogramo);

	//Agrego al escenarioVista el elemento con su id unico
	escenarioVista->addObjeto(paralelogramoVista->getId(), paralelogramoVista);
	escenarioVista->mapaObjetoFisicos[ paralelogramoVista->getId() ] = paralelogramoFisico;
}

void TestFigurasVistaAndFisica2::actualizarFiguras(EscenarioVista* escenarioVista, b2World* mundo){

	// Obtengo el circulo
	CirculoVista* circulo = dynamic_cast<CirculoVista*>(escenarioVista->getObjeto(2));
	Circulo* circuloFisico = dynamic_cast<Circulo*>(escenarioVista->mapaObjetoFisicos[2]);


	sEvento nuevoEstadoCirculo;
	//nuevoEstadoCirculo.evento = EVENTO_ACTUALIZAR_FIGURA;
	nuevoEstadoCirculo.id     = circulo->getId();
	nuevoEstadoCirculo.x      = circuloFisico->getX();
	nuevoEstadoCirculo.y      = Utilities::euclid2sdl_ejeY( circuloFisico->getY());
	nuevoEstadoCirculo.rot    = circuloFisico->getAngulo();
	nuevoEstadoCirculo.alto   = circulo->get_alto();
	nuevoEstadoCirculo.ancho  = circulo->get_ancho();
	nuevoEstadoCirculo.color  = "#FF0000"; // notacion #rrggbb

	circulo->setEstado(nuevoEstadoCirculo);

	// Obtengo el rectangulo
	/*
	RectanguloVista* rectangulo = dynamic_cast<RectanguloVista*>(escenarioVista->getObjeto(3));
	Rectangulo* rectanguloFisico = dynamic_cast<Rectangulo*>(escenarioVista->mapaObjetoFisicos[3]);
	
	sEvento nuevoEstadoRectangulo;			
	nuevoEstadoRectangulo.evento = EVENTO_ACTUALIZAR_FIGURA;
	nuevoEstadoRectangulo.id     = rectangulo->getId();
	nuevoEstadoRectangulo.x      = rectanguloFisico->getX();
	nuevoEstadoRectangulo.y      = Utilities::euclid2sdl_ejeY(rectanguloFisico->getY());
	nuevoEstadoRectangulo.rot    = rectanguloFisico->getAngulo();
	nuevoEstadoRectangulo.alto   = rectangulo->get_alto();
	nuevoEstadoRectangulo.ancho  = rectangulo->get_ancho();	
	nuevoEstadoRectangulo.color  = "#00F00F"; // notacion #rrggbb
	*/

	// Obtengo el paralelogramo
	
	ParalelogramoVista* paralelogramo = dynamic_cast<ParalelogramoVista*>(escenarioVista->getObjeto(7));
	Paralelogramo* paralelogramoFisico = dynamic_cast<Paralelogramo*>(escenarioVista->mapaObjetoFisicos[7]);
	
	int debug_x_fisico_euclid = Utilities::unidadLogica2Pixel( paralelogramoFisico->getX(), Utilities::ANCHO_PX, Utilities::ANCHO_UL);
	int debug_y_fisico_euclid = Utilities::unidadLogica2Pixel(paralelogramoFisico->getY() , Utilities::ALTO_PX, Utilities::ALTO_UL);
	//printf("(%d,%d)\n",debug_x_fisico_euclid,debug_y_fisico_euclid);

	//b2Body* body = paralelogramoFisico->body;
	
	sEvento nuevoEstadoParalelogramo;			
	//nuevoEstadoParalelogramo.evento = EVENTO_ACTUALIZAR_FIGURA;
	nuevoEstadoParalelogramo.id     = paralelogramo->getId();			
	nuevoEstadoParalelogramo.x      = paralelogramoFisico->getX();
	nuevoEstadoParalelogramo.y      = Utilities::euclid2sdl_ejeY( paralelogramoFisico->getY() );//-380-(paralelogramo->get_alto()/2);
	nuevoEstadoParalelogramo.rot    = paralelogramoFisico->getRotacionInicial() + paralelogramoFisico->getAngulo();
	nuevoEstadoParalelogramo.alto   = paralelogramo->get_alto();
	nuevoEstadoParalelogramo.ancho  = paralelogramo->get_ancho();			
	nuevoEstadoParalelogramo.color  = "#FFAABB"; // notacion #rrggbb
	nuevoEstadoParalelogramo.angulo_1 = paralelogramo->get_angulo1();
	//system("pause");
	paralelogramo->setEstado(nuevoEstadoParalelogramo);
	
}