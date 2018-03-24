#include "StdAfx.h"
#include "Parser.h"
#include "Utilities.h"


Parser::Parser(){}

Parser::Parser(string path){
	this->path = path;
	this->isDefault = false;
}

Escenario* Parser::init(){

	this->vectorStructEscenarios.clear();
	this->escenarioActual = -1;

	ifstream myfile(this->path);
	if (!myfile.is_open()){
		Logger::getLogger()->debug( "No se pudo abrir el archivo " + path + " Se va a intentar abrir el archivo default.json " );

		ifstream defaultfile("default.json");
		this->isDefault = true;
		return this->parse(defaultfile);
	} else{
		this->isDefault = false;
		return this->parse(myfile);
	}
}

Escenario* Parser::parse(ifstream & afile){
	string line;
	string allLines;

	Json::Value root;   // will contains the root value after parsing.
	Json::Reader reader;

	while ( getline (afile,line) ){
		allLines = allLines + line + '\n';
	}
	afile.close();
	Logger::getLogger()->debug( "Archivo Json leido" );

	bool parsingSuccessful = reader.parse( allLines, root );
	if ( !parsingSuccessful ){
		// logear que no se pudo parsear, atrapar la exception y dar un mensaje descriptivo
		Logger::getLogger()->error( "No se pudo parsear, verificar la esctructura del json. " + reader.getFormattedErrorMessages());
		if (! this->isDefault) {
			Logger::getLogger()->error( "Se procede con el archivo default");
			this->isDefault = true;
			ifstream defaultfile("default.json");
			return this->parse(defaultfile);
		} else {
			Logger::getLogger()->error( "No se pudo parsear, el archivo default");
			//return NULL;
		}
	}

	//Json::Value jsonEscenarios = root.get("escenarios", "default content" );
	Json::Value jsonEscenarios = root["escenarios"];

	if (!this->validarEscenarios(jsonEscenarios)){
		Logger::getLogger()->error( "No se pudieron validar los escenarios." );
	} 

	this->parsearEscenarios(jsonEscenarios);

	this->excluirSnowboyPrimerEscenario = false;

	Escenario* escenario = new Escenario( this->vectorStructEscenarios.at(0) );
	this->cargarProximoEscenario(escenario);

	return escenario;
}

bool Parser::hayMasEscenarios(){
	return ( (this->escenarioActual+1) < this->vectorStructEscenarios.size() );
}

// Reinicia el puntero para que al cargar el proximo escenario devuelva el primero parseado
void Parser::reiniciarEscenarios(){
	this->escenarioActual = -1;
	this->excluirSnowboyPrimerEscenario = true;
}

// Si no hay mas escenarios devuelve nullptr
bool Parser::cargarProximoEscenario(Escenario* escenario){
	
	if( this->hayMasEscenarios() == false ) return false;

	this->escenarioActual++;

	sEscenario structEscenario = this->vectorStructEscenarios.at(this->escenarioActual);
	escenario->cargarNuevoEscenario(structEscenario);

	sObjeto obj;
	for (int i = 0; i < structEscenario.objetos.size(); i++){
		obj = structEscenario.objetos[i];
		//if( this->escenarioActual == 0 &&/* this->excluirSnowboyPrimerEscenario == true*/ obj.tipo == TIPO_SNOWBOY){
		//	this->excluirSnowboyPrimerEscenario = false;
		//	continue;
		//}
		if( this->escenarioActual > 0 && obj.tipo == TIPO_SNOWBOY ){
			// No vuelvo a cargar snowboys en nuevos niveles.
			continue;
		}
		/*if ( obj.tipo != TIPO_SNOWBOY ){*/
			Escenario::agregarFigura(FabricaDeFiguras::crearFigura(escenario->getFisica()->getMundo(), obj));
		/*}*/
	}

	// Si encuentra dos superpuestas, carga para eliminar la primera
	//escenario->verificarSuperposicion();

	/*for( map<int,Figura*>::iterator itFiguraAEliminar = escenario->getFigurasAEliminar()->begin() ; itFiguraAEliminar != (escenario->getFigurasAEliminar())->end(); ++itFiguraAEliminar){
		Figura* figuraAEliminar = (*itFiguraAEliminar).second;
		sObjeto structObjeto = figuraAEliminar->getDatos();	
		Logger::getLogger()->error( "Figura Superpuesta, se elimino la figura: " + this->getTipoFiguraString(structObjeto.tipo) );		
	}
	*/
	// Elimina las figuras cargadas del mapa de figuras que tiene el escenario
	//escenario->eliminarPrimerFiguraSuperpuesta();

	return true;
}

