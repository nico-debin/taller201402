#pragma once
#ifndef _PoligonoRegularVista_H_
#define _PoligonoRegularVista_H_

#include "Vista.h"

class PoligonoRegularVista : public Vista {
	protected:
		short vertices_x[MAX_LADOS], vertices_y[MAX_LADOS];
		void calcularVertices();
	public:
		PoligonoRegularVista(sEvento estadoEvento);
		void setEstado( sEvento estadoEvento );
		void setVertices(float* x, float* y);
		void dibujar(SDL_Renderer* renderer);
		void dibujarTextura(SDL_Renderer* renderer);
};


#endif /* _PoligonoRegularVista_H_ */