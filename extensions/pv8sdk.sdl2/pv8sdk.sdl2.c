// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sdl.h>
#include "pv8sdk.sdl2.h"
#include "sdldisplaydevice.h"


static void sdl_handleInput(SDL_Event *event,
    SDLControllerDevice controller1,
    SDLControllerDevice controller2);

static void sdl_handleKeyboardInput(SDL_KeyboardEvent *event,
    SDLControllerDevice controller1,
    SDLControllerDevice controller2);

static void sdl_HandleMouseMotionInput(SDL_MouseMotionEvent *event,
    SDLControllerDevice controller1);

void sdl_runGame(GameConsole console,
    SDLControllerDevice controller1,
    SDLControllerDevice controller2)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        exit(EXIT_FAILURE);

    SDL_ShowCursor(SDL_DISABLE);

    gameConsole_PowerOn(console);

    const float fps = 1.0f;
    SDL_Event event;
    int numFrames = 0;
    Uint64 oldTime = 0;
    Uint64 newTime = SDL_GetTicks();
    Uint64 startTime = SDL_GetTicks();
    float deltaTime = 0.0f;
    int end = false;
    while (!end)
    {
        while (SDL_PollEvent(&event))
        {
            sdl_handleInput(&event, controller1, controller2);
            if (event.type == SDL_QUIT)
            {
                end = true;
                break;
            }
        }

        newTime = SDL_GetTicks();
        deltaTime = (float)(newTime - oldTime) / 1000.0f;
        oldTime = newTime;
        gameConsole_Tick(console, deltaTime);

        float fps = (numFrames / (float)(newTime - startTime)) * 1000;
        printf("%f\n", fps);

        gameConsole_Render(console);
        numFrames++;
    }

    gameConsole_PowerOff(console);

    SDL_Quit();
}

static void sdl_handleInput(SDL_Event *event,
    SDLControllerDevice controller1,
    SDLControllerDevice controller2)
{
    switch (event->type)
    {
    case SDL_KEYDOWN:
    case SDL_KEYUP:
        sdl_handleKeyboardInput(&event->key,
            controller1,
            controller2);
        break;

    case SDL_MOUSEMOTION:
        sdl_HandleMouseMotionInput(event, controller1);
        break;

    default:
        break;
    }
}

static void sdl_handleKeyboardInput(SDL_KeyboardEvent *event,
    SDLControllerDevice controller1,
    SDLControllerDevice controller2)
{
    if (controller1 != NULL)
        sdlController_KeyStateChanged(controller1, event->keysym.sym,
            event->type == SDL_KEYDOWN ? PRESSED : RELEASED);

    if (controller2 != NULL)
        sdlController_KeyStateChanged(controller2, event->keysym.sym,
            event->type == SDL_KEYDOWN ? PRESSED : RELEASED);
}

static void sdl_HandleMouseMotionInput(SDL_MouseMotionEvent *event,
    SDLControllerDevice controller1)
{
    if (controller1 == NULL) return;
    sdlController_SetMousePosition(controller1, event->x, event->y);
}