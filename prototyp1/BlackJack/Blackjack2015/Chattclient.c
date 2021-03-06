//Written by Mats Levin
//Edited and commented on by Mats Levin

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "multiOS.h"
#define LENGTH 41                                   //Length of input row
#define LENGTH2 36                                  //Length of printed strnig row

typedef struct Reciveinfo                           //Declare the struct used for client data
{
  TCPsocket SD;                                     //socket that connects to the server
  int* quit;                                        //pointer to quit
  SDLNet_SocketSet sset;                            //A socketset that SD is put into so the checksocket function can be used
  SDL_mutex* mutex;                                 //A SDL_mutex used to lock specific resourses so that colitions are avoided
  char Rendertext[17][65];                          //The twodimentional array used as rows when text is printed in the window
  int Recive;                   //bör kunnas tas bort
}Rinfo;

int function(void* incinfo);                        //function that recives strings and splits them into rows of printable length
void Rendertext(SDL_Renderer* renderer, char* text, int x, int y, TTF_Font* gen);       //Render function

int main(int argc, char **argv)                     //main has a predefined type where these inputs are expected
{
  IPaddress ip;		                            // Server address
  int quit, len, namelenght, i;                 //declaration of variables
  char buffer[512] = {0};
  char writtentext[500] = {0};
  char name[12];
  Rinfo Recivestruct = {0};
  SDL_Window* Window;
  SDL_Renderer* renderer;
  SDL_Event event;
  int windoww, windowh;
  char text[65] = "Skriv";
  char Skrivtext[512] = {0};
  char temp;
  int x = 30;
  int y = 560;
  int antal = 0;
  int flytta = 0;



  Recivestruct.sset = SDLNet_AllocSocketSet(1);     //Allocates space for the sockets that are gonna be put into the socketset
  Recivestruct.mutex = SDL_CreateMutex();           //initializes the mutexes

  /*if (argc < 3)
    {
      fprintf(stderr, "Usage: %s host port\n", argv[0]);
      exit(EXIT_FAILURE);
    }*/

  if (SDLNet_Init() < 0)                            //initializes SDL_net so the functions can be used
    {
      fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
      exit(EXIT_FAILURE);
    }

  // Resolve the host we are connecting to
  if (SDLNet_ResolveHost(&ip, "130.229.162.122", 2000) < 0)           //containes the information needed for conection to the server
    {
      fprintf(stderr, "SDLNet_ResolveHost: %s\n", SDLNet_GetError());
      exit(EXIT_FAILURE);
    }

  // Open a connection with the IP provided (listen on the host's port)
  if (!(Recivestruct.SD = SDLNet_TCP_Open(&ip)))                    //connects the SD struct in the client to a struct on the server
    {
      fprintf(stderr, "SDLNet_TCP_Open: %s\n", SDLNet_GetError());
      exit(EXIT_FAILURE);
    }

  // Send messages
  namelenght = 0;
  printf("Write name:\n");
  fgets(name,12,stdin);                 //gets the name a player entered
  namelenght = strlen(name)+1;          //gets length of name and adds one so that a space can be added
  name[namelenght-2] = ':';             //ends the name with:
  name[namelenght-1] = ' ';             //adds space before the pronted text
  //printf("%d\n", namelenght);
  //printf("%s\n", name);

  quit = 0;
  SDLNet_AddSocket(Recivestruct.sset, Recivestruct.SD);

  Recivestruct.quit = &quit;
  SDL_DetachThread(SDL_CreateThread(function, "Recivethread", (void*)&Recivestruct));

  if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
      printf("initfailed\n");
    }
  if(TTF_Init() < 0)
    {
      printf("TTFfailed\n");
    }

  Window = SDL_CreateWindow("Royal chatt",
			    SDL_WINDOWPOS_CENTERED,
			    SDL_WINDOWPOS_CENTERED,
			    640,
			    640,
			    0);


  if(Window == NULL)
    {
      printf("window error");
    }

    renderer = SDL_CreateRenderer(Window, -1, 0);

    #ifdef _WIN32
    TTF_Font* gen = TTF_OpenFont("fonts\\newroman.regular.ttf", 20);
#else
    TTF_Font* gen = TTF_OpenFont("fonts/newroman.regular.ttf", 20);
    #endif

    //TTF_Font* gen = TTF_OpenFont("newroman.regular.ttf", 20);


