//
//  main.c
//  prototyp1
//
//  Created by Christian Ekenstedt on 2015-04-16.
//  Copyright (c) 2015 Grupp 3. All rights reserved.
//

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "multiOS.h"
#include "gamelogic.h"
#include "functions.h"
#include "coordinates.h"
#define IPADRESS "193.10.39.174"

typedef struct Reciveinfo
{
    TCPsocket sd, tableSocket;
    SDLNet_SocketSet set;
    int* quit1;
    IPaddress ip;

}Rinfo;

Kort kortlek[ANTALKORT];


int reciveInfo(void* info);


//=============================================MAIN==================================================
int main( int argc, char* args[] )                  // Christian Ekenstedt
{
    Rinfo recive;
    TCPsocket sd = NULL;
    IPaddress ip;

    char hostIP[] = "169.254.211.44", command[512],command2[512];
    int window = 0, frame = 0, cardFrame = 0, myClientNr=0,bet = 0, myTurn = 0, money = 500, x=0, y=0, id=-1;
    int bordskort[11], nykort=0, quit1, j, i;
    bool hit = false, quit = false, klar = false, engang = true, endround = false, stand = false, blackjack = false;

    char red[10];
    int temp[10], ready = 0, myValue = 0, dealerValue = 0, temporar = 0;

    /* =============================INITIERINGAR ====================================================*/

    if (SDLNet_Init() < 0)
    {
        fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

    srand(time(NULL));
    positionInit(); // Initierar positioner for knappar osv.
    initiera_kortleken(kortlek); // inti the card deck.

    recive.set = SDLNet_AllocSocketSet(2); //
    recive.quit1 = &quit1; //
    SDLNet_TCP_AddSocket(recive.set, recive.sd);

    for (i=0; i<MAXCLIENTS+1; i++) { // Nollställer arrayen som innehåller idn på kort.
        for (j=0; j<15; j++) {
            tableInfo[i][j] = -1;
        }
    }



    /* =============================================================================================*/
    // ############################ NETWORK INIT ####################################################

    if (SDLNet_ResolveHost(&recive.ip, IPADRESS, 10000) < 0)
    {
        fprintf(stderr, "SDLNet_ResolveHost: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }



    // ############################# NETWORK INIT END ###############################################

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


    while( !quit )
    {
        frame = 0;
        while( SDL_PollEvent( &e ) != 0 )
        {
            SDL_GetMouseState(&x,&y);
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
                }
            }
            else if(e.type == SDL_MOUSEBUTTONDOWN)
            {
                if(PLAYBUTTON && window == START)
                {
                    /* HERE DO WE CONNECT TO THE GAME SERVER */
                    //printf("ANSLUT DÅ!\n");
                    if (!(recive.sd = SDLNet_TCP_Open(&recive.ip)))
                    {
                        fprintf(stderr, "SDLNet_TCP_Open: %s\n", SDLNet_GetError());
                        exit(EXIT_FAILURE);
                    }

                    // Starts the update thread.
                    window=TABLE;
                    SDL_DetachThread(SDL_CreateThread(reciveInfo, "Recive-thread", (void*)&recive));
                    frame=1;
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
                        sendToServer("exit", sd);
                        SDLNet_TCP_Close(sd);
                        window = START;
                    }
                }/*
                else if(BETBUTTON && window == TABLE)
                {
                    klar = true;
                }
                else if(CLEARBUTTON && window == TABLE)
                {
                    money += bet;
                    bet = 0;
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
                }
                else if(STANDBUTTON && window == TABLE)
                {
                    //sendToServer("stand", sd);
                }
                else if(DOUBLEBUTTON && window == TABLE)
                {
                    system("./prototyp1");
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
                printf("Bet: $%d\n",bet);

                sprintf(command2,"Bet: $%d",bet);
                printf("Money: $%d\n", money);
                sprintf(command, "Money: $%d",money);*/
            }
            else if(e.type == SDL_MOUSEMOTION)
            {
                if(PLAYBUTTON)  //Innanför knappen?
                {
                    frame=2;
                }

            }
        }

        if(window == START )
        {
            SDL_RenderCopy(gRenderer,bTexture,NULL,NULL);
            SDL_RenderCopy(gRenderer,mPlayButton,&gSpriteClips[frame],&PlayButton);
            SDL_RenderCopy(gRenderer,exitTexture,NULL,&ExitRect);
            SDL_RenderPresent(gRenderer);
        }
        else if (window==TABLE)
        {
            SDL_RenderCopy(gRenderer, btable, NULL, NULL);
            SDL_RenderPresent(gRenderer);

            if ((SDLNet_TCP_Recv(recive.sd , red, 1024+1) < 0)) {
                fprintf(stderr, "SDLNet_TCP_Recv: %s\n", SDLNet_GetError());
                exit(EXIT_FAILURE);
            }
            if(strstr(red, "ready")){
                temp[0] = red[5];
                myClientNr = atoi(temp);
                printf("My client nr: %d\n",myClientNr);
                ready = 1;
                for (i=0; i<2; i++) {
                    temp[i] = '?';
                }
            }

            while (ready == 1) {
                
                // Game progress here!
                // 0. Recive "ready command" - Check
                // 1. Place your bets.
                // 2. Recive 2 cards - Check
                // 3. Hit or stand - In progress
                
                SDL_Delay(10);
                SDL_GetMouseState(&x,&y);
                SDL_RenderCopy(gRenderer, btable, NULL, NULL);
                


                while (SDL_PollEvent( &e ) != 0)
                {
                    //User requests quit
                    if( e.type == SDL_QUIT )
                    {
                        sendToServer("exit", recive.sd);
                        SDLNet_TCP_Close(recive.sd);
                        quit = true;
                        ready = 0;
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
                            case SDLK_ESCAPE:
                                sendToServer("exit", recive.sd);
                                SDLNet_TCP_Close(recive.sd);
                                window = START;
                                ready = 0;
                                break;
                        }
                    }
                    else if(e.type == SDL_MOUSEBUTTONDOWN)
                    {
                        //printf("NU TRYCKER VI NER MUSKNAPPEN!\n");
                        if(EXITBUTTON)
                        {
                            if(window == START)
                            {
                                quit = true;
                            }
                            else if (window == TABLE)
                            {
                                printf("Exit!\n");
                                sendToServer("exit", recive.sd);
                                SDLNet_TCP_Close(recive.sd);
                                ready = 0;
                                window = START;
                            }
                        }
                        else if(BETBUTTON && window == TABLE)
                        {
                          klar = true;
                        }
                        else if(CLEARBUTTON && window == TABLE)
                        {
                          money += bet;
                          bet = 0;
                          hit = false;
                        }
                        else if(HITBUTTON && window == TABLE && myTurn == 1)
                        {
                              hit = true;
                            

                        }
                        else if(STANDBUTTON && window == TABLE)
                        {
                            stand = true;
                        }
                        else if(DOUBLEBUTTON && window == TABLE)
                        {
                              system("./prototyp1");
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
                          printf("Bet: $%d\n",bet);
                          sprintf(command2,"Bet: $%d",bet);
                          printf("Money: $%d\n", money);
                          sprintf(command, "Money: $%d",money);
                        }
                    }
                // Render the text on the screen, your amount of money and what you bet.
                //ritaText(command2,renderRect2);
                //ritaText(command,renderRect);
                // playerPosition takes the client number you have to placve your cards and chips on the right position.


                


                /* ======================================================*/
                /* BET ROUND */

                /* BET ROUND ENDS*/

                /* RECIVE 2 CARDS FROM THE DEALER */
                if (engang == true) {
                    
                    hit = false;
                    playerPosition(5, bet, hit, nykort, money, bordskort);
                    //nykort++;
                    //SDL_RenderPresent(gRenderer);
                    hit = true;
                    bordskort[1] = IdToVisualCard(tableInfo[5][1], kortlek);
                    SDL_RenderCopy(gRenderer, kort, &cardSheet[bordskort[1]], &tableDealer[1]);
                    dealerValue += IdToValue(tableInfo[5][1],kortlek);
                    //playerPosition(5, bet, hit, nykort, money, bordskort);
                    //SDL_RenderPresent(gRenderer);
                    nykort = 0;
                    printf("DEALER HAVE ID = %d\nAnd a value of %d\n",tableInfo[5][1], dealerValue);
                    
                    //SDL_RenderPresent(gRenderer);
                    SDL_Delay(500);
                    int exit1 = 0;
                    char buffer[512];
                    while (!exit1 ) {
                        if (SDLNet_TCP_Recv(recive.sd, buffer, 512) > 0)
                        {
                            //printf("ID = %s", buffer);

                            id=atoi(buffer); // Stoppar in ID:t i variabel ID.
                            printf("ID = %d", id);
                            if (id == 0){
                                //exit1 = 0;
                                printf("NU FICK VI FEL!\n");
                            }
                            exit1 = 1;

                        }else{
                            fprintf(stderr, "SDLNet_TCP_Recv: %s\n", SDLNet_GetError());
                            exit(EXIT_FAILURE);
                        }
                    }
                    printf("id = %d\n", id);
                    bordskort[nykort++] = IdToVisualCard(id, kortlek);
                    myValue += IdToValue(id,kortlek);
                    hit = true;
                    //SDL_RenderCopy(gRenderer, kort, &cardSheet[bordskort[0]], &tableDealer[0]);
                    playerPosition(myClientNr, bet, hit, nykort, money, bordskort);
                    //SDL_RenderPresent(gRenderer);
                    exit1 = 0;
                    while (!exit1) {
                        if (SDLNet_TCP_Recv(recive.sd, buffer, 512) > 0)
                        {
                            id=atoi(buffer); // Stoppar in ID:t i variabel ID.
                            printf("ID = %d", id);
                            exit1 =1;
                        }else{
                            fprintf(stderr, "SDLNet_TCP_Recv: %s\n", SDLNet_GetError());
                            exit(EXIT_FAILURE);
                        }

                    }
                    printf("id = %d\n", id);
                    bordskort[nykort++] = IdToVisualCard(id, kortlek);
                    hit = true;
                    //SDL_RenderCopy(gRenderer, kort, &cardSheet[bordskort[1]], &table5[1]);
                    playerPosition(myClientNr, bet, hit, nykort, money, bordskort);
                    
                    hit = false;
                    engang = false;
                    myValue += IdToValue(id,kortlek);

                    //printf("uppdaterad\n");
                    SDL_RenderPresent(gRenderer);

                }


                /* CHOSE HIT OR STAND */
                if(hit == true){
                    
                    sendToServer("hit", recive.sd);
                    
                    int exit1 = 0;
                    char buffer[512];
                    while (!exit1 ) {
                        if (SDLNet_TCP_Recv(recive.sd, buffer, 512) > 0)
                        {
                            //printf("ID = %s", buffer);
                            
                            id=atoi(buffer); // Stoppar in ID:t i variabel ID.
                            printf("ID = %d", id);
                            if (id == 0){
                                //exit1 = 0;
                                printf("NU FICK VI FEL!\n");
                            }
                            exit1 = 1;
                            
                        }else{
                            fprintf(stderr, "SDLNet_TCP_Recv: %s\n", SDLNet_GetError());
                            exit(EXIT_FAILURE);
                        }
                    }
                    printf("id = %d\n", id);
                    bordskort[nykort++] = IdToVisualCard(id, kortlek);
                    myValue += IdToValue(id,kortlek);
                    hit = true;
                    //SDL_RenderCopy(gRenderer, kort, &cardSheet[bordskort[0]], &tableDealer[0]);
                    //playerPosition(myClientNr, bet, hit, nykort, money, bordskort);
                    
                    playerPosition(myClientNr, bet, hit, nykort, money, bordskort);
                    SDL_RenderPresent(gRenderer);
                    
                    hit = false;
                }else if (stand == true){
                    sendToServer("stand", recive.sd);
                    endround = true;
                    stand = false;
                }
                
                if (myValue > 21) {
                    endround = true;
                }
                /* END ROUND */
                if (endround){
                    
                    if(SDLNet_TCP_Recv(recive.sd, red, 512+1) < 0){
                        fprintf(stderr, "SDLNet_TCP_Recv: %s", SDLNet_GetError());
                        exit(EXIT_FAILURE);
                    }
                    if (red[0] == '@') {
                        printf("\n-------------------------------Round ended!\n---------------------------\n");
                    }
                    
                    // RÄKNA DEALERS KORT!
                    printf("\n---------------------------------Dealer Plays-----------------------------------\n");
                    dealerValue = 0;
                    for(j = 0;j<MAXCARDS;j++)
                    {
                        if (tableInfo[5][j] == -1) {
                            temporar++;
                        }
                        if(tableInfo[5][j] != -1){
                            IdToCard(tableInfo[5][j],kortlek,1); // 0 for player, 1 for dealer
                            dealerValue += IdToValue(tableInfo[5][j],kortlek);
                            
                        }else j++;
                        if (temporar == MAXCARDS) {
                            dealerValue = 0;
                        }
                        
                    }
                    printf("\n---------------------------------Dealer done!-----------------------------------\n");
                    temporar = 0;
                    printf("\nDealer have %d\nMy value %d\n\n", dealerValue,myValue);
                    
                    printf("\n--------------------------------------------------------------------------------\n");
                    money += whoWon(myValue, dealerValue, bet, blackjack);
                    printf("Your balance is: $%d \n\n", money);
                    
                    
                    myValue = 0;
                    engang = true;
                    endround = false;
                }
                /* */

                //playerPosition(myClientNr, bet, hit, nykort, money, bordskort);
                //ready = 0;
                //SDL_RenderPresent(gRenderer);
                if(klar == true)
                {
                    bet = 0;
                }
                //SDL_RenderPresent(gRenderer);
            }
            //SDL_RenderPresent(gRenderer);
        }

        printf("utanför\n");
    }
    //Free resources and close SDL
    closeW();
    return 0;
}

int reciveInfo(void* info){
    Rinfo* recive = (Rinfo*) info;
    char red[1024+1];

    if (SDLNet_ResolveHost(&recive->ip, IPADRESS, 10001) < 0)
    {
        fprintf(stderr, "SDLNet_ResolveHost: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

    /* Open a connection with the IP provided (listen on the host's port) */
    if (!(recive->tableSocket = SDLNet_TCP_Open(&recive->ip)))
    {
        fprintf(stderr, "SDLNet_TCP_Open: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

    while (1) {
        SDL_Delay(100);
        if ((SDLNet_TCP_Recv(recive->tableSocket , red, 1024+1) > 0)) {
            if (red[0] == '#') {
                stringToArray(red);
            }
        }else {
            fprintf(stderr, "SDLNet_TCP_Recv: %s\n", SDLNet_GetError());
            exit(EXIT_FAILURE);
        }
        
        
        
    }
    return 0;
}
