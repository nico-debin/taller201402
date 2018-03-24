#include "ControlSonido.h"


bool ControlSonido::init = false;

Sonido ControlSonido::sonido;

ControlSonido::ControlSonido() {

    if (init == false)
    {
        this->iniciar();
    }
}

void ControlSonido::tocar(ControlRemoto controlRemoto, Temas tema){    

    if (tema == Temas::MUSICA)
    {
       if (Mix_PlayingMusic() == 0)
        {
           Mix_PlayMusic(sonido.getMixMusic(tema), -1);
        }
        else
        {
            if (Mix_PausedMusic() == 1)
            {
                Mix_ResumeMusic();
            }
            else
            {
                switch (controlRemoto)
                {
                    case ControlRemoto::PARAR : 
                    {
                       Mix_HaltMusic();
                        break;
                    }
					case ControlRemoto::PAUSA :
                    {                       
                        Mix_PauseMusic();
                        break;
                    }
                }
            }
        }
    }

	else if (controlRemoto == ControlRemoto::SINSONIDO)
    {
        // detiene el sonido
        Mix_HaltMusic();
        sonido.close(tema);
    }
	else {
        Mix_PlayChannel(-1, sonido.getMixChunk(tema), 0);
    }
}

void ControlSonido::iniciar() {

    if (!sonido.init())
    {
        printf("Error: no se inicio el sonido.\n");
    }
    else
    {
		/*
        if (!sonido.loadMedia())
        {
            printf("Error: no se cargo el sonido.\n");
        }
		*/
        init = true;
    }
}

ControlSonido::~ControlSonido(){}