#ifdef _WIN32
    SDL_Surface* Background = IMG_Load("bilder\\Chattbg.png");
#else
    SDL_Surface* Background = IMG_Load("bilder/Chattbg.png");
#endif
    SDL_Texture* Backgroundtexture = SDL_CreateTextureFromSurface(renderer, Background);
    SDL_FreeSurface(Background);

    /*SDL_Rect Sendchip = {510, 540, 105, 66};
      SDL_Rect Exitbutton = {10, 604, 90, 31};*/


    while (!quit)
      {
	SDL_RenderClear(renderer);
	SDL_GetWindowSize(Window, &windoww, &windowh);
	SDL_RenderCopy(renderer, Backgroundtexture, NULL, NULL);
	Rendertext(renderer, text, x, y, gen);
	printf("nummer1\n");
	while(1)
	  {
	    printf("nummer2\n");
	    if(1)
	      {
		if(SDL_TryLockMutex(Recivestruct.mutex) == 0)
		  {
		    printf("nummer3\n");
		    for(i = 0;i < 17;i++)
		      {
			printf("%s\n", Recivestruct.Rendertext[16-i]);
			Rendertext(renderer, Recivestruct.Rendertext[16-i], 30, (i*(20+10)), gen);
		      }
		    Recivestruct.Recive = 0;
		    SDL_UnlockMutex(Recivestruct.mutex);
		    break;
		  }
	      }
	    else
	      {
		break;
	      }
	  }
	printf("nummer5\n");
	SDL_RenderPresent(renderer);
	SDL_Delay(1000/60);
	printf("nummer6\n");
	if(SDL_PollEvent(&event))
	  {
	    if(event.type == SDL_QUIT)
	      {
		break;
	      }
	    if(event.type == SDL_TEXTINPUT)
	      {
		Skrivtext[antal] = *(event.text.text);

		if(antal < (LENGTH+1))
		  {
		    text[antal] = Skrivtext[antal];
		  }
		else
		  {
		    for(flytta = 0; flytta < LENGTH;flytta++)
		      {
			text[flytta] = text[flytta+1];
		      }
		    text[LENGTH] = Skrivtext[antal];
		  }
		antal++;
	      }
	    if(event.type == SDL_MOUSEBUTTONDOWN)
	      {
		if(event.button.button == SDL_BUTTON_LEFT)
		  {
		    if(event.button.x > 510 && event.button.x < 615 && event.button.y > 540 && event.button.y < 606)
		      {
			/*printf("Write something:\n>");
			  fgets(writtentext,500,stdin);*/
			len = strlen(Skrivtext) + 1;
			//writtentext[len-2]='\0';

			strncpy(buffer, name, namelenght);
			for(i = 0;i <= len;i++)
			  {
			    buffer[namelenght + i] = Skrivtext[i];
			  }

			len = strlen(buffer) + 1;

			if (SDLNet_TCP_Send(Recivestruct.SD, (void*)buffer, len) < len)
			  {
			    fprintf(stderr, "SDLNet_TCP_Send: %s\n", SDLNet_GetError());
			    exit(EXIT_FAILURE);
			  }

			if(strstr(buffer, "exit"))
			  {
			    quit = 1;
			  }
			if(strstr(buffer, "quit"))
			  {
			    quit = 1;
			  }
			memset(&Skrivtext[0], 0, 500);
			memset(&buffer[0], 0, 500);
			memset(&text[0], 0, 60);

			antal = 0;
		      }
		    if(event.button.x > 10 && event.button.x < 100 && event.button.y > 604 && event.button.y < 635)
		      {
			SDLNet_TCP_Send(Recivestruct.SD, "exit", 4);
			quit =  1;
		      }
		  }
	      }
	    if(event.type == SDL_KEYDOWN)
	      {
		if(event.key.keysym.sym == SDLK_BACKSPACE && antal > (LENGTH+1))
		  {

		    for(flytta = LENGTH; flytta > -1;flytta--)
		      {
			text[flytta] = text[flytta-1];
		      }

		    text[antal-1] = '\0';
		    Skrivtext[antal-1] = '\0';
		    text[0] = Skrivtext[antal-(LENGTH+2)];
		    antal--;
		  }
		else if(event.key.keysym.sym == SDLK_BACKSPACE && antal > 1)
		  {
		    antal--;
		    text[antal] = '\0';
		    Skrivtext[antal] = '\0';
		  }
		else if(event.key.keysym.sym == SDLK_BACKSPACE && antal == 1)
		  {
		    text[antal-1] = ' ';
		    Skrivtext[antal-1] = '\0';
		    antal--;
		  }
		if(event.key.keysym.sym == SDLK_RETURN)
		  {
		    /*printf("Write something:\n>");
		      fgets(writtentext,500,stdin);*/
		    len = strlen(Skrivtext) + 1;
		    //writtentext[len-2]='\0';

		    strncpy(buffer, name, namelenght);
		    for(i = 0;i <= len;i++)
		      {
			buffer[namelenght + i] = Skrivtext[i];
		      }

		    len = strlen(buffer) + 1;

		    if (SDLNet_TCP_Send(Recivestruct.SD, (void*)buffer, len) < len)
		      {
			fprintf(stderr, "SDLNet_TCP_Send: %s\n", SDLNet_GetError());
			exit(EXIT_FAILURE);
		      }

		    if(strstr(buffer, "exit"))
		      {
			quit = 1;
		      }
		    if(strstr(buffer, "quit"))
		      {
			quit = 1;
		      }
		    memset(&Skrivtext[0], 0, 500);
		    memset(&buffer[0], 0, 500);
		    memset(&text[0], 0, 60);

		    antal = 0;
		  }
	      }
	  }
      }
    TTF_CloseFont(gen);
    SDLNet_TCP_Close(Recivestruct.SD);
    SDLNet_Quit();

    return EXIT_SUCCESS;
}

