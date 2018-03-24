#ifndef _estructuraDeDatos_H_
#define _estructuraDeDatos_H_

#include <WinSock2.h>
#include <sstream>
#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include "Constantes.h"

using namespace std;

struct sObjeto {
    int id;
	TIPO_FIGURA tipo;
	float x;
	float y;

	float ancho;
	float alto;
	float radio;
	int lados;
	float escala;
	float baseMayor;
	float baseMenor;

	// Solo para trapecios y paralelogramos
	float angulo_1;
	float angulo_2;

	string color;
	float rot;
	float masa;
	bool estatico;
};

struct sEscenario {
	int alto_px;
    int ancho_px;
	float alto_ul;
    float ancho_ul;
    string imagen_fondo;
	//vector <sPersonaje> personajes;
	vector <sObjeto> objetos;
};


// Struct que el server envia al cliente para que pueda inicializar el escenario
struct sEscenarioCliente{
	int idSnowBoy;
	int alto_px;
    int ancho_px;
	char imagen_fondo[70];

//	std::ostream& serialize(std::stringstream& os) const {
//		os << idSnowBoy << ";" << alto_px << ";" << ancho_px << ";" << imagen_fondo << ";" ;
//		
//		while (os.str().length() < 300 ){
//			os << ".";
//		}
//
//        return os;
//    };
//
//    std::istream& deserialize(std::istream& is) {
//		std::stringstream buffer;
//
//		// id SnowBoy
//		char c = is.get();
//		while(c != ';' && !is.eof() ){ buffer << c; c = is.get(); } buffer << '\0';
//		buffer >> idSnowBoy;
//		buffer.str(std::string());
//		
//		// alto_px
//		c = is.get();
//		while(c != ';' && !is.eof() ){ buffer << c; c = is.get(); } buffer << '\0';
//		buffer >> alto_px;
//		buffer.str(std::string());
//
//		// ancho_px
//		c = is.get();
//		while(c != ';' && !is.eof() ){ buffer << c; c = is.get(); } buffer << '\0';
//		buffer >> ancho_px;
//		buffer.str(std::string());
//
//		// imagen_fondo
//		c = is.get();
//		while(c != ';' && !is.eof() ){ buffer << c; c = is.get(); } buffer << '\0';
//		buffer >> imagen_fondo;
//		buffer.str(std::string());
//
//		return is;
//};
//	int size(){
//		return 300;
//	}
};

// Struct a utilizar para enviar mediante sockets
struct sEvento {
	int id;
	int evento;
	TIPO_FIGURA tipo;
	int x,y;
	float rot; //angulo
	int ancho; // Para poligonos y circulos = radio*2 o escala*2
	int alto;  // Para poligonos y circulos = radio*2 o escala*2
	//string color;

	// Solo para trapecios y paralelogramos
	//float angulo_1;
	//float angulo_2;

	// Se envian vertices para el dibujo particular del paralelogramo
	//int verticesX[MAX_LADOS];
	//int verticesY[MAX_LADOS];

	//std::ostream& serialize(std::stringstream& os) const {
	//	os << id << ";" << evento << ";" << tipo << ";" << x << ";" << y << ";"
	//	   << rot << ";" << ancho << ";" << alto << ";" << angulo_1 << ";" << angulo_2 << ";";
	//	
	//	int i;
	//	for( i=0 ; i<MAX_LADOS ; i++ ) os << verticesX[i] << ";";
	//	for( i=0 ; i<MAX_LADOS ; i++ ) os << verticesY[i] << ";";

	//	os << color.c_str() << ";";

	//	while (os.str().length() < 150){
	//		os << ".";
	//	}
 //       return os;
 //   };
 //   std::istream& deserialize(std::istream& is) {
	//	std::stringstream buffer;

	//	char c = is.get();
	//	int aux_int;
	//	std::string pp;

	//	// id
	//	while(c != ';' && !is.eof() ){ buffer << c; c = is.get(); } buffer << '\0';
	//	buffer >> id;
	//	buffer.str(std::string());

	//	// evento
	//	c = is.get();
	//	while(c != ';' && !is.eof()){ buffer << c; c = is.get(); } buffer << '\0';
	//	buffer >> evento;
	//	buffer.str(std::string());

