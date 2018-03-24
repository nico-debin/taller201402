#pragma once
#include "estructuraDeDatos.h"
#include "Vista.h"
#include <set>

class Camara
{
public:
	Camara(void);
	~Camara(void);

	
	void actualizarCamaraXY(sEvento &estadoEvento);
	SDL_Rect* getRectangulo(int anchoImagen, int altoImagen);
	bool zoomIn();
	bool zoomOut();
	float getZoom();

	void actualizarAnchoAltoEscenarioPX();

	
	void aplicarZoomScrollYDibujar(Vista* vista, SDL_Renderer* renderer);

	void actualizar();

private:
	int x;
	int y;
	float xMax;
	float yMin;

	float zoom;
	bool hayNuevoZoom;
	bool cambioElZoom;

	//Creo que se deben inicializar con los anchos y altos de Utillities
	float anchoEscenarioPX;
	float altoEscenarioPX;

	void verificarLimites();

	void aplicarScroll(sEvento &estadoEvento);
	void aplicarZoom(sEvento &estadoAModificar);

	int aplicarZoomCoordenadaY(int yAModificar);

	
	set<float> zoomUsados;
};




