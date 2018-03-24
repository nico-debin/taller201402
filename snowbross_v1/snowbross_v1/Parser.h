#pragma once

#include "StdAfx.h"
#include <string>
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

class Parser
{
public:
	Parser(string path);
	Parser();
	~Parser(void);
	Escenario *init();

	//b2World* mundo;
private:
	string path;
	bool isDefault;
	sEscenario structEscenario;

	TIPO_FIGURA getTipoFigura(string);
	string getTipoFiguraString(TIPO_FIGURA tipo);
	
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

	int getInt(Json::Value jsonValue, string campo, int defValue);
	float getDouble(Json::Value jsonValue, string campo, long double defValue);
	string getString(Json::Value jsonValue, string campo, string defValue);
	string getColor(Json::Value jsonValue, string campo, string defValue);
	bool getBool(Json::Value jsonValue, string campo, bool defValue);
	
	int validarIntSegunCampo(int valorCampo, string campo);
	float validarDoubleSegunCampo(float valorCampo, string campo);



	sEscenario parsearEscenario(Json::Value jsonEscenario);
	//vector <sPersonaje> parsearPersonajes(Json::Value jsonPersonajes);
    	//sPersonaje parsearPersonaje(Json::Value jsonPersonaje);
    void parsearPersonajes(Json::Value jsonPersonajes, vector<sObjeto>* objetos);
    sObjeto parsearPersonaje(Json::Value jsonPersonaje);

	//vector <sObjeto> parsearObjetos(Json::Value jsonObjetos);	
    void parsearObjetos(Json::Value jsonPersonajes, vector<sObjeto>* objetos);
	sObjeto parsearObjeto(Json::Value jsonObjeto);

	Escenario* parse(ifstream & afile);
};

