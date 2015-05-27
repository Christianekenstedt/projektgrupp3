// Created by Mats Levin
// Edited and commented on by Mats Levin

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "multiOS.h"
#define Kill 0
#define maxnumber 5

typedef struct stringinfo                                       //Creates a struct that contains the information needed for each client
{ 
  TCPsocket socket;                                             //Creates a socket of the TCP type to contain a client connection
  int* quit, clientnumber, clientsocket, recive;                //Creates pointers and ints used in the programe
  char structbuffer[512];                                       //The recive string
}sinfo;

int function(sinfo* incsocket);                                 //Declares the function needed for the threads

int main(int argc, char**argv)
{
  TCPsocket Listensock;                                         //Creates the main listening socket that recives the conections
  IPaddress ip;                                                 //Used to containe the ip adress of the conections that are recived on the listening socket
  sinfo Socketstructarray[maxnumber] = {NULL, NULL, 0, 0, 0};   //Creates the main array of sockets used to store infomration about the conected clients
  int quit, ClientNumber, antal, len, antal2, i;                   //A bunch of ints used in the programe
  char ServerMedelande[15] = "Server is full";                  //A string containing the server is full message printed when server is full
  
  quit = 0;                                                     //Setting a bunch of the declared ints to cero
  antal = 0;
  antal2 = 0;
  len = 0;
  
  if(SDLNet_Init() < 0)                                         //SDL_Net init is needed to initialize the sdl functions, this function also checks if the initializeasion failed by checking what what is returned
    {
      fprintf(stderr, "SDLNet_Init: %\n", SDLNet_GetError());
      exit(EXIT_FAILURE);
    }
  
  if(SDLNet_ResolveHost(&ip, NULL, 2000) < 0)                   //Checks the conection recived on port 2000 and saves info like ip on it befere the socket is actually opedned, also sees if the function failes
    {
      fprintf(stderr, "SDLNet_ResolveHost: %s\n", SDLNet_GetError());
      exit(EXIT_FAILURE);
    }
 
  if(((Listensock = SDLNet_TCP_Open(&ip)) == NULL))             //recives the conetion on port 2000 and conects is to the socket, also looks at the returnvalue to se if the function failes
    {
      fprintf(stderr, "SDLNet_TCP_Open: %s\n", SDLNet_GetError());
      exit(EXIT_FAILURE);
    }
  
  ClientNumber = 0;                                             //Sets clientnumber to cero
  
  while(ClientNumber < maxnumber)                               //The main whileloop that checks most of the programs functions, is only turned of when somone writes quit
    {
      for(antal = 0; antal < maxnumber; antal++)                //forloop that checks at witch spot in the array to connect new clients, and if the max amount is allredy connected
	{
	  if(Socketstructarray[antal].clientsocket == 0)        //If no one is conected at the number antal in the array the int clientsocket is cero hence ClientNumber is set to antal so the next conetion is made there
	    {
	      ClientNumber = antal;
	      break;
	    }
	  if(antal == (maxnumber - 1))                          //Checks if maxnumber is conected, it checks maxnumber - 1 since an array starts at cero and not one
	    {
	      printf("server is full\n");                       //If the maxnumber of clients is connected it prints server is full message on the server
	      TCPsocket Tempsock;                               //Creates a temporary socket for a temporary connection
	      if(Tempsock = SDLNet_TCP_Accept(Listensock))      //If there is a new client trying to connect but the server is full the client it conected to the temp socket and printed the server full message after wich it's closed
		{
		  (SDLNet_TCP_Send(Tempsock, (void*)ServerMedelande, 15));
		  SDLNet_TCP_Close(Tempsock);
		}
	    }
	}
      if(antal < maxnumber)                                     //Checks if antal is less than five to know if there are spots where new clients can be connected
        {
	  if((Socketstructarray[ClientNumber].socket = SDLNet_TCP_Accept(Listensock)))       //Takes the connection on the listensocket and connects it to a socket in the struct array
	    {
	      Socketstructarray[ClientNumber].quit = &quit;                                  //Sends a pointer to quit into the struct so the program can be shut down by the clients
	      Socketstructarray[ClientNumber].clientnumber = ClientNumber;                   //Gives the array spot it's clientnumber so the threds knows wich client it's conected to
	      SDL_DetachThread(SDL_CreateThread(function, "ClientThread", &Socketstructarray[ClientNumber]));       //Creates a thread for each new connected client
	    }
        }
      for(antal = 0; antal < maxnumber; antal++)                //A forloop that prints out the connected sockets so that it's easy to se wich spots are taken on the server
	{
	  printf("sockets: %d\n", Socketstructarray[antal].clientsocket);
	  if(antal == (maxnumber - 1))
	    {
	      printf("Nytt:\n");
	    }
	}
      for(antal = 0; antal <= maxnumber; antal++)              //A forloop that sends the recived strings from the clients to the clients, makes it possible for everyone to se the text that's written
	{
	  if (Socketstructarray[antal].recive == 1)            //When one of the threads recive a string from it's client they set their recive int to 1 so that this loop knowes there is text to send
	    {
	      for(antal2 = 0; antal2 < maxnumber; antal2++)    //Loop that goes throug the struct array and sends the recived strings to all connected clients
		{
		  if(Socketstructarray[antal2].clientsocket == 1)
		    {
		      len = strlen(Socketstructarray[antal].structbuffer) + 1;         //checks length of the recived string so that the size can be sent to the reciveing clients
		      if (SDLNet_TCP_Send(Socketstructarray[antal2].socket, (void*)Socketstructarray[antal].structbuffer, len) < len)    //sends the string recived to all other clients, checks for errors
			{
			  fprintf(stderr, "SDLNet_TCP_Send: %s\n", SDLNet_GetError());
			  exit(EXIT_FAILURE);
			}
		    }
		}
	      printf("%s\n", Socketstructarray[antal].structbuffer);
	      for(i = 0; i < 512; i++)
		{
		  Socketstructarray[antal].structbuffer[i] = '\0';                    //empty array when I have sent the content
		}
	      Socketstructarray[antal].recive = 0;             //When the recived string has been sent the recive int is set to cero in preperation for the next time the thread recives something
	    }
	}
      SDL_Delay(100);                                          //A small delay so that it's possible to read printouts on the server 
      
      if (quit == 1)                                           //If one of the clients sends the quit comand the program will be shut down
	{
	  break;
	}
    }
  while(quit != 1)                                             //Simply a loop that sees if the main whileloop has been broken incorrectly, if so the program is cought in this loop so that exmaination on what went wrong can begin
    {
      SDL_Delay(100);
      printf("quit is= %d\n", quit);
    }
  
  SDLNet_TCP_Close(Listensock);                                //Closes the listeningsocket 
  SDLNet_Quit();                                               //Quits the SDL functions
  
  return Kill;                                                 //Returns one 
}
  

