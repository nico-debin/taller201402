#include "Timer.h"

//Timer* Timer::instance = NULL;
//
//Timer* Timer::getTimer(){
//	if (Timer::instance == NULL){
//		Timer::instance = new Timer();
//	}
//	return Timer::instance;
//}

Timer::Timer(){
    this->startTicks  = 0;
    this->pausedTicks = 0;

    this->iniciado = false;
    this->enPausa  = false;
}

bool Timer::isStarted(){
	return this->iniciado;
}
bool Timer::isPaused(){
	return this->enPausa;
}

void Timer::start(){
    this->iniciado = true;
    this->enPausa  = false;

    this->startTicks  = SDL_GetTicks();
    this->pausedTicks = 0;
}

void Timer::stop(){
    this->iniciado = false;
    this->enPausa  = false;

    this->startTicks  = 0;
    this->pausedTicks = 0;
}

void Timer::pause(){
    if( this->iniciado && !this->enPausa ){
        this->enPausa = true;

        this->pausedTicks = SDL_GetTicks() - this->startTicks;
        this->startTicks = 0;
    }
}

void Timer::unpause(){
    if( this->iniciado && this->enPausa ){
        this->enPausa = false;

        this->startTicks = SDL_GetTicks() - this->pausedTicks;

        this->pausedTicks = 0;
    }
}

Uint32 Timer::getTicks(){
    Uint32 time = 0;

    if( this->iniciado ){
        if( this->enPausa ){
            time = this->pausedTicks;
        }else{
            time = SDL_GetTicks() - this->startTicks;
        }
    }

    return time;
}