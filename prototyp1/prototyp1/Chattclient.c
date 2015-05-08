#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "multiOS.h"

typedef struct Reciveinfo
{
  TCPsocket SD;
  int* quit;
  SDLNet_SocketSet sset;
}Rinfo;

int function(void* incinfo);

int main(int argc, char **argv)
{
<<<<<<< HEAD
  IPaddress ip;		// Server address
=======
<<<<<<< HEAD
  IPaddress ip;		// Server address
=======
>>>>>>> origin/master
>>>>>>> origin/master
  TCPsocket sd;		// Socket descriptor
  int quit, len;
  char buffer[512];
  Rinfo Recivestruct;

  Recivestruct.sset = SDLNet_AllocSocketSet(1);

  // Simple parameter checking
  if (argc < 3)
    {
      fprintf(stderr, "Usage: %s host port\n", argv[0]);
      exit(EXIT_FAILURE);
    }

  if (SDLNet_Init() < 0)
    {
      fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
      exit(EXIT_FAILURE);
    }

  // Resolve the host we are connecting to
  if (SDLNet_ResolveHost(&ip, argv[1], atoi(argv[2])) < 0)
    {
      fprintf(stderr, "SDLNet_ResolveHost: %s\n", SDLNet_GetError());
      exit(EXIT_FAILURE);
    }

  // Open a connection with the IP provided (listen on the host's port)
  if (!(Recivestruct.SD = SDLNet_TCP_Open(&ip)))
    {
      fprintf(stderr, "SDLNet_TCP_Open: %s\n", SDLNet_GetError());
      exit(EXIT_FAILURE);
    }

  // Send messages
  quit = 0;
  SDLNet_AddSocket(Recivestruct.sset, Recivestruct.SD);

  Recivestruct.quit = &quit;
  SDL_DetachThread(SDL_CreateThread(function, "Recivethread", (void*)&Recivestruct));

  while (!quit)
    {
      printf("Write something:\n>");
      fgets(buffer,512,stdin);            //Read into string

      len = strlen(buffer) + 1;
      buffer[len-2]='\0';                 //Remove enter
      if (SDLNet_TCP_Send(Recivestruct.SD, (void*)buffer, len) < len)
        {
	  fprintf(stderr, "SDLNet_TCP_Send: %s\n", SDLNet_GetError());
	  exit(EXIT_FAILURE);
	}

      //printf("SEND: %s, LEN: %d SOCKETFAGGOT: %d\n", SDLNet_TCP_Send(Recivestruct.SD, buffer, len), len);

      if(strcmp(buffer, "exit") == 0)
	{
	  quit = 1;
	}
      if(strcmp(buffer, "quit") == 0)
	{
	  quit = 1;
        }
      /*if((SDLNet_CheckSockets(Socket, 100)) > 0)
	{
	  if (SDLNet_TCP_Recv(Recivestruct.SD, buffer, 512) > 0)
	    {
	      printf("server: %s\n", buffer);
	    }
	    }*/

      /* quit2 = 0; // FÖR RESPONS ENDAST!
	 while (!quit2)
	 {
	 if (SDLNet_TCP_Recv(sd, buffer, 512) > 0)
	 {
	 printf("server answer: %s\n", buffer);
	 quit2 = 1;
	 }
	 }*/
    }

  SDLNet_TCP_Close(Recivestruct.SD);
  SDLNet_Quit();

  return EXIT_SUCCESS;
}

int function(void* incinfo)
{
  Rinfo* inc = (Rinfo*)incinfo;
  char buffer2[512];

  while(!*(inc->quit))
    {
      if((SDLNet_CheckSockets(inc->sset, 100)) > 0)
	{
	  if (SDLNet_TCP_Recv(inc->SD, buffer2, 512) > 0)
	    {
	      printf("server chatt: %s\n", buffer2);
	    }
	}
    }
  SDLNet_TCP_Close(inc->SD);
}