int function(sinfo* incsocket)                                 //Functions that recives a pointer of the sinfo struct type
{
  sinfo* inc = (sinfo*)incsocket;                              //Recives the pointer and puts it into inc
  int exit2 = 0;                                               //Declares int exit2 to cero
  inc->clientsocket = 1;                                       //Sets clientsocket number to 1 so that main knows that this spor in the array is taken
  int len;                                                     //declares an int
  
  while(exit2 != 1)                                            //main while loop that recives strings in the string
    {  
      if(SDLNet_TCP_Recv(inc->socket, inc->structbuffer, 512+1) > 0)      //Recives the client string into the buffer
	{
	  if(strstr(inc->structbuffer, "quit"))                           //Checks if the client has sent quit, if quit is sent the program shuts down
	    {
	      *(inc->quit) = 1;                                           //Sets quit to one to shut down the server
	    }
	  if(strstr(inc->structbuffer, "exit"))                           //Checks if the buffer contains exit if so the string shuts down
	    {
	      exit2 = 1;                                                  //Sets exit2 to one tu shut down the string
	    }
	  inc->recive = 1;                                                //Sets recive to one so that main knows that data has been recived
	  
	  printf("Client%d say: %s\n", inc->clientnumber, inc->structbuffer);
	  
	}
      else SDL_Delay(200);                                                //A small delay so that main has time to send data
    }
  inc->clientsocket = 0;                                                  //Sets clientsocket to one so that main know this spot in the array is free for a connection
  SDLNet_TCP_Close(inc->socket);                                          //Closes socket
}
