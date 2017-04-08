// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include "pvconf.h"
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
static void colorChip_Dispose(ColorChip self);

ColorChip colorChip_Create(colorData colors[], int len)
{
    ColorChip self = NULL;

    self = (ColorChip)calloc(1, sizeof(colorChip));
    if (self == NULL)
        return NULL;

    strncpy(self->base.name, nameof(ColorChip), sizeof(self->base.name) - 1);
    self->base.destroy = colorChip_Destroy;

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

static void colorChip_Dispose(ColorChip self)
{
    assert(self);
    if (self->colors == NULL) return;
    memset(self->colors, 0, self->colorsLen * sizeof(colorData));
    free(self->colors);
    self->colorsLen = 0;
    self->colors = NULL;
}

colorData colorChip_GetColor(ColorChip self, colorId id)
{
    assert(self);
    return id > 0 && id < self->colorsLen && self->colors != NULL
        ? self->colors[id]
        : (colorData){ 255, 0, 255 };
}

void colorChip_SetColor(ColorChip self, colorId id, colorData value)
{
    assert(self);
    if (id >= self->colorsLen || self->colors == NULL) return;
    self->colors[id] = value;
}

colorId colorChip_FindColorId(ColorChip self, colorData color)
{
    assert(self);
    if (self->colors == NULL)
        return -1;

    colorId result = 0;
    
    for (int i = 0; i < self->colorsLen && result == 0; i++)
        if (self->colors[i].r == color.r &&
            self->colors[i].g == color.g &&
            self->colors[i].b == color.b)
            result = i;

    return result;
}

void colorChip_SetBackgroundColorId(ColorChip self, colorId id)
{
    assert(self);
    self->backgroundColor = id;
}

colorId colorChip_GetBackgroundColorId(ColorChip self)
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
            textureData_SetPixelAt(textureData, idx, colorChip_FindColorId(self, color));
        }
    return textureData;
}