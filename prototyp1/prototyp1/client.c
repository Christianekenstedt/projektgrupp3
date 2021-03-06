#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXCLIENTS 5
#include "gamelogic.h"
#include "multiOS.h"

typedef struct Reciveinfo
{
    TCPsocket sd, tableSocket;
    SDLNet_SocketSet set;
    int* quit;
    IPaddress ip;
    
}Rinfo;

int tableInfo[MAXCLIENTS+1][15];
char saker[512];

int reciveInfo(void* info);
int whoWon(int playerValue, int dealerValue, int bet, bool blackjack);
void stringToArray(char sendstring[]);

int main(int argc, char **argv)
{
    /* Server address */
    
    int quit, quit2, len, myValue=0, ready=0, i,j;
    char buffer[512], red[512];
    Kort kortlek[ANTALKORT]; // Deklarerar kortlek
    bool lose = false;
    Rinfo recive;
    int myClientNr = 0;
    int dealerValue = 0;
    int bank = 500;
    int bet = 0;
    bool betround = true, blackjack = false, endround = false;
    
    recive.set = SDLNet_AllocSocketSet(2);
    
    recive.quit = &quit;
    
    for (i=0; i<MAXCLIENTS+1; i++) {
        for (j=0; j<15; j++) {
            tableInfo[i][j] = -1;
        }
    }
    
    initiera_kortleken(kortlek); // bygger upp kortleken så man kan använda och jämföra ID med ett kort.
    
    /* Simple parameter checking */
    /*if (argc < 3)
    {
        fprintf(stderr, "Usage: %s host port\n", argv[0]);
        exit(EXIT_FAILURE);
    }*/
    
    if (SDLNet_Init() < 0)
    {
        fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
    if (SDLNet_ResolveHost(&recive.ip, "169.254.211.44", 2000) < 0)
    {
        fprintf(stderr, "SDLNet_ResolveHost: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
    
    /* Open a connection with the IP provided (listen on the host's port) */
    if (!(recive.sd = SDLNet_TCP_Open(&recive.ip)))
    {
        fprintf(stderr, "SDLNet_TCP_Open: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
    SDLNet_TCP_AddSocket(recive.set, recive.sd);
    
    SDL_DetachThread(SDL_CreateThread(reciveInfo, "Recive-thread", (void*)&recive));
    
    quit = 0;
    int ID=0, temporar=0;
    char temp[2];
    bool engang = true, engang2 = true, engang3 = true;
    
    while (!quit)
    {
        if (engang==true) {
            //ClearScreen();
            printf("Waiting for your turn...\n");
            engang = false;
        }
        
        if((SDLNet_CheckSockets(recive.set, 100))>0) {
            //printf("Oj nu finns det info!\n");
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
        }
        
        while (ready==1)
        {
            //printf("Your balance is %d\nPlease place your bets: ",pott);
            //scanf("%d", &bet);
            if (betround) {
                
                printf("Your balance: $%d\n", bank);
                printf("Place your bets please: ");
                scanf("%d", &bet);
                
                bank -= bet;
                
                betround = false;
            }
            
            printf("Balance: %d\n", bank);
            dealerValue = 0;
            printf("---------- DEALER CARDS ---------------------\n");
            /*for(j = 0;j<MAXCARDS;j++)
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
                
            }*/
            temporar = 0;
            IdToCard(tableInfo[5][0], kortlek, 1);
            dealerValue += IdToValue(tableInfo[5][0],kortlek);
            printf("Dealer has %d\nAnd one card unknown\n", dealerValue);
            if (dealerValue > 21) {
                printf("Dealer lose!");
            }
            printf("\n---------- DEALER CARDS ---------------------\n");
            
            if(engang2 == true ){
                SDL_Delay(1000);
                /* HÄR SKA KLIENTEN FÅ ETT KORT */
                printf("INANN FOR\n");
                int exit1 = 0;
                while (!exit1 ) {
                    if (SDLNet_TCP_Recv(recive.sd, buffer, 512) > 0)
                    {
                        //printf("ID = %s", buffer);
                        
                        ID=atoi(buffer); // Stoppar in ID:t i variabel ID.
                        printf("ID = %d", ID);
                        if (ID == 0){
                            //exit1 = 0;
                            printf("NU FICK VI FEL!\n");
                        }
                        exit1 = 1;
                        
                    }else{
                        fprintf(stderr, "SDLNet_TCP_Recv: %s\n", SDLNet_GetError());
                        exit(EXIT_FAILURE);
                    }
                }
                IdToCard(ID, kortlek,0);
                myValue += IdToValue(ID,kortlek);
                exit1 =0;
                SDL_Delay(100);
                while (!exit1) {
                    if (SDLNet_TCP_Recv(recive.sd, buffer, 512) > 0)
                    {
                        ID=atoi(buffer); // Stoppar in ID:t i variabel ID.
                        printf("ID = %d", ID);
                        exit1 =1;
                    }else{
                        fprintf(stderr, "SDLNet_TCP_Recv: %s\n", SDLNet_GetError());
                        exit(EXIT_FAILURE);
                    }
                    
                }
                IdToCard(ID, kortlek,0);
                myValue += IdToValue(ID,kortlek);
                
                
                //}
                printf("You have a total of %d\n",myValue);
                
                /*------------------------------*/
                engang2 = false;
            }
            
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
                IdToCard(ID, kortlek,0);
                myValue += IdToValue(ID,kortlek);
                printf("You have a total of %d\n",myValue);
                if (myValue > 21) {
                    lose = true;
                }
            }else if(strstr(buffer,"stand")){
                printf("You stand at %d\n", myValue);

                endround = true;
                
                
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
                lose = false;

                endround = true;
            }else if (myValue==21){
                printf("You got blackjack!\n");
                blackjack = true;
                endround = true;
            }
            
            if (endround) {
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
                bank += whoWon(myValue, dealerValue, bet, blackjack);
                printf("Your balance is: $%d \n\n", bank);
                
                endround = false;
                myValue = 0;
                ready = 0;
                bet = 0;
                betround = true;
                engang = true;
                engang2 = true;

            }
        }
        
    }
    SDLNet_TCP_Close(recive.sd);
    SDLNet_Quit();
    return EXIT_SUCCESS;
}

int reciveInfo(void* info){
    Rinfo* recive = (Rinfo*) info;
    char red[1024+1];
    
    if (SDLNet_ResolveHost(&recive->ip, "169.254.211.44", 2001) < 0)
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
                //printf("NU FICK JAG TABLEINFO!!\n");
                stringToArray(red);
                //printf("%s\n", red);
            }
        }else {
            fprintf(stderr, "SDLNet_TCP_Recv: %s\n", SDLNet_GetError());
            exit(EXIT_FAILURE);
        }
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

int whoWon(int playerValue, int dealerValue, int bet, bool blackjack){
    int won=0;
    
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
    }else won = bet*2;
    
    if (playerValue>21 || (dealerValue > playerValue && dealerValue < 21 )) {
        won=0;
    }
    
    printf("Won = %d\n", won);
    blackjack=false;
    return won;
}