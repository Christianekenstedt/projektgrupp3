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

Kort kortlek[ANTALKORT];

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
SDL_Window* textWindow = NULL;
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
SDL_Texture* kort = NULL;
SDL_Texture* pottTexture = NULL;
// Surface
SDL_Surface* gXOut = NULL;
SDL_Surface* text_surface = NULL;
//==========
SDL_Color textColor = {255,255,255};
TTF_Font *font = NULL;

//The event structure
SDL_Event event;
// Renderer
SDL_Renderer* gRenderer = NULL;
// Rects
SDL_Rect gSpriteClips[3], cardSheet[52]; // Sprite
SDL_Rect ExitRect, ClearButton, HitButton, StandButton, DoubleButton, SplitButton, BetButton, PlayButton; // fasta knappar
SDL_Rect Chip1,Chip5,Chip25,Chip50,Chip100; // Marker
SDL_Rect table1, table2;
SDL_Rect renderRect;

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
    TCPsocket sd = NULL;
    IPaddress ip;
    char hostIP[] = "169.254.211.44";
    int window = 0; // Vilken Window som skall visas, main är 0.
    int frame = 0, cardFrame = 0, cardFrame2=0;
    char command[512]= {0};

    srand(time(NULL));
    table1.y = 401;
    table1.x = 479;
    table1.w = 72;
    table1.h = 96;

    table2.y = 401;
    table2.x = 519;
    table2.w = 72;
    table2.h = 96;

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

    //initiera_kortleken(kortlek);
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
    bool hit = false;
    bool quit = false;
    bool engang = true;
    int x=0, y=0, cardNr=0, temp =0, id=-20;
    int pott =0;
    int myTurn = 0;
    while( !quit ){
                frame = 0;
            //Handle events on queue
            while( SDL_PollEvent( &e ) != 0 ){
                //ClearScreen();
                SDL_GetMouseState(&x,&y);
                //printf("x: %d\ny: %d\n",x,y);

                //User requests quit
                if( e.type == SDL_QUIT ){
                    //sendToServer("exit", sd);
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

                            /*if(engang==true && reciveFromServer(sd)){
                                myTurn = 1;
                                engang = false;
                            }*/
                            myTurn = 1;
                        }else if(EXITBUTTON){
                            if(window == START){
                                quit = true;
                            }else if (window == TABLE){
                                /* */
                                //sendToServer("exit", sd);
                                SDLNet_TCP_Close(sd);
                                window = START;
                            }
                        }else if(BETBUTTON && window == TABLE){

                                quit = true;
                        }
                        else if(CLEARBUTTON && window == TABLE){
                            pott = 0;
                            hit = false;
                        }
                        else if(HITBUTTON && window == TABLE && myTurn == 1){

                            //sendToServer("hit", sd);
                            //id = reciveFromServer(sd);
                            printf("\n\nid recived = %d", id);
                            //cardFrame = IdToVisualCard(id,kortlek);
                            //    SDL_Delay(1000);
                            cardFrame = rand()%51+0;
                            printf("cardFrame = %d\n", cardFrame);
                            hit = true;
                            //id += 1;
                        }
                        else if(STANDBUTTON && window == TABLE){
                            //sendToServer("stand", sd);
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
                        printf("Pott: %d\n", pott);
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
                        SDL_RenderCopy(gRenderer, pottTexture, NULL, &renderRect);
                        if(hit == true){
                            SDL_RenderCopy(gRenderer, kort, &cardSheet[cardFrame], &table1);
                        }
                        SDL_RenderPresent(gRenderer);
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

    SDL_Surface* table = IMG_Load("bilder\\bord9999.0.png");

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
//Laddar kortleken
SDL_Surface* cardPic = IMG_Load("bilder\\cards.png");
    kort = SDL_CreateTextureFromSurface(gRenderer, cardPic);

    int x=1,y=1,w=72,h=96, i;

    for(i = 0; i<52; i++){

        cardSheet[i].x = x;
        cardSheet[i].y = y;
        cardSheet[i].w = w;
        cardSheet[i].h = h;
        x = x + w + 1;
        if (i == 12) {
            x = 1;
            y = 99;
        }else if (i == 25){
            x = 1;
            y = 197;
        }else if (i == 38){
            x = 1;
            y = 295;
        }
    }

    //Initialisera SDL ttf
    if(TTF_Init() == -1)
    {
        return false;
    }

    int ritaText()
    {
        font = TTF_OpenFont("Type Keys.ttf", 50);
        text_surface = TTF_RenderText_Solid(font, "FUNKAR DETTA?!", textColor);
        pottTexture = NULL;
        int w=0,h=0;
        if(text_surface != NULL)
        {
            pottTexture = SDL_CreateTextureFromSurface(gRenderer, text_surface);
            w = text_surface -> w;
            h = text_surface -> h;
            SDL_FreeSurface(text_surface);
        }
        else
        {
            printf("Error! Kan en rendera surface! SDL_ttf Error: %s\n", TTF_GetError());
        }

        SDL_Rect renderRect = {250, 300, w, h};
        int result = SDL_RenderCopyEx(gRenderer, pottTexture, NULL, &renderRect, 0.0, NULL, SDL_FLIP_NONE);
        SDL_GetError();
        return true;
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
    /* LADDAR KORTLEK */
    SDL_Surface* cardPic = IMG_Load("bilder/cards.png");
    kort = SDL_CreateTextureFromSurface(gRenderer, cardPic);

    int x=1,y=1,w=72,h=96, i;

    for(i = 0; i<52; i++){

        cardSheet[i].x = x;
        cardSheet[i].y = y;
        cardSheet[i].w = w;
        cardSheet[i].h = h;
        x = x + w + 1;
        if (i == 12) {
            x = 1;
            y = 99;
        }else if (i == 25){
            x = 1;
            y = 197;
        }else if (i == 38){
            x = 1;
            y = 295;
        }

    }

    //Initialisera SDL ttf
    if(TTF_Init() == -1)
    {
        return false;
    }

    int ritaText()
    {
        font = TTF_OpenFont("Type Keys.ttf", 50);
        text_surface = TTF_RenderText_Solid(font, "FUNKAR DETTA?!", textColor);
        pottTexture = NULL;
        int w=0,h=0;
        if(text_surface != NULL)
        {
            pottTexture = SDL_CreateTextureFromSurface(gRenderer, text_surface);
            w = text_surface -> w;
            h = text_surface -> h;
            SDL_FreeSurface(text_surface);
        }
        else
        {
            printf("Error! Kan en rendera surface! SDL_ttf Error: %s\n", TTF_GetError());
        }

        SDL_Rect renderRect = {250, 300, w, h};
        int result = SDL_RenderCopyEx(gRenderer, pottTexture, NULL, &renderRect, 0.0, NULL, SDL_FLIP_NONE);
        SDL_GetError();
        return true;
    }

    return success;
#endif
}
//==================================================CLOSE===============================================
void closeW(){
    //Deallocate surface
    SDL_FreeSurface( gXOut );
    gXOut = NULL;

    //Quit SDL_ttf
    TTF_CloseFont(font);
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


