//
//  multiOS.h
//  prototyp1
//
//  Created by Christian Ekenstedt on 2015-04-22.
//  Copyright (c) 2015 Grupp 3. All rights reserved.
//

#ifndef prototyp1_multiOS_h
#define prototyp1_multiOS_h
/* MAC OSX */
#ifdef __APPLE__
#include <SDL2/SDL.h>
#include <SDL2_net/SDL_net.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_mixer/SDL_mixer.h>
#include <SDL2_ttf/SDL_ttf.h>
#endif
/* Windows*/
#ifdef OS_WINDOWS

#endif
/* LINUX */
#ifdef __linux__
#include "SDL2/SDL.h"
#include "SDL2/SDL_net.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_ttf.h"
#endif

#endif