bool Parser::cargarEscenarioInicial(Escenario* escenario){
	
	if( this->hayMasEscenarios() == false ) return false;

	this->escenarioActual++;

	sEscenario structEscenario = this->vectorStructEscenarios.at(this->escenarioActual);
	escenario->cargarNuevoEscenario(structEscenario);

	sObjeto obj;
	for (int i = 0; i < structEscenario.objetos.size(); i++){
		obj = structEscenario.objetos[i];
		if ( obj.tipo != TIPO_SNOWBOY ){
			Escenario::agregarFigura(FabricaDeFiguras::crearFigura(escenario->getFisica()->getMundo(), obj));
		}
	}

	// Si encuentra dos superpuestas, carga para eliminar la primera
	//escenario->verificarSuperposicion();

	/*for( map<int,Figura*>::iterator itFiguraAEliminar = escenario->getFigurasAEliminar()->begin() ; itFiguraAEliminar != (escenario->getFigurasAEliminar())->end(); ++itFiguraAEliminar){
		Figura* figuraAEliminar = (*itFiguraAEliminar).second;
		sObjeto structObjeto = figuraAEliminar->getDatos();	
		Logger::getLogger()->error( "Figura Superpuesta, se elimino la figura: " + this->getTipoFiguraString(structObjeto.tipo) );		
	}*/
	
	// Elimina las figuras cargadas del mapa de figuras que tiene el escenario
	//escenario->eliminarPrimerFiguraSuperpuesta();

	return true;
}


void Parser::parsearEscenarios(Json::Value jsonEscenarios){
	for ( int i = 0; i < jsonEscenarios.size(); ++i ){
		sEscenario stEscenario = this->parsearEscenario(jsonEscenarios[i]);
		this->vectorStructEscenarios.push_back(stEscenario);
	}
}

sEscenario Parser::parsearEscenario(Json::Value jsonEscenario){
	sEscenario escenario;
	escenario.alto_px = getInt(jsonEscenario, "alto-px",768);
	escenario.ancho_px = getInt(jsonEscenario,"ancho-px",1024);

	escenario.alto_ul = getInt(jsonEscenario,"alto-un",100);
	escenario.ancho_ul = getInt(jsonEscenario,"ancho-un",50);

	Utilities::MAX_JUGADORES = getInt(jsonEscenario,"max-jugadores",4);

	//ESTO LO AGREGO PARA QUE SE PUEDA ADAPTAR AL VALOR DE LA PANTALLA SI EL ANCHO/ALTO-UL ES IGUAL A CERO ( ES MUY FORZADO)
	//TAMBIEN SIRVE PARA VERIFICAR SI ESTA ADENTRO DEL ESCENARIO
	structEscenario.alto_px=escenario.alto_px;
	structEscenario.ancho_px=escenario.ancho_px;
	structEscenario.alto_ul= escenario.alto_ul;
	structEscenario.ancho_ul= escenario.ancho_ul;
	//////////////////////////////////////////////////////////

	escenario.imagen_fondo = getString(jsonEscenario,"imagen_fondo",IMAGEN_FONDO_DEFAULT);

	this->parsearPersonajes(&escenario.objetos);
    this->parsearObjetos(jsonEscenario.get("objetos",NULL), &escenario.objetos);
	this->parsearEnemigos(jsonEscenario.get("enemigos",NULL), &escenario.objetos);

//	parsearBonos(jsonEscenario.get("bonos",NULL), &escenario.objetos);

	this->structEscenario = escenario;
	
	Logger::getLogger()->debug( "Se parseo el escenario");
	return escenario;
}

TIPO_FIGURA Parser::getTipoFigura(string tipo){
	if (tipo == CIRCULO)
		return TIPO_CIRCULO;
	else if (tipo == RECTANGULO)
		return TIPO_RECTANGULO;
	else if (tipo == PARALELOGRAMO)
		return TIPO_PARALELOGRAMO;
	else if (tipo == POLIGONO)
		return TIPO_POLIGONO;
	else if (tipo == TRAPECIO)
		return TIPO_TRAPECIO;
	else {
		return TIPO_NULL;
	}
}

