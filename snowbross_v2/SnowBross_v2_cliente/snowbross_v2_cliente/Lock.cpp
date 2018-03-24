#include "stdafx.h"
#include "Lock.h"
#include "Logger.h"

//Lock* Lock::instance = nullptr;
//
//Lock* Lock::getLock(){
//	if (Lock::instance == nullptr){
//		Lock::instance = new Lock();
//	}
//	return Lock::instance;
//}
Lock::~Lock(){
	DeleteCriticalSection(&this->lock);
}

bool Lock::isLocked(){
	return this->locked;
}

Lock::Lock(){
	InitializeCriticalSection(&this->lock);
	this->locked = false;
}

void Lock::start(){
	//if(this->locked == false){
		EnterCriticalSection(&this->lock);
		//this->locked = true;
	//}
}

void Lock::stop(){
	//if(this->locked == true){
		LeaveCriticalSection(&this->lock);
		//this->locked = false;
	//}
}

CRITICAL_SECTION Lock::getHandle(){
	return this->lock;
}