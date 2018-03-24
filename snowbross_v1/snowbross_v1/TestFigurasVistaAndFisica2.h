#ifndef _TestFigurasVistaAndFisica2_H_
#define _TestFigurasVistaAndFisica2_H_

#include "Vista.h"
#include "EscenarioVista.h"

#include <Box2D/Box2D.h>
#include "Circulo.h"
#include "Rectangulo.h"
#include "PoligonoRegular.h"
#include "Paralelogramo.h"
#include "Constantes.h"

#include "Figura.h"
#include <map>


class TestFigurasVistaAndFisica2{
	protected:
		static int id_actual;

		static b2World* iniciarMundoFisico();	
		static EscenarioVista* iniciarEscenario();
		static void crearFiguras(EscenarioVista*, b2World*);
		static void actualizarFiguras(EscenarioVista*, b2World*);
		static void jugar(EscenarioVista*, b2World*);
	public:

		static void test();
};

#endif