string Parser::getTipoFiguraString(TIPO_FIGURA tipo){
	if (tipo == TIPO_CIRCULO)
		return CIRCULO;
	if (tipo == TIPO_RECTANGULO)
		return RECTANGULO;
	else if (tipo == TIPO_PARALELOGRAMO)
		return PARALELOGRAMO;
	else if (tipo == TIPO_POLIGONO)
		return POLIGONO;
	else if (tipo == TIPO_TRAPECIO)
		return TRAPECIO;
	else {
		return NULL;
	}
}

sObjeto Parser::parsearObjeto(Json::Value jsonObjeto){
	sObjeto structObjeto;
	memset(&structObjeto,0,sizeof(sObjeto));

	structObjeto.tipo = this->getTipoFigura( jsonObjeto.get("tipo",NULL).asString() );

	Logger::getLogger()->debug( "Parseando objeto: " + jsonObjeto.get("tipo",NULL).asString());

	structObjeto.x = getDouble(jsonObjeto,"x",5);
	structObjeto.y = getDouble(jsonObjeto,"y",90);
	structObjeto.color = getColor(jsonObjeto,"color","#00FF00");
	structObjeto.rot =  getDouble(jsonObjeto,"rot",0);
	structObjeto.masa = getDouble(jsonObjeto,"masa",3);
	structObjeto.estatico = getBool(jsonObjeto,"estatico",true);
	
	if (structObjeto.tipo == TIPO_RECTANGULO ){
		structObjeto.ancho = getDouble(jsonObjeto,"ancho",50);
		structObjeto.alto = getDouble(jsonObjeto,"alto",50);
	}
	if (structObjeto.tipo == TIPO_CIRCULO){
		structObjeto.radio = getDouble(jsonObjeto,"radio",10);
	}
	if (structObjeto.tipo == TIPO_POLIGONO){
		structObjeto.lados = getInt(jsonObjeto,"lados",4);
		structObjeto.escala = getDouble(jsonObjeto,"escala",2);
	}
	if (structObjeto.tipo == TIPO_TRAPECIO){
		structObjeto.ancho = getDouble(jsonObjeto,"ancho",50);
		structObjeto.alto = getDouble(jsonObjeto,"alto",50);
		structObjeto.escala = getDouble(jsonObjeto,"escala",2);
		structObjeto.angulo_1 = getDouble(jsonObjeto,"angulo_1",30);
		structObjeto.angulo_2 = getDouble(jsonObjeto,"angulo_2",20);
	}
	if (structObjeto.tipo == TIPO_PARALELOGRAMO){
		structObjeto.ancho = getDouble(jsonObjeto,"ancho",50);
		structObjeto.alto = getDouble(jsonObjeto,"alto",50);
		//structObjeto.escala = getDouble(jsonObjeto,"escala",2);
		structObjeto.angulo_1 = getDouble(jsonObjeto,"angulo_1",30);
	}
	if (structObjeto.tipo == TIPO_NULL){
		Logger::getLogger()->debug( "Error validando una figura que no existe, de tipo: " + jsonObjeto.get("tipo", NULL).asString());
	}
	return structObjeto;
}


void Parser::parsearObjetos(Json::Value jsonObjetos, vector<sObjeto>* objetos){
	if (jsonObjetos.isArray()){
		for (int i = 0; i < jsonObjetos.size(); i++) {
			objetos->push_back(parsearObjeto(jsonObjetos[i]));
		}
	}
}


void Parser::parsearEnemigos(Json::Value jsonEnemigos, vector<sObjeto>* objetos){
	if (jsonEnemigos.isArray()){
		for (int i = 0; i < jsonEnemigos.size(); i++) {
			objetos->push_back(parsearEnemigo(jsonEnemigos[i]));
		}
	}
}

sObjeto Parser::parsearEnemigo(Json::Value jsonEnemigo){
	
	sObjeto structEnemigo;
	memset(&structEnemigo,0,sizeof(sObjeto));

	structEnemigo.tipo = this->getTipoEnemigo( jsonEnemigo.get("tipo",NULL).asString() );
	structEnemigo.x = getDouble(jsonEnemigo,"x",5);
	structEnemigo.y = getDouble(jsonEnemigo,"y",90);

	///////Eran necesarios para el SnowBoy, por las dudas las hardcodeo aca/////
	structEnemigo.color = "#00FF00";
	structEnemigo.rot =  0;
	structEnemigo.masa = 10;
	structEnemigo.estatico = false;
	//////////////////////

	Logger::getLogger()->debug( "Se parseo un enemigo");
	return structEnemigo;
}

