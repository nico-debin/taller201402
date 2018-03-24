#include "stdafx.h"

#include "ConditionVariable.h"

ConditionVariable::ConditionVariable(){
	InitializeConditionVariable(&this->handle);
	this->lock = new Lock();
}
ConditionVariable::ConditionVariable(Lock* lock){
	InitializeConditionVariable(&this->handle);
	this->lock = lock;
}
ConditionVariable::~ConditionVariable(){
	delete this->lock;
	this->lock = nullptr;
}

void ConditionVariable::wake(){
	WakeConditionVariable(&this->handle);
}

void ConditionVariable::wakeAll(){
	WakeAllConditionVariable(&this->handle);
}

void ConditionVariable::sleep(){
	SleepConditionVariableCS(&this->handle, &this->lock->getHandle(), INFINITE);
}

Lock* ConditionVariable::getLock(){
	return this->lock;
}