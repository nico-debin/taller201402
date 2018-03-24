#ifndef _Fisica_H_
#define _Fisica_H_

#include <Box2D/Box2D.h>
#include "Constantes.h"
#include "estructuraDeDatos.h"

class Fisica
{
	protected:
		b2Vec2 gravedad;
		b2World* mundo;

		sUserData userDataParedDerecha;
		sUserData userDataParedIzquierda;
		sUserData userDataPiso;
		sUserData userDataTecho;

	public:
		Fisica(float anchoUl,float altoUl);
		void paso(void);
		void iniciarPisoYPared(float anchoUl,float altoUl);
		b2World* getMundo();
		~Fisica(void);
		


};

#endif