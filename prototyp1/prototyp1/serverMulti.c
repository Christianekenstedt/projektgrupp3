#if 0
#!/bin/sh
gcc -Wall `sdl-config --cflags` tcps.c -o tcps `sdl-config --libs` -lSDL_net

exit
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "multiOS.h"

int main(int argc, char **argv)
{
    TCPsocket sd, csd[6]; /* Socket descriptor, Client socket descriptor */
    IPaddress ip, *remoteIP[6];
    int quit, quit2, len, ClientAmount;
    char buffer[512];
    
    if (SDLNet_Init() < 0)
    {
        fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
    
    /* Resolving the host using NULL make network interface to listen */
    if (SDLNet_ResolveHost(&ip, NULL, 2000) < 0)
    {
        fprintf(stderr, "SDLNet_ResolveHost: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
    
    /* Open a connection with the IP provided (listen on the host's port) */
    if (!(sd = SDLNet_TCP_Open(&ip)))
    {
        fprintf(stderr, "SDLNet_TCP_Open: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
    ClientAmount = 0;
    
    /* Wait for a connection, send data and term */
    quit = 0;
    while (!quit)
    {
        /* This check the sd if there is a pending connection.
         * If there is one, accept that, and open a new socket for communicating */
        if ((csd[ClientAmount] = SDLNet_TCP_Accept(sd)))
        {
            /* Now we can communicate with the client using csd socket
             * sd will remain opened waiting other connections */
            ClientAmount = ClientAmount++;
            
            /* Get the remote address */
            if ((remoteIP[ClientAmount] = SDLNet_TCP_GetPeerAddress(csd[ClientAmount])))
            /* Print the address, converting in the host format */
                printf("Host connected: %x %d\n", SDLNet_Read32(&remoteIP[ClientAmount]->host), SDLNet_Read16(&remoteIP[ClientAmount]->port));
            else
                fprintf(stderr, "SDLNet_TCP_GetPeerAddress: %s\n", SDLNet_GetError());
            
            quit2 = 0;
            while (!quit2)
            {
                if (SDLNet_TCP_Recv(csd[ClientAmount], buffer, 512) > 0)
                {
                    printf("Client say: %s\n", buffer);
                    
                    len = strlen(buffer) + 1;
                    if (SDLNet_TCP_Send(csd[ClientAmount], (void *)buffer, len) < len)
                    {
                        fprintf(stderr, "SDLNet_TCP_Send: %s\n", SDLNet_GetError());
                        exit(EXIT_FAILURE);
                    }
                    
                    if(strcmp(buffer, "EndTurn") == 0)	/* Ends Clients turn */
                    {
                        quit2 = 1;
                        printf("Turn ends for player %d\n", ClientAmount);
                    }
                    if(strcmp(buffer, "quit") == 0)	/* Quits program */
                    {
                        quit2 = 1;
                        quit = 1;
                        printf("Quit program\n");
                    }
                }
            }
            if(ClientAmount == 5)
            {
                ClientAmount = 0;
            }
        }
        
        /* Close the client socket */
        SDLNet_TCP_Close(csd[ClientAmount]);
    }
    
    
    SDLNet_TCP_Close(sd);
    SDLNet_Quit();
    
    return EXIT_SUCCESS;
}
