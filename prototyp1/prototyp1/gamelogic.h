//
//  gamelogic.h
//  prototyp1
//
//  Created by Christian Ekenstedt on 2015-04-28.
//  Copyright (c) 2015 Grupp 3. All rights reserved.
//

#ifndef __prototyp1__gamelogic__
#define __prototyp1__gamelogic__

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define ANTALKORTLEKAR 5
#define ANTALKORT (ANTALKORTLEKAR*52)
#define MAXCLIENTS 5
#define MAXCARDS 15

struct kort
{
    char farg[20];
    int kortnummer; /*varje korts "ID"*/
    int kortvarde;  /*kortetsv√§rde*/
    bool upptagen;
};typedef struct kort Kort;

int randomfunktio(int minimum, int maximum);
void zeroz(int arr[]);
void blanda_kortleken(Kort kortlek[]);
void initiera_kortleken(Kort kortleken[]);
int dra_kort(Kort kortlek[]);
int dra_ID(Kort kortlek[]);
void checka_kort(int kortleksplats, Kort kortlek[]);
int IdToValue(int id, Kort kortlek[]);
void IdToCard(int id,Kort kortlek[]);
void ClearScreen();
int IdToVisualCard(int id,Kort kortlek[]);



#endif /* defined(__prototyp1__gamelogic__) */
