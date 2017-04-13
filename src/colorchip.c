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

ColorChip colorChip_Create(colorData colors[], int len)
{
    ColorChip self = NULL;

    self = (ColorChip)calloc(1, sizeof(colorChip));
    if (self == NULL)
        return NULL;

    colorChip_Init(self, colors, len);
    self->base.destroy = colorChip_Destroy;
}

void colorChip_Destroy(ColorChip self)
{
    assert(self);
    memset(self, 0, sizeof(colorChip));
    free(self);
}

void colorChip_Init(ColorChip self, colorData colors[], int len)
{
    assert(self);
    strncpy(self->base.name, nameof(ColorChip), sizeof(self->base.name) - 1);
    self->backgroundColor = 0; 
    self->colorsLen = len;
    memset(self->colors, 0, sizeof(self->colors));
    memcpy(self->colors, colors, min(len * sizeof(colorData), COLORID_MAX * sizeof(colorData)));
}

colorData colorChip_GetColor(ColorChip self, colorId id)
{
    assert(self);
    return self->colors[clamp(id, 0, self->colorsLen - 1)];
}

void colorChip_SetColor(ColorChip self, colorId id, colorData value)
{
    assert(self);
    self->colors[clamp(id, 0, self->colorsLen - 1)] = value;
}

colorId colorChip_FindColorId(ColorChip self, colorData color)
{
    assert(self);

    colorId result = 0;
    
    for (colorId i = 0; i < self->colorsLen; i++)
    {
        if (self->colors[i].r == color.r &&
            self->colors[i].g == color.g &&
            self->colors[i].b == color.b)
        {
            result = i;
            break;
        }
    }

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