#include "multiOS.h"
#include "gamelogic.h"
#define MAXCLIENTS 5


typedef struct stringinfo{
    TCPsocket* socket;
    int* quit, clientnumber,* clientsocket;
    int clientvalue; //klientes sammalagda kortvärde
    int *ready;
    //int *playerturn;
    //Kort kortlek;
}sinfo;

//void fyllTradar(int value, sinfo clientvalue[]);

Kort kortlek[ANTALKORT];
int playerturn = MAXCLIENTS-1; // FÖrlåt jonas <3 (-1 = ledig)

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
    int freeslots[MAXCLIENTS]={0}, i, readyturn[MAXCLIENTS]={0};
    //int *playerturn = 0; //upp till MAXCLIENTS (0-4)
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
        //clientvalue[i].playerturn = 0;
        clientvalue[i].ready = 0;
    }
    //int r=0;
    //playerturn = 4;
    int vemstur = 4;
    while(ClientNumber < MAXCLIENTS+1)
    {


        for (i=0; i<MAXCLIENTS; i++) {
            if(freeslots[i] == 0){
                ClientNumber = i;
                //printf("I = %d",i);
            }
        }

        /*for(i=0;i<MAXCLIENTS;i++){
            //printf("%d.\n", clientvalue[i].playerturn);
       } //printf("\n");*/



        if(freeslots[ClientNumber] == 0){ /*Kolllar vilken plats som är ledig, 0 = ledig, 1 = upptagen*/
            if((Clientsock[ClientNumber] = SDLNet_TCP_Accept(Listensock)))
            {
                //clientvalue[ClientNumber].ready = false; //får spelaren spela.
                clientvalue[ClientNumber].quit = &quit;
                clientvalue[ClientNumber].clientnumber = ClientNumber;
                clientvalue[ClientNumber].socket = &Clientsock[ClientNumber];
                clientvalue[ClientNumber].clientsocket = &freeslots[ClientNumber];
                //clientvalue[ClientNumber].ready = &readyturn[ClientNumber];
                //clientvalue[ClientNumber].playerturn = ClientNumber; //temp

                SDL_DetachThread(SDL_CreateThread(function, "Client", (void*)&clientvalue[ClientNumber]));
                //ClientNumber++;
            }
            i = 0;/*
            for(int u = 0;u<MAXCLIENTS;u++){//kollar igenom alla klienter, är det bara en ansluten får den clientnumber
                if(freeslots[u] == 1)
                {
                    i++;
                }
            }
            if(i == 1)
            {
                printf("clientnumber: %d, playerturn %d\n",ClientNumber,playerturn);
                playerturn = ClientNumber;
                printf("clientnumber: %d, playerturn %d\n",ClientNumber,playerturn);
                SDL_Delay(1000);
                //system("pause");
            }*/


            //printf("\nReady = %d\n",clientvalue[ClientNumber].ready);
            //system("pause");


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
        for(i=0;i<MAXCLIENTS;i++){
            if(freeslots[i] == 0 && i == playerturn){
                playerturn--;
            }

        }
        /*
        if(playerturn == -1)
        {
            vemstur++;
        }
        for (i=0; i<MAXCLIENTS; i++) {
            if(freeslots[i] == 1){//if free, någon spelar på denna platss
                //printf("1.\n");
                if(clientvalue[i].ready == 0){//if ready == 0, dvs den inte nyss spelade. när klienten vet att det är dennes tur
                    //printf("if ready\n");
                    if(vemstur == clientvalue[i].clientnumber){ //oom det är dennes turn
                       //system("pause");

                        clientvalue[i].ready = 1;
                        playerturn = i;
                        printf("clientnumber: %d",ClientNumber);
                        //readyturn[i] = 1;
                        //clientvalue[ClientNumber].playerturn = ClientNumber;
                       //fyllTradar(i,clientvalue);


                    }
                }
            }
        if(vemstur > MAXCLIENTS){
            vemstur = 0;
            printf("vemstur: %d\n",vemstur);
            system("pause");
        }

        }*/
        /*
        for (i=0; i<MAXCLIENTS; i++) {
            if(freeslots[i] == 1){//if free, någon spelar på denna platss
                if(clientvalue[i].ready == 1){
                    if(playerturn == i && clientvalue[i].ready == 0){
                        playerturn--;
                    }
                }

            }

        }*/
        //r++;
        //fyllTradar(r,clientvalue);
        if(playerturn == -1)
        {
            playerturn = MAXCLIENTS-1;
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
    int value=0,var=1;
    bool lose = false;
    inc.clientvalue = 0;
    bool engang = true;

    *(inc.clientsocket) = 1;
    //*(inc.ready) = 0;


    printf("%d: connected\n", inc.clientnumber);
    while((*(inc.quit)) != 1)
    {
        engang = true;
        /*if(inc.clientnumber == 3)
        {
            printf("inc.ready: %d\n",inc.ready);
        }*/
        if(playerturn == inc.clientnumber)
        {
            inc.ready = 1;
            //printf("inc.ready: %d, clientnumber: %d\n",inc.ready,inc.clientnumber);
        }

        //system("pause");
        while(inc.ready==1)
        {
            //printf("playerturn = %d, inc.clientnumber %d\n",playerturn,inc.clientnumber);
            if(playerturn == inc.clientnumber && engang == true) //om det är din tur
            {
                engang = false;
                //printf("inc.playerturn == inc.clientnumber\n");
                if(SDLNet_TCP_Send(*(inc.socket), "1", 1) < 0) //skicka en 1:a till klienten som signal att det är dennes tur.
                {
                    fprintf(stderr, "SDLNet_TCP_Send: %s\n", SDLNet_GetError());
                    exit(EXIT_FAILURE);
                }
            }
            if(SDLNet_TCP_Recv((*(inc.socket)), buffer2, 512) > 0)
            {
                if(strstr(buffer2,"stand"))
                {
                    //printf("1. Trad: playersturn = %d\n",playerturn);
                    //system("pause");
                    //*inc.playerturn++;
                    playerturn--; //sätts till ledig
                    //printf("2. Trad: playersturn = %d\n",playerturn);
                    //if(inc.playerturn == 5)
                    //{
                        //*inc.playerturn = 0;
                        //fyllTradar(0,sinfo);
                    //}
                    inc.ready = 0;
                    //SDL_Delay(100);
                    //system("pause");
                    //break;
                }
                if(strstr(buffer2, "quit"))
                {
                    *(inc.quit) = 1;
                    printf("Client %d sent server shutdown!\n", inc.clientnumber);
                }
                else if(strstr(buffer2, "exit")){
                    *(inc.clientsocket) = 0;
                    inc.ready = 0;
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
                    playerturn--;
                    inc.ready = 0;
                }


            }else SDL_Delay(200);

        }
    }
    return 0;
}

/*void fyllTradar(int value, sinfo clientvalue[]){
    int i = 0;
    for(i=0;i<MAXCLIENTS;i++){
        clientvalue[i].playerturn = value;
    }
}*/