int function(void* incinfo)
{
  Rinfo* inc = (Rinfo*)incinfo;
  char buffer2[512] = {0};
  int inclength;
  int Rowsplitts = 0;
  int i = 0;

  while(!*(inc->quit))
    {
      if((SDLNet_CheckSockets(inc->sset, 100)) > 0)
	{
	  if (SDLNet_TCP_Recv(inc->SD, buffer2, 512) > 0)
	    {
	      inc->Recive = 1;
	      printf("%s\n", buffer2);
	      while(1)
		{
		  if(SDL_TryLockMutex(inc->mutex) ==0)
		    {
		      inclength = strlen(buffer2);

		      if(inclength > LENGTH2)
			{
			  for(;inclength > 0;inclength-=LENGTH2)
			    {
			      for(i = 15;i > 0;i--)
				{
				  strcpy(inc->Rendertext[i], inc->Rendertext[i-1]);
				}
			      memset(inc->Rendertext[0], 0, 64);
			      memcpy(&inc->Rendertext[0], &buffer2[LENGTH2*Rowsplitts], LENGTH2);
			      Rowsplitts++;
			    }
			}
		      else
			{
			  for(i = 15;i > 0;i--)
			    {
			      strcpy(inc->Rendertext[i], inc->Rendertext[i-1]);
			    }
			  memset(inc->Rendertext[0], 0, 64);
			  memcpy(&inc->Rendertext[0], &buffer2[LENGTH2*Rowsplitts], strlen(buffer2+(LENGTH2*Rowsplitts)));
			}
		      Rowsplitts = 0;
		      SDL_UnlockMutex(inc->mutex);
		      break;
		    }
		}
	      for(i = 0;i < 512;i++)
		{
		  buffer2[i] = '\0';
		}
	    }
	}
      SDL_Delay(200);
    }
  SDLNet_TCP_Close(inc->SD);
  return 0;
}

void Rendertext(SDL_Renderer* renderer, char* text, int x, int y, TTF_Font* gen)
{
  if(strlen(text) < 1)
    {
      return;
    }
  SDL_Colour black = {255,255,255};
  SDL_Surface* letters = TTF_RenderText_Blended(gen, text, black);
  SDL_Rect pos = {x, y, letters->w, letters->h};
  SDL_Texture* letterstexture = SDL_CreateTextureFromSurface(renderer, letters);
  SDL_FreeSurface(letters);
  SDL_RenderCopy(renderer, letterstexture, NULL, &pos);
  SDL_DestroyTexture(letterstexture);
}
