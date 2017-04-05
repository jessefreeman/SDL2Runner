// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include "util.h"
#include "chip.h"
#include "types.h"
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

ColorChip colorChip_Create(colorData colors[], int len)
{
    ColorChip self = NULL;

    self = (ColorChip)calloc(1, sizeof(colorChip));
    if (self == NULL)
        return NULL;

    strncpy(self->base.name, nameof(ColorChip), sizeof(self->base.name) - 1);
    self->base.destroy = colorChip_Destroy;
    self->base.init = colorChip_Init;

    self->colors = (colorData *)calloc(len, sizeof(colorData));
    if (self->colors == NULL)
    {
        free(self);
        return NULL;
    }
    memcpy(self->colors, colors, len * sizeof(colorData));
    self->colorsLen = len;

    return self;
}

static void colorChip_Destroy(ColorChip self)
{
    assert(self);
    colorChip_Dispose(self);
    memset(self, 0, sizeof(colorChip));
    free(self);
}

static void colorChip_Init(ColorChip self, GetChip getChip)
{
    assert(self);
    assert(getChip);
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

TextureData colorChip_MapPixelDataToTexture(ColorChip self, 
    int width, int height, colorData pixels[], TextureData textureData)
{
    assert(self);
    assert(pixels);
    assert(textureData);
    for (int y = 0; y < height; y++)
        for (int x = 0; x < width; x++)
        {
            int idx = coordsToIdx(x, y, width);
            colorData color = pixels[idx];
            textureData_SetPixelAt(textureData, idx, colorChip_FindColorRef(self, color));
        }
    return textureData;
}