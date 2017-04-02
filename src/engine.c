// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#include <assert.h>
#include <stdlib.h>
#include "util.h"
#include "engine.h"

#define NUM_CHIP_SLOTS 16

typedef struct pixelVisionEngine {
    Chip chips[NUM_CHIP_SLOTS];
    int chipCount;
    GetChip getChip;
} pixelVisionEngine;

PixelVisionEngine pixelVisionEngine_Create()
{
    PixelVisionEngine self = NULL;

    self = (PixelVisionEngine)calloc(1, sizeof(pixelVisionEngine));
    if (self == NULL)
        return NULL;

    self->getChip = func_Create(self, pixelVisionEngine_GetChip);
    if (self->getChip == NULL)
    {
        free(self);
        return NULL;
    }

    return self;
}

void pixelVisionEngine_Destroy(PixelVisionEngine self)
{
    assert(self);
    for (int idx = 0; idx < self->chipCount; idx++)
        chip_Destroy(self->chips[idx]);
    memset(self, 0, sizeof(pixelVisionEngine));
    free(self);
}

void pixelVisionEngine_InsertChip(PixelVisionEngine self, Chip chip)
{
    assert(self);
    assert(chip);
    assert(self->chipCount < NUM_CHIP_SLOTS);
    self->chips[self->chipCount] = chip;
    self->chipCount++;
}

Chip pixelVisionEngine_GetChip(PixelVisionEngine self, const char *chipName)
{
    assert(self);
    assert(chipName);
    Chip result = NULL;
    for (int idx = 0; idx < self->chipCount && result == NULL; idx++)
        result = strequals(self->chips[idx]->name, chipName)
            ? self->chips[idx]
            : result;
    return result;
}

void pixelVisionEngine_Init(PixelVisionEngine self)
{
    assert(self);
    for (int idx = 0; idx < self->chipCount; idx++)
        chip_Init(self->chips[idx], self->getChip);
}

void pixelVisionEngine_Update(PixelVisionEngine self, float timeDelta)
{
    assert(self);
    for (int idx = 0; idx < self->chipCount; idx++)
        chip_Update(self->chips[idx], timeDelta);
}

void pixelVisionEngine_Draw(PixelVisionEngine self)
{
    assert(self);
    for (int idx = 0; idx < self->chipCount; idx++)
    {
        Chip currentChip = self->chips[idx];
        chip_Draw(self->chips[idx]);
    }
}