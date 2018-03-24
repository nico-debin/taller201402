#ifndef _ControlSonido_H_
#define _ControlSonido_H_

#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <string>
#include "Sonido.h"

enum ControlRemoto {
	REPRODUCIR, PARAR, PAUSA, SINSONIDO
};
/*
Para llamarlo
controlSonido.tocar(ControlRemoto::REPRODUCIR, Temas::MUSICA);
controlSonido.tocar(ControlRemoto::REPRODUCIR, Temas::BOMBA2);
controlSonido.tocar(ControlRemoto::PARAR, Temas::MUSICA);

	*/
class ControlSonido
{
private: 
    static bool init;
    static Sonido sonido;
	void iniciar();

public:
	ControlSonido();

    void tocar(ControlRemoto controlRemoto, Temas tema);    

	~ControlSonido();
};

#endif