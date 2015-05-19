//
//  clientFunctions.h
//  prototyp1
//
//  Created by Christian Ekenstedt on 2015-05-06.
//  Copyright (c) 2015 Grupp 3. All rights reserved.
//

#ifndef __prototyp1__clientFunctions__
#define __prototyp1__clientFunctions__

#include <stdio.h>
#include <stdbool.h>
#include "multiOS.h"
#include "gamelogic.h"


/* DEFINES */

#define PLAYBUTTON (x>PlayButton.x) && ( x < PlayButton.x + PlayButton.w ) && ( y > PlayButton.y) && (y < PlayButton.y + PlayButton.h)
#define EXITBUTTON (x>ExitRect.x) && ( x < ExitRect.x + ExitRect.w ) && ( y > ExitRect.y) && (y < ExitRect.y + ExitRect.h)
#define BETBUTTON (x>BetButton.x) && ( x < BetButton.x + BetButton.w ) && ( y > BetButton.y) && (y < BetButton.y + BetButton.h)
#define CLEARBUTTON (x>ClearButton.x) && ( x < ClearButton.x + ClearButton.w) && ( y > ClearButton.y) && (y < ClearButton.y + ClearButton.h)
#define HITBUTTON (x>HitButton.x) && ( x < HitButton.x + HitButton.w) && ( y > HitButton.y) && (y < HitButton.y + HitButton.h)
#define STANDBUTTON (x>StandButton.x) && ( x < StandButton.x + StandButton.w) && ( y > StandButton.y) && (y < StandButton.y + StandButton.h)
#define DOUBLEBUTTON (x>DoubleButton.x) && ( x < DoubleButton.x + DoubleButton.w) && ( y > DoubleButton.y) && (y < DoubleButton.y + DoubleButton.h)
#define SPLITBUTTON (x>SplitButton.x) && ( x < SplitButton.x + SplitButton.w) && ( y > SplitButton.y) && (y < SplitButton.y + SplitButton.h)
#define M1 (x>Chip1.x) && ( x < Chip1.x + Chip1.w ) && ( y > Chip1.y) && (y < Chip1.y + Chip1.h)
#define M5 (x>Chip5.x) && ( x < Chip5.x + Chip5.w ) && ( y > Chip5.y) && (y < Chip5.y + Chip5.h)
#define M25 (x>Chip25.x) && ( x < Chip25.x + Chip25.w ) && ( y > Chip25.y) && (y < Chip25.y + Chip25.h)
#define M50 (x>Chip50.x) && ( x < Chip50.x + Chip50.w ) && ( y > Chip50.y) && (y < Chip50.y + Chip50.h)
#define M100 (x>Chip100.x) && ( x < Chip100.x + Chip100.w ) && ( y > Chip100.y) && (y < Chip100.y + Chip100.h)
#define START 0
#define TABLE 1
/* VARIABLER */



//Screen dimension constants

/* FUNKTIONSDEKLARATIONER */

int sendToServer(char command[], TCPsocket socket);
int reciveFromServer(TCPsocket socket);
int IdToVisualCard(int id,Kort kortlek[]);
void initiera_kortleken(Kort kortleken[]);

#endif /* defined(__prototyp1__clientFunctions__) */
