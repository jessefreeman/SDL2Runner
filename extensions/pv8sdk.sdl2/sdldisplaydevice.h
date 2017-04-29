// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#ifndef _sdldisplaydevice_h_
#define _sdldisplaydevice_h_

#include <SDL.h>
#include "displaydevice.h"

typedef struct sdlDisplayDevice {
    displayDevice base; // must be first
    int winWidth;
    int winHeight;
    int dispWidth;
    int dispHeight;
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    byte *pixels;
} sdlDisplayDevice;

typedef struct sdlDisplayDevice *SDLDisplayDevice;

void sdlDisplayDevice_Init(SDLDisplayDevice self,
    int winWidth, int winHeight, int dispWidth, int dispHeight);

#endif