TIPO_FIGURA Parser::getTipoEnemigo(string tipo){
	if (tipo == DIABLO){
		return TIPO_DIABLO;
	} else if (tipo == DRAGON){
		return TIPO_DRAGON;
	}else  {
		return TIPO_NULL;
	}
}


sObjeto Parser::parsearPersonaje(Json::Value jsonPersonaje){	
    sObjeto structPersonaje;
	memset(&structPersonaje,0,sizeof(sObjeto));

    structPersonaje.x = getDouble(jsonPersonaje, "x", 50);
	structPersonaje.y = getDouble(jsonPersonaje, "y", 50);

	///////Necesarios/////
	structPersonaje.color = "#00FF00";
	structPersonaje.rot =  0;
	structPersonaje.masa = 10;
	structPersonaje.estatico = false;
	//////////////////////

	
    structPersonaje.tipo = TIPO_SNOWBOY;    
    Logger::getLogger()->debug( "Se parseo un personaje");
	return structPersonaje;
}

void Parser::parsearPersonajes( vector<sObjeto>* objetos){
	sObjeto structPersonaje;
	memset(&structPersonaje,0,sizeof(sObjeto));
	int divisor =  Utilities::ANCHO_UL - SNOWBOY_ANCHO_UL;					
	structPersonaje.x = 50;
	structPersonaje.y = 50;

	///////Necesarios/////
	structPersonaje.color = "#00FF00";
	structPersonaje.rot =  0;
	structPersonaje.masa = 10;
	structPersonaje.estatico = false;
	//////////////////////

    structPersonaje.tipo = TIPO_SNOWBOY;    
	for (int i = 0; i < Utilities::MAX_JUGADORES; i++) {
		structPersonaje.x = SNOWBOY_ANCHO_UL + (i*SNOWBOY_ANCHO_UL) + 2;
	    structPersonaje.y =  SNOWBOY_ALTO_UL/2 +5;
		objetos->push_back(structPersonaje);
	}
}

bool Parser::validarEscenarios(Json::Value jsonEscenarios){

	for ( int i = 0; i < jsonEscenarios.size(); ++i ){
		//cout << jsonEscenarios[i];
		bool escenarioValido = this->validarEscenario(jsonEscenarios[i]);
		if (!escenarioValido){
			Logger::getLogger()->error( "No se pudo validar el escenario " + Utilities::int2str(i+1));
			return false;
		}
	}

	return true;
}

bool Parser::validarEscenario(Json::Value jsonEscenario){
	bool result =   validarCampoInt(jsonEscenario, "alto-px") &&
					validarCampoInt(jsonEscenario, "ancho-px") &&
					validarCampoFloat(jsonEscenario, "alto-un") &&
					validarCampoFloat(jsonEscenario, "ancho-un") &&
					validarCampoString(jsonEscenario, "imagen_fondo") &&
					//validarPersonaje(jsonEscenario) &&
					validarObjetos(jsonEscenario) ;
	return result;
}
	
bool Parser::validarPersonajes(Json::Value jsonValue){
	return true;
}
	
bool Parser::validarObjetos(Json::Value jsonValue){
	bool result = jsonValue.isMember("objetos") && jsonValue.get("objetos", NULL).isArray();
	if (!result) {
		Logger::getLogger()->debug( "Error en la validacion del array de objetos, el array no existe o no es un array");
		return false;
	}
	for (int i = 0; i < jsonValue.get("objetos", NULL).size(); i++) {
		bool objetoValido = this->validarObjeto(jsonValue.get("objetos", NULL)[i]);
		if (!objetoValido) {
			Logger::getLogger()->warning("Error en la validacion de la figura: " + Utilities::int2str(i));
		}
		result = result && objetoValido;
	}
	return result;
}
	
bool Parser::validarPersonaje(Json::Value jsonValue){
	bool result = validarCampoCompuesto(jsonValue, "personaje") &&
		(jsonValue.get("personaje", NULL) != NULL) &&
		validarCampoInt(jsonValue.get("personaje", NULL), "x") &&
		validarCampoInt(jsonValue.get("personaje", NULL), "y");
		//Logear que el campo personaje no existe
	if (!result) 	Logger::getLogger()->debug( "Error Validando un personaje");
	return result;
}
	
