// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#ifndef _pv8sdk_sdl2_h_
#define _pv8sdk_sdl2_h_

#include "SDL_main.h"
#include "sdldisplaydevice.h"

typedef struct sdl *SDL;

SDL sdl_GetInstance();

SDLDisplayDevice sdl_CreateDisplay(SDL self,
    int windowWidth, int windowHeight,
    int displayWidth, int displayheight);

float sdl_GetElapsedTime(SDL self);

#endif

