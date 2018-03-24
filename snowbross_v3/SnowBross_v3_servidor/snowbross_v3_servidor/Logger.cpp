#include "stdafx.h"
#include "Logger.h"

Logger* Logger::instance = NULL;

Logger* Logger::getLogger(){
	if (Logger::instance == NULL){
		Logger::instance = new Logger();
	}
	return Logger::instance;
}
Logger::~Logger(){
	this->closeLogger();
}
void Logger::setLevel(enum level_log level){
	Logger::getLogger()->_setLevel(level);
}
void Logger::_setLevel(enum level_log level){
	this->lgLevel = level;
}
void Logger::setPrintScreen(bool printScreen){
	Logger::getLogger()->_setPrintScreen(printScreen);
}
void Logger::_setPrintScreen(bool printScreen){
	this->printScreen = printScreen;
}
void Logger::debug(string message) {
	this->debug("",  message);
}

void Logger::debug(string dataText, string message) {
	if (this->lgLevel <= DEBUG_LOG)
		this->log("DEBUG",dataText,message);
}

void Logger::info(string message) {
	this->info("",  message);
}

void Logger::info(string dataText, string message) {
	if (this->lgLevel <= INFO_LOG)
		this->log("INFO",dataText,message);
}

void Logger::warning(string message) {
	this->warning("",  message);
}

void Logger::warning(string dataText, string message) {
	if (this->lgLevel <= WARN_LOG)
		this->log("WARN",dataText,message);
}

void Logger::warn(string message) {
	this->warning(message);
}

void Logger::error(string message) {
	this->error("",  message);
}

void Logger::error(string dataText, string message) {
	if (this->lgLevel <= ERROR_LOG)
		this->log("ERROR",dataText,message);
}

void Logger::fatal(string message) {
	this->fatal("",  message);
}

void Logger::fatal(string dataText, string message) {
	if (this->lgLevel <= FATAL_LOG)
		this->log("FATAL",dataText,message);
}

void Logger::setFileName(string fileName){
	this->fileName = fileName;
}

void Logger::log(string level, string dataText, string message) {
	this->openLogger();
	time_t now = time(NULL);
	struct tm * ptm = localtime(&now);
	char timestamp[32];
	clock_t milisec = clock();
	strftime (timestamp, 32, "%Y-%m-%d %H:%M:%S", ptm);

	//this->logFile << timestamp <<" "<< (float)milisec<< "\t|[" << level << "]|\t" << dataText << "|\t" << message << endl;
	this->logFile << timestamp <<" "<< (float)milisec<< "| [" << level << "]| " << dataText << "| " << message << endl;
	//if(this->printScreen) cout  << timestamp <<"\t|[" << level << "]|\t" << dataText << "|\t" << message << endl;
	if(this->printScreen) cout  << timestamp <<"| [" << level << "]| " << dataText << "| " << message << endl;

	this->logFile.flush();
}

Logger::Logger(){
	string fname(DEFAUL_LOGNAME);
	this->fileName.assign(fname);
	this->lgLevel = ALL_LOG;
	this->removeFile();
	this->_setPrintScreen(true);
}

void Logger::openLogger() {
	if (!this->logFile.is_open()){
		this->logFile.open(this->fileName.c_str(), fstream::in | fstream::out | fstream::app);
		if (!this->logFile.is_open())
			this->logFile.open(this->fileName.c_str(), fstream::in | fstream::out | fstream::trunc);
	}
}
void Logger::closeLogger() {
	this->logFile.flush();
	this->logFile.close();
}

void Logger::removeFile(){
	remove(DEFAUL_LOGNAME);
}

string Logger::intToString(int data){
   stringstream ss;
   ss << data;
   return ss.str();
}

string Logger::longToString(long data){
   stringstream ss;
   ss << data;
   return ss.str();
}
	
string Logger::floatToString(float data){
   stringstream ss;
   ss << data;
   return ss.str();	
}

string Logger::doubleToString(double data){
   stringstream ss;
   ss << data;
   return ss.str();	
}
	
