#ifndef _Personaje3_H_
#define _Personaje3_H_

#include "Figura.h"
#include "SnowBoyRayCastCallback.h"
#include "estructuraDeDatos.h"


class Personaje: public Figura{
	protected:		

		bool habilitacionIzq;
		bool habilitacionDer;
		bool habilitacionSalto;
	
		bool saltarPresionado;
		bool izqPresionado;
		bool derPresionado;
		bool orientacionDerecha;



		//Ahora es una variable por el bonus de velocidad que cambia la vel del SB
		//La velocidad para todos los personajes se carga en el constructor de personaje y VELOCIDAD_HORIZONTAL es la velocidad de default
		/*Los dos valores posibles:
			VELOCIDAD_HORIZONTAL 10
			VELOCIDAD_HORIZONTAL_BONUS 15
		*/
		int velocidadHorizontal;




		//int estado;
		b2Fixture* fixtureDef;


		void crearForma();

		void castearRayosDePiernas();
		float castearRayo(float posX, float posYInicial, float posYFinal);
		float elegirDistanciaAlPiso(float distanciaAlPisoDer, float distanciaAlPisoIzq);

		/*Setea los estados QUIETO_IZQ/DER, SALTANDO_IZQ/DER Y CAMINANDO_IZQ/DER
		  Quieto y Saltando pueden ser comunes al snowBoy y a los enemigos y creo que se setearian de la misma forma
		  de última se puede modificar		
		*/
		virtual void setearEstado(float distanciaAlPiso);


		SnowBoyRayCastCallback* castearRayoHorizontal(float posXFinal);
		virtual void castearRayosHorizontales();
		bool analizarHabilitacionParaRayo(SnowBoyRayCastCallback* callback);

		void desacelerarSiEstaQuieto();

		void analizarMovimientos();

		float calcularArea(){ return 4;}

		b2Fixture* getFixture();


		void cargarBitsDeColisionEnFixtures(uint16 categoryBits, uint16 maskBits);

	public:

		Personaje(b2World* mundo,  sObjeto structObj);

		virtual void actualizar();		
		void moverDerecha();
		void moverIzquierda();
		void frenarDerecha();
		void frenarIzquierda();
		virtual void saltar();

		~Personaje(void);
	//	void disparar();

		//virtual int getEstado();
		virtual float32 getY();

		virtual void matar();

		virtual void lanzar();
		virtual Figura* crearProyectil(sObjeto structDelProyectil) = 0;

		//SOlo de prueba despues llvar a enemigo
		bool saltoHabilitado;
		void castearRayoSalto(float posX, float posYInicial, float posYFinal);
		void habilitarSalto();
		void deshabilitarSalto();
		bool puedeSaltar();
};

#endif