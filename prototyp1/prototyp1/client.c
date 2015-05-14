#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXCLIENTS 5
#include "gamelogic.h"
#include "multiOS.h"

typedef struct Reciveinfo
{
    TCPsocket sd;
    SDLNet_SocketSet set;
    int* quit;
    
}Rinfo;

int tableInfo[MAXCLIENTS+1][15];
char saker[512];

int reciveInfo(void* info);
void stringToArray(char sendstring[]);

int main(int argc, char **argv)
{
    IPaddress ip;		/* Server address */

    int quit, quit2, len, myValue=0, ready=0, i,j;
    char buffer[512], red[512];
    Kort kortlek[ANTALKORT]; // Deklarerar kortlek
    bool lose = false;
    Rinfo recive;
    int myClientNr = 0;
    
    recive.set = SDLNet_AllocSocketSet(1);
    
    recive.quit = &quit;
    for (i=0; i<MAXCLIENTS+1; i++) {
        for (j=0; j<15; j++) {
            tableInfo[i][j] = 9;
        }
    }
    
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
    if (!(recive.sd = SDLNet_TCP_Open(&ip)))
    {
        fprintf(stderr, "SDLNet_TCP_Open: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
    SDLNet_AddSocket(recive.set, recive.sd);
    SDL_DetachThread(SDL_CreateThread(reciveInfo, "Recive-thread", (void*)&recive));
    
    quit = 0;
    int ID=0;
    char temp[2];
    bool engang = true;

    while (!quit)
    {
        if (engang==true) {
            ClearScreen();
            printf("Waiting for your turn...\n");
            engang = false;
        }
        
        if((SDLNet_CheckSockets(recive.set, 100))>0) {
            printf("Oj nu finns det info!\n");
            if ((SDLNet_TCP_Recv(recive.sd , red, 1024+1) < 0)) {
                fprintf(stderr, "SDLNet_TCP_Recv: %s\n", SDLNet_GetError());
                exit(EXIT_FAILURE);
            }
            
            //printf("red = %s\n",red);
            
            if(strstr(red, "ready")){
                temp[0] = red[5];
                myClientNr = atoi(temp);
                printf("My client nr: %d\n",myClientNr);
                ready = 1;
                for (i=0; i<2; i++) {
                    temp[i] = '?';
                }
                
            }else if(red[0] == '#'){
                stringToArray(red);
                
            }
        
        }

        while (ready==1)
        {
            printf("Hit or Stand> ");
            scanf("%s", buffer);
            
            len = strlen(buffer) + 1;
            if (SDLNet_TCP_Send(recive.sd, (void *)buffer, len) < len)
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
                    if (SDLNet_TCP_Recv(recive.sd, buffer, 512) > 0)
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
    SDLNet_TCP_Close(recive.sd);
    SDLNet_Quit();
    return EXIT_SUCCESS;
}

int reciveInfo(void* info){
    Rinfo* recive = (Rinfo*) info;
    int i,j;
    while (1) {
        for(i = 0;i<MAXCLIENTS+1;i++)
        {
            for(j = 0;j<MAXCARDS;j++)
            {
                
                printf("Player [%d][%d] = %d\n",i,j,tableInfo[i][j]);
                
                
            }
        }
        printf("\n");
        SDL_Delay(4000);
    }
    
    
    return 0;
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
/*
 if (sendstring[j] == '\0') {
 break;
 }else if(sendstring[j] == '.'){
 j++;
 }else
 temp2[0] = sendstring[j];
 
 //strcpy(temp2, &sendstring[j]);
 
 
 tableInfo[i][j] = atoi(temp2);
 
 */