bool Parser::validarObjeto(Json::Value jsonValue){
	//dependiendo de lo que viene en tipo se valida una u otra cosa
	bool result = validarCampoFloat(jsonValue, "x") &&
					validarCampoFloat(jsonValue, "y") &&
					validarCampoStringHexa(jsonValue, "color") &&
					validarCampoFloat(jsonValue, "rot") &&
					validarCampoFloat(jsonValue, "masa") &&
					validarCampoBool(jsonValue, "estatico");

	TIPO_FIGURA tipo = this->getTipoFigura( jsonValue.get("tipo", NULL).asString() );

	if (tipo == TIPO_RECTANGULO) {
		result = result && validarCampoFloat(jsonValue, "alto") &&
						   validarCampoFloat(jsonValue, "ancho");
	}

	if (tipo == TIPO_CIRCULO) {
		result = result && validarCampoFloat(jsonValue, "radio") ;
	}

	if (tipo == TIPO_POLIGONO) {
		result = result && validarCampoInt(jsonValue, "lados") &&
						   validarCampoFloat(jsonValue, "escala") ;
	}

	if (tipo == TIPO_TRAPECIO) {
		result = result && validarCampoFloat(jsonValue, "alto") &&
						   validarCampoFloat(jsonValue, "ancho") &&
						   //validarCampoInt(jsonValue, "lados") && //el trapecio tiene 4 lados
						   validarCampoFloat(jsonValue, "escala") &&
						   validarCampoFloat(jsonValue, "angulo_1") &&
						   validarCampoFloat(jsonValue, "angulo_2");
	}

	if (tipo == TIPO_PARALELOGRAMO) {
		result = result && validarCampoFloat(jsonValue, "alto") &&
						   validarCampoFloat(jsonValue, "ancho") &&
						   validarCampoFloat(jsonValue, "angulo_1");
	}
	if (!result) 	Logger::getLogger()->warning( "Error validando una figura de tipo: " + jsonValue.get("tipo", NULL).asString());
	if( tipo == TIPO_NULL ) {
		Logger::getLogger()->warning( "Error validando una figura que no existe, de tipo: " + jsonValue.get("tipo", NULL).asString());
		result = false;
	}
	return result;
}

bool Parser::validarCampoInt(Json::Value jsonValue, string campo){
	if (!jsonValue.isMember(campo)){
		Logger::getLogger()->warning( "Error parseando los datos, no existe el campo: " + campo);
		return false;
	}
	if (!jsonValue.get(campo, NULL).isInt()){
		//logear que el campo no era entero
		Logger::getLogger()->warning( "Error parseando los datos, el campo " + campo + " no es de tipo entero");
		return false;
	}
	return true;
}

bool Parser::validarCampoFloat(Json::Value jsonValue, string campo){
	if (!jsonValue.isMember(campo)){
		Logger::getLogger()->warning( "Error parseando los datos, no existe el campo: " + campo);
		return false;
	}
	if (!jsonValue.get(campo, NULL).isDouble() && !jsonValue.get(campo, NULL).isInt()){
		//logear que el campo no era FLOAT
		Logger::getLogger()->warning( "Error parseando los datos, el campo " + campo + " no es de tipo float");
		return false;
	}
	return true;
}

bool Parser::validarCampoString(Json::Value jsonValue, string campo){
	if (!jsonValue.isMember(campo)){
		Logger::getLogger()->warning( "Error parseando los datos, no existe el campo: " + campo);
		return false;
	}
	if (!jsonValue.get(campo, NULL).isString()){
		//logear que el campo no era string
		Logger::getLogger()->warning( "Error parseando los datos, el campo " + campo + " no es de tipo String");
		return false;
	}
	return true;
}

bool Parser::validarCampoStringHexa(Json::Value jsonValue, string campo){
	if (!jsonValue.isMember(campo)){
		Logger::getLogger()->warning( "Error parseando los datos, no existe el campo: " + campo);
		return false;
	}
	if (!jsonValue.get(campo, NULL).isString()){
		//logear que el campo no era string
		Logger::getLogger()->warning( "Error parseando los datos, el campo " + campo + " no es de tipo String");
		return false;
	}
	
	std::string s = jsonValue.get(campo, NULL).asString();
	if (s.length() != 7){
		Logger::getLogger()->warning( "Error parseando los datos, el campo " + campo + " esta mal formado, debe se un hexa de 7 digitos, comenzando por # ");
		return false;
	}
	if (!isHexNotation(s)){	
		Logger::getLogger()->warning( "Error parseando los datos, el campo " + campo + " esta mal formado: " + s);
		return false;
	}
	return true;
}



