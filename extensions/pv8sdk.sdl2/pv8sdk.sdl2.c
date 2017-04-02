// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#include <assert.h>
#include <stdlib.h>
#include <sdl.h>
#include "pv8sdk.sdl2.h"

struct sdl {
    char pass;
};

static struct sdl sdlRef;

static Sdl sdl = NULL;

Sdl sdl_GetInstance()
{
    if (sdl == NULL)
    {
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
            exit(EXIT_FAILURE);

        sdl = &sdlRef;
    }

    return sdl;
}

extern SdlDisplay sdlDisplay_Create(int winWidth, int winHeight, int dispWidth, int dispHeight);

SdlDisplay sdl_CreateDisplay(Sdl self,
    int windowWidth, int windowHeight,
    int displayWidth, int displayheight)
{
    assert(self);
    return sdlDisplay_Create(windowWidth, windowHeight, displayWidth, displayheight);
}