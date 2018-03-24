#ifndef _RectanguloVista_H_
#define _RectanguloVista_H_

#include "Vista.h"

class RectanguloVista : virtual public Vista {
	private:
		short vertices_x[4], vertices_y[4];
		void calcularVertices();
	public:
		RectanguloVista(sEvento estadoEvento);
		void setEstado( sEvento estadoEvento );
		void setVertices(float* x, float* y);
		void dibujar(SDL_Renderer* renderer);
		void dibujarTextura(SDL_Renderer* renderer);
};


#endif /* _RectanguloVista_H_ */