bool Parser::validarCampoBool(Json::Value jsonValue, string campo){
	if (!jsonValue.isMember(campo)){
		Logger::getLogger()->warning( "Error parseando los datos, no existe el campo: " + campo);
		return false;
	}
	if (!jsonValue.get(campo, NULL).isBool()){
		//logear que el campo no era bool
		Logger::getLogger()->warning( "Error parseando los datos, el campo " + campo + " no es de tipo bool");
		return false;
	}
	return true;
}
bool Parser::validarCampoCompuesto(Json::Value jsonValue, string campo){
	if (!jsonValue.isMember(campo)){
		//Logear que el campo no existe
		Logger::getLogger()->warning( "Error parseando los datos, no existe el campo: " + campo);
		return false;
	}
	if (!jsonValue.get(campo, NULL).isObject()){
		//logear que el campo no era un objeto json
		Logger::getLogger()->warning( "Error parseando los datos, el campo " + campo + " no es un objeto Json");
		return false;
	}
	return true;
}
bool Parser::isHexNotation(std::string const& s)
{
	return s.compare(0, 1, "#") == 0
      && s.size() > 2
      && s.find_first_not_of("0123456789abcdefABCDEF", 1) == std::string::npos;
}

int Parser::getInt(Json::Value jsonValue, string campo, int defValue){
	stringstream ss;
	ss << defValue;
	string def = ss.str();
	if (!jsonValue.isMember(campo)){
		Logger::getLogger()->warning( "Error parseando los datos, no existe el campo: " + campo + " se usa el valor " + def );
		return defValue;
	} else if (!jsonValue.get(campo, NULL).isInt()){
		//logear que el campo no era entero
		Logger::getLogger()->warning( "Error parseando los datos, el campo " + campo + " no es de tipo entero, se usa el valor " + def);
		return defValue;
	} else{
		if (jsonValue.get(campo, NULL).asInt()<0){
				Logger::getLogger()->warning( "Error parseando los datos, el campo " + campo + " es negativo, se cambia el signo del valor ");
				return validarIntSegunCampo(-(jsonValue.get(campo, NULL).asInt()),campo);
			}else{
				//segun el campo, si cumple con las condiciones de ese campo, se pasa el valor del json
				//si no se pasa uno por default
				return validarIntSegunCampo(jsonValue.get(campo, NULL).asInt(),campo);
			}
	}
}

int Parser::validarIntSegunCampo(int valorCampo, string campo){
	
	int valorADevolver=valorCampo;

	if (campo == "lados"){
		if (valorCampo<3){
			Logger::getLogger()->warning( "Error parseando los datos, el campo lados es menor a 3, se usa este valor como valor del campo");
			valorADevolver=3;
		}
		if (valorCampo>6){
			Logger::getLogger()->warning( "Error parseando los datos, el campo lados es mayor a 6, se usa este valor como valor del campo");
			valorADevolver=6;
		}
	}

	if ((campo == "ancho-un")||(campo == "alto-un")){
		if (valorCampo==0){
			if (campo=="ancho-un"){valorADevolver=(structEscenario.ancho_px/10);}
			if (campo=="alto-un"){valorADevolver=(structEscenario.alto_px/10);}
			Logger::getLogger()->warning( "Error parseando los datos, el campo "+campo+" tiene valor cero, se adapta el campo al tamanio de la pantalla");
		}
	}
	if (campo == "ancho-px"){
		if (valorCampo<100){
			valorADevolver=100;
			Logger::getLogger()->warning( "Error parseando los datos, el campo "+campo+" tiene que ser mayor o igual a 100, se establecese establece 100 como valor del campo");
		}
	}
	
	if (campo == "alto-px"){
		if (valorCampo<10){
			valorADevolver=10;
			Logger::getLogger()->warning( "Error parseando los datos, el campo "+campo+" tiene que ser mayor o igual a 10, se establecese establece 10 como valor del campo");
		}
	}

	return valorADevolver;
}

