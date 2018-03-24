#ifndef _Escenario_H_
#define _Escenario_H_

#include "estructuraDeDatos.h"
#include "Figura.h"
#include "SnowBoy.h"
#include "Fisica.h"
#include "Cliente.h"
#include "Logger.h"
#include <string>
#include <map>

class Escenario{
	private:
		int alto_px;
		int ancho_px;
		float alto_ul;
		float ancho_ul;
		string imagen_fondo;

		static map<int,Figura*>* figuras;
        //map<int,SnowBoy*>* personajes;
		map<int,Figura*>* figurasAEliminar;
		vector<sEvento*>* vectorEventosServidor;

		Fisica* fisica;
		void haySuperposicion(Figura* figura, map<int,Figura*>* figuraAEliminar);
		// Devuelve un struct con los datos actualizados de las figuras
		sEvento* crearEventoFigura(Figura*);

		void destruirEscenario(bool eliminarSnowboys = true);

	public:
		Escenario(sEscenario datosEscenario);
		~Escenario();

		void cargarNuevoEscenario(sEscenario);

		int getAltoPixel();
		int getAnchoPixel();
		float getAltoLogico();
		float getAnchoLogico();

		int getCantidadDeEnemigos();

		void verificarSuperposicion();
		string getPathImagenFondo();
		map<int,Figura*>* getFigurasAEliminar();
		void eliminarPrimerFiguraSuperpuesta();
		
		vector<sEvento*>* getEventosEscenario( bool todoLosEventos, map<string,Cliente*> clientes );
		
		Fisica* getFisica();


		static map<int,Figura*>* getFiguras(){
			return Escenario::figuras;
		}

		static void agregarFigura(Figura* figura){
			(*Escenario::figuras)[figura->getId()] = figura;
		}
		//void agregarPersonaje(SnowBoy* snowBoy);
};

#endif