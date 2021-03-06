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
//#define IPADRESS "130.229.190.95"
#define IPADRESS "169.254.211.44"
//#define IPADRESS "193.10.39.174"
typedef struct Reciveinfo
{
    TCPsocket sd, tableSocket;
    SDLNet_SocketSet set;
    int* quit1;
    IPaddress ip;
    int myClientNr;

}Rinfo;

Kort kortlek[ANTALKORT];

int updateRender(int clientNr, int kortantal,int bordskort[]);
int reciveInfo(void* info);


//=============================================MAIN==================================================
int main( int argc, char* args[] )                  // Christian Ekenstedt
{
    Rinfo recive;
    TCPsocket sd = NULL;


    char  command[512],command2[512];
    int window = 0, frame = 0, cardFrame = 0, bet = 0, myTurn = 0, money = 500, x=0, y=0, id=-1;
    int bordskort[11]= {0}, nykort=0, quit1, k, j, i;
    bool hit = false, quit = false, klar = false, engang = true, endround = false, dubbel = false, dubbelcheck = false, stand = false, blackjack = false, betround = true;
    recive.myClientNr=0;
    char red[10];
    int temp[10], ready = 0, myValue = 0, dealerValue = 0, temporar = 0, kortantal=0;

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

            SDL_GetMouseState(&x,&y);

            /*while (SDL_PollEvent( &e ) != 0)
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
                    else if(SPLITBUTTON && window == TABLE)
                    {
                        system("./prototyp1");
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
             */

            //if (SDLNet_CheckSockets(recive.set, 100)>0) {
                if ((SDLNet_TCP_Recv(recive.sd , red, 512+1) < 0)) {
                    fprintf(stderr, "SDLNet_TCP_Recv: %s\n", SDLNet_GetError());
                    exit(EXIT_FAILURE);
                }
                if(strstr(red, "ready")){
                    temp[0] = red[5];
                    recive.myClientNr = atoi(temp);
                    printf("My client nr: %d\n",recive.myClientNr);
                    ready = 1;
                    for (i=0; i<2; i++) {
                        temp[i] = '?';
                    }
                }
            //}

            updateRender(0, kortantal, bordskort);
            updateRender(1, kortantal, bordskort);
            updateRender(2, kortantal, bordskort);
            updateRender(3, kortantal, bordskort);
            updateRender(4, kortantal, bordskort);

            while (ready == 1) {

                // Game progress here!
                // 0. Recive "ready command" - Check
                // 1. Place your bets. - Comming soon!
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
                            dubbel = true;
                        }
                        else if(SPLITBUTTON && window == TABLE)
                        {
                            system("start Chattclient.exe");
                            //quit = true;
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


                /* BET ROUND */
                if (betround){
                    engang = false;
                    playerPosition(recive.myClientNr, bet, false, 0, dubbelcheck, money, false, 0);

                    if (klar) {
                        engang = true;
                        betround = false;
                    }
                    SDL_RenderPresent(gRenderer);

                }
                /* BET ROUND ENDS*/

                /* RECIVE 2 CARDS FROM THE DEALER */
                if (engang == true) {
                    tableInfo[5][2] = -1;
                    playerPosition(5, bet, false, nykort, dubbelcheck, money, false, bordskort); // Visar baksidan av kortet
                    bordskort[1] = IdToVisualCard(tableInfo[5][1], kortlek); // Visar det ena kortet
                    SDL_RenderCopy(gRenderer, kort, &cardSheet[bordskort[1]], &tableDealer[1]);
                    dealerValue += IdToValue(tableInfo[5][1],kortlek);

                    printf("DEALER HAVE ID = %d\nAnd a value of %d\n",tableInfo[5][1], dealerValue);

                    /*while (!exit1 ) {
                        if (SDLNet_TCP_Recv(recive.sd, buffer, 512) > 0)
                        {
                            //printf("ID = %s", buffer);

                            //id=atoi(buffer); // Stoppar in ID:t i variabel ID.
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
                    }*/
                    SDL_Delay(1000);
                    kortantal = 0;
                    id = tableInfo[recive.myClientNr][kortantal];
                    printf("id = %d\n", id);
                    printf("%d: id = %d\n och kortantal = %d\n", recive.myClientNr,tableInfo[recive.myClientNr][kortantal], kortantal);
                    bordskort[kortantal] = IdToVisualCard(id, kortlek);
                    myValue += IdToValue(id,kortlek);

                    kortantal++;
                    updateRender(recive.myClientNr, kortantal, bordskort);

                    printf("nu");
                    /*while (!exit1) {
                        if (SDLNet_TCP_Recv(recive.sd, buffer, 512) > 0)
                        {
                            //id=atoi(buffer); // Stoppar in ID:t i variabel ID.
                            printf("ID = %d", id);
                            exit1 =1;
                        }else{
                            fprintf(stderr, "SDLNet_TCP_Recv: %s\n", SDLNet_GetError());
                            exit(EXIT_FAILURE);
                        }

                    }*/
                    printf("Kortantal = %d\n", kortantal);
                    id = tableInfo[recive.myClientNr][kortantal];printf("id = %d\n", id);
                    bordskort[kortantal] = IdToVisualCard(id, kortlek);
                    myValue += IdToValue(id,kortlek);


                    kortantal++;
                    updateRender(recive.myClientNr, kortantal, bordskort);
                    hit = false;
                    engang = false;


                    SDL_RenderPresent(gRenderer);

                }


                playerPosition(5, bet, false, nykort, dubbelcheck, money, false, bordskort);

                for (i=1; i<2; i++)
                {
                    SDL_RenderCopy(gRenderer, kort, &cardSheet[IdToVisualCard(tableInfo[5][i],kortlek)], &tableDealer[i]);

                }

                /* CHOSE HIT OR STAND */
                if(hit == true){

                    sendToServer("hit", recive.sd); // Vi skickar till servern att vi vill ha ett nytt kort

                    /*while (!exit1 ) {
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
                    }*/
                    updateRender(recive.myClientNr, kortantal, bordskort);
                    SDL_Delay(1000);
                    id = tableInfo[recive.myClientNr][kortantal];printf("id = %d\n", id);
                    bordskort[kortantal] = IdToVisualCard(id, kortlek);
                    myValue += IdToValue(id,kortlek);
                    kortantal++;
                    updateRender(recive.myClientNr, kortantal, bordskort);
                    //SDL_RenderCopy(gRenderer, kort, &cardSheet[bordskort[kortantal]], &table5[kortantal]);
                    //playerPosition(recive.myClientNr, bet, true, kortantal, money, bordskort);

                    SDL_RenderPresent(gRenderer);
                    hit = false;
                }else if (stand == true){
                    sendToServer("stand", recive.sd);
                    endround = true;
                    stand = false;
                }else if (dubbel == true){
                    sendToServer("dubbel", recive.sd);
                    updateRender(recive.myClientNr, kortantal, bordskort);
                    SDL_Delay(1000);
                    id = tableInfo[recive.myClientNr][kortantal];printf("id = %d\n", id);
                    bordskort[kortantal] = IdToVisualCard(id, kortlek);
                    myValue += IdToValue(id,kortlek);
                    kortantal++;
                    updateRender(recive.myClientNr, kortantal, bordskort);
                    //SDL_RenderCopy(gRenderer, kort, &cardSheet[bordskort[kortantal]], &table5[kortantal]);
                    //playerPosition(recive.myClientNr, bet, true, kortantal, money, bordskort);


                    engang = false;
                    dubbelcheck = true;
                    playerPosition(recive.myClientNr, bet, false, 0, dubbelcheck, money, false, 0);
                    //dubbelcheck = false;

                    SDL_RenderPresent(gRenderer);
                    endround = true;
                    dubbel = false;

                }else if (myValue > 21) {

                    for(j=0;j<kortantal;j++)
                    {
                        for(k=0;k<ANTALKORT;k++)
                        {
                            if(id == (13*j))
                            {
                                myValue = (myValue-10);
                            }
                        }
                    }

                    SDL_Delay(10);
                    endround = true;
                }else if (myValue == 21){
                    SDL_Delay(10);
                    endround = true;
                    blackjack = true;
                }
                /* END ROUND */
                if (endround){

                    kortantal--;
                    if(SDLNet_TCP_Recv(recive.sd, red, 512+1) < 0){
                        fprintf(stderr, "SDLNet_TCP_Recv: %s", SDLNet_GetError());
                        exit(EXIT_FAILURE);
                    }
                    if (red[0] == '@') {
                        printf("\n-------------------------------Round ended!\n---------------------------\n");
                    }

                    // RÄKNA DEALERS KORT!
                    printf("\n---------------------------------Dealer Plays-----------------------------------\n");

                    bordskort[0] = IdToVisualCard(tableInfo[5][0], kortlek);
                    SDL_RenderCopy(gRenderer, kort, &cardSheet[bordskort[0]], &tableDealer[0]);

                    bordskort[1] = IdToVisualCard(tableInfo[5][1], kortlek);
                    SDL_RenderCopy(gRenderer, kort, &cardSheet[bordskort[1]], &tableDealer[1]);

                    dealerValue = 0;
                    for(j = 0;j<MAXCARDS;j++)
                    {
                        if (tableInfo[5][j] == -1) {
                            temporar++;
                        }
                        if(tableInfo[5][j] != -1){
                            bordskort[j] = IdToVisualCard(tableInfo[5][j],kortlek); // 0 for player, 1 for dealer
                            dealerValue += IdToValue(tableInfo[5][j],kortlek);
                            SDL_RenderCopy(gRenderer, kort, &cardSheet[bordskort[j]], &tableDealer[j]);
                        }else j++;
                        if (temporar == MAXCARDS) {
                            dealerValue = 0;
                        }

                    }
                    SDL_Delay(100);
                    printf("\n---------------------------------Dealer done!-----------------------------------\n");
                    temporar = 0;
                    printf("\nDealer have %d\nMy value %d\n\n", dealerValue,myValue);

                    printf("\n--------------------------------------------------------------------------------\n");
                    money += whoWon(myValue, dealerValue, bet, blackjack, dubbelcheck);
                    dubbelcheck = false;
                    printf("Your balance is: $%d \n\n", money);
                    //ritaText(command,renderRect);
                    SDL_Delay(5000);
                    SDL_RenderPresent(gRenderer);
                    SDL_Delay(5000);
                    myValue = 0;
                    engang = true;
                    endround = false;
                    betround = true;
                    klar = false;
                    kortantal = 0;
                    nykort = 0;
                    ready = 0;
                    bet=0;
                    //SDL_RenderClear(gRenderer);
                    SDL_RenderCopy(gRenderer, btable, NULL, NULL);
                    SDL_RenderPresent(gRenderer);



                }

            }
        }
    }
    //Free resources and close SDL
    closeW();
    return 0;
}

