#ifndef _SnowBoy_H_
#define _SnowBoy_H_

//#include "Nieve.h"
#include "Personaje.h"
#include "Nieve.h"
class Enemigo;

class SnowBoy: public Personaje{
	protected:	
		Enemigo* enemigoBolaTocadaDer;
		Enemigo* enemigoBolaTocadaIzq;
		Enemigo* bolaCaptora;

		//int vidas;

		virtual void setearEstado(float distanciaAlPiso);



		void cargarBitsDeColisionDeSnowboyInvencible();
		void cargarBitsDeColisionDeSnowboyNormal();
		void cargarBitsDeColisionDeSnowboyMuerto();

		int stepsDuracionInvulnerabilidad;
		void iniciarInvulnerabilidad();
		void terminarInvulnerabilidad();
		void analizarInvulnerabilidad();
		
		int stepsParaCambiarDeEstadoADevolver;
		bool devolverEstadoInvulnerable;
		bool cambioEstadoDeInvulnerabilidad;

		//Va a funcionar igual que el timer agonia pero cuando se termina el SB renace en vez de morir
		Timer* timerRenacer;
		//void renacer();
		int stepsParaRenacer;
		int xInicial;
		int yInicial;


	public:
		SnowBoy(b2World* mundo,  sObjeto structObj);
		void castearRayosHorizontales();
		Enemigo* analizarBolaTocada(SnowBoyRayCastCallback* callback);
		void lanzar();
		
		void atraparEnBolaDeNieve(Enemigo* bola);
		void liberarDeBolaDeNieve(b2Vec2 posicionDeLaBolaDestruida);
		virtual void saltar();
		virtual void matar();
		virtual bool estaMuerta();

		void moverAlInicio();

		~SnowBoy();

		void actualizar();
		int getVidas();
		void setVida(int vida);
		void incrementarVida();
		void incrementarVelocidad();
		virtual Figura* crearProyectil(sObjeto structDelProyectil);
		

		bool getDevolverEstadoInvulnerable();
		bool getCambioEstadoDeInvulnerabilidad();

		void renacer();

		void inmortal();
};


#endif