#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "multiOS.h"
TCPsocket sd, csd[6];
IPaddress *remoteIP;
int handleNewClients(int client);
int handleData(int client);

int main(int argc, char **argv)
{
     /* Socket descriptor, Client socket descriptor */
    IPaddress ip, *remoteIP;
    int quit, quit2, numready=0;
    char buffer[512];
    SDLNet_SocketSet set;
    
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
    
    numready = SDLNet_TCP_AddSocket(set, csd[i]);
    
    /* Wait for a connection, send data and term */
    quit = 0;
    while (!quit)
    {

    }
    
    SDLNet_TCP_Close(sd);
    SDLNet_Quit();
    
    return EXIT_SUCCESS;
}

int handleNewClients(int client){
    
    /* This check the sd if there is a pending connection.
     * If there is one, accept that, and open a new socket for communicating */
    if ((csd = SDLNet_TCP_Accept(sd)))
    {
        /* Now we can communicate with the client using csd socket
         * sd will remain opened waiting other connections */
        
        /* Get the remote address */
        if ((remoteIP = SDLNet_TCP_GetPeerAddress(csd)))
        /* Print the address, converting in the host format */
            printf("Host connected: %x %d\n", SDLNet_Read32(&remoteIP->host), SDLNet_Read16(&remoteIP->port));
        else
            fprintf(stderr, "SDLNet_TCP_GetPeerAddress: %s\n", SDLNet_GetError());
    }
    
    
    return client;
}

void handleData(int client){
    int quit2 = 0;
    char buffer[512];
    
    while (!quit2)
    {
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
                printf("Quit program\n");
                /* Close the client socket */
                SDLNet_TCP_Close(csd);
            }
        }
        if (SDLNet_TCP_Send(csd, buffer, 512)<0) {
            fprintf(stderr, "SDLNet_TCP_send: %s", SDLNet_GetError());
            
        }
    }
}
