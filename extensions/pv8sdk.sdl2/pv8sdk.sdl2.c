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

float sdl_GetElapsedTime(Sdl self)
{
    // TODO: this is a temporary, ugly hack that needs to be cleaned up.

    static Uint32 oldTime = 0;
    static Uint32 currentTime = -1;
    
    if (currentTime < 0)
        currentTime = SDL_GetTicks();

    oldTime = currentTime;
    currentTime = SDL_GetTicks();
    float delta = (currentTime - oldTime) / 1000.0f;

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        //handleInput(&event);
        if (event.type == SDL_QUIT)
        {
            // TODO: clean this up
            exit(EXIT_SUCCESS);
            break;
        }
    }

    return delta;
}