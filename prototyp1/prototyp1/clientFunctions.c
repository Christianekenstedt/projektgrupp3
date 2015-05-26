//
//  clientFunctions.c
//  prototyp1
//
//  Created by Christian Ekenstedt on 2015-05-06.
//  Copyright (c) 2015 Grupp 3. All rights reserved.
//

#include "clientFunctions.h"

//#include "multiOS.h"
//#include "gamelogic.h"


int sendToServer(char command[], TCPsocket socket){
    int success;
    if (SDLNet_TCP_Send(socket, command, strlen(command)+1) < 0)
    {
        fprintf(stderr, "SDLNet_TCP_Send: %s\n", SDLNet_GetError());
        success = -1;
    }else success = 1;
    return success;
}

int reciveFromServer(TCPsocket socket){
    char buffer[512];
    int value = 0, quit;
    while (!quit) {
        if (SDLNet_TCP_Recv(socket, buffer, strlen(buffer)+1) > 0)
        {
            printf("In Recv: buffer = %s\n", buffer);
            value = atoi(buffer);
            printf("In Recv: value = %d\n", value);
            quit = 1;
        }else {
            fprintf(stderr, "SDLNet_TCP_Send: %s\n", SDLNet_GetError());
            exit(EXIT_FAILURE);
        }
    }

    return value;
}

int IdToVisualCard(int id,Kort kortlek[])
{
    int i = 0, cardNr = 0, temp = 0;
    for(i = 0;i<ANTALKORT;i++)
    {
        if(id == kortlek[i].kortnummer)
        {
            printf("\n");
            printf("You have card:\n");
            printf("Suite: %s\n",kortlek[i].farg);
            printf("Value: %d\n\n",kortlek[i].kortvarde);
            if (strstr(kortlek[i].farg, "Hjarter")) {
                cardNr = 26;
                cardNr += kortlek[i].kortvarde;
            }else if (strstr(kortlek[i].farg, "Spader")) {
                cardNr = 13;
                cardNr += kortlek[i].kortvarde ;
            }else if (strstr(kortlek[i].farg, "Ruter")) {
                cardNr = 39;
                cardNr += kortlek[i].kortvarde ;
            }else if (strstr(kortlek[i].farg, "Klover")) {
                cardNr = 0;
                cardNr += kortlek[i].kortvarde ;
            }
        }
    }
    printf("Going to return CardNr = %d \n", cardNr-1);
    return cardNr-1;
}

/*void initiera_kortleken(Kort kortleken[])
{
    int farg = 0,i;
    int temp_kortvarde = 1;
    for(i = 0;i<ANTALKORT;i++)
    {
        kortleken[i].kortnummer = i;


        if(temp_kortvarde == 14)
        {
            temp_kortvarde = 1;
        }
        kortleken[i].kortvarde = temp_kortvarde;


        if(farg == 52)
        {
            farg = 0;
        }

        if(farg <= 12)
        {
            strcpy(kortleken[i].farg, "Klover");
            kortleken[i].farg[6] = '\0';
        }
        else if(farg >= 13 && farg <= 25)
        {
            strcpy(kortleken[i].farg,"Spader");
            kortleken[i].farg[6] = '\0';
        }
        else if(farg >= 26 && farg <= 38)
        {
            strcpy(kortleken[i].farg,"Hjarter");
            kortleken[i].farg[7] = '\0';
        }
        else if(farg >= 39 && farg <= 51)
        {
            strcpy(kortleken[i].farg,"Ruter");
            kortleken[i].farg[5] = '\0';
        }
        kortleken[i].upptagen = false;
        farg++;
        temp_kortvarde++;
    }
}*/