int reciveInfo(void* info){
    Rinfo* recive = (Rinfo*) info;
    char red[1024+1];
    int j, i, nykort = 0, bordskort[11], id = -1;

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
    }else printf("UPDATE SOCKET CONNECTED!\n");

    while (1) {
        SDL_Delay(50);
        if ((SDLNet_TCP_Recv(recive->tableSocket , red, 1024+1) > 0)) {
            if (red[0] == '#') {
                stringToArray(red);
            }
        }else {
            fprintf(stderr, "SDLNet_TCP_Recv: %s\n", SDLNet_GetError());
            exit(EXIT_FAILURE);
        }

        //printf("\n%s\n", red);

    }
    return 0;
}

/*int updateRender(int clientNr, int kortantal, int bordskort[]){
    int i=0;
    if (clientNr == 0) {
        for (i=0; i<kortantal; i++) // FUNGERAR ENDAST FÖR SPELARE 0
        {
            SDL_RenderCopy(gRenderer, kort, &cardSheet[bordskort[i]], &table1[i]);

        }
    }else if (clientNr == 1) {
        for (i=0; i<kortantal; i++) // FUNGERAR ENDAST FÖR SPELARE 1
        {
            SDL_RenderCopy(gRenderer, kort, &cardSheet[bordskort[i]], &table2[i]);

        }
    }else if (clientNr == 2) {
        for (i=0; i<kortantal; i++) // FUNGERAR ENDAST FÖR SPELARE 2
        {
            SDL_RenderCopy(gRenderer, kort, &cardSheet[bordskort[i]], &table3[i]);

        }
    }else if (clientNr == 3) {
        for (i=0; i<kortantal; i++) // FUNGERAR ENDAST FÖR SPELARE 3
        {
            SDL_RenderCopy(gRenderer, kort, &cardSheet[bordskort[i]], &table4[i]);

        }
    }else if (clientNr == 4) {
        for (i=0; i<kortantal; i++) // FUNGERAR ENDAST FÖR SPELARE 4
        {
            SDL_RenderCopy(gRenderer, kort, &cardSheet[bordskort[i]], &table5[i]);

        }
    }*/

