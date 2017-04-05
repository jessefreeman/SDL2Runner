// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#include <assert.h>
#include <stdlib.h>
#include "util.h"
#include "texturedata.h"

typedef struct textureData {
    int width;
    int height;
    int pixelsLength;
    size_t pixelsSize;
    int *pixels;
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
    self->pixelsSize = self->pixelsLength * sizeof(int);
    self->pixels = (int *)calloc(1, self->pixelsSize);
    if (self->pixels == NULL)
    {
        free(self);
        return NULL;
    }

    textureData_Clear(self, -1);
    return self;
}

void textureData_Destroy(TextureData self)
{
    assert(self);
    if (self->pixels != NULL)
    {
        memset(self->pixels, 0, self->pixelsLength * sizeof(int));
        free(self->pixels);
    }
    memset(self, 0, sizeof(textureData));
    free(self);
}

void textureData_Clear(TextureData self, int color)
{
    assert(self);
    for (int idx = 0; idx < self->pixelsLength; idx++)
        self->pixels[idx] = color;
}

int textureData_GetPixelAt(TextureData self, int idx)
{
    assert(self);
    if (idx < 0 || idx >= self->pixelsLength || self->pixels == NULL)
        return -1;
    return self->pixels[idx];
}

void textureData_SetPixelAt(TextureData self, int idx, int value)
{
    assert(self);
    if (idx < 0 || idx >= self->pixelsLength || self->pixels == NULL)
        return;
    self->pixels[idx] = value;
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
    int targetY = y % target->height;
    int targetIdx = coordsToIdx(x, targetY, target->width);
    for (int selfIdx = 0, selfX = 0; selfIdx < self->pixelsLength; selfIdx++, selfX++)
    {
        if (selfX >= self->width)
        {
            targetY++;
            targetY = targetY % target->height;
            selfX = 0;
            targetIdx = coordsToIdx(x, targetY, target->width);
        }

        int val = textureData_GetPixelAt(self, selfIdx);
        textureData_SetPixelAt(target, targetIdx, val);
        targetIdx++;
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
