#pragma once

#include "StdAfx.h"
#include <string>
#include <vector>
#include "Figura.h"
#include "Rectangulo.h"
#include "Circulo.h"
#include "Constantes.h"
#include <json/json.h>
#include <iostream>
#include <fstream>
#include <string>
#include <Box2D/Box2D.h>
#include "Circulo.h"
#include "Rectangulo.h"
#include "FabricaDeFiguras.h"
#include "Escenario.h"
#include "Logger.h"

#include "Diablo.h"

class Parser{
	public:
		Parser(string path);
		Parser();
		~Parser(void){};

		Escenario* init();
		bool hayMasEscenarios();
		bool cargarProximoEscenario(Escenario* escenario);
		bool cargarEscenarioInicial(Escenario* escenario);
		void reiniciarEscenarios();

	private:
		string path;
		bool isDefault;
		vector<sEscenario> vectorStructEscenarios;
		sEscenario structEscenario;

		int escenarioActual;

		Escenario* parse(ifstream & afile);

		bool excluirSnowboyPrimerEscenario;

		TIPO_FIGURA getTipoFigura(string);
		string getTipoFiguraString(TIPO_FIGURA tipo);
	
		bool validarEscenarios(Json::Value jsonEscenarios);
		bool validarEscenario(Json::Value jsonEscenario);
		bool validarPersonajes(Json::Value jsonValue);
		bool validarObjetos(Json::Value jsonValue);
		bool validarPersonaje(Json::Value jsonValue);
		bool validarObjeto(Json::Value jsonValue);
		bool validarCampoInt(Json::Value jsonValue, string nombre);
		bool validarCampoString(Json::Value jsonValue, string nombre);
		bool validarCampoStringHexa(Json::Value jsonValue, string nombre);
		bool validarCampoFloat(Json::Value jsonValue, string nombre);
		bool validarCampoCompuesto(Json::Value jsonValue, string nombre);
		bool validarCampoBool(Json::Value jsonValue, string nombre);
		bool isHexNotation(std::string const& s);
	
		int validarIntSegunCampo(int valorCampo, string campo);
		float validarDoubleSegunCampo(float valorCampo, string campo);

		void parsearEscenarios(Json::Value jsonEscenarios);
		sEscenario parsearEscenario(Json::Value jsonEscenario);

		void parsearPersonajes(vector<sObjeto>* objetos);
		sObjeto parsearPersonaje(Json::Value jsonPersonaje);

		void parsearObjetos(Json::Value jsonPersonajes, vector<sObjeto>* objetos);
		sObjeto parsearObjeto(Json::Value jsonObjeto);

		void parsearEnemigos(Json::Value jsonEnemigos, vector<sObjeto>* objetos);
		sObjeto parsearEnemigo(Json::Value jsonEnemigo);

		void parsearBonos(Json::Value jsonBonos, vector<sObjeto>* objetos);
		sObjeto parsearBono(Json::Value jsonBono);

		TIPO_FIGURA getTipoBono(string tipo);
		TIPO_FIGURA getTipoEnemigo(string tipo);

		int getInt(Json::Value jsonValue, string campo, int defValue);
		float getDouble(Json::Value jsonValue, string campo, long double defValue);
		string getString(Json::Value jsonValue, string campo, string defValue);
		string getColor(Json::Value jsonValue, string campo, string defValue);
		bool getBool(Json::Value jsonValue, string campo, bool defValue);
};

