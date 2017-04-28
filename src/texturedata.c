// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include "util.h"
#include "texturedata.h"

typedef struct textureData {
    int width;
    int height;
    int pixelsLength;
    size_t pixelsSize;
    colorId *pixels;
    bool wrapMode;
} textureData;

TextureData textureData_Create(int width, int height)
{
    TextureData self = NULL;

    self = (TextureData)calloc(1, sizeof(textureData));
    if (self == NULL)
        return NULL;

    self->width = width;
    self->height = height;
    self->pixelsLength = width * height;
    self->pixelsSize = self->pixelsLength * sizeof(colorId);
    self->pixels = (colorId *)calloc(1, self->pixelsSize);
    if (self->pixels == NULL)
    {
        free(self);
        return NULL;
    }

    self->wrapMode = true;

    textureData_Clear(self, -1);
    return self;
}

void textureData_Destroy(TextureData self)
{
    assert(self);
    if (self->pixels != NULL)
    {
        memset(self->pixels, 0, self->pixelsLength * sizeof(colorId));
        free(self->pixels);
    }
    memset(self, 0, sizeof(textureData));
    free(self);
}

void textureData_SetWrapMode(TextureData self, bool value)
{
    assert(self);
    self->wrapMode = value;
}

bool textureData_GetWrapMode(TextureData self)
{
    assert(self);
    return self->wrapMode;
}

void textureData_Clear(TextureData self, colorId id)
{
    assert(self);
    for (int idx = 0; idx < self->pixelsLength; idx++)
        self->pixels[idx] = id;
}

colorId textureData_GetPixel(TextureData self, int idx)
{
    assert(self);
    if (idx < 0 || idx >= self->pixelsLength || self->pixels == NULL)
        return -1;
    return self->pixels[idx];
}

void textureData_SetPixelAt(TextureData self, int idx, colorId id)
{
    assert(self);
    if (idx < 0 || idx >= self->pixelsLength || self->pixels == NULL)
        return;
    self->pixels[idx] = id;
}

void textureData_SetPixelAtPos(TextureData self, int x, int y, colorId id)
{
    assert(self);
    if (!self->wrapMode && (x >= self->width || y >= self->height)) return;
    textureData_SetPixelAt(self, coordsToIdx(x, y, self->width), id);
}

int textureData_GetPixelCount(TextureData self)
{
    assert(self);
    return self->pixelsLength;
}

void textureData_CopyTo(TextureData self, TextureData target)
{
    assert(self);
    assert(target);
    textureData_CopyToAtPos(self, target, 0, 0);
}

void textureData_CopyToAtPos(TextureData self, TextureData target, int x, int y)
{
    assert(self);
    assert(target);
    for (int sy = 0; sy < self->height; sy++)
    {
        for (int sx = 0; sx < self->width; sx++)
        {
            int sIdx = coordsToIdx(sx, sy, self->width);
            colorId colorId = textureData_GetPixel(self, sIdx);
            int ty = y + sy;
            int tx = x + sx;
            textureData_SetPixelAtPos(target, tx, ty, colorId);
        }
    }
}

int textureData_GetWidth(TextureData self) 
{
    assert(self);
    return self->width;
}

int textureData_GetHeight(TextureData self)
{
    assert(self);
    return self->height;
}
