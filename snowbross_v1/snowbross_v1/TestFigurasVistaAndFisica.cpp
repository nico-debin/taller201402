#include "stdafx.h"
#include <stdio.h>
#include <windows.h>

#include "TestFigurasVistaAndFisica.h"

#include "Utilities.h"
#include "ControladorCliente.h"


#include "CirculoVista.h"
#include "TrianguloVista.h"
#include "RectanguloVista.h"
#include "PentagonoVista.h"
#include "HexagonoVista.h"
#include "ParalelogramoVista.h"
#include "TrapecioVista.h"

#include "SnowBoyVista.h"

int TestFigurasVistaAndFisica::id_actual = 1;



void TestFigurasVistaAndFisica::test(){

	int id_actual = 2;

	b2World* mundo = TestFigurasVistaAndFisica::iniciarMundoFisico();
	EscenarioVista* escenarioVista = TestFigurasVistaAndFisica::iniciarEscenario();

	TestFigurasVistaAndFisica::crearFiguras(escenarioVista, mundo);
	
	TestFigurasVistaAndFisica::jugar(escenarioVista, mundo);
	delete mundo;
	delete escenarioVista;
}

b2World* TestFigurasVistaAndFisica::iniciarMundoFisico(){
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

EscenarioVista* TestFigurasVistaAndFisica::iniciarEscenario(){
	//Se utilizan variables globlales para estos datos del escenario
	Utilities::ANCHO_PX = 800;
	Utilities::ALTO_PX = 600;
	Utilities::ANCHO_UL = 80;
	Utilities::ALTO_UL = 60;
	Utilities::IMAGEN_FONDO_NIVEL1 = "./imagenes/fondo.01.800x600.png";

	
	/*********************** ESCENARIO ****************************/
	//Inicializo el evento de creación del escenario
	/*
	sEvento eventoCreacionEscenario;
	eventoCreacionEscenario.id = 1; // Este valor debe ser unico y definido por el servidor
	eventoCreacionEscenario.evento = EVENTO_CREAR_ESCENARIO_1;
	eventoCreacionEscenario.x      = -1; // no se utiliza en el escenario vista
	eventoCreacionEscenario.y      = -1; // no se utiliza en el escenario vista
	eventoCreacionEscenario.rot    = -1; // no se utiliza en el escenario vista
	eventoCreacionEscenario.alto   = -1; // no se utiliza en el escenario vista
	eventoCreacionEscenario.ancho  = -1; // no se utiliza en el escenario vista
	eventoCreacionEscenario.color  = -1; // no se utiliza en el escenario vista
	*/
	
	return new EscenarioVista();
}

void TestFigurasVistaAndFisica::jugar(EscenarioVista* escenarioVista, b2World* mundo){
	ControladorCliente controladorCliente(escenarioVista);
	controladorCliente.personajeId = 100;

	float32 timeStep = 1.0f / 30.0f;
	int32 velocityIterations = 8;
	int32 positionIterations = 3;

	SDL_Event evento;
	bool quit = false;
	while (!quit){
		escenarioVista->dibujar();
		Sleep(1);
		//system("pause");
		mundo->Step(timeStep, velocityIterations, positionIterations);				
		TestFigurasVistaAndFisica::actualizarFiguras(escenarioVista,mundo);

		if(SDL_PollEvent(&evento)){
			quit = controladorCliente.procesar(evento);
		}else{
			escenarioVista->dibujar();
			//system("pause");
		}
	} 
}

void TestFigurasVistaAndFisica::crearFiguras(EscenarioVista* escenarioVista, b2World* mundo){
	
	sEvento eventoPersonaje;
	eventoPersonaje.id     = 100; // Este valor debe ser unico y definido por el servidor
	eventoPersonaje.evento = 0;
	eventoPersonaje.x      = 400; // Centro de la pantalla en pixeles
	eventoPersonaje.y      = 300; // Centro de la pantalla en pixeles
	eventoPersonaje.rot    = 0; // Utilizado para mover el punto que permite visualizar que gira
	eventoPersonaje.alto   = 60; 
	eventoPersonaje.ancho  = 60; 
	eventoPersonaje.color  = "#FF0000"; // notacion #rrggbb
	
	SnowBoyVista* snowBoy = new SnowBoyVista(eventoPersonaje);


	//Agrego al escenarioVista el elemento con su id unico
	escenarioVista->addObjeto(snowBoy->getId(),snowBoy);

	/*********************** CIRCULO ****************************/
	//Inicializo el evento de creación del circulo

	float radio = 3;
	Circulo* circuloFisico = new Circulo(mundo, 48.5, 60, radio, "Rojo", 0, 10, false);

	sEvento eventoCreacionCirculo;
	eventoCreacionCirculo.id     = 2; // Este valor debe ser unico y definido por el servidor
	//eventoCreacionCirculo.evento = EVENTO_CREAR_CIRCULO;
	eventoCreacionCirculo.x      = Utilities::unidadLogica2Pixel( circuloFisico->getX(), Utilities::ANCHO_PX, Utilities::ANCHO_UL);
	eventoCreacionCirculo.y      = Utilities::euclid2sdl_ejeY( Utilities::unidadLogica2Pixel(circuloFisico->getY() , Utilities::ALTO_PX, Utilities::ALTO_UL));
	eventoCreacionCirculo.rot    = circuloFisico->getAngulo();
	eventoCreacionCirculo.alto   = Utilities::unidadLogica2Pixel(circuloFisico->getRadio()*2 , Utilities::ALTO_PX, Utilities::ALTO_UL);
	eventoCreacionCirculo.ancho  = Utilities::unidadLogica2Pixel(circuloFisico->getRadio()*2, Utilities::ANCHO_PX, Utilities::ANCHO_UL);
	eventoCreacionCirculo.color  = "#FF0000"; // notacion #rrggbb

	CirculoVista* circuloVista = new CirculoVista(eventoCreacionCirculo);

	//Agrego al escenarioVista el elemento con su id unico
	escenarioVista->addObjeto(circuloVista->getId(), circuloVista);
	escenarioVista->mapaObjetoFisicos[ circuloVista->getId() ]= circuloFisico;

	/*********************** RECTANGULO ****************************/
	//Inicializo el evento de creación de un rectangulo
	
	Rectangulo* rectanguloFisico = new Rectangulo(mundo, 40, 10, 30.0f,10.0f, "Rojo", -15, 1, true);

	sEvento eventoCreacionRectangulo;
	eventoCreacionRectangulo.id     = 3;// Este valor debe ser unico y definido por el servidor
	//eventoCreacionRectangulo.evento = EVENTO_CREAR_RECTANGULO;
	eventoCreacionRectangulo.x      = Utilities::unidadLogica2Pixel( rectanguloFisico->getX(), Utilities::ANCHO_PX, Utilities::ANCHO_UL);
	eventoCreacionRectangulo.y      = Utilities::euclid2sdl_ejeY( Utilities::unidadLogica2Pixel(rectanguloFisico->getY() , Utilities::ALTO_PX, Utilities::ALTO_UL));
	eventoCreacionRectangulo.rot    = rectanguloFisico->getAngulo();
	eventoCreacionRectangulo.alto   = Utilities::unidadLogica2Pixel(rectanguloFisico->getAltura() , Utilities::ALTO_PX, Utilities::ALTO_UL);
	eventoCreacionRectangulo.ancho  = Utilities::unidadLogica2Pixel( rectanguloFisico->getBase(), Utilities::ANCHO_PX, Utilities::ANCHO_UL);
	eventoCreacionRectangulo.color  = "#00F00F"; // notacion #rrggbb
	

	RectanguloVista* rectanguloVista = new RectanguloVista(eventoCreacionRectangulo);

	escenarioVista->addObjeto(rectanguloVista->getId(), rectanguloVista);
	escenarioVista->mapaObjetoFisicos[ rectanguloVista->getId() ]= rectanguloFisico;
	
	/*********************** TRIANGULO ****************************/
	//Inicializo el evento de creación de un rectangulo
	sObjeto datosTriangulo;
	datosTriangulo.id = 4;
	datosTriangulo.alto = 8;
	datosTriangulo.ancho = 8;
	datosTriangulo.color = "#00F00F";
	datosTriangulo.escala =datosTriangulo.ancho/2;
	datosTriangulo.estatico = false;
	datosTriangulo.lados = 3;
	datosTriangulo.masa = 10;
	datosTriangulo.rot = 0;
	datosTriangulo.tipo = TIPO_TRIANGULO;
	datosTriangulo.x = 62;
	datosTriangulo.y = 50;
		
	PoligonoRegular* trianguloFisico = new PoligonoRegular(mundo, datosTriangulo);

	sEvento eventoCreacionTriangulo;
	eventoCreacionTriangulo.id     = datosTriangulo.id;
	//eventoCreacionTriangulo.evento = EVENTO_CREAR_TRIANGULO;
	eventoCreacionTriangulo.x      = Utilities::unidadLogica2Pixel( trianguloFisico->getX(), Utilities::ANCHO_PX, Utilities::ANCHO_UL);
	eventoCreacionTriangulo.y      = Utilities::euclid2sdl_ejeY( Utilities::unidadLogica2Pixel(trianguloFisico->getY() , Utilities::ALTO_PX, Utilities::ALTO_UL));
	eventoCreacionTriangulo.rot    = trianguloFisico->getAngulo();
	eventoCreacionTriangulo.alto   = Utilities::unidadLogica2Pixel( datosTriangulo.alto , Utilities::ALTO_PX, Utilities::ALTO_UL);
	eventoCreacionTriangulo.ancho  = Utilities::unidadLogica2Pixel( datosTriangulo.ancho, Utilities::ANCHO_PX, Utilities::ANCHO_UL);
	eventoCreacionTriangulo.color  = datosTriangulo.color;

	TrianguloVista* trianguloVista = new TrianguloVista(eventoCreacionTriangulo);

	escenarioVista->addObjeto(trianguloVista->getId(), trianguloVista);
	escenarioVista->mapaObjetoFisicos[ trianguloVista->getId() ]= trianguloFisico;

	/*********************  PENTAGONO ***************************/
	//Inicializo el evento de creación de un pentagono
	sObjeto structPentagono;

	structPentagono.id	= 5;
	structPentagono.tipo	= TIPO_PENTAGONO;
	structPentagono.x	= 60;
	structPentagono.y	= 40;

	structPentagono.ancho	= 8;
	structPentagono.alto	= 8;
	structPentagono.lados	= LADOS_PENTAGONO;
	structPentagono.escala	= structPentagono.ancho/2;

	structPentagono.color	= "#AA00AA";
	structPentagono.rot	= Utilities::grados2radianes(0);
	structPentagono.masa	= 10;
	structPentagono.estatico = false;

	PoligonoRegular* pentagonoFisico = new PoligonoRegular(mundo,structPentagono);

	sEvento eventoCreacionPentagono;
	eventoCreacionPentagono.id = structPentagono.id;// Este valor debe ser unico y definido por el servidor
	//eventoCreacionPentagono.evento = EVENTO_CREAR_PENTAGONO;
	eventoCreacionPentagono.x      = Utilities::unidadLogica2Pixel( pentagonoFisico->getX(), Utilities::ANCHO_PX, Utilities::ANCHO_UL);
	eventoCreacionPentagono.y      = Utilities::euclid2sdl_ejeY( Utilities::unidadLogica2Pixel(pentagonoFisico->getY() , Utilities::ALTO_PX, Utilities::ALTO_UL));
	eventoCreacionPentagono.rot	   = pentagonoFisico->getAngulo();
	eventoCreacionPentagono.alto   = Utilities::unidadLogica2Pixel( structPentagono.alto , Utilities::ALTO_PX, Utilities::ALTO_UL);
	eventoCreacionPentagono.ancho  = Utilities::unidadLogica2Pixel( structPentagono.ancho, Utilities::ANCHO_PX, Utilities::ANCHO_UL);
	eventoCreacionPentagono.color = "#AA00AA"; // notacion #rrggbb

	PentagonoVista* pentagonoVista = new PentagonoVista(eventoCreacionPentagono);

	escenarioVista->addObjeto(pentagonoVista->getId(), pentagonoVista);
	escenarioVista->mapaObjetoFisicos[ pentagonoVista->getId() ]= pentagonoFisico;


	/*********************** HEXAGONO ****************************/
	//Inicializo el evento de creación de un pentagono
	sObjeto structHexagono;

	structHexagono.id	= 6;
	structHexagono.tipo	= TIPO_HEXAGONO;
	structHexagono.x	= 15;
	structHexagono.y	= 30;

	structHexagono.ancho	= 4;
	structHexagono.alto		= 4;
	structHexagono.lados	= LADOS_HEXAGONO;
	structHexagono.escala	= structHexagono.ancho/2;

	structHexagono.color	= "#666644";
	structHexagono.rot		= 5;
	structHexagono.masa		= 10;
	structHexagono.estatico = false;

	PoligonoRegular* hexagonoFisico = new PoligonoRegular(mundo,structHexagono);

	sEvento eventoCreacionHexagono;
	eventoCreacionHexagono.id		= structHexagono.id;// Este valor debe ser unico y definido por el servidor
	//eventoCreacionHexagono.evento	= EVENTO_CREAR_HEXAGONO;
	eventoCreacionHexagono.x      = Utilities::unidadLogica2Pixel( hexagonoFisico->getX(), Utilities::ANCHO_PX, Utilities::ANCHO_UL);
	eventoCreacionHexagono.y      = Utilities::euclid2sdl_ejeY( Utilities::unidadLogica2Pixel(hexagonoFisico->getY() , Utilities::ALTO_PX, Utilities::ALTO_UL));
	eventoCreacionHexagono.rot	  = hexagonoFisico->getAngulo();
	eventoCreacionHexagono.alto   = Utilities::unidadLogica2Pixel( structHexagono.alto , Utilities::ALTO_PX, Utilities::ALTO_UL);
	eventoCreacionHexagono.ancho  = Utilities::unidadLogica2Pixel( structHexagono.ancho, Utilities::ANCHO_PX, Utilities::ANCHO_UL);
	eventoCreacionHexagono.color	= "#666644"; // notacion #rrggbb

	HexagonoVista* hexagonoVista = new HexagonoVista(eventoCreacionHexagono);

	escenarioVista->addObjeto(hexagonoVista->getId(), hexagonoVista);
	escenarioVista->mapaObjetoFisicos[ hexagonoVista->getId() ] = hexagonoFisico;
	

	/*********************** PARALELOGRAMO ****************************/
	//Inicializo el evento de creación de un paralelogramo
	
	sObjeto structParalelogramo;

	structParalelogramo.id	= 7;
	structParalelogramo.tipo= TIPO_PARALELOGRAMO;
	structParalelogramo.x	= 32.0; //+ 5;
	structParalelogramo.y	= 50;

	structParalelogramo.ancho	= 8;
	structParalelogramo.alto	= 4;
	//structParalelogramo.baseMayor = structParalelogramo.ancho;
	//structParalelogramo.baseMenor = structParalelogramo.ancho;
	structParalelogramo.lados	= LADOS_PARALELOGRAMO;
	structParalelogramo.escala	= structParalelogramo.ancho/2;

	structParalelogramo.color	= "#FFAABB";
	structParalelogramo.rot		= Utilities::grados2radianes(40);
	structParalelogramo.angulo_1  = Utilities::grados2radianes(55);
	structParalelogramo.masa		= 10;
	structParalelogramo.estatico = false;

	Paralelogramo* paralelogramoFisico = new Paralelogramo(mundo,structParalelogramo);

	sEvento eventoCreacionParalelogramo;
	eventoCreacionParalelogramo.id     = structParalelogramo.id; // Este valor debe ser unico y definido por el servidor
	//eventoCreacionParalelogramo.evento = EVENTO_CREAR_PARALELOGRAMO;

	eventoCreacionParalelogramo.x      = Utilities::unidadLogica2Pixel( structParalelogramo.x, Utilities::ANCHO_PX, Utilities::ANCHO_UL); // Centro de la pantalla en pixeles
	eventoCreacionParalelogramo.y      = Utilities::euclid2sdl_ejeY( Utilities::unidadLogica2Pixel(structParalelogramo.y , Utilities::ALTO_PX, Utilities::ALTO_UL)); // Centro de la pantalla en pixeles
	eventoCreacionParalelogramo.alto   = Utilities::unidadLogica2Pixel( structParalelogramo.alto, Utilities::ALTO_PX, Utilities::ALTO_UL);
	eventoCreacionParalelogramo.ancho  = Utilities::unidadLogica2Pixel( structParalelogramo.ancho, Utilities::ANCHO_PX, Utilities::ANCHO_UL);
	eventoCreacionParalelogramo.rot    = structParalelogramo.rot; // Utilizado para mover el punto que permite visualizar que gira
	eventoCreacionParalelogramo.color  = "#FFAABB"; // notacion #rrggbb
	eventoCreacionParalelogramo.angulo_1  = Utilities::grados2radianes(55);

	ParalelogramoVista* paralelogramoVista = new ParalelogramoVista(eventoCreacionParalelogramo);

	//Agrego al escenarioVista el elemento con su id unico
	escenarioVista->addObjeto(paralelogramoVista->getId(), paralelogramoVista);
	escenarioVista->mapaObjetoFisicos[ paralelogramoVista->getId() ] = paralelogramoFisico;
	

	/*********************** TRAPECIO ****************************/
	//Inicializo el evento de creación de un Trapecio
	/*
	sObjeto structTrapecio;

	structTrapecio.id	= 8;
	structTrapecio.tipo= TIPO_TRAPECIO;
	structTrapecio.x	= 65;
	structTrapecio.y	= 45;

	structTrapecio.ancho	= 8;
	structTrapecio.alto	= 4;
	structTrapecio.baseMayor = structParalelogramo.ancho;
	structTrapecio.lados	= LADOS_PARALELOGRAMO;
	structTrapecio.escala	= structParalelogramo.ancho/2;

	structTrapecio.color	= "#FFAABB";
	structTrapecio.rot		= Utilities::grados2radianes(0);
	structTrapecio.angulo_1  = Utilities::grados2radianes(55);
	structTrapecio.angulo_2  = Utilities::grados2radianes(45);
	structTrapecio.masa		= 10;
	structTrapecio.estatico = false;

	Trapecio* trapecioFisico = new Trapecio(mundo,structTrapecio);

	sEvento eventoCreacionTrapecio;
	eventoCreacionTrapecio.id     = structTrapecio.id; // Este valor debe ser unico y definido por el servidor
	eventoCreacionTrapecio.evento = EVENTO_CREAR_TRAPECIO;

	eventoCreacionTrapecio.x      = Utilities::unidadLogica2Pixel( structTrapecio.x, Utilities::ANCHO_PX, Utilities::ANCHO_UL); // Centro de la pantalla en pixeles
	eventoCreacionTrapecio.y      = Utilities::euclid2sdl_ejeY( Utilities::unidadLogica2Pixel(structTrapecio.y , Utilities::ALTO_PX, Utilities::ALTO_UL)); // Centro de la pantalla en pixeles
	eventoCreacionTrapecio.alto   = Utilities::unidadLogica2Pixel( structTrapecio.alto, Utilities::ALTO_PX, Utilities::ALTO_UL);
	eventoCreacionTrapecio.ancho  = Utilities::unidadLogica2Pixel( structTrapecio.ancho, Utilities::ANCHO_PX, Utilities::ANCHO_UL);
	eventoCreacionTrapecio.rot    = structTrapecio.rot; // Utilizado para mover el punto que permite visualizar que gira
	eventoCreacionTrapecio.color  = "#FFAABB"; // notacion #rrggbb
	eventoCreacionTrapecio.angulo_1  = Utilities::grados2radianes(55);

	TrapecioVista* trapecioVista = new TrapecioVista(eventoCreacionTrapecio);

	//Agrego al escenarioVista el elemento con su id unico
	escenarioVista->addObjeto(trapecioVista->getId(), trapecioVista);
	escenarioVista->mapaObjetoFisicos[ trapecioVista->getId() ] = trapecioFisico;
	*/
}

void TestFigurasVistaAndFisica::actualizarFiguras(EscenarioVista* escenarioVista, b2World* mundo){

	// Obtengo el circulo
	CirculoVista* circulo = dynamic_cast<CirculoVista*>(escenarioVista->getObjeto(2));
	Circulo* circuloFisico = dynamic_cast<Circulo*>(escenarioVista->mapaObjetoFisicos[2]);


	sEvento nuevoEstadoCirculo;
	//nuevoEstadoCirculo.evento = EVENTO_ACTUALIZAR_FIGURA;
	nuevoEstadoCirculo.id     = circulo->getId();
	nuevoEstadoCirculo.x      = Utilities::unidadLogica2Pixel( circuloFisico->getX(), Utilities::ANCHO_PX, Utilities::ANCHO_UL);
	nuevoEstadoCirculo.y      = Utilities::euclid2sdl_ejeY( Utilities::unidadLogica2Pixel(circuloFisico->getY() , Utilities::ALTO_PX, Utilities::ALTO_UL));
	nuevoEstadoCirculo.rot    = circuloFisico->getAngulo();
	nuevoEstadoCirculo.alto   = circulo->get_alto();
	nuevoEstadoCirculo.ancho  = circulo->get_ancho();
	nuevoEstadoCirculo.color  = "#FF0000"; // notacion #rrggbb

	circulo->setEstado(nuevoEstadoCirculo);

	// Obtengo el rectangulo
	
	RectanguloVista* rectangulo = dynamic_cast<RectanguloVista*>(escenarioVista->getObjeto(3));
	Rectangulo* rectanguloFisico = dynamic_cast<Rectangulo*>(escenarioVista->mapaObjetoFisicos[3]);
	
	sEvento nuevoEstadoRectangulo;			
	//nuevoEstadoRectangulo.evento = EVENTO_ACTUALIZAR_FIGURA;
	nuevoEstadoRectangulo.id     = rectangulo->getId();
	nuevoEstadoRectangulo.x      = Utilities::unidadLogica2Pixel( rectanguloFisico->getX(), Utilities::ANCHO_PX, Utilities::ANCHO_UL);
	nuevoEstadoRectangulo.y      = Utilities::euclid2sdl_ejeY( Utilities::unidadLogica2Pixel(rectanguloFisico->getY() , Utilities::ALTO_PX, Utilities::ALTO_UL));
	nuevoEstadoRectangulo.rot    = rectanguloFisico->getAngulo();
	nuevoEstadoRectangulo.alto   = rectangulo->get_alto();
	nuevoEstadoRectangulo.ancho  = rectangulo->get_ancho();	
	nuevoEstadoRectangulo.color  = "#00F00F"; // notacion #rrggbb

	//rectangulo->setEstado(nuevoEstadoRectangulo);
	

	// Obtengo el triangulo
	TrianguloVista* triangulo = dynamic_cast<TrianguloVista*>(escenarioVista->getObjeto(4));
	PoligonoRegular* trianguloFisico = dynamic_cast<PoligonoRegular*>(escenarioVista->mapaObjetoFisicos[4]);

	sEvento nuevoEstadoTriangulo;		
	//nuevoEstadoTriangulo.evento = EVENTO_ACTUALIZAR_FIGURA;
	nuevoEstadoTriangulo.id     = triangulo->getId();
	nuevoEstadoTriangulo.x      = Utilities::unidadLogica2Pixel( trianguloFisico->getX(), Utilities::ANCHO_PX, Utilities::ANCHO_UL);
	nuevoEstadoTriangulo.y      = Utilities::euclid2sdl_ejeY( Utilities::unidadLogica2Pixel(trianguloFisico->getY() , Utilities::ALTO_PX, Utilities::ALTO_UL));
	nuevoEstadoTriangulo.rot    = trianguloFisico->getAngulo();// + Utilities::grados2radianes(270);
	nuevoEstadoTriangulo.alto   = triangulo->get_alto();
	nuevoEstadoTriangulo.ancho  = triangulo->get_ancho();
	nuevoEstadoTriangulo.color  = "#00F00F"; // notacion #rrggbb

	triangulo->setEstado(nuevoEstadoTriangulo);

	// Obtengo el pentagono
	PentagonoVista* pentagono = dynamic_cast<PentagonoVista*>(escenarioVista->getObjeto(5));
	PoligonoRegular* pentagonoFisico = dynamic_cast<PoligonoRegular*>(escenarioVista->mapaObjetoFisicos[5]);
	
	sEvento nuevoEstadoPentagono;			
	//nuevoEstadoPentagono.evento = EVENTO_ACTUALIZAR_FIGURA;
	nuevoEstadoPentagono.id     = pentagono->getId();
	nuevoEstadoPentagono.x      = Utilities::unidadLogica2Pixel( pentagonoFisico->getX(), Utilities::ANCHO_PX, Utilities::ANCHO_UL);
	nuevoEstadoPentagono.y      = Utilities::euclid2sdl_ejeY( Utilities::unidadLogica2Pixel(pentagonoFisico->getY() , Utilities::ALTO_PX, Utilities::ALTO_UL));
	nuevoEstadoPentagono.rot    = pentagonoFisico->getAngulo();
	nuevoEstadoPentagono.alto   = pentagono->get_alto();
	nuevoEstadoPentagono.ancho  = pentagono->get_ancho();			
	nuevoEstadoPentagono.color  = "#AA00AA"; // notacion #rrggbb

	pentagono->setEstado(nuevoEstadoPentagono);

	// Obtengo el hexagono
	HexagonoVista* hexagono = dynamic_cast<HexagonoVista*>(escenarioVista->getObjeto(6));
	PoligonoRegular* hexagonoFisico = dynamic_cast<PoligonoRegular*>(escenarioVista->mapaObjetoFisicos[6]);
	
	sEvento nuevoEstadoHexagono;			
	//nuevoEstadoHexagono.evento = EVENTO_ACTUALIZAR_FIGURA;
	nuevoEstadoHexagono.id     = hexagono->getId();
	nuevoEstadoHexagono.x      = Utilities::unidadLogica2Pixel( hexagonoFisico->getX(), Utilities::ANCHO_PX, Utilities::ANCHO_UL);
	nuevoEstadoHexagono.y      = Utilities::euclid2sdl_ejeY( Utilities::unidadLogica2Pixel(hexagonoFisico->getY() , Utilities::ALTO_PX, Utilities::ALTO_UL));
	nuevoEstadoHexagono.rot    = hexagonoFisico->getAngulo();
	nuevoEstadoHexagono.alto   = hexagono->get_alto();
	nuevoEstadoHexagono.ancho  = hexagono->get_ancho();			
	nuevoEstadoHexagono.color  = "#666644"; // notacion #rrggbb

	hexagono->setEstado(nuevoEstadoHexagono);
	

	// Obtengo el paralelogramo
	
	ParalelogramoVista* paralelogramo = dynamic_cast<ParalelogramoVista*>(escenarioVista->getObjeto(7));
	Paralelogramo* paralelogramoFisico = dynamic_cast<Paralelogramo*>(escenarioVista->mapaObjetoFisicos[7]);
	
	//int debug_x_fisico_euclid = Utilities::unidadLogica2Pixel( paralelogramoFisico->getX(), Utilities::ANCHO_PX, Utilities::ANCHO_UL);
	//int debug_y_fisico_euclid = Utilities::unidadLogica2Pixel(paralelogramoFisico->getY() , Utilities::ALTO_PX, Utilities::ALTO_UL);
	//printf("(%d,%d)\n",debug_x_fisico_euclid,debug_y_fisico_euclid);

	//b2Body* body = paralelogramoFisico->body;
	/*
	sEvento nuevoEstadoParalelogramo;			
	nuevoEstadoParalelogramo.evento = EVENTO_ACTUALIZAR_FIGURA;
	nuevoEstadoParalelogramo.id     = paralelogramo->getId();		
	nuevoEstadoParalelogramo.x      = Utilities::unidadLogica2Pixel( paralelogramoFisico->getX(), Utilities::ANCHO_PX, Utilities::ANCHO_UL);
	nuevoEstadoParalelogramo.y      = Utilities::euclid2sdl_ejeY( Utilities::unidadLogica2Pixel(paralelogramoFisico->getY() , Utilities::ALTO_PX, Utilities::ALTO_UL));//-380-(paralelogramo->get_alto()/2);
	nuevoEstadoParalelogramo.rot    = paralelogramoFisico->getRotacionInicial() + paralelogramoFisico->getAngulo();
	nuevoEstadoParalelogramo.alto   = paralelogramo->get_alto();
	nuevoEstadoParalelogramo.ancho  = paralelogramo->get_ancho();			
	nuevoEstadoParalelogramo.color  = "#FFAABB"; // notacion #rrggbb
	nuevoEstadoParalelogramo.angulo_1 = paralelogramo->get_angulo1();
	//system("pause");
	paralelogramo->setEstado(nuevoEstadoParalelogramo);
	*/


	
	float vertices_x[4], vertices_y[4];
	paralelogramoFisico->getVertices(vertices_x,vertices_y);
	paralelogramo->setVertices(vertices_x,vertices_y);

	// La posicion es solo para poder dibujar su centro geometrico, sino no hace falta y se puede omitir el calculo.
	paralelogramo->setPosition(Utilities::unidadLogica2Pixel( paralelogramoFisico->getX(), Utilities::ANCHO_PX, Utilities::ANCHO_UL), Utilities::euclid2sdl_ejeY( Utilities::unidadLogica2Pixel(paralelogramoFisico->getY() , Utilities::ALTO_PX, Utilities::ALTO_UL)));
	
}