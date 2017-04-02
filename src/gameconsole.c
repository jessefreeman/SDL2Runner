// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include "util.h"
#include "chips.h"
#include "engine.h"
#include "display.h"
#include "gameconsole.h"

typedef struct gameConsole {
    PixelVisionEngine engine;
    Display display;
} gameConsole;

static void gameConsole_RenderToDisplay(GameConsole self, DisplayChip displayChip, ColorChip colorChip);

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
    if (self->display != NULL) display_Destroy(self->display);
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

void gameConsole_InsertCartridge(GameConsole self, Cartridge cartridge)
{

}

void gameConsole_InsertController(GameConsole self, Controller controller)
{

}

void gameConsole_InsertDisplay(GameConsole self, Display display)
{
    assert(self);
    if (self->display != NULL) display_Destroy(self->display);
    self->display = display;
}

void gameConsole_Run(GameConsole self, GetElapsedTime getElapsedTime)
{
    assert(self);
    pixelVisionEngine_Init(self->engine);
    display_Init(self->display);
    DisplayChip displayChip = (DisplayChip)pixelVisionEngine_GetChip(self->engine, nameof(DisplayChip));
    ColorChip colorChip = (ColorChip)pixelVisionEngine_GetChip(self->engine, nameof(ColorChip));
    while (true)
    {
        float timeDelta = func_Invoke(getElapsedTime);
        pixelVisionEngine_Update(self->engine, timeDelta); // TODO: time
        pixelVisionEngine_Draw(self->engine);
        gameConsole_RenderToDisplay(self, displayChip, colorChip);
    }
}

static void gameConsole_RenderToDisplay(GameConsole self, DisplayChip displayChip, ColorChip colorChip)
{
    // should this be moved to display?
    int pixelsLen = displayChip_GetPixelCount(displayChip);
    colorData *pixels = (colorData *)calloc(pixelsLen, sizeof(colorData));
    for (int i = 0; i < pixelsLen; i++)
    {
        int colorRef = displayChip_GetPixelAt(displayChip, i);
        if (colorRef < 0)
        {
            pixels[i].r = 255;
            pixels[i].g = 0;
            pixels[i].b = 255;
        }
        else
        {
            colorData color = colorChip_GetColorAt(colorChip, colorRef);
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
    display_Render(self->display, pixelsLen, pixels);
    free(pixels);
}
