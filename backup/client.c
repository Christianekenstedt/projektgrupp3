#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "multiOS.h"


int main(int argc, char **argv)
{
    IPaddress ip;		/* Server address */
    TCPsocket sd;		/* Socket descriptor */
<<<<<<< HEAD
    int quit, quit2, len,intbuffer=0;
    char buffer[512],UserInputIP[12];
=======
    int quit, quit2, len;
    char buffer[512], UserInputIP[20]="169.254.211.44";
>>>>>>> origin/master

    /* Simple parameter checking */
    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s host port\n", argv[0]);
        exit(EXIT_FAILURE);
<<<<<<< HEAD
    }*/
    printf("Please enter IP: ");
    scanf("%s",&UserInputIP);
=======
    }

>>>>>>> origin/master
    if (SDLNet_Init() < 0)
    {
        fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

    /* Resolve the host we are connecting to */
<<<<<<< HEAD
    if (SDLNet_ResolveHost(&ip, /*argv[1]*/UserInputIP, /*atoi(argv[2])*/2000) < 0)
=======
    if (SDLNet_ResolveHost(&ip, "169.254.211.44", 2000) < 0)
>>>>>>> origin/master
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

    /* Send messages */
    quit = 0;
    while (!quit)
    {
        printf("Write something:\n>");
        scanf("%s", buffer);

        len = strlen(buffer) + 1;
        if (SDLNet_TCP_Send(sd, (void *)buffer, len) < len)
        {
            fprintf(stderr, "SDLNet_TCP_Send: %s\n", SDLNet_GetError());
            exit(EXIT_FAILURE);
        }

        if(strcmp(buffer, "exit") == 0)
            quit = 1;
        if(strcmp(buffer, "quit") == 0)
            quit = 1;

        quit2 = 0; // FÖR RESPONS ENDAST!
        if (strstr(buffer,"card")) {
            while (!quit2)
            {
                printf("inne i quit2\n");
                if (SDLNet_TCP_Recv(sd, buffer, 512) > 0)
                {
                    printf("Server answer: %d\n", atoi(buffer));
                    quit2 = 1;
                }else{
                    fprintf(stderr, "SDLNet_TCP_Recv: %s\n", SDLNet_GetError());
                    exit(EXIT_FAILURE);
                }
            }
        }
    }

    SDLNet_TCP_Close(sd);
    SDLNet_Quit();

    return EXIT_SUCCESS;
}
