#include "StdAfx.h"
#include "ParalelogramoVista.h"

ParalelogramoVista::ParalelogramoVista(sEvento estadoEvento) : Vista(estadoEvento){
	this->tipo = PARALELOGRAMO;
	this->lados = LADOS_PARALELOGRAMO;

	//  Se comenta calculo por error graficando figura
	//this->calcularVertices();
	
	// Se envian vertices para el dibujo particular del paralelogramo
	for( short i = 0; i < 4 ; i++ ){
		this->vertices_x[i] = estadoEvento.verticesX[i];
		this->vertices_y[i] = estadoEvento .verticesY[i];
	}

	Logger::getLogger()->debug( "Paralelogramo Vista creado" );
}

void ParalelogramoVista::calcularVertices(){

	//// Un paralelogramo se define por su ancho, su alto y su angulo interno

	///* Ubicacion de los vertices:
	// V1 ______ V2 
	//   /      / 
 //     /______/
	// V4      V3
	//*/
	//float aux_vertices_x[4];
	//float aux_vertices_y[4];
	//float base = this->get_ancho();
	//float altura = this->get_alto();
	//float angulo_inf_izq = this->angulo_1;
	//float rotacion = this->get_rot();
	//int x = Utilities::redondeo(this->get_x());
	//int y = Utilities::redondeo(this->get_y());

	//float base_triangulo = altura / tan(angulo_inf_izq);

	//// V1
	//aux_vertices_x[0] = (-1) * ( (base - base_triangulo)/2 );
	//aux_vertices_y[0] = (-1) * altura/2;

	//// V2
	//aux_vertices_x[1] = aux_vertices_x[0] + base;
	//aux_vertices_y[1] = aux_vertices_y[0];

	//// V3
	//aux_vertices_x[2] = aux_vertices_x[1] - base_triangulo;
	//aux_vertices_y[2] = (-1) * aux_vertices_y[1];

	//// V4
	//aux_vertices_x[3] = aux_vertices_x[2] - base;
	//aux_vertices_y[3] = aux_vertices_y[2];

	//for(short i = 0 ; i<this->lados ; i++){
	//	this->vertices_x[i] = Utilities::redondeo(aux_vertices_x[i]);
	//	this->vertices_y[i] = Utilities::redondeo(aux_vertices_y[i]);
	//}

	////cout << "Vista: " << endl;
	////for(int i = 0; i<4; i++) printf("(%d,%d)\n", this->vertices_x[i], this->vertices_y[i]);

	//Utilities::rotarVertices( rotacion , this->vertices_x, this->vertices_y, this->lados, 0, 0);

	////cout << "rotacion: " << rotacion << endl;
	////cout << endl;
	////for(int i = 0; i<4; i++) printf("(%d,%d)\n", this->vertices_x[i], this->vertices_y[i]);

	//for(short i = 0 ; i<this->lados ; i++){
	//	this->vertices_x[i] += x;
	//	this->vertices_y[i] += y;
	//}
	//

	////cout << endl;
	////for(int i = 0; i<4; i++) printf("(%d,%d)\n", this->vertices_x[i], this->vertices_y[i]);

	///*
	//float base_triangulo = this->get_alto() / tan(this->angulo_1);

	//// V1
	//this->vertices_x[0] = this->get_x() - (this->get_ancho() - base_triangulo)/2;
	//this->vertices_y[0] = this->get_y() - this->get_alto()/2;

	//// V2
	//this->vertices_x[1] = this->vertices_x[0] + this->get_ancho();
	//this->vertices_y[1] = this->get_y() - this->get_alto()/2;

	//// V3
	//this->vertices_x[2] = this->vertices_x[1] - base_triangulo;
	//this->vertices_y[2] = this->get_y() + this->get_alto()/2;

	//// V4
	//this->vertices_x[3] = this->vertices_x[2] - this->get_ancho();
	//this->vertices_y[3] = this->get_y() + this->get_alto()/2;

	//Utilities::rotarVertices( this->get_rot() , this->vertices_x, this->vertices_y, this->lados, this->get_x(), this->get_y());
	//*/
	////cout << "Rot Vista: "  << this->get_rot() << endl;
}

void ParalelogramoVista::setEstado( sEvento estadoEvento ){
	Vista::setEstado( estadoEvento );
	//this->calcularVertices();

	// Se envian vertices para el dibujo particular del paralelogramo
	for( short i = 0; i < 4 ; i++ ){
		this->vertices_x[i] = estadoEvento.verticesX[i];
		this->vertices_y[i] = estadoEvento .verticesY[i];
	}
}


// Setea vertices con unidades logicas
void ParalelogramoVista::setVertices(float* x, float* y){
	/*for (int i = 0; i < this->lados; i++){
		this->vertices_x[i] = Utilities::unidadLogica2Pixel(x[i], Utilities::ANCHO_PX, Utilities::ANCHO_UL);
		this->vertices_y[i] = Utilities::euclid2sdl_ejeY( Utilities::unidadLogica2Pixel(y[i], Utilities::ALTO_PX, Utilities::ALTO_UL) );
	}*/
}

void ParalelogramoVista::dibujar(SDL_Renderer* renderer){



	if( this->texturaObjeto == nullptr ){
		this->texturaObjeto = SDLController::loadTexture("imagenes/TexturaParalelogramo.png",renderer);
	}

	SDL_Rect renderQuad;

	renderQuad.x =  this->get_x() - this->get_ancho()/2;
	renderQuad.y =  this->get_y() - this->get_alto()/2;

	renderQuad.w = this->get_ancho();
	renderQuad.h = this->get_alto();
	
	//printf("radianes: %.3f, grados: %.3f    ",this->get_rot(),-Utilities::radianes2grados(this->get_rot()));


	SDL_RenderCopyEx( renderer, this->texturaObjeto, NULL, &renderQuad, -Utilities::radianes2grados(this->get_rot()), NULL,SDL_FLIP_NONE);

	if(Utilities::debugDraw){
		polygonRGBA(renderer,this->vertices_x,this->vertices_y,this->lados,this->get_color_r(),this->get_color_g(),this->get_color_b(),255);
		filledPolygonRGBA(renderer,this->vertices_x,this->vertices_y,this->lados,this->get_color_r(),this->get_color_g(),this->get_color_b(),255);
	}



	// Dibuja centro x,y
	this->dibujar_centro(renderer);
}

void ParalelogramoVista::dibujarTextura(SDL_Renderer* renderer){}