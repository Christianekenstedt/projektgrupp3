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

int ritaMoney(char word[], SDL_Rect position);
int ritaBett(char word[],SDL_Rect position);

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
SDL_Rect table1[MAXCARDS], table2[MAXCARDS],table3[MAXCARDS], table4[MAXCARDS],table5[MAXCARDS];
SDL_Rect Betcirkel1_1[MAX], Betcirkel1_5[MAX], Betcirkel1_25[MAX], Betcirkel1_50[MAX], Betcirkel1_100[MAX]; // Betcirklar där pokermarker ska hamna
SDL_Rect Betcirkel2_1[MAX], Betcirkel2_5[MAX], Betcirkel2_25[MAX], Betcirkel2_50[MAX], Betcirkel2_100[MAX];
SDL_Rect Betcirkel3_1[MAX], Betcirkel3_5[MAX], Betcirkel3_25[MAX], Betcirkel3_50[MAX], Betcirkel3_100[MAX];
SDL_Rect Betcirkel4_1[MAX], Betcirkel4_5[MAX], Betcirkel4_25[MAX], Betcirkel4_50[MAX], Betcirkel4_100[MAX];
SDL_Rect Betcirkel5_1[MAX], Betcirkel5_5[MAX], Betcirkel5_25[MAX], Betcirkel5_50[MAX], Betcirkel5_100[MAX];
SDL_Rect renderRect,renderRect2;


