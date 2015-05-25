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
int freeslots[MAXCLIENTS]={0};
bool connected[MAXCLIENTS];

int function(sinfo* incsocket);
void gameInit(Kort kortlek[]);
void PlayerCardInfo(int option);
void arrayToStringSend(char sendstring[]);
int updateFunction();

int main (int argc, char *argv[])
{
    TCPsocket Listensock = NULL;
    IPaddress ip;
    sinfo clientvalue[MAXCLIENTS] ={NULL, NULL, 0, NULL, 0, 0, 0}; //bör initsiras
    int quit = 0, ClientNumber=0;
    int i, j;
    bool engang = true;
    int dealervalue = 0;
    int temp = 0, plats=0;
    char sendstring[1024];
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

    /* ============================== tråd för uppdatering =========================== */

    SDL_DetachThread(SDL_CreateThread(updateFunction, "updateThred", NULL ));

    /* =============================================================================== */

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
        for(i=0;i<MAXCLIENTS;i++)
        {
            if(freeslots[i] == 0 && i == playerturn)
            {
                playerturn--;
            }
        }
        if(playerturn == -1)
        {
            playerturn = MAXCLIENTS;
        }
        for(i = MAXCLIENTS-1;i>-1;i--) //kollar spelare om de är med i spel och där efter vad de ska göra.
        {
            /*if(engang == true)
            {
                printf("Engang = true, i = %d\n", i);
            }
            else if(engang == false)
            {
                printf("Engang = false, i = %d\n", i);
            }
            system("pause");*/

            if(playerturn == MAXCLIENTS && (freeslots[4] == 1 || freeslots[3] == 1 || freeslots[2] == 1 || freeslots[1] == 1 || freeslots[0] == 1) && engang == true)
            {
                engang = false;
                temp = dra_ID(kortlek);
                player_card[5][plats++] = temp;
                dealervalue = IdToValue(temp,kortlek);

                temp = dra_ID(kortlek);
                player_card[5][plats++] = temp;
                dealervalue = dealervalue + IdToValue(temp,kortlek);

                printf("\nDra id. %d\n",player_card[5][0]);
                printf("Dra id. %d\n",player_card[5][1]);
                printf("Dealer has: %d\n",dealervalue);

                playerturn--;
            }
            else if(engang == false && playerturn == 5)//när alla spelare är klara blir det dealers tur igen
            {
                //====================== Servens logik ================================
                while(dealervalue < 17)
                {
                    temp = dra_ID(kortlek);
                    player_card[5][plats++] = temp;
                    dealervalue = dealervalue + IdToValue(temp,kortlek);
                    printf("Dealer has: %d============================================\n",dealervalue);
                    //system("pause");
                }
                //============================= regler ============================
                if(dealervalue > 21){
                    printf("Dealer lose!\n");
                    //PlayerCardInfo(0);
                }
                else if (dealervalue == 21)
                {
                    printf("Dealer got blackjack!");
                }
                //system("pause");
                engang = true;
                plats = 0;
                PlayerCardInfo(0);
                //playerturn--;
            }
        }
    }
    printf("exit from while\n");
    SDLNet_TCP_Close(Listensock);
    SDLNet_Quit();
    return 0;
}

int updateFunction()
{
    IPaddress ip;
    TCPsocket updatesocket[MAXCLIENTS], listen;
    int i = 0;
    int j = 0;
    char sendstring[1024];


    if(SDLNet_ResolveHost(&ip, NULL, 2001) < 0)
    {
        fprintf(stderr, "SDLNet_ResolveHost: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

    if(((listen = SDLNet_TCP_Open(&ip)) == NULL))
    {
        fprintf(stderr, "SDLNet_TCP_Open: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
    for(i = 0; i < MAXCLIENTS; i++)
    {
        connected[i] = false;
    }
    int t = 0;
    while(true)
    {
        //SDL_Delay(3000);
        printf("t = %d\n",t);
        t++;


        for(i = 0; i < MAXCLIENTS; i++)
        {
            if(freeslots[i] == 1 && !connected[i])
            {

                if((updatesocket[i] = SDLNet_TCP_Accept(listen))<0)
                {
                    fprintf(stderr, "SDLNet_TCP_Accept: %s\n", SDLNet_GetError());
                    exit(EXIT_FAILURE);
                }
                connected[i] = true;
                printf("Connected %d = true\n", i);


            }
        }

        for(i = MAXCLIENTS-1; i > -1; i--)
        {
            if(connected[i])
            {
                arrayToStringSend(sendstring);
                //printf("i = %d\n", i);
                //system("pause");
                //printf("stranegn: %s\n",sendstring);

                if(SDLNet_TCP_Send(updatesocket[i], sendstring ,1024+1) < 0)
                {
                    fprintf(stderr, "SDLNet_TCP_send: %s\n", SDLNet_GetError());
                    exit(EXIT_FAILURE);
                }
            }

        }
    }


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
    inc->recive = 1; //ger main tråden tillåtelse att skicka ut all info om bordet till den nya anslutna klienten
    int ID = -1; //kort ID
    char cvalue[512],cID[512];

    SDL_Delay(1000);

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

                if(SDLNet_TCP_Send(inc->socket, &red, 512+1) < 0) //skicka en ready till klienten som signal att det är dennes tur.
                {
                    fprintf(stderr, "SDLNet_TCP_Send: %s\n", SDLNet_GetError());//======================================================================
                    exit(EXIT_FAILURE);
                }

                 //skickar första kortet till den ny anslutna klienten
                 //SDL_Delay(1000);

                ID = dra_ID(kortlek);
                printf("ID: %d\n", ID);

                player_card[inc->clientnumber][temp] = ID;
                temp++;


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


                /* skickar andra kortet till den ny anslutna klienten */
                SDL_Delay(1000);

                ID = dra_ID(kortlek);
                printf("ID: %d\n", ID);

                player_card[inc->clientnumber][temp] = ID;
                temp++;

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

                //====================================================
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
                    //connected[inc->clientnumber] = false;
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
                    ID = dra_ID(kortlek);
                    printf("ID: %d\n", ID);

                    player_card[inc->clientnumber][temp] = ID;
                    temp++;

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
                    //connected[inc->clientnumber] = false;
                }
                else if(inc->clientvalue == 21)
                {
                    printf("Client [%d] got Blackjack\n");
                    inc->clientvalue = 0;
                    inc->ready = 0;
                    inc->recive = 1;
                    //connected[inc->clientnumber] = false;
                    playerturn--;
                }
            }else SDL_Delay(200);
        }
    }
    return 0;
}

void gameInit(Kort kortlek[]){
    initiera_kortleken(kortlek);
    //blanda_kortleken(kortlek);
    PlayerCardInfo(0);
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
