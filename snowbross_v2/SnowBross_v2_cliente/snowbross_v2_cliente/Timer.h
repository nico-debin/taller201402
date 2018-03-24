#pragma once
#ifndef _Timer_H_
#define _Timer_H_

#include <SDL2\SDL.h>

class Timer{

	public:
		static Timer* getTimer();

		bool isStarted();
		bool isPaused();

		Uint32 getTicks();

        void start();
        void stop();
        void pause();
        void unpause();

	private:
		Timer();
		static Timer* instance;

		// Ticks cuando se hace un start
		Uint32 startTicks;

		// Ticks cuando se hace pausa
		Uint32 pausedTicks;

		bool iniciado;
		bool enPausa;
};

#endif