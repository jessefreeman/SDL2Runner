// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <SDL.h>
#include "util.h"
#include "displaychip.h"
#include "displaydevice.h"
#include "sdldisplaydevice.h"

static void sdlDisplay_Destroy(SDLDisplayDevice self);
static void sdlDisplay_PowerOn(SDLDisplayDevice self);
static void sdlDisplay_PowerOff(SDLDisplayDevice self);
static void sdlDisplayDevice_Refresh(SDLDisplayDevice self, DisplayChip displayChip);

void sdlDisplayDevice_Init(SDLDisplayDevice self,
    int winWidth, int winHeight, int dispWidth, int dispHeight)
{
    memset(self, 0, sizeof(sdlDisplayDevice));
    self->base.base.destroy = sdlDisplay_Destroy;
    self->base.base.powerOn = sdlDisplay_PowerOn;
    self->base.base.powerOff = sdlDisplay_PowerOff;
    self->base.refresh = sdlDisplayDevice_Refresh;
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
        -1, SDL_RENDERER_ACCELERATED);// | SDL_RENDERER_PRESENTVSYNC);

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

static void sdlDisplayDevice_Refresh(SDLDisplayDevice self, DisplayChip displayChip)
{
    int x = 0;
    int y = 0;
    int displayIdx = 0;
    colorData pixelBuffer[256] = { 0 };
    displayChip_BeginRead(displayChip);
    int pixelsRead = displayChip_Read(displayChip, pixelBuffer, arraylen(pixelBuffer));
    while (pixelsRead > 0)
    {
        for (int i = 0; i < pixelsRead; i++)
        {
            colorData current = pixelBuffer[i];
            SDL_SetRenderDrawColor(self->renderer, current.r, current.g, current.b, 255);
            x = displayIdx % self->dispWidth;
            y = displayIdx / self->dispWidth;
            SDL_RenderDrawPoint(self->renderer, x, y);
            displayIdx++;
        }
        pixelsRead = displayChip_Read(displayChip, pixelBuffer, arraylen(pixelBuffer));
    }
    SDL_RenderPresent(self->renderer);
}