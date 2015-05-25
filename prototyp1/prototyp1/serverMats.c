#include "multiOS.h"
#include "gamelogic.h"
#define MAXCLIENTS 5
#define MAXCARDS 15

typedef struct stringinfo{
    TCPsocket socket;
    int* quit, clientnumber, *clientsocket;
    int clientvalue; //klientes sammalagda kortvärde
    int ready;
    int recive;
}sinfo;

Kort kortlek[ANTALKORT];
int playerturn = MAXCLIENTS-1;
int player_card[MAXCLIENTS+1][MAXCARDS];

int function(sinfo* incsocket);
void gameInit(Kort kortlek[]);
void PlayerCardInfo(int option);

int main (int argc, char *argv[])
{
    TCPsocket Listensock = NULL;
    IPaddress ip;
    sinfo clientvalue[MAXCLIENTS] ={NULL, NULL, 0, NULL, 0, 0, 0}; //bör initsiras
    int quit = 0, ClientNumber=0;
    int freeslots[MAXCLIENTS]={0}, i, j;
/* ########################## VIKTIGA SAKER ATT KÖRA ######################################## */

    srand(time(NULL));
    gameInit(kortlek);

/* ########################## NÄTVERKS INIT, INKL ÖPPNA SOCKET ######################################## */
    if(SDLNet_Init() < 0)
    {
        fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

    if(SDLNet_ResolveHost(&ip, NULL, 2000) < 0)
    {
        fprintf(stderr, "SDLNet_ResolveHost: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

    if(((Listensock = SDLNet_TCP_Open(&ip)) == NULL))
    {
        fprintf(stderr, "SDLNet_TCP_Open: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

    for(i=0;i<MAXCLIENTS;i++){ //initierar allt
        clientvalue[i].clientsocket = 0;
        clientvalue[i].ready = 0;
        clientvalue[i].recive = 0;
    }
    while(ClientNumber < MAXCLIENTS+1)
    {

        for (i=0; i<MAXCLIENTS; i++)
        {
            if(freeslots[i] == 0)
            {
                ClientNumber = i;
            }
        }
        if(freeslots[ClientNumber] == 0)
        { /*Kolllar vilken plats som är ledig, 0 = ledig, 1 = upptagen*/
            if((clientvalue[ClientNumber].socket = SDLNet_TCP_Accept(Listensock)))
            {
                clientvalue[ClientNumber].quit = &quit;
                clientvalue[ClientNumber].clientnumber = ClientNumber;
                //clientvalue[ClientNumber].socket = &Clientsock[ClientNumber];
                clientvalue[ClientNumber].clientsocket = &freeslots[ClientNumber];
                SDL_DetachThread(SDL_CreateThread(function, "Client", &clientvalue[ClientNumber]));
            }
        }
        else if(clientvalue[ClientNumber].clientsocket == 1)
        {
            ClientNumber++;
            printf("ClienNumber increased to: %d from %d due to full\n", ClientNumber,ClientNumber-1);
        }
        else if(ClientNumber == MAXCLIENTS)
        {
            ClientNumber = 0;
            printf("clientNumber set to: %d\n",ClientNumber);
        }
        for(i=0;i<MAXCLIENTS;i++)
        {
            if(freeslots[i] == 0 && i == playerturn)
            {
                playerturn--;
            }
        }
        if(playerturn == -1)
        {
            playerturn = MAXCLIENTS-1;
        }
        /*for(i = 0;i<MAXCLIENTS;i++)
        {
            //printf("recive =%d, i = %d\n",clientvalue[i].recive,i);
            //system("pause");
            if(clientvalue[i].recive == 1)
            {
                printf("1.\n");

                for(j = 0;j<MAXCLIENTS;j++)
                {
                    if(clientvalue[j].clientsocket == 1)
                    {
                        printf("2.\n");
                        system("pause");
                        if(SDLNet_TCP_Send(clientvalue[j].socket, player_card ,sizeof(player_card)) < 0)
                        {
                            fprintf(stderr, "SDLNet_TCP_send: %s\n", SDLNet_GetError());
                            exit(EXIT_FAILURE);
                        }
                    }
                }
                //clientvalue[i].recive = 0;
            }
        }*/

    }

    printf("exit from while\n");

    SDLNet_TCP_Close(Listensock);
    SDLNet_Quit();
    return 0;
}

int function(sinfo* incsocket)
{
    sinfo* inc = (sinfo*) incsocket;
    char buffer2[512];
    int value=0,var=1, temp=0;
    bool lose = false;
    inc->clientvalue = 0;
    bool engang = true;
    *(inc->clientsocket) = 1;
    char readystring[8] = "1";

    //strcpy(readystring,"1");

    printf("%d: connected\n", inc->clientnumber);
    while(*(inc->quit) != 1)
    {
        engang = true;
        if(playerturn == inc->clientnumber)
        {
            inc->ready = 1;
            printf("inc->ready = %d\n", inc->ready);
        }
        while((inc->ready)==1)
        {
            if(playerturn == inc->clientnumber && engang == true) //om det är din tur
            {
                engang = false;

                if(SDLNet_TCP_Send(inc->socket, readystring, (strlen(readystring)+1) < 0) //skicka en 1:a till klienten som signal att det är dennes tur.
                {
                    fprintf(stderr, "SDLNet_TCP_Sendsdfghjkl: %s\n", SDLNet_GetError());//======================================================================
                    exit(EXIT_FAILURE);
                }
                system("pause");
            }
            if(SDLNet_TCP_Recv(inc->socket, buffer2, 512) > 0)
            {
                inc->recive = 1;
                printf("inc.reciv i tråden: %d\n",inc->recive);
                if(strstr(buffer2,"stand"))
                {
                    PlayerCardInfo(1);
                    playerturn--;
                    inc->ready = 0;
                    temp=0;
                    inc->clientvalue = 0;
                }
                if(strstr(buffer2, "quit"))
                {
                    *(inc->quit) = 1;
                    printf("Client %d sent server shutdown!\n", inc->clientnumber);
                }
                else if(strstr(buffer2, "exit")){
                    *(inc->clientsocket) = 0;
                    inc->ready = 0;
                    SDLNet_TCP_Close(inc->socket);
                    printf("Client %d disconnected!\n", inc->clientnumber);
                    return 0;
                }
                else if (strstr(buffer2, "!help")) {
                    printf("##################    HELP   ############################\n");
                    printf("exit to safely disconnect\nquit to terminate the server\n");
                    printf("################## HELP 1 (1) ###########################\n\n");
                }
                else if (strstr(buffer2, "card") || strstr(buffer2, "hit"))
                {
                    int ID = dra_ID(kortlek);
                    printf("ID: %d\n", ID);

                    player_card[inc->clientnumber][temp] = ID;
                    temp++;

                    char cvalue[512],cID[512];
                    itoa(value,cvalue,10);
                    itoa(ID,cID,10);
                    if(SDLNet_TCP_Send(inc->socket, cID, strlen(cID)+1) < 0)
                    {
                        fprintf(stderr, "SDLNet_TCP_Send: %s\n", SDLNet_GetError());
                    }
                    IdToCard(ID,kortlek); //visar på skärmen en spelares spelbord
                    inc->clientvalue = inc->clientvalue + IdToValue(ID,kortlek);
                    if(inc->clientvalue > 21)
                    {
                        lose = true;
                    }
                    printf("Client [%d] has a card value of %d\n", inc->clientnumber, inc->clientvalue);
                }
                else
                {
                    printf("Client [%d] say: %s\n", inc->clientnumber, buffer2);
                }

                if(lose)
                {
                    printf("Client [%d] lose, ",inc->clientnumber);
                    if(inc->clientvalue > 21)
                    {
                        printf("(Bust)\n");
                    }
                    inc->clientvalue = 0;
                    lose = false;
                    playerturn--;
                    inc->ready = 0;
                }
                else if(inc->clientvalue == 21)
                {
                    printf("Client [%d] got Blackjack\n");
                    inc->clientvalue = 0;
                    inc->ready = 0;
                    playerturn--;
                }
            }else SDL_Delay(200);
        }
    }
    return 0;
}

void gameInit(Kort kortlek[]){
    initiera_kortleken(kortlek);
    blanda_kortleken(kortlek);
    PlayerCardInfo(0);
}

void PlayerCardInfo(int option)
{
    //o = clearar alla spelare kort
    //1 = printar ut info om alla

    int i = 0,j = 0; //i = varje spelare, j = varje kortid i ordning
    for(i = 0;i<MAXCLIENTS;i++)
    {
        for(j = 0;j<MAXCARDS;j++)
        {
            if(option == 0)
            {
                player_card[i][j] = -1;
            }
            else if(option == 1)
            {
                printf("Player [%d][%d] = %d\n",i,j,player_card[i][j]);
            }

        }
    }
    printf("\n");
}