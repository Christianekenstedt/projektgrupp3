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

/* DEFINES */

#define PLAYBUTTON (x>PlayButton.x) && ( x < PlayButton.y + PlayButton.w ) && ( y > PlayButton.y) && (y < PlayButton.y + PlayButton.h)
#define EXITBUTTON (x>ExitRect.x) && ( x < ExitRect.y + ExitRect.w ) && ( y > ExitRect.y) && (y < ExitRect.y + ExitRect.h)
#define BETBUTTON (x>BetButton.x) && ( x < BetButton.y + BetButton.w ) && ( y > BetButton.y) && (y < BetButton.y + BetButton.h)
#define CLEARBUTTON (x>ClearButton.x) && ( x < ClearButton.y + ClearButton.w) && ( y > ClearButton.y) && (y < ClearButton.y + ClearButton.h)
#define HITBUTTON (x>HitButton.x) && ( x < HitButton.y + HitButton.w) && ( y > HitButton.y) && (y < HitButton.y + HitButton.h)
#define STANDBUTTON (x>StandButton.x) && ( x < StandButton.y + StandButton.w) && ( y > StandButton.y) && (y < StandButton.y + StandButton.h)
#define DOUBLEBUTTON (x>DoubleButton.x) && ( x < DoubleButton.y + DoubleButton.w) && ( y > DoubleButton.y) && (y < DoubleButton.y + DoubleButton.h)
#define SPLITBUTTON (x>SplitButton.x) && ( x < SplitButton.y + SplitButton.w) && ( y > SplitButton.y) && (y < SplitButton.y + SplitButton.h)
#endif /* defined(__prototyp1__clientFunctions__) */
