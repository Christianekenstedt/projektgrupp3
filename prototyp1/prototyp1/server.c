#include "multiOS.h"
#include "gamelogic.h"
#define MAXCLIENTS 5


typedef struct stringinfo{
    TCPsocket* socket;
    int* quit, clientnumber,* clientsocket;
    int clientvalue; //klientes sammalagda kortvärde
    //bool ready;
    //Kort kortlek;
}sinfo;

Kort kortlek[ANTALKORT];

SDL_ThreadFunction* function(void* incsocket);

void gameInit(Kort kortlek[]){
    initiera_kortleken(kortlek);
    //checka_kort(0,kortlek);
    blanda_kortleken(kortlek);
    //checka_kort(0,kortlek);
}

int main (int argc, char *argv[])
{
    TCPsocket Listensock, Clientsock[MAXCLIENTS];
    IPaddress* ip;
    sinfo clientvalue[MAXCLIENTS];
    int quit = 0, ClientNumber=0;
    //Kort kortlek[ANTALKORT];
    int freeslots[MAXCLIENTS]={0}, i;
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

    for(i=0;i<MAXCLIENTS;i++){
        clientvalue[i].clientsocket=0;
    }


    while(ClientNumber < MAXCLIENTS+1)
    {
        for (i=0; i<MAXCLIENTS; i++) {
            if(freeslots[i] == 0){
                ClientNumber = i;
                //printf("I = %d",i);
            }
        }


        if(freeslots[ClientNumber] == 0){ /*Kolllar vilken plats som är ledig, 0 = ledig, 1 = upptagen*/
            if((Clientsock[ClientNumber] = SDLNet_TCP_Accept(Listensock)))
            {
                //clientvalue[ClientNumber].ready = false; //får spelaren spela.
                clientvalue[ClientNumber].quit = &quit;
                clientvalue[ClientNumber].clientnumber = ClientNumber;
                clientvalue[ClientNumber].socket = &Clientsock[ClientNumber];
                clientvalue[ClientNumber].clientsocket = &freeslots[ClientNumber];
                SDL_DetachThread(SDL_CreateThread(function, "Client", (void*)&clientvalue[ClientNumber]));
                //ClientNumber++;
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

    }
    printf("exit from while\n");

    while(!quit){
        SDL_Delay(100);
    }

    SDLNet_TCP_Close(Listensock);
    SDLNet_Quit();

    return 0;

}


SDL_ThreadFunction* function(void* incsocket)
{
    sinfo inc = *((sinfo*)incsocket);
    char buffer2[512];
    int value=0;
    bool lose = false;
    inc.clientvalue = 0;

    *(inc.clientsocket) = 1;

    printf("%d: connected\n", inc.clientnumber);

    while((*(inc.quit)) != 1)
    {

        if(SDLNet_TCP_Recv((*(inc.socket)), buffer2, 512) > 0)
        {
            if(strstr(buffer2, "quit"))
            {
                *(inc.quit) = 1;
                printf("Client %d sent server shutdown!\n", inc.clientnumber);
            }
            else if(strstr(buffer2, "exit")){
                *(inc.clientsocket) = 0;
                SDLNet_TCP_Close(*(inc.socket));


                printf("Client %d disconnected!\n", inc.clientnumber);
                return 0;
            }
            else if (strstr(buffer2, "!help")) {
                printf("##################    HELP   ############################\n");
                printf("exit to safely disconnect\nquit to terminate the server\n");
                printf("################## HELP 1 (1) ###########################\n\n");
            }

            else if (strstr(buffer2, "card") || strstr(buffer2, "hit")) {
                //Funktion
                //value=dra_kort(kortlek);
                int ID = dra_ID(kortlek);
                printf("ID: %d\n", ID);
                //printf("kortvarde: %d\nkortID: %d\n",value, ID);
                char cvalue[512],cID[512];
                itoa(value,cvalue,10);
                itoa(ID,cID,10);
                if(SDLNet_TCP_Send(*(inc.socket), cID, strlen(cID)+1) < 0)
                {
                    fprintf(stderr, "SDLNet_TCP_Send: %s\n", SDLNet_GetError());
                    //exit(EXIT_FAILURE);
                }

                IdToCard(ID,kortlek); //visar på skärmen en spelares spelbord

                inc.clientvalue = inc.clientvalue + IdToValue(ID,kortlek);
                if(inc.clientvalue > 21)
                {
                    lose = true;
                }
                printf("Client [%d] has a card value of %d\n", inc.clientnumber, inc.clientvalue);
            }
            else if (strstr(buffer2, "stand")){
                printf("Client [%d] stopped at %d", inc.clientnumber, inc.clientvalue);
                inc.clientvalue = 0;
            }
            else{
            printf("Client [%d] say: %s\n", inc.clientnumber, buffer2);
            }

            if(lose)
            {
                printf("Client [%d] lose, ",inc.clientnumber);
                if(inc.clientvalue > 21)
                {
                    printf("(Bust)\n");
                }
                inc.clientvalue = 0;
                lose = false;
            }


        }else SDL_Delay(200);
    }
    return 0;
}
