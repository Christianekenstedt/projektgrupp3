//
//  server.c
//  prototyp1
//
//  Created by Christian Ekenstedt on 2015-04-16.
//  Copyright (c) 2015 Grupp 3. All rights reserved.
//
#if 0
#!/bin/sh
gcc -Wall `sdl-config --cflags` tcps.c -o tcps `sdl-config --libs` -lSDL_net

exit
#endif
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "multiOS.h"

TCPsocket csd[6], sd;
IPaddress ip, *remoteIP;
SDL_Thread *thread;

struct thread{
    //TCPsocket csd;
    int client;
};typedef struct thread Thread;

static int threadFunction(void *ptr){
    struct thread *p = (struct thread *) ptr;
    int i;
    printf("INNE I THREADFUNC\n");
    
    while(1){
        for(i=0;i<p->client+1;i++){
           // printf("ClientTråd --> for-loop\n");
            /* This check the sd if there is a pending connection.
             * If there is one, accept that, and open a new socket for communicating */
            if ((csd[i] = SDLNet_TCP_Accept(sd)))
            {
                //SDL_Delay(1000);
                /* Get the remote address */
                if ((remoteIP = SDLNet_TCP_GetPeerAddress(csd[i])))
                /* Print the address, converting in the host format */
                    printf("Host connected: %x %d\n", SDLNet_Read32(&remoteIP->host), SDLNet_Read16(&remoteIP->port));
                else
                    fprintf(stderr, "SDLNet_TCP_GetPeerAddress: %s\n", SDLNet_GetError());
                p->client++;
            }
        }
    }
    return 0;
}

int main(int argc, char **argv)
{
    
    Thread parametrar;
    int i=0, quit=0;
    char buffer[512];
    parametrar.client = 0;
    
    
    
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
    
    thread = SDL_CreateThread(threadFunction, "SearchNewClients", &parametrar);
    
    /*if (NULL == thread) {
        printf("\nSDL_CreateThread failed: %s\n", SDL_GetError());
    } else {
        SDL_WaitThread(thread, &threadReturnValue);
        quit2= threadReturnValue;
    }*/

    /* Wait for a connection, send data and term */
    //quit = 0;
    while (1)
    {
        //printf("adlijwljdlij\n");
        for(i=0; i<parametrar.client;i++){
            //printf("Huvudtråd --> for-loop\n");
            if (SDLNet_TCP_Recv(csd[i], buffer, 512) > 0)
            {
                printf("Client say: %s\n",buffer);
                
                if(strcmp(buffer, "exit") == 0)	/* Terminate this connection */
                {
                    quit = 1;
                    printf("Terminate connection\n");
                    /* Close the client socket */
                    SDLNet_TCP_Close(csd[i]);
                    parametrar.client--;
                }
                
                if(strcmp(buffer, "quit") == 0)	/* Quit the program */
                {
                    quit = 1;
                    //quit = 1;
                    printf("Quit program\n");
                    /* Close the client socket */
                    SDLNet_TCP_Close(csd[i]);
                    parametrar.client--;
                    SDLNet_TCP_Close(sd);
                    SDLNet_Quit();
                }
                /*if (SDLNet_TCP_Send(csd[i], buffer, 512) < 0){
                    fprintf(stderr, "SDLNet_TCP_Send: %s\n", SDLNet_GetError());
                 }*/ parametrar.client--;
            }else if (SDLNet_TCP_Recv(csd[i], buffer, 512) < 0){
                fprintf(stderr, "SDLNet_TCP_Recv: %s\n", SDLNet_GetError());
                quit=1;
            }

        }
    }
    return EXIT_SUCCESS;
}
