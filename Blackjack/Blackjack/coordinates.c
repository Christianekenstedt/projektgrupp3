//
//  coordinates.c
//  Blackjack
//
//  Created by Christian Ekenstedt on 2015-05-27.
//  Copyright (c) 2015 Grupp 3. All rights reserved.
//

#include "coordinates.h"
#include "functions.h"

int positionInit(){
    int i;
    renderRect.x = 420;
    renderRect.y = 517;
    renderRect.h = 60;
    renderRect.w = 250;
    
    renderRect2.x = 230;
    renderRect2.y = 450;
    renderRect2.h = 50;
    renderRect2.w = 230;
    
    
    
    for (i=0; i<11; i++)
    {
        table1[i].y = 120;
        table1[i].x = 164+(i*15);
        table1[i].w = 72;
        table1[i].h = 96;
    }
    for (i=0; i<11; i++)
    {
        table2[i].y = 210;
        table2[i].x = 320+(i*15);
        table2[i].w = 72;
        table2[i].h = 96;
    }
    for (i=0; i<11; i++)
    {
        table3[i].y = 300;
        table3[i].x = 475+(i*15);
        table3[i].w = 72;
        table3[i].h = 96;
    }
    for (i=0; i<11; i++)
    {
        table4[i].y = 216;
        table4[i].x = 620+(i*15);
        table4[i].w = 72;
        table4[i].h = 96;
    }
    for (i=0; i<11; i++)
    {
        table5[i].y = 125;
        table5[i].x = 760+(i*15);
        table5[i].w = 72;
        table5[i].h = 96;
    }
    for (i=0; i<11; i++)
    {
        tableDealer[i].y = 15;
        tableDealer[i].x = 475+(i*15);
        tableDealer[i].w = 72;
        tableDealer[i].h = 96;
    }
    
    
    //Mark 1
    Chip1.y = 517;
    Chip1.x = 8;
    Chip1.w = 68;
    Chip1.h = 59;
    //Mark 5
    Chip5.y = 517;
    Chip5.x = 94;
    Chip5.w = 68;
    Chip5.h = 59;
    //Mark 25
    Chip25.y = 517;
    Chip25.x = 180;
    Chip25.w = 68;
    Chip25.h = 59;
    //Mark 50
    Chip50.y = 517;
    Chip50.x = 266;
    Chip50.w = 68;
    Chip50.h = 59;
    //Mark 100
    Chip100.y = 517;
    Chip100.x = 352;
    Chip100.w = 68;
    Chip100.h = 59;
    // Betcirkel 1   för plats pokerchip1
    for(i = 0; i < MAX; i++)
    {
        Betcirkel1_1[i].x = 178;
        Betcirkel1_1[i].y = 255-(i*5);
        Betcirkel1_1[i].w = 23;
        Betcirkel1_1[i].h = 23;
    }
    
    // Betcrikel 1 för plats pokerchip5
    for(i = 0; i < MAX; i++)
    {
        Betcirkel1_5[i].x = 201;
        Betcirkel1_5[i].y = 255-(i*5);
        Betcirkel1_5[i].w = 23;
        Betcirkel1_5[i].h = 23;
    }
    
    // Betcirkel 1 för plats pokerchip 25
    for(i = 0; i < MAX; i++)
    {
        Betcirkel1_25[i].x = 224;
        Betcirkel1_25[i].y = 255-(i*5);
        Betcirkel1_25[i].w = 23;
        Betcirkel1_25[i].h = 23;
    }
    
    // Betcirkel 1 för plats pokerchip 50
    for(i = 0; i < MAX; i++)
    {
        Betcirkel1_50[i].x = 184;
        Betcirkel1_50[i].y = 270-(i*5);
        Betcirkel1_50[i].w = 23;
        Betcirkel1_50[i].h = 23;
    }
    
    // Betcirkel 1 för plats pokerchip 100
    for(i = 0; i < MAX; i++)
    {
        Betcirkel1_100[i].x = 207;
        Betcirkel1_100[i].y = 270-(i*5);
        Betcirkel1_100[i].w = 23;
        Betcirkel1_100[i].h = 23;
    }
    
    // Betcirkel 2 för plats pokerchip 1
    for(i = 0; i < MAX; i++)
    {
        Betcirkel2_1[i].x = 325;
        Betcirkel2_1[i].y = 348-(i*5);
        Betcirkel2_1[i].w = 23;
        Betcirkel2_1[i].h = 23;
    }
    
    // Betcirekl 2 för plats pokerchip 5
    for(i = 0; i < MAX; i++)
    {
        Betcirkel2_5[i].x = 348;
        Betcirkel2_5[i].y = 348-(i*5);
        Betcirkel2_5[i].w = 23;
        Betcirkel2_5[i].h = 23;
    }
    
    // Betcirkel 2 för plats pokerchip 25
    for(i = 0; i < MAX; i++)
    {
        Betcirkel2_25[i].x = 371;
        Betcirkel2_25[i].y = 348-(i*5);
        Betcirkel2_25[i].w = 23;
        Betcirkel2_25[i].h = 23;
    }
    
    // Betcirkel 2 för plats pokerchip 50
    for(i = 0; i < MAX; i++)
    {
        Betcirkel2_50[i].x = 344;
        Betcirkel2_50[i].y = 360-(i*5);
        Betcirkel2_50[i].w = 23;
        Betcirkel2_50[i].h = 23;
    }
    
    // Betcirkel 2 för plats pokerchip 100
    for(i = 0; i < MAX; i++)
    {
        Betcirkel2_100[i].x = 367;
        Betcirkel2_100[i].y = 360-(i*5);
        Betcirkel2_100[i].w = 23;
        Betcirkel2_100[i].h = 23;
    }
    
    
    // Betcirkel 3 för plats pokercip 1
    for(i = 0; i < MAX; i++)
    {
        Betcirkel3_1[i].x = 482;
        Betcirkel3_1[i].y = 435-(i*5);
        Betcirkel3_1[i].w = 23;
        Betcirkel3_1[i].h = 23;
    }
    
    // Betcirkel 3 för plats pokerchip 5
    for(i = 0; i < MAX; i++)
    {
        Betcirkel3_5[i].x = 505;
        Betcirkel3_5[i].y = 435-(i*5);
        Betcirkel3_5[i].w = 23;
        Betcirkel3_5[i].h = 23;
    }
    
    // Betcirkel 3 för plats pokerchip 25
    for(i = 0; i < MAX; i++)
    {
        Betcirkel3_25[i].x = 528;
        Betcirkel3_25[i].y = 435-(i*5);
        Betcirkel3_25[i].w = 23;
        Betcirkel3_25[i].h = 23;
    }
    
    // Betcirkel 3 för plats pokerchip 50
    for(i = 0; i < MAX; i++)
    {
        Betcirkel3_50[i].x = 500;
        Betcirkel3_50[i].y = 450-(i*5);
        Betcirkel3_50[i].w = 23;
        Betcirkel3_50[i].h = 23;
    }
    
    // Betcirkel 3 för plats pokerchip 100
    for(i = 0; i < MAX; i++)
    {
        Betcirkel3_100[i].x = 523;
        Betcirkel3_100[i].y = 450-(i*5);
        Betcirkel3_100[i].w = 23;
        Betcirkel3_100[i].h = 23;
    }
    
    // Betcirkel 4 för plats pokerchip 1
    for(i = 0; i < MAX; i++)
    {
        Betcirkel4_1[i].x = 650;
        Betcirkel4_1[i].y = 350-(i*5);
        Betcirkel4_1[i].w = 23;
        Betcirkel4_1[i].h = 23;
    }
    
    // Betcirkel 4 för plats pokerchip 5
    for(i = 0; i < MAX; i++)
    {
        Betcirkel4_5[i].x = 673;
        Betcirkel4_5[i].y = 350-(i*5);
        Betcirkel4_5[i].w = 23;
        Betcirkel4_5[i].h = 23;
    }
    
    // Betcirkel 4 för plats pokerchip 25
    for(i = 0; i < MAX; i++)
    {
        Betcirkel4_25[i].x = 696;
        Betcirkel4_25[i].y = 350-(i*5);
        Betcirkel4_25[i].w = 23;
        Betcirkel4_25[i].h = 23;
    }
    
    // Betcirkel 4 för plats pokerchip 50
    for(i = 0; i < MAX; i++)
    {
        Betcirkel4_50[i].x = 665;
        Betcirkel4_50[i].y = 367-(i*5);
        Betcirkel4_50[i].w = 23;
        Betcirkel4_50[i].h = 23;
    }
    
    // Betcirkel 4 för plats pokerchip 100
    for(i = 0; i < MAX; i++)
    {
        Betcirkel4_100[i].x = 688;
        Betcirkel4_100[i].y = 367-(i*5);
        Betcirkel4_100[i].w = 23;
        Betcirkel4_100[i].h = 23;
    }
    
    
    // Betcirkel 5 för plats pokerchip 1
    for(i = 0; i < MAX; i++)
    {
        Betcirkel5_1[i].x = 777;
        Betcirkel5_1[i].y = 260-(i*5);
        Betcirkel5_1[i].w = 23;
        Betcirkel5_1[i].h = 23;
    }
    
    // Betcirkel 5 för plats pokerchip 5
    for(i = 0; i < MAX; i++)
    {
        Betcirkel5_5[i].x = 800;
        Betcirkel5_5[i].y = 260-(i*5);
        Betcirkel5_5[i].w = 23;
        Betcirkel5_5[i].h = 23;
    }
    
    // Betcirkel 5 för plats pokerchip 25
    for(i = 0; i < MAX; i++)
    {
        Betcirkel5_25[i].x = 823;
        Betcirkel5_25[i].y = 260-(i*5);
        Betcirkel5_25[i].w = 23;
        Betcirkel5_25[i].h = 23;
    }
    
    // Betcirkel 5 för plats pokerchip 50
    for(i = 0; i < MAX; i++)
    {
        Betcirkel5_50[i].x = 795;
        Betcirkel5_50[i].y = 275-(i*5);
        Betcirkel5_50[i].w = 23;
        Betcirkel5_50[i].h = 23;
    }
    
    // Betcirkel 5 för plats pokerchip 100
    for(i = 0; i < MAX; i++)
    {
        Betcirkel5_100[i].x = 818;
        Betcirkel5_100[i].y = 275-(i*5);
        Betcirkel5_100[i].w = 23;
        Betcirkel5_100[i].h = 23;
    }
    
    //PlayButton
    PlayButton.y = SCREEN_HEIGHT/2+40;
    PlayButton.x = SCREEN_WIDTH/2-147;
    PlayButton.w = 294;
    PlayButton.h = 107;
    // Exit Button
    ExitRect.y = 0;
    ExitRect.x = 0;
    ExitRect.w = 80;
    ExitRect.h = 40;
    // Bet Button
    BetButton.y = 418;
    BetButton.x = 15;
    BetButton.w = 93;
    BetButton.h = 90;
    // Clear Button
    ClearButton.y = 418;
    ClearButton.x = 123;
    ClearButton.w = 93;
    ClearButton.h = 90;
    // Hit Button
    HitButton.y = 474;
    HitButton.x = 689;
    HitButton.w = 93;
    HitButton.h = 90;
    // Stand Button
    StandButton.y = 474;
    StandButton.x = 793;
    StandButton.w = 93;
    StandButton.h = 90;
    // Double Button
    DoubleButton.y = 372;
    DoubleButton.x = 918;
    DoubleButton.w = 93;
    DoubleButton.h = 90;
    // Split Button
    SplitButton.y = 474;
    SplitButton.x = 918;
    SplitButton.w = 93;
    SplitButton.h = 90;
    
    return 0;
}