//
//  functions.c
//  Blackjack
//
//  Created by Christian Ekenstedt on 2015-05-27.
//  Copyright (c) 2015 Grupp 3. All rights reserved.
//

#include "functions.h"
#include "gamelogic.h"

int tableInfo[MAXCLIENTS+1][15];

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 576;
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
SDL_Texture* pokerchip100 = NULL;
SDL_Texture* pokerchip50 = NULL;
SDL_Texture* pokerchip25 = NULL;
SDL_Texture* pokerchip5 = NULL;
SDL_Texture* pokerchip1 = NULL;
SDL_Texture* baksida = NULL;
// Surface
SDL_Surface* gXOut = NULL;
SDL_Surface* text_surface = NULL;
//==========
SDL_Color textColor = {255,255,255,255};
TTF_Font *font = NULL;

//The event structure
SDL_Event event;
// Renderer
SDL_Renderer* gRenderer = NULL;
// Rects
SDL_Rect gSpriteClips[3], cardSheet[52]; // Sprite
SDL_Rect ExitRect, ClearButton, HitButton, StandButton, DoubleButton, SplitButton, BetButton, PlayButton; // fasta knappar
SDL_Rect Chip1,Chip5,Chip25,Chip50,Chip100; // Marker
SDL_Rect table1[MAXCARDS], table2[MAXCARDS],table3[MAXCARDS], table4[MAXCARDS],table5[MAXCARDS],tableDealer[MAXCARDS];
SDL_Rect Betcirkel1_1[MAX], Betcirkel1_5[MAX], Betcirkel1_25[MAX], Betcirkel1_50[MAX], Betcirkel1_100[MAX]; // Betcirklar där pokermarker ska hamna
SDL_Rect Betcirkel2_1[MAX], Betcirkel2_5[MAX], Betcirkel2_25[MAX], Betcirkel2_50[MAX], Betcirkel2_100[MAX];
SDL_Rect Betcirkel3_1[MAX], Betcirkel3_5[MAX], Betcirkel3_25[MAX], Betcirkel3_50[MAX], Betcirkel3_100[MAX];
SDL_Rect Betcirkel4_1[MAX], Betcirkel4_5[MAX], Betcirkel4_25[MAX], Betcirkel4_50[MAX], Betcirkel4_100[MAX];
SDL_Rect Betcirkel5_1[MAX], Betcirkel5_5[MAX], Betcirkel5_25[MAX], Betcirkel5_50[MAX], Betcirkel5_100[MAX];
SDL_Rect renderRect,renderRect2;



