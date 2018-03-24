#pragma once
#ifndef _LOGGER_H_
#define _LOGGER_H_
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>


/**
 * esta es una macro que es reemplazada por el precompilador
 * y muestra el nombre del archivo
 */
#define LINE_LOG_FILENAME __FILE__
/**
 * indica el nombre de un archivo de log por defecto
 */
#define DEFAUL_LOGNAME "./log.txt"

/**
 * los niveles de log son incrementales y restrictivos a medida que se incrementa
 * si se selecciona el 1ero incluye a los demas
 * pero si se seleccionar por ejemplo INFO_LOG
 * incluira INFO_LOG, WARN_LOG, ERROR_LOG, FATAL_LOG
 * pero no DEBUG_LOG
 * la relacion es esta
 * DEBUG < INFO < WARN < ERROR < FATAL
 */
enum level_log
{
	ALL_LOG,
	DEBUG_LOG,
	INFO_LOG,
	WARN_LOG,
	ERROR_LOG,
	FATAL_LOG
};

using namespace std;
/**
 * esta clase se maneja como un singlenton
 * cada mensaje recibe dos textos, el mensaje en si y un texto alternativo
 * se imprime la fecha completa segun este formato
 * %Y-%m-%d %H:%M:%S
 * por ejemplo
 * 2013-04-19 14:32:48	[INFO]	mensaje		data
 * 
 */
class Logger
{
public:
	virtual ~Logger();
	void debug(string message);
	void debug(string dataText, string message);
	void info(string message);
    void info(string dataText, string message);
	void warning(string message);
	void warning(string dataText, string message);
	void warn(string message);
	void error(string message);
	void error(string dataText, string message);
	void fatal(string message);
	void fatal(string dataText, string message);
	
	void setFileName(string fileName);
	void openLogger();
	void closeLogger();

	static Logger* getLogger();
	
	static void setLevel(enum level_log level);
	void _setLevel(enum level_log level);

	static void setPrintScreen(bool);
	void _setPrintScreen(bool);

	string intToString(int data);
	string longToString(long data);
	string floatToString(float data);
	string doubleToString(double data);	
private:
	Logger();
	static Logger* instance;
	enum level_log lgLevel;
	void log(string level, string dataText, string message);
	ofstream logFile;
	string	fileName;
	bool printScreen;

	void removeFile();

};

#endif /*_LOGGERCUSTOM_H_*/