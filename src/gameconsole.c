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
    ControllerDevice controllers[MAX_CONTROLLERS];
    DisplayChip displayChip;
    ColorChip colorChip;
    ControllerChip controllerChip;
} gameConsole;

static void gameConsole_RenderToDisplay(GameConsole self, bool init);

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
    if (self->display != NULL) device_Destroy((Device)self->display);
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

void gameConsole_InsertController(GameConsole self, int slotIdx, ControllerDevice controller)
{
    assert(self);
    self->controllers[clamp(slotIdx, 0, MAX_CONTROLLERS - 1)] = controller;
}

void gameConsole_InsertDisplayDevice(GameConsole self, DisplayDevice display)
{
    assert(self);
    if (self->display != NULL) device_Destroy((Device)self->display);
    self->display = display;
}

void gameConsole_PowerOn(GameConsole self)
{
    assert(self);
    device_PowerOn((Device)self->display);
    pixelVisionEngine_Init(self->engine);
    self->displayChip = (DisplayChip)pixelVisionEngine_GetChip(self->engine, nameof(DisplayChip));
    self->colorChip = (ColorChip)pixelVisionEngine_GetChip(self->engine, nameof(ColorChip));
    self->controllerChip = (ControllerChip)pixelVisionEngine_GetChip(self->engine, nameof(ControllerChip));
    if (self->controllerChip != NULL)
    {
        for (int i = 0; i < arraylen(self->controllers); i++)
        {
            if (self->controllers[i] == NULL) continue;
            controllerChip_InsertController(self->controllerChip, i, self->controllers[i]);
        }
    }
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
    displayDevice_Refresh(self->display, self->displayChip);
}

void gameConsole_PowerOff(GameConsole self)
{
    device_PowerOff((Device)self->display);
    // TODO: clear controllers
}