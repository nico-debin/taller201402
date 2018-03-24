// proyecto-base.cpp : Defines the entry point for the application.


#include "stdafx.h"
#include <iostream>

#include "test.SDL.h"
#include "test.BOX2D.h"
#include "test.JSON.h"
#include "test.Logger.h"

int main(int argc, char **argv){
	int status = 0;

	status = test_SDL();
	if(status != 0){
		std::cout << "Error en SDL" << std::endl;
	}

	status = test_SDL_mixer();
	if(status != 0){
		std::cout << "Error en SDL Mixer" << std::endl;
	}

	status = test_BOX2D();
	if(status != 0){
		std::cout << "Error en BOX2D" << std::endl;
	}

	status = test_JSON();
	if(status != 0){
		std::cout << "Error en JSON" << std::endl;
	}

	status = test_Logger();
	if(status != 0){
		std::cout << "Error en Logger" << std::endl;
	}

	system("pause");
    return status;
}