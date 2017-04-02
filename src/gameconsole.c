// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include "util.h"
#include "chips.h"
#include "engine.h"
#include "gameconsole.h"

typedef struct gameConsole {
    PixelVisionEngine engine;
    Display display;
} gameConsole;

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

void gameConsole_Run(GameConsole self)
{
    assert(self);
    pixelVisionEngine_Init(self->engine);
    if (self->display != NULL) display_Init(self->display);
    DisplayChip displayChip = (DisplayChip)pixelVisionEngine_GetChip(self->engine, nameof(DisplayChip));
    while (true)
    {
        // TODO: input?
        pixelVisionEngine_Update(self->engine, 0.0f); // TODO: time
        pixelVisionEngine_Draw(self->engine);
        //display_Render(self->display, )
    }
}
