// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sdl.h>
#include "pv8sdk.sdl2.h"
#include "sdldisplaydevice.h"

struct sdl {
    char pass;
};

static struct sdl sdlRef;

static SDL sdl = NULL;

SDL sdl_GetInstance()
{
    if (sdl == NULL)
    {
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
            exit(EXIT_FAILURE);

        sdl = &sdlRef;
    }

    return sdl;
}

extern SDLDisplayDevice sdlDisplay_Create(int winWidth, int winHeight, int dispWidth, int dispHeight);

SDLDisplayDevice sdl_CreateDisplay(SDL self,
    int windowWidth, int windowHeight,
    int displayWidth, int displayheight)
{
    assert(self);
    return sdlDisplay_Create(windowWidth, windowHeight, displayWidth, displayheight);
}

void sdl_RunGame(SDL self, GameConsole console)
{
    const float fps = 1.0f;
    SDL_Event event;
    Uint64 oldTime = 0;
    Uint64 newTime = SDL_GetTicks();
    float deltaTime = 0.0f;
    int end = false;
    while (!end)
    {
        while (SDL_PollEvent(&event))
        {
            //handleInput(&event);
            if (event.type == SDL_QUIT)
            {
                end = true;
                break;
            }
        }

        newTime = SDL_GetTicks();
        deltaTime = (float)(newTime - oldTime)/1000.0f;
        oldTime = newTime;
        gameConsole_Tick(console, deltaTime);

        gameConsole_Render(console);
    }
}