// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <SDL.h>
#include "displaydevice.h"
#include "sdldisplaydevice.h"

static void sdlDisplay_Destroy(SDLDisplayDevice self);
static void sdlDisplay_PowerOn(SDLDisplayDevice self);
static void sdlDisplay_PowerOff(SDLDisplayDevice self);
static void sdlDisplay_Render(SDLDisplayDevice self, int pixelsLen, colorData pixels[]);

void sdlDisplayDevice_Init(SDLDisplayDevice self,
    int winWidth, int winHeight, int dispWidth, int dispHeight)
{
    memset(self, 0, sizeof(sdlDisplayDevice));
    self->base.base.destroy = sdlDisplay_Destroy;
    self->base.base.powerOn = sdlDisplay_PowerOn;
    self->base.base.powerOff = sdlDisplay_PowerOff;
    self->base.render = sdlDisplay_Render;
    self->winWidth = winWidth;
    self->winHeight = winHeight;
    self->dispWidth = dispWidth;
    self->dispHeight = dispHeight;
}

static void sdlDisplay_Destroy(SDLDisplayDevice self)
{
    sdlDisplay_PowerOff(self);
    memset(self, 0, sizeof(sdlDisplayDevice));
}

static void sdlDisplay_PowerOn(SDLDisplayDevice self)
{
    assert(self);
    sdlDisplay_PowerOff(self);

    self->window = SDL_CreateWindow("PV8",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        self->winWidth, self->winHeight,
        SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE);
    
    //SDL_SetWindowFullscreen(self->window, SDL_WINDOW_FULLSCREEN);

    self->renderer = SDL_CreateRenderer(self->window, 
        -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);

    SDL_RenderSetLogicalSize(self->renderer, self->dispWidth, self->dispHeight);
}

static void sdlDisplay_PowerOff(SDLDisplayDevice self)
{
    assert(self);
    if (self->renderer != NULL)
    {
        SDL_DestroyRenderer(self->renderer);
        self->renderer = NULL;
    }
    if (self->window != NULL)
    {
        SDL_DestroyWindow(self->window);
        self->window = NULL;
    }
}

static void sdlDisplay_Render(SDLDisplayDevice self, int pixelsLen, colorData pixels[])
{
    int x = 0;
    int y = 0;
    //SDL_RenderClear(self->renderer);
    for (int i = 0; i < pixelsLen; i++)
    {
        colorData current = pixels[i];
        SDL_SetRenderDrawColor(self->renderer, current.r, current.g, current.b, 255);
        x = i % self->dispWidth;
        y = i / self->dispWidth;
        SDL_RenderDrawPoint(self->renderer, x, y);
    }
    SDL_RenderPresent(self->renderer);
}