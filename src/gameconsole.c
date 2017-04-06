// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include "util.h"
#include "chips.h"
#include "engine.h"
#include "displaydevice.h"
#include "gameconsole.h"

typedef struct gameConsole {
    PixelVisionEngine engine;
    DisplayDevice display;
    DisplayChip displayChip;
    ColorChip colorChip;
} gameConsole;

static void gameConsole_RenderToDisplay(GameConsole self);

GameConsole gameConsole_Create()
{
    GameConsole self = NULL;

    self = (GameConsole)calloc(1, sizeof(gameConsole));
    if (self == NULL)
        return NULL;

    self->engine = pixelVisionEngine_Create();
    if (self->engine == NULL)
    {
        free(self);
        return NULL;
    }

    return self;
}

void gameConsole_Destroy(GameConsole self)
{
    assert(self);
    if (self->display != NULL) displayDevice_Destroy(self->display);
    pixelVisionEngine_Destroy(self->engine);
    memset(self, 0, sizeof(gameConsole));
    free(self);
}

void gameConsole_InsertChip(GameConsole self, Chip chip)
{
    assert(self);
    assert(chip);
    pixelVisionEngine_InsertChip(self->engine, chip);
}

void gameConsole_InsertController(GameConsole self, ControllerDevice controller)
{

}

void gameConsole_InsertDisplayDevice(GameConsole self, DisplayDevice display)
{
    assert(self);
    if (self->display != NULL) displayDevice_Destroy(self->display);
    self->display = display;
}

void gameConsole_PowerOn(GameConsole self)
{
    assert(self);
    displayDevice_Init(self->display);
    pixelVisionEngine_Init(self->engine);
    self->displayChip = (DisplayChip)pixelVisionEngine_GetChip(self->engine, nameof(DisplayChip));
    self->colorChip = (ColorChip)pixelVisionEngine_GetChip(self->engine, nameof(ColorChip));
    gameConsole_RenderToDisplay(self, true);
}

void gameConsole_Tick(GameConsole self, float timeDelta)
{
    assert(self);
    pixelVisionEngine_Update(self->engine, timeDelta);
}

void gameConsole_Render(GameConsole self)
{
    assert(self);
    pixelVisionEngine_Draw(self->engine);
    gameConsole_RenderToDisplay(self, false);
}

void gameConsole_PowerOff(GameConsole self)
{
    gameConsole_RenderToDisplay(self, true);
}

static void gameConsole_RenderToDisplay(GameConsole self, bool init)
{
    // should this be moved to display?
    static int pixelsLen = 0; 
    static colorData *pixels = NULL; 
    if (init)
    {
        if (pixels == NULL)
        {
            pixelsLen = displayChip_GetPixelCount(self->displayChip);
            pixels = (colorData *)calloc(pixelsLen, sizeof(colorData));
        }
        else
        {
            free(pixels);
            pixelsLen = 0;
        }
    }

    for (int i = 0; i < pixelsLen; i++)
    {
        int colorRef = displayChip_GetPixelAt(self->displayChip, i);
        if (colorRef < 0)
        {
            pixels[i].r = 255;
            pixels[i].g = 0;
            pixels[i].b = 255;
        }
        else
        {
            colorData color = colorChip_GetColorAt(self->colorChip, colorRef);
            // magenta transparent for now
            if (color.r != 255 || color.g != 0 || color.b != 255)
            {
                pixels[i] = color;
            }
            else
            {
                pixels[i].r = 0;
                pixels[i].g = 0;
                pixels[i].b = 0;
            }
        }
    }

    displayDevice_Render(self->display, pixelsLen, pixels);
}
