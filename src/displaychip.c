// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#include <assert.h>
#include <stdlib.h>
#include "util.h"
#include "chip.h"
#include "texturedata.h"
#include "colorchip.h"
#include "displaychip.h"

typedef struct displayChip {
    chip base; // must be first
    TextureData texture;
    ColorChip colorChip;
} displayChip;

static void displayChip_Destroy(DisplayChip self);
static void displayChip_Init(DisplayChip self, GetChip getChip);

DisplayChip displayChip_Create(int width, int height)
{
    DisplayChip self = NULL;

    self = (DisplayChip)calloc(1, sizeof(displayChip));
    if (self == NULL)
        return NULL;

    strncpy(self->base.name, nameof(DisplayChip), sizeof(self->base.name) - 1);
    self->base.destroy = displayChip_Destroy;
    self->base.init = displayChip_Init;
    self->texture = textureData_Create(width, height);
    if (self->texture == NULL)
    {
        free(self);
        return NULL;
    }

    return self;
}

static void displayChip_Destroy(DisplayChip self)
{
    assert(self);
    memset(self, 0, sizeof(displayChip));
    free(self);
}

static void displayChip_Init(DisplayChip self, GetChip getChip)
{
    assert(self);
    self->colorChip = (ColorChip)func_Invoke(getChip, nameof(ColorChip));
}

int displayChip_GetPixelCount(DisplayChip self)
{
    assert(self);
    return textureData_GetPixelCount(self->texture);
}

int displayChip_GetPixelAt(DisplayChip self, int idx)
{
    assert(self);
    return textureData_GetPixelAt(self->texture, idx);
}

void displayChip_Clear(DisplayChip self)
{
    assert(self);

    int backgroundColor = self->colorChip != NULL
        ? colorChip_GetBackgroundColor(self->colorChip)
        : -1;

    textureData_Clear(self->texture, backgroundColor);
}

void displayChip_Draw(DisplayChip self, TextureData pixelData, int x, int y)
{
    assert(self);
    textureData_CopyToAtPos(pixelData, self->texture, x, y);
}