//==============================================INIT=================================================
bool init()  // Christian
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
        gWindow = SDL_CreateWindow( "♦︎♥︎♣︎♠︎ Blackjack by RoyalKing Casino ♦︎♥︎♣︎♠︎", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( gWindow == NULL )
        {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            //Get window surface
            //gScreenSurface = SDL_GetWindowSurface( gWindow );
        }
        //Initialize SDL_mixer
        if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
        {
            printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
            success = false;
        }
    }
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    if(gRenderer == NULL)
    {
        fprintf(stderr,"SDL_CreateRenderer: %s\n", SDL_GetError());
        success = false;
    }

    if(TTF_Init() == -1)
    {
        return false;
    }

    return success;
}
//============================================LOAD MEDIA================================================
bool loadMedia()  // Christian
{
    //Loading success flag
    bool success = true;

#ifdef _WIN32
    //Load splash image

    SDL_Surface* table = IMG_Load("bilder\\bordet!!!.jpg");

    btable = SDL_CreateTextureFromSurface(gRenderer, table);
    gXOut = SDL_LoadBMP( "bilder\\background.bmp" );
    bTexture = SDL_CreateTextureFromSurface(gRenderer, gXOut);
    if( gXOut == NULL )
    {
        printf( "Unable to load image %s! SDL Error: %s\n", "bilder/background.bmp", SDL_GetError() );
        success = false;
    }
    //Load music
    gMusic = Mix_LoadMUS( "musik\\bg.wav" );
    if( gMusic == NULL )
    {
        printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }
    //load sprite sheet
    SDL_Surface* gPlayButton = SDL_LoadBMP("bilder\\testplay.bmp");
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
    if( gExitButton == NULL )
    {
        printf( "Unable to load image %s! SDL Error: %s\n", "bilder\\EXIT.bmp", SDL_GetError() );
        success = false;
    }
    //Laddar kortleken
    SDL_Surface* cardPic = IMG_Load("bilder\\cards.png");
    kort = SDL_CreateTextureFromSurface(gRenderer, cardPic);

    int x=1,y=1,w=72,h=96, i;

    for(i = 0; i<52; i++)
    {

        cardSheet[i].x = x;
        cardSheet[i].y = y;
        cardSheet[i].w = w;
        cardSheet[i].h = h;
        x = x + w + 1;
        if (i == 12)
        {
            x = 1;
            y = 99;
        }
        else if (i == 25)
        {
            x = 1;
            y = 197;
        }
        else if (i == 38)
        {
            x = 1;
            y = 295;
        }
    }

    SDL_Surface* baksida1 = IMG_Load("bilder\\baksida.png");
    baksida = SDL_CreateTextureFromSurface(gRenderer, baksida1);

    SDL_Surface* temp100 = IMG_Load("bilder\\100mark_mini.png");
    pokerchip100 = SDL_CreateTextureFromSurface(gRenderer, temp100);

    SDL_Surface* temp50 = IMG_Load("bilder\\50mark_mini.png");
    pokerchip50 = SDL_CreateTextureFromSurface(gRenderer, temp50);

    SDL_Surface* temp25 = IMG_Load("bilder\\25mark_mini.png");
    pokerchip25 = SDL_CreateTextureFromSurface(gRenderer, temp25);

    SDL_Surface* temp5 = IMG_Load("bilder\\5mark_mini.png");
    pokerchip5 = SDL_CreateTextureFromSurface(gRenderer, temp5);

    SDL_Surface* temp1 = IMG_Load("bilder\\1mark_mini.png");
    pokerchip1 = SDL_CreateTextureFromSurface(gRenderer, temp1);


#else

    //Load splash image
    SDL_Surface* table = IMG_Load("bilder/bordet!!!.jpg");
    btable = SDL_CreateTextureFromSurface(gRenderer, table);
    gXOut = SDL_LoadBMP( "bilder/background.bmp" );
    bTexture = SDL_CreateTextureFromSurface(gRenderer, gXOut);
    //Load music
    gMusic = Mix_LoadMUS( "musik/bg.wav" );
    if( gMusic == NULL )
    {
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
    if( gExitButton == NULL )
    {
        printf( "Unable to load image %s! SDL Error: %s\n", "bilder/EXIT.bmp", SDL_GetError() );
        success = false;
    }
    /* LADDAR KORTLEK */
    SDL_Surface* cardPic = IMG_Load("bilder/cards.png");
    kort = SDL_CreateTextureFromSurface(gRenderer, cardPic);

    int x=1,y=1,w=72,h=96, i;

    for(i = 0; i<52; i++)
    {

        cardSheet[i].x = x;
        cardSheet[i].y = y;
        cardSheet[i].w = w;
        cardSheet[i].h = h;
        x = x + w + 1;
        if (i == 12)
        {
            x = 1;
            y = 99;
        }
        else if (i == 25)
        {
            x = 1;
            y = 197;
        }
        else if (i == 38)
        {
            x = 1;
            y = 295;
        }

    }

    SDL_Surface* baksida1 = IMG_Load("bilder/baksida.png");
    baksida = SDL_CreateTextureFromSurface(gRenderer, baksida1);


    //SDL_GetError();

    SDL_Surface* temp100 = IMG_Load("bilder/100mark_mini.png");
    pokerchip100 = SDL_CreateTextureFromSurface(gRenderer, temp100);

    SDL_Surface* temp50 = IMG_Load("bilder/50mark_mini.png");
    pokerchip50 = SDL_CreateTextureFromSurface(gRenderer, temp50);

    SDL_Surface* temp25 = IMG_Load("bilder/25mark_mini.png");
    pokerchip25 = SDL_CreateTextureFromSurface(gRenderer, temp25);

    SDL_Surface* temp5 = IMG_Load("bilder/5mark_mini.png");
    pokerchip5 = SDL_CreateTextureFromSurface(gRenderer, temp5);

    SDL_Surface* temp1 = IMG_Load("bilder/1mark_mini.png");
    pokerchip1 = SDL_CreateTextureFromSurface(gRenderer, temp1);

#endif
    return success;
}
//==================================================CLOSE===============================================
void closeW()  // Christian
{
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

int ritaText(char word[], SDL_Rect position) // fick det att funka (Christian)
{
#ifdef _WIN32
    font = TTF_OpenFont("fonts\\KeepCalm-Medium.ttf", 28);
#else
    font = TTF_OpenFont("fonts/KeepCalm-Medium.ttf", 28);
#endif
    font = TTF_OpenFont("fonts/KeepCalm-Medium.ttf", 28);
    text_surface = TTF_RenderText_Solid(font, word, textColor);
    if(text_surface != NULL)
    {
        pottTexture = SDL_CreateTextureFromSurface(gRenderer, text_surface);
    }
    else
    {
        printf("Error! Kan en rendrera surface! SDL_ttf Error: %s\n", TTF_GetError());
    }

    SDL_RenderCopy(gRenderer, pottTexture, NULL, &position);

    SDL_GetError();
    return true;
}

int sendToServer(char command[], TCPsocket socket){
    int success = 0;
    if (SDLNet_TCP_Send(socket, command, 512) < 0)
    {
        fprintf(stderr, "SDLNet_TCP_Send: %s\n", SDLNet_GetError());
        success = -1;
    }else success = 1;
    return success;
}

int reciveFromServer(TCPsocket socket){
    char buffer[512];
    int value = 0, quit;
    while (!quit) {
        if (SDLNet_TCP_Recv(socket, buffer, strlen(buffer)+1) > 0)
        {
            printf("In Recv: buffer = %s\n", buffer);
            value = atoi(buffer);
            printf("In Recv: value = %d\n", value);
            quit = 1;
        }else {
            fprintf(stderr, "SDLNet_TCP_Send: %s\n", SDLNet_GetError());
            exit(EXIT_FAILURE);
        }
    }

    return value;
}

int IdToVisualCard(int id,Kort kortlek[])
{
    int i = 0, cardNr = 0, temp = 0;
    for(i = 0;i<ANTALKORT;i++)
    {
        if(id == kortlek[i].kortnummer)
        {
            //printf("\n");
            //printf("You have card:\n");
            //printf("Suite: %s\n",kortlek[i].farg);
            //printf("Value: %d\n\n",kortlek[i].kortvarde);
            if (strstr(kortlek[i].farg, "Hjarter")) {
                cardNr = 26;
                cardNr += kortlek[i].kortvarde;
            }else if (strstr(kortlek[i].farg, "Spader")) {
                cardNr = 13;
                cardNr += kortlek[i].kortvarde ;
            }else if (strstr(kortlek[i].farg, "Ruter")) {
                cardNr = 39;
                cardNr += kortlek[i].kortvarde ;
            }else if (strstr(kortlek[i].farg, "Klover")) {
                cardNr = 0;
                cardNr += kortlek[i].kortvarde ;
            }
        }
    }
    //printf("Going to return CardNr = %d \n", cardNr-1);
    return cardNr-1;
}

void playerPosition(int myClientNr, int bet, int hit, int nykort, bool dubbel, int money, int stand, int bordskort[]){
    int i;
    if(myClientNr == 0)
    {
        if(dubbel == true && ((tableInfo[0][0] + tableInfo[0][1]) >= 7 && (tableInfo[0][0] + tableInfo[0][1]) <= 11))
        {
            money = (money - bet);
            bet = (bet*2);
            for(i=0; i<nykort; i++)
            {
                SDL_RenderCopy(gRenderer, kort, &cardSheet[bordskort[i]], &table1[i]);
            }
            stand = true;
        }

        if(hit == true)
        {

            for (i=0; i<nykort; i++)
            {
                SDL_RenderCopy(gRenderer, kort, &cardSheet[bordskort[i]], &table1[i]);

            }
        }

        if(bet == 1 || bet <= 4)
        {


            for(i = 0; i < bet; i++)
            {

                SDL_RenderCopy(gRenderer, pokerchip1, NULL, &Betcirkel1_1[i]);

            }
        }

        else if(bet==5 || bet <= 24 )
        {

            for(i = 0; i < (bet / 5); i++)
            {
                SDL_RenderCopy(gRenderer, pokerchip5, NULL, &Betcirkel1_5[i]);
            }
        }

        else if(bet==25 || bet <= 49)
        {

            for(i = 0; i < (bet / 25); i++)
            {
                SDL_RenderCopy(gRenderer, pokerchip25, NULL, &Betcirkel1_25[i]);
            }
        }

        else if(bet == 50 && bet > 50 || bet <= 99)
        {

            for(i = 0; i < (bet / 50); i++)
            {
                SDL_RenderCopy(gRenderer, pokerchip50, NULL, &Betcirkel1_50[i]);
            }
        }

        else if(bet == 100 || bet > 100)
        {

            for(i = 0; i < (bet / 100); i++)
            {
                SDL_RenderCopy(gRenderer, pokerchip100, NULL, &Betcirkel1_100[i]);
            }
        }
    }


    if(myClientNr == 1)
    {

        if(dubbel == true && ((tableInfo[1][0] + tableInfo[1][1]) >= 7 && (tableInfo[1][0] + tableInfo[1][1]) <= 11))
        {
            money = (money - bet);
            bet = (bet*2);
            for(i=0; i<nykort; i++)
            {
                SDL_RenderCopy(gRenderer, kort, &cardSheet[bordskort[i]], &table1[i]);
            }
            stand = true;
        }

        if(hit == true)
        {
            for (i=0; i<nykort; i++)
            {
                SDL_RenderCopy(gRenderer, kort, &cardSheet[bordskort[i]], &table2[i]);

            }
        }

        if(bet == 1 || bet <= 4)
        {
            for(i = 0; i < bet; i++)
            {
                SDL_RenderCopy(gRenderer, pokerchip1, NULL, &Betcirkel2_1[i]);
            }
        }

        else if(bet ==5 || bet <= 24)
        {
            for(i = 0; i<(bet/5); i++)
            {
                SDL_RenderCopy(gRenderer, pokerchip5, NULL, &Betcirkel2_5[i]);
            }
        }

        else if(bet == 25 || bet <= 49)
        {
            for(i = 0; i <(bet/25); i++)
            {
                SDL_RenderCopy(gRenderer, pokerchip25, NULL, &Betcirkel2_25[i]);
            }
        }

        else if(bet == 50 || bet <= 99)
        {
            for(i = 0; i < (bet/50); i++)
            {
                SDL_RenderCopy(gRenderer, pokerchip50, NULL, &Betcirkel2_50[i]);
            }
        }

        else if(bet == 100 || bet > 100)
        {
            for(i = 0; i < (bet / 100); i++)
            {
                SDL_RenderCopy(gRenderer, pokerchip100, NULL, &Betcirkel2_100[i]);
            }
        }

    }

    if(myClientNr == 2)
    {

        if(dubbel == true && ((tableInfo[2][0] + tableInfo[2][1]) >= 7 && (tableInfo[2][0] + tableInfo[2][1]) <= 11))
        {
            money = (money - bet);
            bet = (bet*2);
            for(i=0; i<nykort; i++)
            {
                SDL_RenderCopy(gRenderer, kort, &cardSheet[bordskort[i]], &table1[i]);
            }
            stand = true;
        }

        if(hit == true)
        {
            for (i=0; i<nykort; i++)
            {
                SDL_RenderCopy(gRenderer, kort, &cardSheet[bordskort[i]], &table3[i]);

            }
        }
        if(bet == 1 || bet <= 4)
        {

            for(i = 0; i < bet; i++)
            {
                SDL_RenderCopy(gRenderer, pokerchip1, NULL, &Betcirkel3_1[i]);
            }
        }

        else if(bet == 5 || bet <= 24)
        {
            for(i = 0; i < (bet/5); i++)
            {
                SDL_RenderCopy(gRenderer, pokerchip5, NULL, &Betcirkel3_5[i]);
            }
        }

        else if(bet == 25 || bet <= 49)
        {
            for(i = 0; i < (bet/25); i++)
            {
                SDL_RenderCopy(gRenderer, pokerchip25, NULL, &Betcirkel3_25[i]);
            }
        }

        else if(bet == 50  || bet <= 99)
        {
            for(i = 0; i < (bet/50); i++)
            {
                SDL_RenderCopy(gRenderer, pokerchip50, NULL, &Betcirkel3_50[i]);
            }
        }

        else if(bet == 100 || bet > 100)
        {
            for(i = 0; i < (bet/100); i++)
            {
                SDL_RenderCopy(gRenderer, pokerchip100, NULL, &Betcirkel3_100[i]);
            }
        }

    }



    if(myClientNr == 3)
    {

        if(dubbel == true && ((tableInfo[3][0] + tableInfo[3][1]) >= 7 && (tableInfo[3][0] + tableInfo[3][1]) <= 11))
        {
            money = (money - bet);
            bet = (bet*2);
            for(i=0; i<nykort; i++)
            {
                SDL_RenderCopy(gRenderer, kort, &cardSheet[bordskort[i]], &table1[i]);
            }
            stand = true;
        }

        if(hit == true)
        {
            for (i=0; i<nykort; i++)
            {
                SDL_RenderCopy(gRenderer, kort, &cardSheet[bordskort[i]], &table4[i]);

            }
        }
        if(bet == 1 || bet <= 4)
        {
            for(i = 0; i < bet; i++)
            {
                SDL_RenderCopy(gRenderer, pokerchip1, NULL, &Betcirkel4_1[i]);
            }
        }

        else if(bet == 5 || bet <= 24)
        {
            for(i = 0; i < (bet/5); i++)
            {
                SDL_RenderCopy(gRenderer, pokerchip5, NULL, &Betcirkel4_5[i]);
            }
        }

        else if(bet == 25 || bet <= 49)
        {
            for(i = 0; i < (bet/25); i++)
            {
                SDL_RenderCopy(gRenderer, pokerchip25, NULL, &Betcirkel4_25[i]);
            }
        }

        else if(bet == 50 || bet <= 99)
        {
            for(i = 0; i < (bet/50); i++)
            {
                SDL_RenderCopy(gRenderer, pokerchip50, NULL, &Betcirkel4_50[i]);
            }
        }

        else if(bet == 100 || bet > 100)
        {
            for(i = 0; i < (bet/100); i++)
            {
                SDL_RenderCopy(gRenderer, pokerchip100, NULL, &Betcirkel4_100[i]);
            }
        }
    }

    if(myClientNr == 4)
    {

        if(dubbel == true && ((tableInfo[4][0] + tableInfo[4][1]) >= 7 && (tableInfo[4][0] + tableInfo[4][1]) <= 11))
        {
            money = (money - bet);
            bet = (bet*2);
            for(i=0; i<nykort; i++)
            {
                SDL_RenderCopy(gRenderer, kort, &cardSheet[bordskort[i]], &table1[i]);
            }
            stand = true;
        }

        if(hit == true)
        {
            for (i=0; i<nykort; i++)
            {
                SDL_RenderCopy(gRenderer, kort, &cardSheet[bordskort[i]], &table5[i]);

            }
        }

        if(bet == 1 || bet <= 4)
        {
            for(i = 0; i < bet; i++)
            {
                SDL_RenderCopy(gRenderer, pokerchip1, NULL, &Betcirkel5_1[i]);
            }
        }

        else if(bet == 5 || bet <= 24)
        {
            for(i = 0; i < (bet/5); i++)
            {
                SDL_RenderCopy(gRenderer, pokerchip5, NULL, &Betcirkel5_5[i]);
            }
        }

        else if(bet == 25 || bet <= 49)
        {
            for(i = 0; i < (bet/25); i++)
            {
                SDL_RenderCopy(gRenderer, pokerchip25, NULL, &Betcirkel5_25[i]);
            }
        }

        else if(bet == 50 || bet <= 99)
        {
            for(i = 0; i < (bet/50); i++)
            {
                SDL_RenderCopy(gRenderer, pokerchip50, NULL, &Betcirkel5_50[i]);
            }
        }
        else if(bet == 100 || bet > 100)
        {
            for(i = 0; i < (bet/100); i++)
            {
                SDL_RenderCopy(gRenderer, pokerchip100, NULL, &Betcirkel5_100[i]);
            }
        }
    }
    if(myClientNr == 5)
    {

        if(hit == true)
        {
            for (i=0; i<nykort; i++)
            {
                SDL_RenderCopy(gRenderer, kort, &cardSheet[bordskort[i]], &tableDealer[i]);

            }
        }else{
            SDL_RenderCopy(gRenderer, baksida, NULL, &tableDealer[0]);
        }
    }
    //SDL_RenderPresent(gRenderer);

}

void stringToArray(char sendstring[])
{
    char temp2[4];
    int i = 0;
    int j = 0,n=0,k=0,l=0;

    if(sendstring[n]=='#')
    {
        n++;
        while (sendstring[n] != '\0') {
            if(sendstring[n] == '.'){
                n++;
            }else{
                l=n;
                //printf("%c\n", sendstring[n]);
                while (sendstring[n] != '.') {
                    temp2[k] = sendstring[n];
                    k++;
                    n++;
                }
                k=0;
                tableInfo[i][j] = atoi(temp2);
                for (l=0; l<4; l++) {
                    temp2[l] = '?';
                }
                j++;
            }

            if (j == MAXCARDS) {
                j = 0;
                i++;
            }
        }
    }else printf("STRANG TRASIG\n");
    SDL_Delay(100);
}

int whoWon(int playerValue, int dealerValue, int bet, bool blackjack, bool dubbel){
    int won=0;

    printf("bet = %d\n", bet);
    if (blackjack){
        printf("Won = %d\n", (won = bet * 3));
        return (won = bet * 3); // Hur många gånger pengarna får man vid blackjack?
    }

    if (dealerValue < 21) {
        if (playerValue < 21) {
            if (playerValue > dealerValue) {
                won = bet * 2;
            }else if (playerValue == dealerValue){
                won = bet;
            }
        }
    }
    else if(dubbel == true)
    {
        won = bet*4;
    }
    else won = bet*2;


    if (playerValue>21 || (dealerValue > playerValue && dealerValue < 21 )) {
        won=0;
    }

    printf("Won = %d\n", won);
    blackjack=false;
    return won;
}
