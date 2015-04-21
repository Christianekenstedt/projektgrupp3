//
//  main.c
//  prototyp1
//
//  Created by Christian Ekenstedt on 2015-04-16.
//  Copyright (c) 2015 Grupp 3. All rights reserved.
//

//Using SDL and standard IO
#include <SDL2/SDL.h>
#include <SDL2_net/SDL_net.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_mixer/SDL_mixer.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void closeW();

// The music woll be played
Mix_Music *gMusic = NULL;

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

//The image we will load and show on the screen
SDL_Surface* gXOut = NULL;

int main( int argc, char* args[] ){
    //Start up SDL and create window
    if( !init() ){
        printf( "Failed to initialize!\n" );
    }else{
        //Load media
        if( !loadMedia() ){
            printf( "Failed to load media!\n" );
        }else{
            //Main loop flag
            bool quit = false;
            
            //Event handler
            SDL_Event e;
            
            //While application is running
            while( !quit ){
                //Handle events on queue
                while( SDL_PollEvent( &e ) != 0 ){
                    //User requests quit
                    if( e.type == SDL_QUIT ){
                        quit = true;
                    }
                }
                //Apply the image
                SDL_BlitSurface( gXOut, NULL, gScreenSurface, NULL );
                //Update the surface
                SDL_UpdateWindowSurface( gWindow );
            }
        }
    }
    //Free resources and close SDL
    closeW();
    return 0;
}

bool init()
{
    //Initialization flag
    bool success = true;
    
    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0 )
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {
        //Create window
        gWindow = SDL_CreateWindow( "Hello, World!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( gWindow == NULL )
        {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            //Get window surface
            gScreenSurface = SDL_GetWindowSurface( gWindow );
        }
        //Initialize SDL_mixer
        if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
        {
            printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
            success = false;
        }
    }
    
    return success;
}

bool loadMedia()
{
    //Loading success flag
    bool success = true;
    
    //Load splash image
    gXOut = IMG_Load( "bilder/bild.png" );
    if( gXOut == NULL )
    {
        printf( "Unable to load image %s! SDL Error: %s\n", "bilder/bild.png", SDL_GetError() );
        success = false;
    }
    //Load music
    gMusic = Mix_LoadMUS( "musik/bg.wav" );
    if( gMusic == NULL )
    {
        printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }
    
    return success;
}

void closeW()
{
    //Deallocate surface
    SDL_FreeSurface( gXOut );
    gXOut = NULL;
    
    //Free the music
    Mix_FreeMusic( gMusic );
    gMusic = NULL;
    
    //Destroy window
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    
    //Quit SDL subsystems
    SDL_Quit();
}