string Parser::getString(Json::Value jsonValue, string campo, string defValue){
	if (!jsonValue.isMember(campo)){
		Logger::getLogger()->warning( "Error parseando los datos, no existe el campo: " + campo + " se usa el valor " + defValue );
		return defValue;
	} else if (!jsonValue.get(campo, NULL).isString()){
		//logear que el campo no era entero
		Logger::getLogger()->warning( "Error parseando los datos, el campo " + campo + " no es de tipo string, se usa el valor " + defValue);
		return defValue;
	} else {
		return jsonValue.get(campo, NULL).asString();
	}
}
string Parser::getColor(Json::Value jsonValue, string campo, string defValue){
	if (!jsonValue.isMember(campo)){
		Logger::getLogger()->warning( "Error parseando los datos, no existe el campo: " + campo + " se usa el valor " + defValue );
		return defValue;
	} else if (!jsonValue.get(campo, NULL).isString()){
		//logear que el campo no era entero
		Logger::getLogger()->warning( "Error parseando los datos, el campo " + campo + " no es de tipo string, se usa el valor " + defValue);
		return defValue;
	} else if (!isHexNotation(jsonValue.get(campo, NULL).asString())) {
		Logger::getLogger()->warning( "Error parseando los datos, el campo " + campo + " no tiene el formato predefinido para los colores, se usa el valor " + defValue);
		return defValue;	
	} else {
		return jsonValue.get(campo, NULL).asString();
	}
}

float Parser::getDouble(Json::Value jsonValue, string campo, long double defValue){
	if (!jsonValue.isMember(campo)){
		Logger::getLogger()->warning( "Error en el parseo, no existe el campo: " + campo + " se usa el valorpor defecto " + std::to_string(defValue));
		return defValue;
	} else if (!jsonValue.get(campo, NULL).isDouble()){
		//logear que el campo no era entero
		Logger::getLogger()->warning( "Error en el parseo, el campo " + campo + " no es de tipo flotante, se usa el valor por defecto " + std::to_string(defValue));
		return defValue;
	} else {
		if (jsonValue.get(campo, NULL).asInt()<0){
				Logger::getLogger()->warning( "Error parseando los datos, el campo " + campo + " es negativo, se cambia el signo del valor ");
				return validarDoubleSegunCampo(-(jsonValue.get(campo, NULL).asDouble()), campo);
			}else{
				return validarDoubleSegunCampo(jsonValue.get(campo, NULL).asDouble(), campo);
			}
	}
}

float Parser::validarDoubleSegunCampo(float valorCampo, string campo){

	float valorADevolver=valorCampo;
	
	if (campo == "x"){
		if ((valorCampo==0)||(valorCampo>=structEscenario.ancho_ul)){
			Logger::getLogger()->warning( "Error parseando los datos, el campo x no se encuentra en los limites del escenario, se establece el centro del escenario como valor de x");
			valorADevolver=structEscenario.ancho_ul/2;
		}
	}
	if ((campo == "escala")||(campo == "masa")||(campo == "ancho")||(campo == "alto")||(campo == "radio")){
		if (valorCampo==0){
			ostringstream valorOSS;
			string x="";
			valorADevolver=1;
			valorOSS<<valorADevolver;
			Logger::getLogger()->warning( "Error parseando los datos, el campo "+campo +" es igual a cero, se establece "+valorOSS.str()+" como valor del campo");
			
		}
	}

	if (campo == "rot"){
		if (valorCampo>359){
			//nunca va a ser menor a cero porque se le cambia el signo si es negativo
			valorADevolver=valorCampo;
			while (valorADevolver>359){
				valorADevolver-=360;
			}
			ostringstream valorOSS;
			string x="";
			valorOSS<<valorADevolver;
			Logger::getLogger()->warning( "Error parseando los datos, el campo "+campo +" debe estar entre 0 y 359, se modifica el valor para que este dentro de este rango, se establece "+valorOSS.str()+ " como valor del campo");
		}
	}

	
	return valorADevolver;
}






bool Parser::getBool(Json::Value jsonValue, string campo, bool defValue){
	string def = defValue ? "true" : "false";
	if (!jsonValue.isMember(campo)){
		Logger::getLogger()->warning( "Error en el parseo, no existe el campo: " + campo + " se usa el valor por defecto " + def);
		return defValue;
	} else if (!jsonValue.get(campo, NULL).isBool()){
		//logear que el campo no era entero
		Logger::getLogger()->warning( "Error en el parseo, el campo " + campo + " no es de tipo booleano, se usa el valor: por defecto " + def);
		return defValue;
	} else {
		return jsonValue.get(campo, NULL).asBool();
	}
}