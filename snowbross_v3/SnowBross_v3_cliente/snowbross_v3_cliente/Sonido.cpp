#include "Sonido.h"


Sonido::Sonido(){
	
   musica = NULL;
  
   for (int i = 0; i < MAX; ++i){
        chunks[i] = NULL;
    }

}

Mix_Chunk * Sonido::getMixChunk(Temas tema) {

    return this->loadMedia(tema);
}

Mix_Chunk* Sonido::getChunkCargado(std::string path){
	if( this->chunksCargados.find(path) == this->chunksCargados.end() ){
		return nullptr;
	}
	return this->chunksCargados[path];
}
Mix_Music* Sonido::getSonidoCargado(std::string path){
	if( this->sonidosCargados.find(path) == this->sonidosCargados.end() ){
		return nullptr;
	}
	return this->sonidosCargados[path];
}

Mix_Music * Sonido::getMixMusic(Temas tema) {
	string archivo = "sonido/" + archTemas[tema];
	Mix_Music* music = this->getSonidoCargado(archivo);
	if(music == nullptr){
		music = Mix_LoadMUS(archivo.c_str());
		this->sonidosCargados[archivo] = music;
	}
    return music;
}

bool Sonido::init()
{
	bool success = true;			
		
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
		{
			printf("SDL_mixer no puede iniciarse: %s\n", Mix_GetError());
			success = false;
		}

	return success;
}

Mix_Chunk* Sonido::loadMedia(Temas tema)
{
	string archivo = "sonido/" + archTemas[tema] ;

	
	Mix_Chunk* chunk = this->getChunkCargado(archivo);
	if(chunk == nullptr){
		chunk = Mix_LoadWAV(archivo.c_str());
		this->chunksCargados[archivo] = chunk;
	}

    chunks[tema] = chunk;
  
	return chunk;
}

void  Sonido::close(Temas tema)
{
	
 /*   for (int i = 0; i < MAX; ++i)
    {
        Mix_FreeChunk(chunks[i]);
    }
*/		
	
	//Mix_FreeChunk(chunks[tema]);
	
}

Sonido::~Sonido(){
	printf("Entra en el destructor de Sonido------------------------\n");
	//Mix_FreeMusic(musica);
	//delete musica;
	//Mix_Quit();
	//this->close();
}
