//
//  gamelogic.c
//  prototyp1
//
//  Created by Christian Ekenstedt on 2015-04-28.
//  Copyright (c) 2015 Grupp 3. All rights reserved.
//

#include "gamelogic.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define ANTALKORTLEKAR 5
#define ANTALKORT (ANTALKORTLEKAR*52)


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
    char tempfarg[10];
    int tempnummer = 0;
    int temp_kortvarde = 0;
    bool tempupptagen = false;
    
    for(int i = 0;i<100000;i++)
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
    int farg = 0;
    int temp_kortvarde = 1;
    for(int i = 0;i<ANTALKORT;i++)
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
        
        if(farg <= 13)
        {
            
            strcpy(kortleken[i].farg, "Hjärter");
            
            kortleken[i].farg[7] = '\0';
        }
        else if(farg > 13 && farg <= 26)
        {
            strcpy(kortleken[i].farg,"Spader");
            kortleken[i].farg[6] = '\0';
        }
        else if(farg > 26 && farg <= 39)
        {
            strcpy(kortleken[i].farg,"Ruter");
            kortleken[i].farg[5] = '\0';
        }
        else if(farg > 39 && farg <= 51)
        {
            
            strcpy(kortleken[i].farg,"Klöver");
            
            kortleken[i].farg[6] = '\0';
        }
        kortleken[i].upptagen = false;
        farg++;
        temp_kortvarde++;
    }
}

int dra_kort(Kort kortlek[])
{
    
    int i, blackjackvarde = 0; /*alla kl√§dda kort √§r v√§rda 10 i blackjack*/
    
    for(i = 0;0<ANTALKORT;i++)
    {
        if(kortlek[i].upptagen == false) /*hittar ett ledigt kort*/
        {
            kortlek[i].upptagen = true; /*kortet √§r nu draget*/
            
            blackjackvarde = kortlek[i].kortvarde;
            if(blackjackvarde > 10)
            {
                blackjackvarde = 10;
            }
            break;
        }
    }
    return blackjackvarde;
}
