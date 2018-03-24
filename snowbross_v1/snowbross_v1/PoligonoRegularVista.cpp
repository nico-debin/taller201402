#include "StdAfx.h"
#include "PoligonoRegularVista.h"


PoligonoRegularVista::PoligonoRegularVista(sEvento estadoEvento) : Vista(estadoEvento){}


void PoligonoRegularVista::calcularVertices(){
/*		
	// Froma de Juanma para poligonos regulares
	float angulo = this->rot*-1;
	float radio = this->get_ancho() / 2;

	Utilities::calcularVerticesPoligonoRegularVista(  this->vertices_x, this->vertices_y, this->lados ,radio, angulo );

	// Aplico transformacino a coordenadas originales
	for(short i = 0 ; i < this->lados ; i++){
		this->vertices_x[i] += this->get_x();
		this->vertices_y[i] += this->get_y();
	}
*/
}

void PoligonoRegularVista::setEstado( sEvento estadoEvento ){
	Vista::setEstado( estadoEvento );
	//this->calcularVertices();

	// Se envian vertices para el dibujo particular del paralelogramo
	for( short i = 0; i < this->lados ; i++ ){
		this->vertices_x[i] = estadoEvento.verticesX[i];
		this->vertices_y[i] = estadoEvento.verticesY[i];
	}
}


// Setea vertices con unidades logicas
void PoligonoRegularVista::setVertices(float* x, float* y){
	/*
	for (int i = 0; i < this->lados; i++){
		this->vertices_x[i] = Utilities::unidadLogica2Pixel(x[i], Utilities::ANCHO_PX, Utilities::ANCHO_UL);
		this->vertices_y[i] = Utilities::unidadLogica2Pixel(y[i], Utilities::ALTO_PX, Utilities::ALTO_UL);
	}
	*/
}

void PoligonoRegularVista::dibujar(SDL_Renderer* renderer){
	polygonRGBA(renderer,this->vertices_x,this->vertices_y,this->lados,this->get_color_r(),this->get_color_g(),this->get_color_b(),255);
	filledPolygonRGBA(renderer,this->vertices_x,this->vertices_y,this->lados,this->get_color_r(),this->get_color_g(),this->get_color_b(),255);

	// Dibuja centro x,y
	this->dibujar_centro(renderer);
}

void PoligonoRegularVista::dibujarTextura(SDL_Renderer* renderer){	

	
}
	
	//	render( ( SCREEN_WIDTH - gArrowTexture.getWidth() ) / 2, ( SCREEN_HEIGHT - gArrowTexture.getHeight() ) / 2, NULL, degrees, NULL, flipType );
/*
	void LTexture::render( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip )
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx( gRenderer, mTexture, clip, &renderQuad, angle, center, flip );
}
*/