//=============================================MAIN==================================================
int main( int argc, char* args[] )                  // Christian Ekenstedt
{
    TCPsocket sd = NULL;
    IPaddress ip;
    char hostIP[] = "169.254.211.44";
    int window = 0, i; // Vilken Window som skall visas, main är 0.
    int frame = 0, cardFrame = 0, cardFrame2=0;
    int myClientNr=0, nymark=0;
    char command[512];
    char command2[512];
    int bordskort[11], nykort=0;

    renderRect.x = 420;
    renderRect.y = 517;
    renderRect.h = 60;
    renderRect.w = 250;

    renderRect2.x = 230;
    renderRect2.y = 450;
    renderRect2.h = 50;
    renderRect2.w = 230;


    srand(time(NULL));
    for (i=0; i<11; i++)
    {
        table1[i].y = 120;
        table1[i].x = 164+(i*15);
        table1[i].w = 72;
        table1[i].h = 96;
    }
    for (i=0; i<11; i++)
    {
        table2[i].y = 210;
        table2[i].x = 320+(i*15);
        table2[i].w = 72;
        table2[i].h = 96;
    }
    for (i=0; i<11; i++)
    {
        table3[i].y = 300;
        table3[i].x = 475+(i*15);
        table3[i].w = 72;
        table3[i].h = 96;
    }
    for (i=0; i<11; i++)
    {
        table4[i].y = 216;
        table4[i].x = 620+(i*15);
        table4[i].w = 72;
        table4[i].h = 96;
    }
    for (i=0; i<11; i++)
    {
        table5[i].y = 125;
        table5[i].x = 760+(i*15);
        table5[i].w = 72;
        table5[i].h = 96;
    }


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
    // Betcirkel 1   för plats pokerchip1
    for(i = 0; i < MAX; i++)
    {
        Betcirkel1_1[i].x = 178;
        Betcirkel1_1[i].y = 255-(i*5);
        Betcirkel1_1[i].w = 23;
        Betcirkel1_1[i].h = 23;
    }

    // Betcrikel 1 för plats pokerchip5
    for(i = 0; i < MAX; i++)
    {
        Betcirkel1_5[i].x = 201;
        Betcirkel1_5[i].y = 255-(i*5);
        Betcirkel1_5[i].w = 23;
        Betcirkel1_5[i].h = 23;
    }

    // Betcirkel 1 för plats pokerchip 25
    for(i = 0; i < MAX; i++)
    {
        Betcirkel1_25[i].x = 224;
        Betcirkel1_25[i].y = 255-(i*5);
        Betcirkel1_25[i].w = 23;
        Betcirkel1_25[i].h = 23;
    }

    // Betcirkel 1 för plats pokerchip 50
    for(i = 0; i < MAX; i++)
    {
        Betcirkel1_50[i].x = 184;
        Betcirkel1_50[i].y = 270-(i*5);
        Betcirkel1_50[i].w = 23;
        Betcirkel1_50[i].h = 23;
    }

    // Betcirkel 1 för plats pokerchip 100
    for(i = 0; i < MAX; i++)
    {
        Betcirkel1_100[i].x = 207;
        Betcirkel1_100[i].y = 270-(i*5);
        Betcirkel1_100[i].w = 23;
        Betcirkel1_100[i].h = 23;
    }

    // Betcirkel 2 för plats pokerchip 1
    for(i = 0; i < MAX; i++)
    {
        Betcirkel2_1[i].x = 325;
        Betcirkel2_1[i].y = 348-(i*5);
        Betcirkel2_1[i].w = 23;
        Betcirkel2_1[i].h = 23;
    }

    // Betcirekl 2 för plats pokerchip 5
    for(i = 0; i < MAX; i++)
    {
        Betcirkel2_5[i].x = 348;
        Betcirkel2_5[i].y = 348-(i*5);
        Betcirkel2_5[i].w = 23;
        Betcirkel2_5[i].h = 23;
    }

    // Betcirkel 2 för plats pokerchip 25
    for(i = 0; i < MAX; i++)
    {
        Betcirkel2_25[i].x = 371;
        Betcirkel2_25[i].y = 348-(i*5);
        Betcirkel2_25[i].w = 23;
        Betcirkel2_25[i].h = 23;
    }

    // Betcirkel 2 för plats pokerchip 50
    for(i = 0; i < MAX; i++)
    {
        Betcirkel2_50[i].x = 344;
        Betcirkel2_50[i].y = 360-(i*5);
        Betcirkel2_50[i].w = 23;
        Betcirkel2_50[i].h = 23;
    }

    // Betcirkel 2 för plats pokerchip 100
    for(i = 0; i < MAX; i++)
    {
        Betcirkel2_100[i].x = 367;
        Betcirkel2_100[i].y = 360-(i*5);
        Betcirkel2_100[i].w = 23;
        Betcirkel2_100[i].h = 23;
    }


    // Betcirkel 3 för plats pokercip 1
    for(i = 0; i < MAX; i++)
    {
        Betcirkel3_1[i].x = 482;
        Betcirkel3_1[i].y = 435-(i*5);
        Betcirkel3_1[i].w = 23;
        Betcirkel3_1[i].h = 23;
    }

    // Betcirkel 3 för plats pokerchip 5
    for(i = 0; i < MAX; i++)
    {
        Betcirkel3_5[i].x = 505;
        Betcirkel3_5[i].y = 435-(i*5);
        Betcirkel3_5[i].w = 23;
        Betcirkel3_5[i].h = 23;
    }

    // Betcirkel 3 för plats pokerchip 25
    for(i = 0; i < MAX; i++)
    {
        Betcirkel3_25[i].x = 528;
        Betcirkel3_25[i].y = 435-(i*5);
        Betcirkel3_25[i].w = 23;
        Betcirkel3_25[i].h = 23;
    }

    // Betcirkel 3 för plats pokerchip 50
    for(i = 0; i < MAX; i++)
    {
        Betcirkel3_50[i].x = 500;
        Betcirkel3_50[i].y = 450-(i*5);
        Betcirkel3_50[i].w = 23;
        Betcirkel3_50[i].h = 23;
    }

    // Betcirkel 3 för plats pokerchip 100
    for(i = 0; i < MAX; i++)
    {
        Betcirkel3_100[i].x = 523;
        Betcirkel3_100[i].y = 450-(i*5);
        Betcirkel3_100[i].w = 23;
        Betcirkel3_100[i].h = 23;
    }

    // Betcirkel 4 för plats pokerchip 1
    for(i = 0; i < MAX; i++)
    {
        Betcirkel4_1[i].x = 650;
        Betcirkel4_1[i].y = 350-(i*5);
        Betcirkel4_1[i].w = 23;
        Betcirkel4_1[i].h = 23;
    }

    // Betcirkel 4 för plats pokerchip 5
    for(i = 0; i < MAX; i++)
    {
        Betcirkel4_5[i].x = 673;
        Betcirkel4_5[i].y = 350-(i*5);
        Betcirkel4_5[i].w = 23;
        Betcirkel4_5[i].h = 23;
    }

    // Betcirkel 4 för plats pokerchip 25
    for(i = 0; i < MAX; i++)
    {
        Betcirkel4_25[i].x = 696;
        Betcirkel4_25[i].y = 350-(i*5);
        Betcirkel4_25[i].w = 23;
        Betcirkel4_25[i].h = 23;
    }

    // Betcirkel 4 för plats pokerchip 50
    for(i = 0; i < MAX; i++)
    {
        Betcirkel4_50[i].x = 665;
        Betcirkel4_50[i].y = 367-(i*5);
        Betcirkel4_50[i].w = 23;
        Betcirkel4_50[i].h = 23;
    }

    // Betcirkel 4 för plats pokerchip 100
    for(i = 0; i < MAX; i++)
    {
        Betcirkel4_100[i].x = 688;
        Betcirkel4_100[i].y = 367-(i*5);
        Betcirkel4_100[i].w = 23;
        Betcirkel4_100[i].h = 23;
    }


    // Betcirkel 5 för plats pokerchip 1
    for(i = 0; i < MAX; i++)
    {
        Betcirkel5_1[i].x = 777;
        Betcirkel5_1[i].y = 260-(i*5);
        Betcirkel5_1[i].w = 23;
        Betcirkel5_1[i].h = 23;
    }

    // Betcirkel 5 för plats pokerchip 5
    for(i = 0; i < MAX; i++)
    {
        Betcirkel5_5[i].x = 800;
        Betcirkel5_5[i].y = 260-(i*5);
        Betcirkel5_5[i].w = 23;
        Betcirkel5_5[i].h = 23;
    }

    // Betcirkel 5 för plats pokerchip 25
    for(i = 0; i < MAX; i++)
    {
        Betcirkel5_25[i].x = 823;
        Betcirkel5_25[i].y = 260-(i*5);
        Betcirkel5_25[i].w = 23;
        Betcirkel5_25[i].h = 23;
    }

    // Betcirkel 5 för plats pokerchip 50
    for(i = 0; i < MAX; i++)
    {
        Betcirkel5_50[i].x = 795;
        Betcirkel5_50[i].y = 275-(i*5);
        Betcirkel5_50[i].w = 23;
        Betcirkel5_50[i].h = 23;
    }

    // Betcirkel 5 för plats pokerchip 100
    for(i = 0; i < MAX; i++)
    {
        Betcirkel5_100[i].x = 818;
        Betcirkel5_100[i].y = 275-(i*5);
        Betcirkel5_100[i].w = 23;
        Betcirkel5_100[i].h = 23;
    }

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

    initiera_kortleken(kortlek);
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
    if( !init() )
    {
        printf( "Failed to initialize!\n" );
    }
    else
    {
        //Load media
        if( !loadMedia() )
        {
            printf( "Failed to load media!\n" );
        }
        else
        {
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

    int x=0, y=0, cardNr=0, id=-1;
    int money = 500;
    int myTurn = 0;
    int bet = 0;
    bool klar = false;



    while( !quit )
    {
        frame = 0;
        //Handle events on queue
        while( SDL_PollEvent( &e ) != 0 )
        {
            //ClearScreen();
            SDL_GetMouseState(&x,&y);
            //printf("x: %d\ny: %d\n",x,y);

            //User requests quit
            if( e.type == SDL_QUIT )
            {
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
            }
            else if(e.type == SDL_MOUSEBUTTONDOWN)
            {
                if(PLAYBUTTON && window == START)
                {

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
                }
                else if(EXITBUTTON)
                {
                    if(window == START)
                    {
                        quit = true;
                    }
                    else if (window == TABLE)
                    {
                        /* */
                        //sendToServer("exit", sd);
                        SDLNet_TCP_Close(sd);
                        window = START;
                    }
                }
                else if(BETBUTTON && window == TABLE)
                {
                    //bet = 0;
                    klar = true;
                }
                else if(CLEARBUTTON && window == TABLE)
                {
                    bet = 0;
                    money=500;
                    hit = false;
                }
                else if(HITBUTTON && window == TABLE && myTurn == 1)
                {

                    //id = reciveFromServer(sd);

                    id = rand()%260+0;
                    printf("\n\nid recived = %d\n", id);

                    cardFrame = IdToVisualCard(id,kortlek);
                    bordskort[nykort] = cardFrame;
                    nykort++;

                    printf("cardFrame = %d\n", cardFrame);
                    hit = true;
                    //id += 1;
                }
                else if(STANDBUTTON && window == TABLE)
                {
                    //sendToServer("stand", sd);
                }
                else if(DOUBLEBUTTON && window == TABLE)
                {
                    quit = true;
                }
                else if(SPLITBUTTON && window == TABLE)
                {
                    quit = true;
                }
                else if(M1 && window == TABLE)
                {
                    money -= 1;
                    bet += 1;

                }
                else if(M5 && window == TABLE)
                {
                    money -=5;
                    bet += 5;

                }
                else if(M25 && window == TABLE)
                {
                    money -=25;
                    bet += 25;

                }
                else if(M50 && window == TABLE)
                {
                    money -=50;

                    bet += 50;

                }
                else if(M100 && window == TABLE)
                {
                    money -=100;
                    bet += 100;

                }
                // money -= bet;
                printf("Bet: $%d",bet);

                sprintf(command2,"Bet: $%d",bet);
                printf("Money: $%d\n", money);
                sprintf(command, "Money: $%d",money);
                //bet = 0;


            }
            else if(e.type == SDL_MOUSEMOTION)
            {
                if(PLAYBUTTON)  //Innanför knappen?
                {
                    frame=2;
                }
            }
            if(window == START )
            {
                SDL_RenderCopy(gRenderer,bTexture,NULL,NULL);
                SDL_RenderCopy(gRenderer,mPlayButton,&gSpriteClips[frame],&PlayButton);
                SDL_RenderCopy(gRenderer,exitTexture,NULL,&ExitRect);
                //SDL_RenderCopy(gRenderer, pottTexture,NULL,&renderRect);
                SDL_RenderPresent(gRenderer);
            }
            else if (window==TABLE)
            {
                SDL_RenderCopy(gRenderer, btable, NULL, NULL);
                ritaText(command2,renderRect2);
                ritaText(command,renderRect);


                    if(hit == true)
                    {
                        for (i=0; i<nykort; i++)
                        {
                            SDL_RenderCopy(gRenderer, kort, &cardSheet[bordskort[i]], &table1[i]);

                        }
                    }


                    if(myClientNr == 0)
                    {
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


                    if(myClientNr == 1)
                    {
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

                        else if(bet ==5 || bet < 25)
                        {
                            for(i = 0; i<(bet/5); i++)
                            {
                                SDL_RenderCopy(gRenderer, pokerchip5, NULL, &Betcirkel2_5[i]);
                            }
                        }

                        else if(bet == 25 || bet < 50)
                        {
                            for(i = 0; i <(bet/25); i++)
                            {
                                SDL_RenderCopy(gRenderer, pokerchip25, NULL, &Betcirkel2_25[i]);
                            }
                        }

                        else if(bet == 50 || bet < 100)
                        {
                            for(i = 0; i < (bet/50); i++)
                            {
                                SDL_RenderCopy(gRenderer, pokerchip50, NULL, &Betcirkel2_50[i]);
                            }
                        }

                        else if(bet == 100)
                        {
                            for(i = 0; i < (bet / 100); i++)
                            {
                                SDL_RenderCopy(gRenderer, pokerchip100, NULL, &Betcirkel2_100[i]);
                            }
                        }

                    }

                    if(myClientNr == 2)
                    {
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

                        else if(bet == 5 || bet < 25)
                        {
                            for(i = 0; i < (bet/5); i++)
                            {
                                SDL_RenderCopy(gRenderer, pokerchip5, NULL, &Betcirkel3_5[i]);
                            }
                        }

                        else if(bet == 25 || bet < 50)
                        {
                            for(i = 0; i < (bet/25); i++)
                            {
                                SDL_RenderCopy(gRenderer, pokerchip25, NULL, &Betcirkel3_25[i]);
                            }
                        }

                        else if(bet == 50  || bet < 100)
                        {
                            for(i = 0; i < (bet/50); i++)
                            {
                                SDL_RenderCopy(gRenderer, pokerchip50, NULL, &Betcirkel3_50[i]);
                            }
                        }

                        else if(bet == 100)
                        {
                            for(i = 0; i < (bet/100); i++)
                            {
                                SDL_RenderCopy(gRenderer, pokerchip100, NULL, &Betcirkel3_100[i]);
                            }
                        }

                    }



                    if(myClientNr == 3)
                    {
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

                        else if(bet == 5 || bet < 25)
                        {
                            for(i = 0; i < (bet/5); i++)
                            {
                                SDL_RenderCopy(gRenderer, pokerchip5, NULL, &Betcirkel4_5[i]);
                            }
                        }

                        else if(bet == 25 || bet < 50)
                        {
                            for(i = 0; i < (bet/25); i++)
                            {
                                SDL_RenderCopy(gRenderer, pokerchip25, NULL, &Betcirkel4_25[i]);
                            }
                        }

                        else if(bet == 50 || bet < 100)
                        {
                            for(i = 0; i < (bet/50); i++)
                            {
                                SDL_RenderCopy(gRenderer, pokerchip50, NULL, &Betcirkel4_50[i]);
                            }
                        }

                        else if(bet == 100)
                        {
                            for(i = 0; i < (bet/100); i++)
                            {
                                SDL_RenderCopy(gRenderer, pokerchip100, NULL, &Betcirkel4_100[i]);
                            }
                        }
                    }

                    if(myClientNr == 4)
                    {
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

                        else if(bet == 5 || bet < 25)
                        {
                            for(i = 0; i < (bet/5); i++)
                            {
                                SDL_RenderCopy(gRenderer, pokerchip5, NULL, &Betcirkel5_5[i]);
                            }
                        }

                        else if(bet == 25 || bet < 50)
                        {
                            for(i = 0; i < (bet/25); i++)
                            {
                                SDL_RenderCopy(gRenderer, pokerchip25, NULL, &Betcirkel5_25[i]);
                            }
                        }

                        else if(bet == 50 || bet < 100)
                        {
                            for(i = 0; i < (bet/50); i++)
                            {
                                SDL_RenderCopy(gRenderer, pokerchip50, NULL, &Betcirkel5_50[i]);
                            }
                        }

                        else if(bet == 100)
                        {
                            for(i = 0; i < (bet/100); i++)
                            {
                                SDL_RenderCopy(gRenderer, pokerchip100, NULL, &Betcirkel5_100[i]);
                            }
                        }
                    }
                    if(klar == true)
                    {
                        //money -= bet;
                        bet = 0;
                    }
                    ritaText(command,renderRect);




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

    SDL_Surface* table = IMG_Load("bilder\\bord9999.0.png");

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
    SDL_Surface* table = IMG_Load("bilder/bord9999.0.png");
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


    SDL_GetError();



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
#ifdef __WIN
    font = TTF_OpenFont("fonts\\KeepCalm-Medium.ttf", 28);
#else
    font = TTF_OpenFont("fonts/KeepCalm-Medium.ttf", 28);
#endif
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


