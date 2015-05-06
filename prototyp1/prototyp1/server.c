#include "multiOS.h"
#include "gamelogic.h"
#define MAXCLIENTS 6


typedef struct stringinfo{
    TCPsocket* socket;
    int* quit, clientnumber;
}sinfo;

<<<<<<< HEAD
static int threadFunction(void *ptr){

    struct thread *p = (struct thread *) ptr;
    //TCPsocket csd = p->csd;

    int i=0, quit2=0;
    char buffer[512];
    for(i=0; i<512; i++){
        buffer[i]='\0';
    }
    printf("INNE I THREADFUNC\n");
    while(!quit2){
        if (SDLNet_TCP_Recv(csd, buffer, 512) > 0)
        {
            printf("Client say: %s\n", buffer);

            if(strcmp(buffer, "exit") == 0)	/* Terminate this connection */
            {
                quit2 = 1;
                printf("Terminate connection\n");
            }

            if(strcmp(buffer, "quit") == 0)	/* Quit the program */
            {
                quit2 = 1;
                //quit = 1;
                printf("Quit program\n");
            }
            if (SDLNet_TCP_Send(csd, buffer, 512) < 0){
                fprintf(stderr, "SDLNet_TCP_Send: %s\n", SDLNet_GetError());
            }
        }else if (SDLNet_TCP_Recv(csd, buffer, 512) < 0){
            fprintf(stderr, "SDLNet_TCP_Recv: %s\n", SDLNet_GetError());
            quit2=1;
        }
    }//wile
    return quit2;
}//funktion
=======
SDL_ThreadFunction* function(void* incsocket);
>>>>>>> 7ce2e83f1d585956ce3bec9a01b25172ed5f26ee

int main (int argc, char *argv[])
{
<<<<<<< HEAD
<<<<<<< HEAD
    TCPsocket sd; //csd; /* Socket descriptor, Client socket descriptor */
    IPaddress ip, *remoteIP;
    SDL_Thread *thread;
    Thread parametrar;
    int quit, quit2, threadReturnValue;

    if (SDLNet_Init() < 0)
=======
    
    TCPsocket Listensock, Clientsock[10];
=======

    TCPsocket Listensock, Clientsock[MAXCLIENTS];
>>>>>>> 3fda9495a90b586452bf3b8d54a44b86ef229d42
    IPaddress* ip;
    sinfo clientvalue[10];

    int quit = 0, ClientNumber;
/* ########################## VIKTIGA SAKER ATT KÖRA ######################################## */
    srand(time(NULL));
/* ########################## NÄTVERKS INIT, INKL ÖPPNA SOCKET ######################################## */
    if(SDLNet_Init() < 0)
>>>>>>> 7ce2e83f1d585956ce3bec9a01b25172ed5f26ee
    {
        fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
<<<<<<< HEAD
<<<<<<< HEAD

    /* Resolving the host using NULL make network interface to listen */
    if (SDLNet_ResolveHost(&ip, NULL, 2000) < 0)
=======
    
=======

>>>>>>> 3fda9495a90b586452bf3b8d54a44b86ef229d42
    if(SDLNet_ResolveHost(&ip, NULL, 2000) < 0)
>>>>>>> 7ce2e83f1d585956ce3bec9a01b25172ed5f26ee
    {
        fprintf(stderr, "SDLNet_ResolveHost: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
<<<<<<< HEAD
<<<<<<< HEAD

    /* Open a connection with the IP provided (listen on the host's port) */
    if (!(sd = SDLNet_TCP_Open(&ip)))
=======
    
=======

>>>>>>> 3fda9495a90b586452bf3b8d54a44b86ef229d42
    if(((Listensock = SDLNet_TCP_Open(&ip)) == NULL))
>>>>>>> 7ce2e83f1d585956ce3bec9a01b25172ed5f26ee
    {
        fprintf(stderr, "SDLNet_TCP_Open: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
<<<<<<< HEAD
<<<<<<< HEAD

    /* Wait for a connection, send data and term */
    quit = 0;
    while (!quit)
=======
    
    ClientNumber = 0;
    
    while(ClientNumber < 10)
>>>>>>> 7ce2e83f1d585956ce3bec9a01b25172ed5f26ee
    {
        if((Clientsock[ClientNumber] = SDLNet_TCP_Accept(Listensock)))
        {
<<<<<<< HEAD

            /* Now we can communicate with the client using csd socket
             * sd will remain opened waiting other connections */

            /* Get the remote address */
            if ((remoteIP = SDLNet_TCP_GetPeerAddress(csd)))
            /* Print the address, converting in the host format */
                printf("Host connected: %x %d\n", SDLNet_Read32(&remoteIP->host), SDLNet_Read16(&remoteIP->port));
            else
                fprintf(stderr, "SDLNet_TCP_GetPeerAddress: %s\n", SDLNet_GetError());

            quit2 = 0;
            while (!quit2)
            {
                /* TRÅDFUNKTION BEHÖVS HÄR*/
                printf("INNE I WHILE :)\n");
                thread = SDL_CreateThread(threadFunction, "TestThread", &parametrar);
                if (NULL == thread) {
                    printf("\nSDL_CreateThread failed: %s\n", SDL_GetError());
                } else {
                    SDL_WaitThread(thread, &threadReturnValue);
                    quit2= threadReturnValue;
                    quit = quit2;
                }
            }

            /* Close the client socket */
            SDLNet_TCP_Close(csd);
        }
    }

    SDLNet_TCP_Close(sd);
    SDLNet_Quit();

    return EXIT_SUCCESS;
=======
            motherfucker[ClientNumber].quit = &quit;
            motherfucker[ClientNumber].clientnumber = ClientNumber;
            motherfucker[ClientNumber].socket = &Clientsock[ClientNumber];
            SDL_DetachThread(SDL_CreateThread(function, "TheThreadOfDoom", (void*)&motherfucker[ClientNumber]));
=======

    ClientNumber = 0;

    while(ClientNumber < MAXCLIENTS)
    {
        if((Clientsock[ClientNumber] = SDLNet_TCP_Accept(Listensock)))
        {
            clientvalue[ClientNumber].quit = &quit;
            clientvalue[ClientNumber].clientnumber = ClientNumber;
            clientvalue[ClientNumber].socket = &Clientsock[ClientNumber];
            SDL_DetachThread(SDL_CreateThread(function, "Client", (void*)&clientvalue[ClientNumber]));
>>>>>>> 3fda9495a90b586452bf3b8d54a44b86ef229d42
            ClientNumber++;
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

    while((*(inc.quit)) != 1)
    {
        if(SDLNet_TCP_Recv((*(inc.socket)), buffer2, 512) > 0)
        {
            printf("Client%d say: %s\n", inc.clientnumber, buffer2);
            if(strstr(buffer2, "quit"))
            {
                *(inc.quit) = 1;
                printf("Client %d disconnected!\n", inc.clientnumber);
            }

        }
        else SDL_Delay(200);
    }

    SDLNet_TCP_Close(*(inc.socket));
    return 0;
>>>>>>> 7ce2e83f1d585956ce3bec9a01b25172ed5f26ee
}
