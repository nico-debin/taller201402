#pragma once

#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <string>
#include <map>

#define MAX 8

using namespace std;

enum Temas {
    MUSICA, BOMBA1,BOMBA2, BOMBA3, SALTO, MURIENDO, BONUS
};

const string archTemas[] = { "MusicaSnowBros.mp3", "Bomba1.wav", "Bomba2.wav", "Bomba3.wav", "Salto.wav", "Muriendo.wav", "Bonus4.wav" };

class Sonido
{
private:

    Mix_Chunk *chunks[MAX];
	Mix_Music *musica;
	
	Mix_Music* getSonidoCargado(string);
	Mix_Chunk* getChunkCargado(string);
	map<string,Mix_Music*> sonidosCargados;
	map<string,Mix_Chunk*> chunksCargados;

public:

    Mix_Chunk * getMixChunk(Temas tema);
	Mix_Music * getMixMusic(Temas tema);
	Sonido();
	bool init();
	Mix_Chunk* loadMedia(Temas tema);
	void close(Temas tema);	
	~Sonido();

};