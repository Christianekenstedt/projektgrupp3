#include "multiOS.h"
#include "gamelogic.h"
#define MAXCLIENTS 5
#define MAXCLIENTSANDSERVER (MAXCLIENTS+1)
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
int player_card[MAXCLIENTSANDSERVER][MAXCARDS]; //+1 för servern

int function(sinfo* incsocket);
void gameInit(Kort kortlek[]);
void PlayerCardInfo(int option);
void arrayToStringSend(char sendstring[]);

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
        for(i = MAXCLIENTS-1;i>-1;i--)
        {
            if(clientvalue[i].recive == 1)
            {
                for(j = MAXCLIENTS-1;j>-1;j--)
                {
                    if(freeslots[j] == 1)
                    {
                        char sendstring[1024];
                        arrayToStringSend(sendstring);
                        printf("stranegn: %s\n",sendstring);

                        if(SDLNet_TCP_Send(clientvalue[i].socket, sendstring ,1024+1) < 0)
                        {
                            fprintf(stderr, "SDLNet_TCP_send: %s\n", SDLNet_GetError());
                            exit(EXIT_FAILURE);
                        }
                    }
                }
                clientvalue[i].recive = 0;
            }
        }
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
    char red[8] = {'r','e','a','d','y','4','\0'};//skickar "ready" till client när det är dennes tur
    char cnr[3];
    int temp2 = inc->clientnumber;
    itoa(temp2,cnr,10);//lägger clientnummret i en separat variabel
    *(inc->clientsocket) = 1;

    printf("Client %d connected\n", inc->clientnumber);

    while(*(inc->quit) != 1)
    {
        engang = true;
        if(playerturn == inc->clientnumber)
        {
            inc->ready = 1;
        }
        while((inc->ready)==1)
        {
            if(playerturn == inc->clientnumber && engang == true) //om det är din tur
            {
                engang = false;
                red[5] = cnr[0];//för att clienten skall veta vem denne är i nummer

                if(SDLNet_TCP_Send(inc->socket, &red, 512+1) < 0) //skicka en 1:a till klienten som signal att det är dennes tur.
                {
                    fprintf(stderr, "SDLNet_TCP_Send: %s\n", SDLNet_GetError());//======================================================================
                    exit(EXIT_FAILURE);
                }
            }
            if(SDLNet_TCP_Recv(inc->socket, buffer2, 512) > 0)
            {
                if(strstr(buffer2,"stand"))
                {
                    //PlayerCardInfo(1);
                    playerturn--;
                    inc->ready = 0;
                    temp=0;
                    inc->clientvalue = 0;
                    inc->recive = 1;
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
                    if(SDLNet_TCP_Send(inc->socket, cID, 512) < 0)
                    {
                        fprintf(stderr, "SDLNet_TCP_Send: %s\n", SDLNet_GetError());
                    }
                    IdToCard(ID,kortlek,0); //visar på skärmen en spelares spelbord
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

    /* Tillfälligt ger dealern två påhittade IDn (För att testa klienten)*/
    player_card[5][0] = 12;
    player_card[5][1] = 135;

}

void PlayerCardInfo(int option)
{
    //0 = clearar alla spelare kort
    //1 = printar ut info om alla
    int i = 0,j = 0; //i = varje spelare, j = varje kortid i ordning
    for(i = 0;i<MAXCLIENTSANDSERVER;i++)
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

void arrayToStringSend(char sendstring[])
{
    int temp = 0;
    char temp2[10];
    int i = 0;
    int j = 0;

    strcpy(sendstring,"#");
    for(i = 0;i<MAXCLIENTSANDSERVER;i++)
    {
        for(j = 0;j<MAXCARDS;j++)
        {
            temp = player_card[i][j];
            itoa(temp,temp2,10);
            strcat(sendstring,temp2);
            strcat(sendstring,".");
        }
    }
    SDL_Delay(100);
    strcat(sendstring,"\0");
}
