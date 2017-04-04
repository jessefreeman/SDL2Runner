// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include "util.h"
#include "chip.h"
#include "types.h"
#include "cartridgechip.h"
#include "colorchip.h"

typedef struct colorChip {
    chip base; // must be first
    int colorsLen;
    int backgroundColor;
    colorData *colors;
} colorChip;

static void colorChip_Destroy(ColorChip self);
static void colorChip_Init(ColorChip self, GetChip getChip);
static void colorChip_Dispose(ColorChip self);

ColorChip colorChip_Create()
{
    ColorChip self = NULL;

    self = (ColorChip)calloc(1, sizeof(colorChip));
    if (self == NULL)
        return NULL;

    strncpy(self->base.name, nameof(ColorChip), sizeof(self->base.name) - 1);
    self->base.destroy = colorChip_Destroy;
    self->base.init = colorChip_Init;

    return self;
}

static void colorChip_Destroy(ColorChip self)
{
    assert(self);
    colorChip_Dispose(self);
    memset(self, 0, sizeof(colorChip));
    free(self);
}

colorData colorChip_GetColorAt(ColorChip self, int idx)
{
    assert(self);
    return idx > 0 && idx < self->colorsLen && self->colors != NULL
        ? self->colors[idx]
        : (colorData){ 255, 0, 255 };
}

void colorChip_SetColorAt(ColorChip self, int idx, colorData value)
{
    assert(self);
    if (idx < 0 || idx >= self->colorsLen || self->colors == NULL) return;
    self->colors[idx] = value;
}

int colorChip_FindColorRef(ColorChip self, colorData color)
{
    assert(self);
    if (self->colors == NULL)
        return -1;

    int result = -1;
    
    for (int i = 0; i < self->colorsLen && result < 0; i++)
        if (self->colors[i].r == color.r &&
            self->colors[i].g == color.g &&
            self->colors[i].b == color.b)
            result = i;

    return result;
}

void colorChip_SetBackgroundColor(ColorChip self, int id)
{
    assert(self);
    self->backgroundColor = id;
}

int colorChip_GetBackgroundColor(ColorChip self)
{
    assert(self);
    return self->backgroundColor;
}

static void colorChip_Init(ColorChip self, GetChip getChip)
{
    assert(self);
    assert(getChip);

    CartridgeChip cartridgeChip = (CartridgeChip)func_Invoke(getChip, nameof(CartridgeChip));
    if (cartridgeChip == NULL)
        return;

    colorChip_Dispose(self);
    self->colors = cartridgeChip_GetColors(cartridgeChip, &self->colorsLen);
}

static void colorChip_Dispose(ColorChip self)
{
    assert(self);
    if (self->colors == NULL) return;
    memset(self->colors, 0, self->colorsLen * sizeof(colorData));
    free(self->colors);
    self->colorsLen = 0;
    self->colors = NULL;
}