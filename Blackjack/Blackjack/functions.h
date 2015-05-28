//
//  functions.h
//  Blackjack
//
//  Created by Christian Ekenstedt on 2015-05-27.
//  Copyright (c) 2015 Grupp 3. All rights reserved.
//

#ifndef __Blackjack__functions__
#define __Blackjack__functions__

#include <stdio.h>
#include <stdbool.h>
#include "multiOS.h"
#include "gamelogic.h"

extern int tableInfo[MAXCLIENTS+1][15];

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

// The music woll be played
extern Mix_Music *gMusic;
//The window we'll be rendering to
extern SDL_Window* gWindow;
extern SDL_Window* textWindow;
//The surface contained by the window
extern SDL_Surface* gScreenSurface;
//The image we will load and show on the screen
// Textures
extern SDL_Texture* bTexture;
extern SDL_Texture* mPlayButton;
extern SDL_Texture* exitTexture;
extern SDL_Texture* betTexture;
extern SDL_Texture* clearTexture;
extern SDL_Texture* hitTexture;
extern SDL_Texture* standTexture;
extern SDL_Texture* doubleTexture;
extern SDL_Texture* splitTexture;
extern SDL_Texture* btable;
extern SDL_Texture* kort;
extern SDL_Texture* pottTexture;
extern SDL_Texture* pokerchip100;
extern SDL_Texture* pokerchip50;
extern SDL_Texture* pokerchip25;
extern SDL_Texture* pokerchip5;
extern SDL_Texture* pokerchip1;
extern SDL_Texture* baksida;
// Surface
extern SDL_Surface* gXOut;
extern SDL_Surface* text_surface;
//==========
extern SDL_Color textColor;
extern TTF_Font *font;

//The event structure
extern SDL_Event event;
// Renderer
extern SDL_Renderer* gRenderer;
// Rects
extern SDL_Rect gSpriteClips[3], cardSheet[52]; // Sprite
extern SDL_Rect ExitRect, ClearButton, HitButton, StandButton, DoubleButton, SplitButton, BetButton, PlayButton; // fasta knappar
extern SDL_Rect Chip1,Chip5,Chip25,Chip50,Chip100; // Marker
extern SDL_Rect table1[MAXCARDS], table2[MAXCARDS],table3[MAXCARDS], table4[MAXCARDS],table5[MAXCARDS],tableDealer[MAXCARDS];
extern SDL_Rect Betcirkel1_1[MAX], Betcirkel1_5[MAX], Betcirkel1_25[MAX], Betcirkel1_50[MAX], Betcirkel1_100[MAX]; // Betcirklar där pokermarker ska hamna
extern SDL_Rect Betcirkel2_1[MAX], Betcirkel2_5[MAX], Betcirkel2_25[MAX], Betcirkel2_50[MAX], Betcirkel2_100[MAX];
extern SDL_Rect Betcirkel3_1[MAX], Betcirkel3_5[MAX], Betcirkel3_25[MAX], Betcirkel3_50[MAX], Betcirkel3_100[MAX];
extern SDL_Rect Betcirkel4_1[MAX], Betcirkel4_5[MAX], Betcirkel4_25[MAX], Betcirkel4_50[MAX], Betcirkel4_100[MAX];
extern SDL_Rect Betcirkel5_1[MAX], Betcirkel5_5[MAX], Betcirkel5_25[MAX], Betcirkel5_50[MAX], Betcirkel5_100[MAX];
extern SDL_Rect renderRect,renderRect2;





void ClearScreen();
//Starts up SDL and creates window
bool init();
//Loads media
bool loadMedia();
//Frees media and shuts down SDL
void closeW();

//int ritaMoney(char word[], SDL_Rect position);
int ritaText(char word[],SDL_Rect position);

int sendToServer(char command[], TCPsocket socket);
int reciveFromServer(TCPsocket socket);
int IdToVisualCard(int id,Kort kortlek[]);
void initiera_kortleken(Kort kortleken[]);
void playerPosition(int myClientNr, int bet, int hit, int nykort, int money, int bordskort[]);
void stringToArray(char sendstring[]);
int whoWon(int playerValue, int dealerValue, int bet, bool blackjack);

#endif /* defined(__Blackjack__functions__) */
