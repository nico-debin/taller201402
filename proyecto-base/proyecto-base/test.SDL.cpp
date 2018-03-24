// http://www.willusher.io/pages/sdl2/

#include "stdafx.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#include "SDLFunctions.h"

const int SCREEN_WIDTH  = 640;
const int SCREEN_HEIGHT = 480;
const int TILE_SIZE = 40;

int test_SDL(){

	// Test SDL_Image
	if ((IMG_Init(IMG_INIT_JPG) & IMG_INIT_JPG) != IMG_INIT_JPG){
		logSDLError("SDL_Image");
		SDL_Quit();
		return 1;
	}

	// Test SDL_Image
	if (TTF_Init() != 0){
		logSDLError("TTF_Init");
		SDL_Quit();
		return 1;
	}
	

    //Start SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0){
		logSDLError("SDL_Init");
		return 1;
	}
    
	//SDL_Window* pantalla = SDL_CreateWindow("Proyecto Base",100,100,800,600,SDL_WINDOW_RESIZABLE);
	SDL_Window *pantalla = SDL_CreateWindow("Proyecto Base", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (pantalla == nullptr){
		logSDLError("SDL_CreateWindow");
		SDL_Quit();
		return 1;
	}

	SDL_Renderer* renderer = SDL_CreateRenderer(pantalla,-1,SDL_RENDERER_SOFTWARE);
	if (renderer == nullptr){
		SDL_DestroyWindow(pantalla);
		logSDLError("SDL_CreateRenderer");
		SDL_Quit();
		return 1;
	}

	/*
	SDL_Texture *background = loadTexture("background.png", renderer);
	SDL_Texture *image = loadTexture("image.png", renderer);
	//Make sure they both loaded ok
	if (background == nullptr || image == nullptr){
		//cleanup(background, image, renderer, pantalla);
		IMG_Quit();
		SDL_Quit();
		return 1;
	}
	*/
	
	SDL_Texture *background = loadTexture(  "imagenes/background.png", renderer);
	SDL_Texture *image = loadTexture("imagenes/image.png", renderer);
	//Make sure they both loaded ok
	if (background == nullptr || image == nullptr){
		//cleanup(background, image, renderer, window);
		IMG_Quit();
		SDL_Quit();
		return 1;
	}

	//Determine how many tiles we'll need to fill the screen
	int xTiles = SCREEN_WIDTH / TILE_SIZE;
	int yTiles = SCREEN_HEIGHT / TILE_SIZE;

	//Draw the tiles by calculating their positions
	for (int i = 0; i < xTiles * yTiles; ++i){
		int x = i % xTiles;
		int y = i / xTiles;
		renderTexture(background, renderer, x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE,
			TILE_SIZE);
	}

	int iW, iH;
	SDL_QueryTexture(image, NULL, NULL, &iW, &iH);
	int x = SCREEN_WIDTH / 2 - iW / 2;
	int y = SCREEN_HEIGHT / 2 - iH / 2;
	renderTexture(image, renderer, x, y);
	SDL_RenderPresent(renderer);
	SDL_Delay(2000);
	
	/*
	//SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, bmp);
	SDL_Texture *tex = loadTexture(imagePath, renderer);
	SDL_FreeSurface(img);
	if (tex == nullptr){
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(pantalla);
		std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}
	*/
	/*
	SDL_RenderClear(renderer);
	//SDL_RenderCopy(renderer, tex, NULL, NULL);
	SDL_RenderPresent(renderer);
	
	//SDL_Delay(2000);


	SDL_Event evento;


	while(true){
		SDL_RenderClear(renderer); 

		while(SDL_PollEvent(&evento)){

			if (evento.type == SDL_QUIT){
				//SDL_DestroyTexture(tex);
				SDL_DestroyRenderer(renderer);
				SDL_DestroyWindow(pantalla);
				SDL_Quit(); 
				return 0;
			}	

		}
	}
	*/
    //Quit SDL
    SDL_Quit();
    
    return 0;    
}



/* Mix_Music actually holds the music information.  */
Mix_Chunk *phaser = NULL;
Mix_Music *music = NULL;

int phaserChannel = -1;

void handleKey(SDL_KeyboardEvent key);
void musicDone();

int test_SDL_mixer(void) {

  SDL_Window *screen;
  SDL_Event event;
  int done = 0;

  /* Same setup as before */
  int audio_rate = 22050;
  Uint16 audio_format = AUDIO_S16; 
  int audio_channels = 2;
  int audio_buffers = 4096;

  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

  if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers)) {
    printf("Unable to open audio!\n");
    exit(1);
  }

  /* Pre-load sound effects */
  phaser = Mix_LoadWAV("sonidos/phaser.wav");

  screen = SDL_CreateWindow("SDL Mixer test (teclas: M, P)", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 320, 240, SDL_WINDOW_SHOWN);

  while(!done) {
    while(SDL_PollEvent(&event)) {
      switch(event.type) {
      case SDL_QUIT:
	done = 1;
	break;
      case SDL_KEYDOWN:
      case SDL_KEYUP:
	handleKey(event.key);
	break;
      }
    }
    SDL_Delay(50);

  }

  Mix_CloseAudio();
  SDL_Quit();

  return 0;
}

void handleKey(SDL_KeyboardEvent key) {
  switch(key.keysym.sym) {
  case SDLK_p:

    if(key.type == SDL_KEYDOWN) {

      if(phaserChannel < 0) {
	phaserChannel = Mix_PlayChannel(-1, phaser, -1);
      }
    } else {
      Mix_HaltChannel(phaserChannel);
      
      phaserChannel = -1;
    }
    break;
  case SDLK_m:
    if(key.state == SDL_PRESSED) {

      if(music == NULL) {
	
	music = Mix_LoadMUS("sonidos/music.ogg");
	Mix_PlayMusic(music, 0);
	Mix_HookMusicFinished(musicDone);
	
      } else {
	Mix_HaltMusic();
	Mix_FreeMusic(music);
	music = NULL;
      }
    }
    break;
  }
}

/* This is the function that we told SDL_Mixer to call when the music
   was finished. In our case, we're going to simply unload the music
   as though the player wanted it stopped.  In other applications, a
   different music file might be loaded and played. */
void musicDone() {
  Mix_HaltMusic();
  Mix_FreeMusic(music);
  music = NULL;
}