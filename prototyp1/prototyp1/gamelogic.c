//
//  gamelogic.c
//  prototyp1
//
//  Created by Christian Ekenstedt on 2015-04-28.
//  Copyright (c) 2015 Grupp 3. All rights reserved.
//

#include "gamelogic.h"
#include "multiOS.h"
#include "clientFunctions.h"
#include "knappar.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

//#define ANTALKORTLEKAR 5
//#define ANTALKORT (ANTALKORTLEKAR*52)


int randomfunktio(int minimum, int maximum)
{
    int ran = 0;
    do
    {
        ran = (rand() %maximum) + minimum;
    }while((ran > maximum && ran != maximum) || (ran < minimum && ran != minimum)); /*sÂ l‰nge detta ‰r sant kˆr loopen. N‰r vilkoret st‰mmer ‰r det ett tal som ‰r oˆnskat, dÂ
                                                                                     //kˆrs loopen igen och slumpar dÂ fram ett nytt tal. Detta tills den ger ett tal som ‰r ˆnskat*/
    return ran;
}

void zeroz(int arr[])
{
    int i;
    for(i = 0;i<ANTALKORT;i++)
    {
        arr[i] = 0;
    }
}

void blanda_kortleken(Kort kortlek[])
{
    char tempfarg[20]={0};
    int tempnummer = 0, i;
    int temp_kortvarde = 0;
    bool tempupptagen = false;


    for(i = 0;i<100000;i++)
    {
        int temp1 = randomfunktio(0,ANTALKORT);
        int temp2 = randomfunktio(0,ANTALKORT);

        strcpy(tempfarg, kortlek[temp1].farg);
        tempnummer = kortlek[temp1].kortnummer;
        tempupptagen = kortlek[temp1].upptagen;
        temp_kortvarde = kortlek[temp1].kortvarde;

        strcpy(kortlek[temp1].farg, kortlek[temp2].farg);
        kortlek[temp1].kortnummer = kortlek[temp2].kortnummer;
        kortlek[temp1].upptagen = kortlek[temp2].upptagen;
        kortlek[temp1].kortvarde = kortlek[temp2].kortvarde;

        strcpy(kortlek[temp2].farg, tempfarg);
        kortlek[temp2].kortnummer = tempnummer;
        kortlek[temp2].upptagen = tempupptagen;
        kortlek[temp2].kortvarde = temp_kortvarde;
    }
}

void initiera_kortleken(Kort kortleken[])
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
}
void checka_kort(int kortleksplats, Kort kortlek[])
{

    printf("Farg: \t\t%s (%d)\n", kortlek[kortleksplats].farg, kortleksplats);
    printf("Kortnummer: \t%d \n", kortlek[kortleksplats].kortnummer);
    printf("Kortvarde: \t%d \n", kortlek[kortleksplats].kortvarde);

    if(kortlek[kortleksplats].upptagen == true)
    {
        printf("Upptagen: \tTrue\n");
    }
    else
    printf("Upptagen: \tFalse\n");
    printf("===================================\n");
}


int dra_kort(Kort kortlek[])
{

    int i, blackjackvarde = 0; /*alla kl√§dda kort √§r v√§rda 10 i blackjack*/

    for(i = 0;0<ANTALKORT;i++)
    {
        if(kortlek[i].upptagen == false) /*hittar ett ledigt kort*/
        {
            //checka_kort(i,kortlek);
            kortlek[i].upptagen = true; /*kortet √§r nu draget*/
            printf("0: %d\n", kortlek[i].kortvarde);
            blackjackvarde = kortlek[i].kortvarde;
            printf("1: %d\n", blackjackvarde);
            if(blackjackvarde > 10)
            {
                blackjackvarde = 10;
            }
            break;
        }
    }
    printf("2: %d\n", blackjackvarde);
    return blackjackvarde;
}

int dra_ID(Kort kortlek[])
{

    int i, ID = 0; /*alla kl√§dda kort √§r v√§rda 10 i blackjack*/

    for(i = 0;0<ANTALKORT;i++)
    {
        if(kortlek[i].upptagen == false) /*hittar ett ledigt kort*/
        {
            //checka_kort(i,kortlek);
            kortlek[i].upptagen = true; /*kortet √§r nu draget*/
            ID = kortlek[i].kortnummer;
            break;
        }
    }
    return ID;
}

int IdToValue(int id, Kort kortlek[])
{
    int value = 0;//kortvärdet som ska retuneras
    int i = 0;
    for(i = 0;i<ANTALKORT;i++)
    {
        if(id == kortlek[i].kortnummer)
        {
            break;
        }
    }
    value = kortlek[i].kortvarde;
    if(value > 10)
    {
        value = 10;
    }
    return value;
}

void IdToCard(int id,Kort kortlek[],int PoD)
{
    int i = 0;
    for(i = 0;i<ANTALKORT;i++)
    {
        if(id == kortlek[i].kortnummer)
        {
            printf("\n");
            if(PoD == 1){
                printf("DEALER have card:\n");

            }else printf("You have card:\n");
            
            printf("Suite: %s\n",kortlek[i].farg);
            printf("Value: %d\n",kortlek[i].kortvarde);
            break;
        }
    }
}


/*void ClearScreen(){
#ifdef _WIN32
    system("cls");
#else //
    system("clear");
#endif // rest
}*/

