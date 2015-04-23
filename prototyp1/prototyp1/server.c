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

//#include <SDL2/SDL.h>
//#include <SDL2/SDL_net.h>
//#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL_thread.h>
#include <SDL2/SDL_timer.h>
#include <SDL2_net/SDL_net.h>

struct thread{
    TCPsocket csd;
};typedef struct thread Thread;

static int threadFunction(void *ptr){
    
    struct thread *p = (struct thread *) ptr;
    TCPsocket csd = p->csd;
    
    int quit = 1, i=0;
    char buffer[512];
    for(i=0; i<512; i++){
        buffer[i]='\0';
    }
    
    if (SDLNet_TCP_Recv(csd, buffer, 512) > 0)
    {
        printf("Client say: %s\n", buffer);
        
        if(strcmp(buffer, "exit") == 0)	/* Terminate this connection */
        {
            quit = 1;
            printf("Terminate connection\n");
        }
        
        if(strcmp(buffer, "quit") == 0)	/* Quit the program */
        {
            quit = 1;
            //quit = 1;
            printf("Quit program\n");
        }
        if (SDLNet_TCP_Send(csd, buffer, 512) < 0){
            fprintf(stderr, "SDLNet_TCP_Send");
        }
    }
    return quit;
}

int main(int argc, char **argv)
{
    TCPsocket sd, csd; /* Socket descriptor, Client socket descriptor */
    IPaddress ip, *remoteIP;
    SDL_Thread *thread;
    Thread parametrar;
    int quit, quit2, threadReturnValue;
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
    
    /* Wait for a connection, send data and term */
    quit = 0;
    while (!quit)
    {
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
            
            quit2 = 0;
            while (!quit2)
            {
                /* TRÅDFUNKTION BEHÖVS HÄR*/
                thread = SDL_CreateThread(threadFunction, "TestThread", &parametrar);
                if (NULL == thread) {
                    printf("\nSDL_CreateThread failed: %s\n", SDL_GetError());
                } else {
                    SDL_WaitThread(thread, &threadReturnValue);
                    quit2= threadReturnValue;
                }
            }
            
            /* Close the client socket */
            SDLNet_TCP_Close(csd);
        }
    }
    
    SDLNet_TCP_Close(sd);
    SDLNet_Quit();
    
    return EXIT_SUCCESS;
}