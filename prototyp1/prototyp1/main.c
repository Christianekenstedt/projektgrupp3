//
//  main.c
//  prototyp1
//
//  Created by Christian Ekenstedt on 2015-04-16.
//  Copyright (c) 2015 Grupp 3. All rights reserved.
//

#include "multiOS.h"
#include "clientFunctions.h"
#include "gamelogic.h"
#include "knappar.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>



//Screen dimension constants

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 576;

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
// Textures
SDL_Texture* bTexture=NULL;
SDL_Texture* mPlayButton = NULL;
SDL_Texture* exitTexture = NULL;
SDL_Texture* betTexture = NULL;
SDL_Texture* clearTexture = NULL;
SDL_Texture* hitTexture = NULL;
SDL_Texture* standTexture = NULL;
SDL_Texture* doubleTexture = NULL;
SDL_Texture* splitTexture = NULL;
SDL_Texture* btable = NULL;
// Surface
SDL_Surface* gXOut = NULL;
SDL_Surface* message = NULL;

//The event structure
SDL_Event event;

//The font that's going to be used
TTF_Font* font = NULL;
//The color of the font
SDL_Color textColor = {255, 255, 255};
// Renderer
SDL_Renderer* gRenderer = NULL;
// Rects
SDL_Rect gSpriteClips[3]; // Sprite
SDL_Rect ExitRect, ClearButton, HitButton, StandButton, DoubleButton, SplitButton, BetButton, PlayButton; // fasta knappar
SDL_Rect Chip1,Chip5,Chip25,Chip50,Chip100; // Marker

