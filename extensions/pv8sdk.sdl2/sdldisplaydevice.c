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

typedef struct sdlDisplayDevice {
    displayDevice base; // must be first
    int winWidth;
    int winHeight;
    int dispWidth;
    int dispHeight;
    SDL_Window *window;
    SDL_Renderer *renderer;
} sdlDisplay;

static void sdlDisplay_Destroy(SDLDisplayDevice self);
static void sdlDisplay_Init(SDLDisplayDevice self);
static void sdlDisplay_Dispose(SDLDisplayDevice self);
static void sdlDisplay_Render(SDLDisplayDevice self, int pixelsLen, colorData pixels[]);

SDLDisplayDevice sdlDisplay_Create(int winWidth, int winHeight, int dispWidth, int dispHeight)
{
    SDLDisplayDevice self = NULL;

    self = (SDLDisplayDevice)calloc(1, sizeof(sdlDisplay));
    if (self == NULL)
        return NULL;

    self->base.destroy = sdlDisplay_Destroy;
    self->base.init = sdlDisplay_Init;
    self->base.render = sdlDisplay_Render;

    self->winWidth = winWidth;
    self->winHeight = winHeight;
    self->dispWidth = dispWidth;
    self->dispHeight = dispHeight;

    return self;
}

static void sdlDisplay_Destroy(SDLDisplayDevice self)
{
    assert(self);
    sdlDisplay_Dispose(self);
    memset(self, 0, sizeof(sdlDisplay));
    free(self);
}

static void sdlDisplay_Init(SDLDisplayDevice self)
{
    assert(self);
    sdlDisplay_Dispose(self);
    self->window = SDL_CreateWindow("PV8",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        self->winWidth, self->winHeight,
        SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE);
    
    self->renderer = SDL_CreateRenderer(self->window, 
        -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);

    SDL_RenderSetLogicalSize(self->renderer, self->dispWidth, self->dispHeight);
}

static void sdlDisplay_Dispose(SDLDisplayDevice self)
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
    SDL_RenderClear(self->renderer);
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