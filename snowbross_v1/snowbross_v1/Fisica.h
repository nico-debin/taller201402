#pragma once
#include <Box2D/Box2D.h>
#include "Constantes.h"

class Fisica
{
	protected:
		b2Vec2 gravedad;
		b2World* mundo;

	public:
		Fisica(float anchoUl,float altoUl);
		void paso(void);
		void iniciarPisoYPared(float anchoUl,float altoUl);
		b2World* getMundo();
		~Fisica(void);
		


};

