//
//  main.c
//  prototyp1
//
//  Created by Christian Ekenstedt on 2015-04-16.
//  Copyright (c) 2015 Grupp 3. All rights reserved.
//

#include "multiOS.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

struct playButton{
    int xPos;
    int yPos;
    int wPos;
    int hPos;
};
typedef struct playButton Play;
struct exitButton{
    int xPos;
    int yPos;
    int wPos;
    int hPos;
};
typedef struct exitButton Exit;

//Screen dimension constants
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 800;



void ClearScreen();

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
SDL_Surface* gPlayButton = NULL;
SDL_Surface* gExitButton = NULL;
SDL_Surface* table = NULL;
SDL_Texture* bTexture=NULL;
SDL_Texture* mPlayButton = NULL;
SDL_Texture* exitTexture = NULL;
SDL_Texture* betTexture = NULL;
SDL_Texture* clearTexture = NULL;
SDL_Texture* hitTexture = NULL;
SDL_Texture* standTexture = NULL;
SDL_Texture* doubleTexture = NULL;
SDL_Texture* splitTexture = NULL;
SDL_Renderer* gRenderer = NULL;
SDL_Texture* btable = NULL;

SDL_Rect gSpriteClips[3];
SDL_Rect ExitRect;

//=============================================MAIN==================================================
int main( int argc, char* args[] ){
    int window = 0; // Vilken Window som skall visas, main är 0.
    int frame = 0;

    Play button, BetButton, ClearButton, HitButton, StandButton, DoubleButton, SplitButton;

    SDL_Rect poss;
    poss.y = 546;
    poss.x = 493;
    poss.w = 294;
    poss.h = 107;

    // Play Button
    button.yPos = 546;
    button.xPos = 493;
    button.wPos = 252;
    button.hPos = 107;
    // Exit Button
    ExitRect.y = 0;
    ExitRect.x = 0;
    ExitRect.w = 80;
    ExitRect.h = 40;
    // Bet Button
    BetButton.yPos = 584;
    BetButton.xPos = 54;
    BetButton.wPos = 81;
    BetButton.hPos = 56;
    // Clear Button
    ClearButton.yPos = 584;
    ClearButton.xPos = 640;
    ClearButton.wPos = 86;
    ClearButton.hPos = 56;
    // Hit Button
    HitButton.yPos = 685;
    HitButton.xPos = 968;
    HitButton.wPos = 68;
    HitButton.hPos = 53;
    // Stand Button
    StandButton.yPos = 685;
    StandButton.xPos = 1077;
    StandButton.wPos = 74;
    StandButton.hPos = 53;
    // Double Button
    DoubleButton.yPos = 485;
    DoubleButton.xPos = 1180;
    DoubleButton.wPos = 80;
    DoubleButton.hPos = 40;
    // Split Button
    SplitButton.yPos = 580;
    SplitButton.xPos = 1180;
    SplitButton.wPos = 80;
    SplitButton.hPos = 45;

    //Event handler
    SDL_Event e;
    //Start up SDL and create window
    if( !init() ){
        printf( "Failed to initialize!\n" );
    }else{
        //Load media
        if( !loadMedia() ){
            printf( "Failed to load media!\n" );
        }else{
            //Main loop flag
            //Play the music
            Mix_PlayMusic( gMusic, -1 );

            SDL_SetRenderDrawColor(gRenderer, 0xFF,0xFF,0xFF,0xFF);
            SDL_RenderClear(gRenderer);
        }
    }
    //While application is running

    bool quit = false;
    int x=0, y=0;

    while( !quit ){
                frame = 0;
            //Handle events on queue
            while( SDL_PollEvent( &e ) != 0 ){
                ClearScreen();
                SDL_GetMouseState(&x,&y);
                printf("x: %d\ny: %d\n",x,y);

                //User requests quit
                if( e.type == SDL_QUIT ){
                    quit = true;
                }//Handle key press
                else if( e.type == SDL_KEYDOWN )
                {
                    switch( e.key.keysym.sym )
                    {
                        case SDLK_9:
                            //If there is no music playing
                            if( Mix_PlayingMusic() == 0 )
                            {
                                //Play the music
                                Mix_PlayMusic( gMusic, -1 );
                            }
                            //If music is being played
                            else
                            {
                                //If the music is paused
                                if( Mix_PausedMusic() == 1 )
                                {
                                    //Resume the music
                                    Mix_ResumeMusic();
                                }
                                //If the music is playing
                                else
                                {
                                    //Pause the music
                                    Mix_PauseMusic();
                                }
                            }
                            break;

                            case SDLK_0:
                            //Stop the music
                            Mix_HaltMusic();
                            break;
                            case SDLK_UP:
                                printf("Key UP\n");
                                break;
                            case SDLK_DOWN:
                                printf("Key DOWN\n");
                                break;
                            case SDLK_RIGHT:
                                printf("Key RIGHT\n");
                                break;
                            case SDLK_LEFT:
                                printf("Key LEFT\n");
                                break;
                        }
                    }else if(e.type == SDL_MOUSEBUTTONDOWN){
                        if((x>button.xPos) && ( x < button.yPos + button.wPos ) && ( y > button.yPos) && (y < button.yPos + button.hPos)){
                            frame=1;
                            window=1;
                        }else if((x>ExitRect.x) && ( x < ExitRect.y + ExitRect.w ) && ( y > ExitRect.y) && (y < ExitRect.y + ExitRect.h)){
                            quit = true;
                        }else if((x>BetButton.xPos) && ( x < BetButton.yPos + BetButton.wPos ) && ( y > BetButton.yPos) && (y < BetButton.yPos + BetButton.hPos)){
                            quit = true;
                        }
                        else if((x>ClearButton.xPos) && ( x < ClearButton.yPos + ClearButton.wPos) && ( y > ClearButton.yPos) && (y < ClearButton.yPos + ClearButton.hPos)){
                            quit = true;
                        }
                        else if((x>HitButton.xPos) && ( x < HitButton.yPos + HitButton.wPos) && ( y > HitButton.yPos) && (y < HitButton.yPos + HitButton.hPos)){
                            quit = true;
                        }
                        else if((x>StandButton.xPos) && ( x < StandButton.yPos + StandButton.wPos) && ( y > StandButton.yPos) && (y < StandButton.yPos + StandButton.hPos)){
                            quit = true;
                        }
                        else if((x>DoubleButton.xPos) && ( x < DoubleButton.yPos + DoubleButton.wPos) && ( y > DoubleButton.yPos) && (y < DoubleButton.yPos + DoubleButton.hPos)){
                            quit = true;
                        }
                        else if((x>SplitButton.xPos) && ( x < SplitButton.yPos + SplitButton.wPos) && ( y > SplitButton.yPos) && (y < SplitButton.yPos + SplitButton.hPos)){
                            quit = true;
                        }
                    }else if(e.type == SDL_MOUSEMOTION){
                        if((x>button.xPos) && ( x < button.yPos + button.wPos ) && ( y > button.yPos) && (y < button.yPos + button.hPos)){ //Innanför knappen?
                            frame=2;
                        }
                    }
                    if(window == 0 ){
                        SDL_RenderCopy(gRenderer,bTexture,NULL,NULL);
                        SDL_RenderCopy(gRenderer,mPlayButton,&gSpriteClips[frame],&poss);
                        SDL_RenderCopy(gRenderer,exitTexture,NULL,&ExitRect);
                        SDL_RenderPresent(gRenderer);
                    }else if (window==1){
                        SDL_RenderCopy(gRenderer, btable, NULL, NULL);
                        SDL_RenderCopy(gRenderer, betTexture, NULL, &BetButton);
                        SDL_RenderCopy(gRenderer, clearTexture, NULL, &ClearButton);
                        SDL_RenderCopy(gRenderer, hitTexture, NULL, &HitButton);
                        SDL_RenderCopy(gRenderer, standTexture, NULL, &StandButton);
                        SDL_RenderCopy(gRenderer, doubleTexture, NULL, &DoubleButton);
                        SDL_RenderCopy(gRenderer, splitTexture, NULL, &SplitButton);
                        SDL_RenderCopy(gRenderer, exitTexture, NULL, &ExitRect);
                        SDL_RenderPresent(gRenderer);
                        /*SDL_Delay(5000);
                        window = 0;*/
                    }

                }
        }
    //Free resources and close SDL
    closeW();
    return 0;
}
//==============================================INIT=================================================
bool init(){
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0 ){
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        success = false;
    }else{
        //Create window
        gWindow = SDL_CreateWindow( "♦︎♥︎♣︎♠︎ Blackjack by RoyalKing Casino ♦︎♥︎♣︎♠︎", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( gWindow == NULL ){
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
            success = false;
        }else{
            //Get window surface
            //gScreenSurface = SDL_GetWindowSurface( gWindow );
        }
        //Initialize SDL_mixer
        if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ){
            printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
            success = false;
        }
    }
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    if(gRenderer == NULL){
        fprintf(stderr,"SDL_CreateRenderer: %s\n", SDL_GetError());
        success = false;
    }
    return success;
}
//============================================LOAD MEDIA================================================
bool loadMedia(){
    //Loading success flag
    bool success = true;

    #ifdef _WIN32
    //Load splash image
    table = IMG_Load("bilder\\TABLE.png");
    btable = SDL_CreateTextureFromSurface(gRenderer, table);
    gXOut = SDL_LoadBMP( "bilder\\background.bmp" );
    bTexture = SDL_CreateTextureFromSurface(gRenderer, gXOut);
    if( gXOut == NULL ){
        printf( "Unable to load image %s! SDL Error: %s\n", "bilder\\background.bmp", SDL_GetError() );
        success = false;
    }
    //Load music
    gMusic = Mix_LoadMUS( "musik\\bg.wav" );
    if( gMusic == NULL ){
        printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }
    //load sprite sheet
    gPlayButton = SDL_LoadBMP("bilder\\testplay.bmp");
    SDL_SetColorKey( gPlayButton, SDL_TRUE, SDL_MapRGB( gPlayButton->format, 0xFF, 0xFF, 0xFF ) );
    mPlayButton = SDL_CreateTextureFromSurface(gRenderer, gPlayButton);

    gSpriteClips[ 0 ].x = 0;
    gSpriteClips[ 0 ].y = 0;
    gSpriteClips[ 0 ].w = 294;
    gSpriteClips[ 0 ].h = 107;

    gSpriteClips[ 1 ].x = 0;
    gSpriteClips[ 1 ].y = 107;
    gSpriteClips[ 1 ].w = 294;
    gSpriteClips[ 1 ].h = 107;

    gSpriteClips[ 2 ].x = 0;
    gSpriteClips[ 2 ].y = 214;
    gSpriteClips[ 2 ].w = 294;
    gSpriteClips[ 2 ].h = 107;

    gExitButton = SDL_LoadBMP("bilder\\EXIT.bmp");
    SDL_SetColorKey( gExitButton, SDL_TRUE, SDL_MapRGB( gExitButton->format, 0xFF, 0xFF, 0xFF ) );
    exitTexture = SDL_CreateTextureFromSurface(gRenderer, gExitButton);
    if( gExitButton == NULL ){
        printf( "Unable to load image %s! SDL Error: %s\n", "bilder\\EXIT.bmp", SDL_GetError() );
        success = false;
    }
    return success;
    #else

    //Load splash image
    table = IMG_Load("bilder/TABLE.png");
    btable = SDL_CreateTextureFromSurface(gRenderer, table);
    gXOut = SDL_LoadBMP( "bilder/background.bmp" );
    bTexture = SDL_CreateTextureFromSurface(gRenderer, gXOut);
    if( (gXOut == NULL) || (gPlayButton == NULL)){
        printf( "Unable to load image %s! SDL Error: %s\n", "bilder/background.png", SDL_GetError() );
        success = false;
    }
    //Load music
    gMusic = Mix_LoadMUS( "musik/bg.wav" );
    if( gMusic == NULL ){
        printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }
    //load sprite sheet

    SDL_Surface* gPlayButton = IMG_Load("bilder/testplay.bmp");
    SDL_SetColorKey( gPlayButton, SDL_TRUE, SDL_MapRGB( gPlayButton->format, 0xFF, 0xFF, 0xFF ) );
    mPlayButton = SDL_CreateTextureFromSurface(gRenderer, gPlayButton);

    gSpriteClips[ 0 ].x = 0;
    gSpriteClips[ 0 ].y = 0;
    gSpriteClips[ 0 ].w = 294;
    gSpriteClips[ 0 ].h = 107;

    gSpriteClips[ 1 ].x = 0;
    gSpriteClips[ 1 ].y = 107;
    gSpriteClips[ 1 ].w = 294;
    gSpriteClips[ 1 ].h = 107;

    gSpriteClips[ 2 ].x = 0;
    gSpriteClips[ 2 ].y = 214;
    gSpriteClips[ 2 ].w = 294;
    gSpriteClips[ 2 ].h = 107;

    gExitButton = SDL_LoadBMP("bilder/EXIT.bmp");
    SDL_SetColorKey( gExitButton, SDL_TRUE, SDL_MapRGB( gExitButton->format, 0xFF, 0xFF, 0xFF ) );
    exitTexture = SDL_CreateTextureFromSurface(gRenderer, gExitButton);
    if( gExitButton == NULL ){
        printf( "Unable to load image %s! SDL Error: %s\n", "bilder/EXIT.bmp", SDL_GetError() );
        success = false;
    }

    return success;
    #endif
}

//==================================================CLOSE===============================================
void closeW(){
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
//====================================================================================================
void ClearScreen(){
    #ifdef _WIN32
    system("cls");
    #else //
    system("clear");
    #endif // rest
}
