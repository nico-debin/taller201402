#include "stdafx.h"

#include "ConditionVariable.h"

ConditionVariable::ConditionVariable(Lock* lock){
	InitializeConditionVariable(&this->handle);
	this->lock = lock;
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