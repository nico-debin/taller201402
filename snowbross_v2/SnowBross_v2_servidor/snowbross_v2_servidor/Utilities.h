#pragma once
#ifndef _UTILITIES_H_
#define _UTILITIES_H_

#include <string>
#include "Constantes.h"

#include "SDLController.h"
#include <Box2D/Box2D.h>

class Utilities {
	private:
		static int GetDecimal(char hexchar);
		static void calcularVerticePoligonoRegularFisico( float &x, float &y, float radio, float angulo );
		static void calcularVerticePoligonoRegularVista(  short &x, short &y,   int radio, float angulo );

		static int ID_INCREMENTAL;
	public:
		static int ALTO_PX;
		static int ANCHO_PX;
		static int ALTO_UL;
		static int ANCHO_UL;
		static int MAX_JUGADORES;
		
		static bool PAUSA_CLIENTE;

		static std::string IMAGEN_FONDO_NIVEL1;

		static int getNewID();

		static int sdl2euclid_ejeY(int y);
		static int euclid2sdl_ejeY(int y);

		static void strColorHexaToRGB( std::string strColorHexa, int* vectorRGB);
		static float pixel2unidadLogica(int pixel, int pixelsTotales, float uLogicasTotales);
		static int unidadLogica2Pixel(float uLogica, int pixelsTotales, float uLogicasTotales);

		static int redondeo(float num);

		static float grados2radianes(float grados); 
		static float radianes2grados(float radianes);

		static void calcularVerticesPoligonoRegularFisico( float vertices_x[MAX_LADOS], float vertices_y[MAX_LADOS], int cantidadVertices, float radio, float angulo );
		static void calcularVerticesPoligonoRegularVista(  short vertices_x[MAX_LADOS], short vertices_y[MAX_LADOS], int cantidadVertices,   int radio, float angulo );

		static void calcularVerticesTrapecioFisico( float vertices_x[4], float vertices_y[4], float base_inferior, float altura, float angulo_interno_izq, float angulo_interno_der, float rotacion );
		static void calcularVerticesTrapecioVista( short vertices_x[4], short vertices_y[4], float base_inferior, float altura, float angulo_interno_izq, float angulo_interno_der, float rotacion );

		//static void calcularVerticesParalelogramoFisico(float vertices_x[4], float vertices_y[4], float x, float y,float base, float altura, float angulo_inf_izq, float rotacion );

		static void rotarVertices(float anguloRadianes, short vertices_x[MAX_LADOS], short vertices_y[MAX_LADOS], short cantidadVertices, short centro_x, short centro_y);
		static void rotarVertices(float anguloRadianes, float vertices_x[MAX_LADOS], float vertices_y[MAX_LADOS], short cantidadVertices, short centro_x, short centro_y);

		static void DrawBox2DBody(SDL_Renderer *buffer, b2Body *body, int fr, int fg, int fb, int falpha, int lr, int lg, int lb, int lalpha, bool aa);
};

#endif /*UTILITIES_H*/