// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#include <assert.h>
#include <stdlib.h>
#include "util.h"
#include "texturedata.h"
#include "sprite.h"

void sprite_Init(Sprite self, int width, int height, colorId *spritePixels)
{
    assert(self);
    memset(self, 0, sizeof(sprite));
    self->width = width;
    self->height = height;
    self->maxPixels = width * height;
    if (spritePixels == NULL) return;
    memcpy(self->pixels, spritePixels, min(MAX_PIXELS_PER_SPRITE, width * height));
}

void sprite_SetPixel(Sprite self, int idx, colorId id)
{
    assert(self);
    self->pixels[clamp(idx, 0, min(MAX_PIXELS_PER_SPRITE, self->width * self->height) - 1)] = id;
}

void sprite_CopyToTextureAtPos(Sprite self, TextureData target, int x, int y)
{
    assert(self);
    assert(target);
    int textureHeight = textureData_GetHeight(target);
    int textureWidth = textureData_GetWidth(target);
    for (int sy = 0; sy < self->height; sy++)
    {
        for (int sx = 0; sx < self->width; sx++)
        {
            int sIdx = coordsToIdx(sx, sy, self->width);
            colorId colorId = self->pixels[sIdx];
            int ty = y + sy;
            int tx = x + sx;
            textureData_SetPixelAtPos(target, tx, ty, colorId);
        }
    }
}
