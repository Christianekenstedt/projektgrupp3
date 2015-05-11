#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXCLIENTS 5
#include "gamelogic.h"
#include "multiOS.h"

typedef struct Reciveinfo
{
    TCPsocket SD;
    int* quit;
    SDLNet_SocketSet set;
}Rinfo;

TCPsocket sd;		/* Socket descriptor */

int tableInfo[MAXCLIENTS+1][15];
int reciveInfo(void* info);
int main(int argc, char **argv)
{
    IPaddress ip;		/* Server address */

    int quit, quit2, len, myValue=0, ready=0;
    char buffer[512], red[1];
    Kort kortlek[ANTALKORT]; // Deklarerar kortlek
    bool lose = false;
    Rinfo recive;
    
    recive.set = SDLNet_AllocSocketSet(1);
    SDLNet_AddSocket(recive.set, sd);
    recive.quit = &quit;
    SDL_DetachThread(SDL_CreateThread(reciveInfo, "Recive-thread", (void*)&recive));
    
    initiera_kortleken(kortlek); // bygger upp kortleken så man kan använda och jämföra ID med ett kort.
    
    /* Simple parameter checking */
    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s host port\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (SDLNet_Init() < 0)
    {
        fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

    /* Resolve the host we are connecting to */
    if (SDLNet_ResolveHost(&ip, argv[1], 2000) < 0)
    {
        fprintf(stderr, "SDLNet_ResolveHost: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

    /* Open a connection with the IP provided (listen on the host's port) */
    if (!(sd = SDLNet_TCP_Open(&ip)))
    {
        fprintf(stderr, "SDLNet_TCP_Open: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

    quit = 0;
    int ID=0;
    bool engang = true;
    while (!quit)
    {
        if (engang==true) {
            ClearScreen();
            printf("Waiting for your turn...\n");
            engang = false;
        }
        
        if ((SDLNet_TCP_Recv(sd , red, 1) < 0)) {
            fprintf(stderr, "SDLNet_TCP_Recv: %s\n", SDLNet_GetError());
            exit(EXIT_FAILURE);
        }
        printf("ready= %s\n",red);
        ready = atoi(red);
        //ready = 1;
        while (ready==1)
        {
            printf("Hit or Stand> ");
            scanf("%s", buffer);
            
            len = strlen(buffer) + 1;
            if (SDLNet_TCP_Send(sd, (void *)buffer, len) < len)
            {
                fprintf(stderr, "SDLNet_TCP_Send: %s\n", SDLNet_GetError());
                exit(EXIT_FAILURE);
            }
            
            if(strcmp(buffer, "exit") == 0){
                quit = 1;
                ready = 0;
            }
            if(strcmp(buffer, "quit") == 0)
                quit = 1;
            
            quit2 = 0; // FÖR RESPONS ENDAST!
            if (strstr(buffer,"card") || strstr(buffer,"hit")) {
                while (!quit2)
                {
                    if (SDLNet_TCP_Recv(sd, buffer, 512) > 0)
                    {
                        
                        ID=atoi(buffer); // Stoppar in ID:t i variabel ID.
                        printf("ID = %d", ID);
                        quit2 = 1;
                    }else{
                        fprintf(stderr, "SDLNet_TCP_Recv: %s\n", SDLNet_GetError());
                        exit(EXIT_FAILURE);
                    }
                }
                IdToCard(ID, kortlek);
                myValue += IdToValue(ID,kortlek);
                printf("You have a total of %d\n",myValue);
                if (myValue > 21) {
                    lose = true;
                }
            }else if(strstr(buffer,"stand")){
                printf("You stand at %d\n", myValue);
                myValue = 0;
                ready=0;
                engang = true;
            }else if (strstr(buffer, "!help")){
                
                printf("##################    HELP   ############################\n\n");
                printf("PLAY COMMANDS\nhit/card to ask for a new card.\nstand to stop your round.\n\nSERVER COMMANDS\n");
                printf("exit to safely disconnect.\nquit to terminate the server.\n\n");
                printf("################## HELP 1 (1) ###########################\n\n");
            }
            
            if (lose) {
                printf("You lose!\n");
                if (myValue>21) {
                    printf("Bust\n");
                }
                myValue = 0;
                lose = false;
                
                myValue = 0;
                ready=0;
                engang = true;
            }else if (myValue==21){
                printf("You got blackjack!\n");
                ready = 0;
                myValue = 0;
            }

        }
    }
    SDLNet_TCP_Close(sd);
    SDLNet_Quit();

    return EXIT_SUCCESS;
}
int reciveInfo(void* info){
    Rinfo* recive = (Rinfo*) info;
    int temp=0;
    while(!*(recive->quit))
    {
        if((temp = SDLNet_CheckSockets(recive->set, 0))>0) {
            if(SDLNet_TCP_Recv(recive->SD, &tableInfo, sizeof(tableInfo))<0){
                fprintf(stderr, "SDLNet_TCP_Recv: %s\n", SDLNet_GetError());
            }
            printf("Oj nu finns det info!\n");
        }else if(temp == -1){
            fprintf(stderr, "SDLNet_CheckSockets: %s\n",SDLNet_GetError());
        }
        else{
            printf("Inget att hämta!\n");
        }
        
        int i = 0,j = 0; //i = varje spelare, j = varje kortid i ordning
        for(i = 0;i<MAXCLIENTS;i++)
        {
            for(j = 0;j<15;j++)
                {
                    printf("Player [%d][%d] = %d\n",i,j,tableInfo[i][j]);
                }

        }
        printf("\n");

        
        
        
        
        
        
        
    }
    SDLNet_TCP_Close(sd);
    return 0;
}
