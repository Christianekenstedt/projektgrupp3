#include "multiOS.h"
#include "gamelogic.h"


#define Killitmotherfucker 0

typedef struct stringinfo{
    TCPsocket* socket;
    int* quit, clientnumber;
}sinfo;

SDL_ThreadFunction* function(void* incsocket);

int main (int argc, char *argv[])
{

    TCPsocket Listensock, Clientsock[10];
    IPaddress* ip;
    sinfo motherfucker[10];
    int quit = 0, ClientNumber;
/* */
    srand(time(NULL));
/* ########################## NÄTVERKS INIT, INKL ÖPPNA SOCKET ########################################*/
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

    ClientNumber = 0;

    while(ClientNumber < 10)
    {
        if((Clientsock[ClientNumber] = SDLNet_TCP_Accept(Listensock)))
        {
            motherfucker[ClientNumber].quit = &quit;
            motherfucker[ClientNumber].clientnumber = ClientNumber;
            motherfucker[ClientNumber].socket = &Clientsock[ClientNumber];
            SDL_DetachThread(SDL_CreateThread(function, "TheThreadOfDoom", (void*)&motherfucker[ClientNumber]));
            ClientNumber++;
        }
    }
    while(!quit){
        SDL_Delay(100);
    }

    SDLNet_TCP_Close(Listensock);
    SDLNet_Quit();

    return Killitmotherfucker;

}


SDL_ThreadFunction* function(void* incsocket)
{
    sinfo inc = *((sinfo*)incsocket);
    char buffer2[512];

    while((*(inc.quit)) != 1)
    {
        if(SDLNet_TCP_Recv((*(inc.socket)), buffer2, 512) > 0)
        {
            if(strstr(buffer2, "quit"))
            {
                *(inc.quit) = 1;
            }
            printf("Client%d say: %s\n", inc.clientnumber, buffer2);
        }
        else SDL_Delay(200);
    }

    SDLNet_TCP_Close(*(inc.socket));
    return 0;
}