int updateRender(int clientNr, int kortantal, int bordskort[]){
    int i = 0,j=0;
    for(clientNr=MAXCLIENTS-1;clientNr>0;clientNr--)
    {
        printf("clientNR4: %d\n", clientNr);
        if (clientNr == 4)
        {
            for(j=0;j<MAXCARDS;j++)
            {
                if(tableInfo[4][j] > 0)
                {
                    for (i=0; i<kortantal; i++) // FUNGERAR ENDAST FÖR SPELARE 0
                    {
                        SDL_RenderCopy(gRenderer, kort, &cardSheet[bordskort[i]], &table5[i]);

                    }
                }
                else if(tableInfo[4][j] == -1)
                {
                    break;
                }
            }

        }else if (clientNr == 3) {
            for(j=0;j<MAXCARDS;j++)
            {
                if(tableInfo[3][j] > 0)
                {
                    for (i=0; i<kortantal; i++) // FUNGERAR ENDAST FÖR SPELARE 0
                    {
                                printf("clientNR1: %d\n", clientNr);
                        SDL_RenderCopy(gRenderer, kort, &cardSheet[bordskort[i]], &table4[i]);

                    }
                }
                else if(tableInfo[3][j] == -1)
                {
                    break;
                }
            }
        }else if (clientNr == 2) {
            for(j=0;j<MAXCARDS;j++)
            {
                if(tableInfo[2][j] > 0)
                {
                    for (i=0; i<kortantal; i++) // FUNGERAR ENDAST FÖR SPELARE 0
                    {
                                printf("clientNR2: %d\n", clientNr);
                        SDL_RenderCopy(gRenderer, kort, &cardSheet[bordskort[i]], &table3[i]);

                    }
                }
                else if(tableInfo[2][j] == -1)
                {
                    break;
                }
            }
        }else if (clientNr == 1) {
            for(j=0;j<MAXCARDS;j++)
            {
                if(tableInfo[1][j] > 0)
                {
                    for (i=0; i<kortantal; i++) // FUNGERAR ENDAST FÖR SPELARE 0
                    {
                                printf("clientNR3: %d\n", clientNr);
                        SDL_RenderCopy(gRenderer, kort, &cardSheet[bordskort[i]], &table2[i]);

                    }
                }
                else if(tableInfo[1][j] == -1)
                {
                    break;
                }
            }
        }else if (clientNr == 0) {
            for(j=0;j<MAXCARDS;j++)
            {
                if(tableInfo[0][j] > 0)
                {
                    for (i=0; i<kortantal; i++) // FUNGERAR ENDAST FÖR SPELARE 0
                    {
                                printf("clientNR4: %d\n", clientNr);
                        SDL_RenderCopy(gRenderer, kort, &cardSheet[bordskort[i]], &table1[i]);

                    }
                }
                else if(tableInfo[0][j] == -1)
                {
                    break;
                }
            }
        }
    }
    //SDL_RenderPresent(gRenderer);
    return 0;
}
