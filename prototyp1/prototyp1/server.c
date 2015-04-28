#include "multiOS.h"
#include "gamelogic.h"
#define MAXCLIENTS 3


typedef struct stringinfo{
    TCPsocket* socket;
    int* quit, clientnumber;
    bool busy;      // anger om Client-socketen är upptagen eller ej.
}sinfo;

SDL_ThreadFunction* function(void* incsocket);

void gameInit(Kort kortlek[]){
    initiera_kortleken(kortlek);
    blanda_kortleken(kortlek);
}

int main (int argc, char *argv[])
{
    TCPsocket Listensock, Clientsock[MAXCLIENTS];
    IPaddress* ip;
    sinfo clientvalue[MAXCLIENTS];
    int quit = 0, ClientNumber=0;
    Kort kortlek[ANTALKORT];
/* ########################## VIKTIGA SAKER ATT KÖRA ######################################## */
    
    srand(time(NULL));
    //gameInit(kortlek);
    
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

    
    
    
    while(ClientNumber < MAXCLIENTS+1)
    {
        
        if(clientvalue[ClientNumber].busy == false){
            
            if((Clientsock[ClientNumber] = SDLNet_TCP_Accept(Listensock)))
            {
                clientvalue[ClientNumber].quit = &quit;
                clientvalue[ClientNumber].clientnumber = ClientNumber;
                clientvalue[ClientNumber].socket = &Clientsock[ClientNumber];
                clientvalue[ClientNumber].busy = true;
                SDL_DetachThread(SDL_CreateThread(function, "Client", (void*)&clientvalue[ClientNumber]));
                ClientNumber++;
            }
        }
        else if( clientvalue[ClientNumber].busy == true){
            ClientNumber++;
        }
        if(ClientNumber == MAXCLIENTS){
            ClientNumber = 0;
        }
        
    
    }
    
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
    struct stringinfo clientinfo[MAXCLIENTS];

    while((*(inc.quit)) != 1)
    {
        
        if(SDLNet_TCP_Recv((*(inc.socket)), buffer2, 512) > 0)
        {
            printf("Client [%d] say: %s\n", inc.clientnumber, buffer2);
            if(strstr(buffer2, "quit"))
            {
                *(inc.quit) = 1;
                printf("Client %d sent server shutdown!\n", inc.clientnumber);
            }
            if(strstr(buffer2, "exit")){
                clientinfo[inc.clientnumber].busy = false; printf("Sätter ");// Sätter busy-flagga till falsk
                
                if(clientinfo[inc.clientnumber].busy == true)
                {
                    printf("true\n");
                }
                else if(clientinfo[inc.clientnumber].busy  == false)
                {
                    printf("false\n");
                }
                
                
                SDLNet_TCP_Close(*(inc.socket));

                printf("Client %d disconnected!\n", inc.clientnumber);
            }
            if (strstr(buffer2, "!help")) {
                printf("##################    HELP   ############################\n");
                printf("exit to safely disconnect\nquit to terminate the server\n");
                printf("################## HELP 1 (1) ###########################\n\n");
            }

        }
        else SDL_Delay(200);
    }
    return 0;
}