	//	// tipo
	//	c = is.get();
	//	while(c != ';' && !is.eof()){ buffer << c; c = is.get(); } buffer << '\0';
	//	buffer >> aux_int; tipo = (TIPO_FIGURA)aux_int;
	//	buffer.str(std::string());

	//	// x
	//	c = is.get();
	//	while(c != ';' && !is.eof()){ buffer << c; c = is.get(); } buffer << '\0';
	//	buffer >> x;
	//	buffer.str(std::string());

	//	// y
	//	c = is.get();
	//	while(c != ';' && !is.eof()){ buffer << c; c = is.get(); } buffer << '\0';
	//	buffer >> y;
	//	buffer.str(std::string());

	//	// rot
	//	c = is.get();
	//	while(c != ';'){ buffer << c; c = is.get(); } buffer << '\0';
	//	buffer >> rot;
	//	buffer.str(std::string());

	//	// ancho
	//	c = is.get();
	//	while(c != ';' && !is.eof()){ buffer << c; c = is.get(); } buffer << '\0';
	//	buffer >> ancho;
	//	buffer.str(std::string());

	//	// alto
	//	c = is.get();
	//	while(c != ';' && !is.eof()){ buffer << c; c = is.get(); } buffer << '\0';
	//	buffer >> alto;
	//	buffer.str(std::string());

	//	// angulo_1
	//	c = is.get();
	//	while(c != ';' && !is.eof()){ buffer << c; c = is.get(); } buffer << '\0';
	//	pp = buffer.str();
	//	buffer >> angulo_1;
	//	buffer.str(std::string());

	//	// angulo_2
	//	c = is.get();
	//	while(c != ';' && !is.eof()){ buffer << c; c = is.get(); } buffer << '\0';
	//	pp = buffer.str();
	//	buffer >> angulo_2;
	//	buffer.str(std::string());

	//	// verticesX
	//	int i;
	//	for( i=0 ; i<MAX_LADOS ; i++ ){
	//		c = is.get();
	//		while(c != ';' && !is.eof()){ buffer << c; c = is.get(); } buffer << '\0';
	//		pp = buffer.str();
	//		buffer >> verticesX[i];
	//		buffer.str(std::string());
	//	}
	//	// verticesY
	//	for( i=0 ; i<MAX_LADOS ; i++ ){
	//		c = is.get();
	//		while(c != ';' && !is.eof()){ buffer << c; c = is.get(); } buffer << '\0';
	//		buffer >> verticesY[i];
	//		buffer.str(std::string());
	//	}

	//	// color
	//	c = is.get();
	//	while(c != ';' && !is.eof()){ buffer << c; c = is.get(); } buffer << '\0';
	//	buffer >> color;
	//	buffer.str(std::string());

	//	return is;
	//};
	//int size(){
	//	return 150;
	//}

};

struct sSprite{
	SDL_Rect sprites[3];
};

// Eventos enviados desde el Controlador Cliente al Servidor
struct sEventoCliente {
	int id;
	EVENTOS_RECIBIDOS_SERVIDOR evento;

//std::ostream& serialize(std::stringstream& os) const {
//		os << id << ";" << evento << ";" << '\0';
//
//		while (os.str().length() < 11){
//			os << ".";
//		}
//
//        return os;
//    };
//    std::istream& deserialize(std::istream& is) {
//		std::stringstream buffer;
//		char c = is.get();
//		std::string pp;
//
//		while(c != ';' && !is.eof()){ 
//			buffer << c; 
//			c = is.get(); 
//		}
//		buffer << '\0';
//		pp = buffer.str();
//		buffer >> id;
//		buffer.str(std::string());
//		pp = buffer.str();
//
//		c = is.get();
//		while(c != ';' && !is.eof()){ 
//			buffer << c; 
//			c = is.get(); 
//		}
//		buffer << '\0';
//		pp = buffer.str();
//		int aux_int;
//		buffer >> aux_int;
//		evento = (EVENTOS_RECIBIDOS_SERVIDOR)aux_int;
//
//        return is;
//    };
//	int size(){
//		return 11;
//    }
};
struct sEventoClienteConectar {
	char nombre_usuario[30];
};

#endif