// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sdl.h>
#include "pv8sdk.sdl2.h"
#include "sdldisplaydevice.h"

static void sdl_handleInput(SDL_Event *event, GameConsole console);
static void sdl_handleKeyboardInput(SDL_KeyboardEvent *event, GameConsole console);

void sdl_runGame(GameConsole console)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        exit(EXIT_FAILURE);

    gameConsole_PowerOn(console);

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
            sdl_handleInput(&event, console);
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

    gameConsole_PowerOff(console);

    SDL_Quit();
}

static void sdl_handleInput(SDL_Event *event, GameConsole console)
{
    switch (event->type)
    {
    case SDL_KEYDOWN:
    case SDL_KEYUP:
        sdl_handleKeyboardInput(&event->key, console);
        break;
    default:
        break;
    }
}

static void sdl_handleKeyboardInput(SDL_KeyboardEvent *event, GameConsole console)
{

}