//===================================================================================================
void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination )
{
    //Make a temporary rectangle to hold the offsets
    SDL_Rect offset;

    //Give the offsets to the rectangle
    offset.x = x;
    offset.y = y;
}
//=============================================MAIN==================================================
int main( int argc, char* args[] ){
    TCPsocket sd;
    IPaddress ip;
    char hostIP[] = "169.254.211.44";
    int window = 0; // Vilken Window som skall visas, main är 0.
    int frame = 0;
    //Mark 1
    Chip1.y = 517;
    Chip1.x = 8;
    Chip1.w = 68;
    Chip1.h = 59;
    //Mark 5
    Chip5.y = 517;
    Chip5.x = 94;
    Chip5.w = 68;
    Chip5.h = 59;
    //Mark 25
    Chip25.y = 517;
    Chip25.x = 180;
    Chip25.w = 68;
    Chip25.h = 59;
    //Mark 50
    Chip50.y = 517;
    Chip50.x = 266;
    Chip50.w = 68;
    Chip50.h = 59;
    //Mark 100
    Chip100.y = 517;
    Chip100.x = 352;
    Chip100.w = 68;
    Chip100.h = 59;

    //PlayButton
    PlayButton.y = SCREEN_HEIGHT/2+40;
    PlayButton.x = SCREEN_WIDTH/2-147;
    PlayButton.w = 294;
    PlayButton.h = 107;
    // Exit Button
    ExitRect.y = 0;
    ExitRect.x = 0;
    ExitRect.w = 80;
    ExitRect.h = 40;
    // Bet Button
    BetButton.y = 418;
    BetButton.x = 15;
    BetButton.w = 93;
    BetButton.h = 90;
    // Clear Button
    ClearButton.y = 418;
    ClearButton.x = 123;
    ClearButton.w = 93;
    ClearButton.h = 90;
    // Hit Button
    HitButton.y = 474;
    HitButton.x = 689;
    HitButton.w = 93;
    HitButton.h = 90;
    // Stand Button
    StandButton.y = 474;
    StandButton.x = 793;
    StandButton.w = 93;
    StandButton.h = 90;
    // Double Button
    DoubleButton.y = 372;
    DoubleButton.x = 918;
    DoubleButton.w = 93;
    DoubleButton.h = 90;
    // Split Button
    SplitButton.y = 474;
    SplitButton.x = 918;
    SplitButton.w = 93;
    SplitButton.h = 90;
    // NETWORK INIT ####################################################
    
    /* Resolve the host we are connecting to */
    if (SDLNet_ResolveHost(&ip, hostIP, 2000) < 0)
    {
        fprintf(stderr, "SDLNet_ResolveHost: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
    
    // #################################################################

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
    int pott =0;
    while( !quit ){
                frame = 0;
            //Handle events on queue
            while( SDL_PollEvent( &e ) != 0 ){
                //ClearScreen();
                SDL_GetMouseState(&x,&y);
                printf("x: %d\ny: %d\n",x,y);
                printf("Pott: %d\n",pott);
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
                        if(PLAYBUTTON && window == START){

                            frame=1;
                            window=TABLE;
                            /* Open a connection with the IP provided (listen on the host's port) */
                            /*if ((sd = SDLNet_TCP_Open(&ip))< 1)
                            {
                                window = START;
                                fprintf(stderr, "SDLNet_TCP_Open: %s\n", SDLNet_GetError());
                                //exit(EXIT_FAILURE);
                            }*/
                            
                        }else if(EXITBUTTON){
                            if(window == START){
                                quit = true;
                            }else if (window == TABLE){
                                /* */
                                window = START;
                            }
                        }else if(BETBUTTON && window == TABLE){

                                quit = true;
                        }
                        else if(CLEARBUTTON && window == TABLE){
                            pott = 0;
                        }
                        else if(HITBUTTON && window == TABLE){
                            quit = true;
                        }
                        else if(STANDBUTTON && window == TABLE){
                            quit = true;
                        }
                        else if(DOUBLEBUTTON && window == TABLE){
                            quit = true;
                        }
                        else if(SPLITBUTTON && window == TABLE){
                            quit = true;
                        }else if(M1 && window == TABLE){
                            pott += 1;
                        }else if(M5 && window == TABLE){
                            pott +=5;
                        }else if(M25 && window == TABLE){
                            pott +=25;
                        }else if(M50 && window == TABLE){
                            pott +=50;
                        }else if(M100 && window == TABLE){
                            pott +=100;
                        }
                    }else if(e.type == SDL_MOUSEMOTION){
                        if(PLAYBUTTON){ //Innanför knappen?
                            frame=2;
                        }
                    }
                    if(window == 0 ){
                        SDL_RenderCopy(gRenderer,bTexture,NULL,NULL);
                        SDL_RenderCopy(gRenderer,mPlayButton,&gSpriteClips[frame],&PlayButton);
                        SDL_RenderCopy(gRenderer,exitTexture,NULL,&ExitRect);
                        SDL_RenderPresent(gRenderer);
                    }else if (window==1){
                        SDL_RenderCopy(gRenderer, btable, NULL, NULL);
                        SDL_RenderPresent(gRenderer);
                    }
                }
        }
    //Render the text
    message = TTF_RenderText_Solid(font, "FUNKAR DETTA ELLER?!", textColor);

    //If there was an error in rendering the text
    if(message == NULL)
    {
        return 1;
    }

    //Apply the imaged to the screen
    apply_surface(0, 0, gXOut, gWindow);
    apply_surface(0, 150, message, gWindow);

    /*Update the screen
    if(SDL_Flip(gWindow)== -1)
    {
        return 1;
    }*/
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

    //Initialize SDL ttf
    if(TTF_Init()== -1)
    {
        return false;
    }

    //Set the window caption
    SDL_WM_SetCaption("TTF Test", NULL);

    //If everything initialized fine
    return true;
}
//============================================LOAD MEDIA================================================
bool loadMedia(){
    //Loading success flag
    bool success = true;

#ifdef _WIN32
    //Load splash image

<<<<<<< HEAD
//<<<<<<< HEAD
    SDL_Surface* table = IMG_Load("bilder\\TABLE.png");
//=======
=======
<<<<<<< HEAD
    SDL_Surface* table = NULL; = IMG_Load("bilder\\bord9999.0.png");
=======
>>>>>>> origin/master
    table = IMG_Load("bilder/TABLE.png");
//>>>>>>> origin/master
    btable = SDL_CreateTextureFromSurface(gRenderer, table);
    gXOut = SDL_LoadBMP( "bilder/background.bmp" );
    bTexture = SDL_CreateTextureFromSurface(gRenderer, gXOut);
    if( gXOut == NULL ){
        printf( "Unable to load image %s! SDL Error: %s\n", "bilder/background.bmp", SDL_GetError() );
        success = false;
    }
    //Load music
    gMusic = Mix_LoadMUS( "musik\\bg.wav" );
    if( gMusic == NULL ){
        printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }
    //load sprite sheet
    SDL_Surface* gPlayButton = SDL_LoadBMP("bilder/testplay.bmp");
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

    SDL_Surface* gExitButton = SDL_LoadBMP("bilder\\EXIT.bmp");
    SDL_SetColorKey( gExitButton, SDL_TRUE, SDL_MapRGB( gExitButton->format, 0xFF, 0xFF, 0xFF ) );
    exitTexture = SDL_CreateTextureFromSurface(gRenderer, gExitButton);
    if( gExitButton == NULL ){
        printf( "Unable to load image %s! SDL Error: %s\n", "bilder\\EXIT.bmp", SDL_GetError() );
        success = false;
    }
    return success;
#else

    //Load splash image
    SDL_Surface* table = IMG_Load("bilder/bord9999.0.png");
    btable = SDL_CreateTextureFromSurface(gRenderer, table);
    gXOut = SDL_LoadBMP( "bilder/background.bmp" );
    bTexture = SDL_CreateTextureFromSurface(gRenderer, gXOut);
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

    SDL_Surface* gExitButton = SDL_LoadBMP("bilder/EXIT.bmp");
    SDL_SetColorKey( gExitButton, SDL_TRUE, SDL_MapRGB( gExitButton->format, 0xFF, 0xFF, 0xFF ) );
    exitTexture = SDL_CreateTextureFromSurface(gRenderer, gExitButton);
    if( gExitButton == NULL ){
        printf( "Unable to load image %s! SDL Error: %s\n", "bilder/EXIT.bmp", SDL_GetError() );
        success = false;
    }

    return success;
#endif
    //Open the font
    font = TTF_OpenFont("KeepCalm-Medium", 20);

    //If there was a problem in loading the font
    if(font == NULL)
    {
        return false;
    }

    //if everything loaded fine
    return true;
}
//==================================================CLOSE===============================================
void closeW(){
    //Deallocate surface
    SDL_FreeSurface( gXOut );
    gXOut = NULL;
    SDL_FreeSurface (message);

    //Close the font what was used
    TTF_CloseFont(font);

    //Quit SDL_ttf
    TTF_Quit();

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


