// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#ifndef _pv8sdk_sdl2_h_
#define _pv8sdk_sdl2_h_

#include "SDL_main.h"
#include "sdldisplay.h"

typedef struct sdl *Sdl;

Sdl sdl_GetInstance();

SdlDisplay sdl_CreateDisplay(Sdl self,
    int windowWidth, int windowHeight,
    int displayWidth, int displayheight);

#endif

