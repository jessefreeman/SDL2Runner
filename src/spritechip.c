// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#include <assert.h>
#include <stdlib.h>
#include "util.h"
#include "chip.h"
#include "spritechip.h"
#include "colorchip.h"

typedef struct spriteChip {
    chip base; // must be first
    int spriteWidth;
    int spriteHeight;
    int spritesLen;
    TextureData *sprites;
} spriteChip;

static void spriteChip_Destroy(SpriteChip self);
static void spriteChip_Init(SpriteChip self, GetChip getChip);
static void spriteChip_Dispose(SpriteChip self);

SpriteChip spriteChip_Create(int spriteWidth, int spriteHeight, TextureData spriteSheet)
{
    assert(spriteWidth > 0); 
    assert(spriteHeight > 0);
    assert(spriteSheet);

    SpriteChip self = NULL;

    self = (SpriteChip)calloc(1, sizeof(spriteChip));
    if (self == NULL)
        return NULL;

    strncpy(self->base.name, nameof(SpriteChip), sizeof(self->base.name) - 1);
    self->base.destroy = spriteChip_Destroy;
    self->base.init = spriteChip_Init;

    self->spriteWidth = spriteWidth;
    self->spriteHeight = spriteHeight;
    self->spritesLen = 0;

    spriteChip_AddSprites(self, spriteSheet, NULL);

    return self;
}

static void spriteChip_Destroy(SpriteChip self)
{
    assert(self);
    spriteChip_Dispose(self);
    memset(self, 0, sizeof(spriteChip));
    free(self);
}

static void spriteChip_Init(SpriteChip self, GetChip getChip)
{
    assert(self);
    assert(getChip);
}

static void spriteChip_Dispose(SpriteChip self)
{
    assert(self);
    if (self->sprites != NULL)
    {
        for (int idx = 0; idx < self->spritesLen; idx++)
        {
            if (self->sprites[idx] == NULL)
                continue;

            textureData_Destroy(self->sprites[idx]);
            self->sprites[idx] = NULL;
        }
        self->spritesLen = 0;
    }
}

TextureData spriteChip_GetSprite(SpriteChip self, int id)
{
    assert(self);
    assert(id >= 0);
    if (self->sprites == NULL || id >= self->spritesLen) return NULL;
    return self->sprites[id];
}

int spriteChip_GetSpriteWidth(SpriteChip self)
{
    assert(self);
    return self->spriteWidth;
}

int spriteChip_GetSpriteHeight(SpriteChip self)
{
    assert(self);
    return self->spriteHeight;
}

void spriteChip_AddSprites(SpriteChip self, TextureData spriteSheet, int *map)
{
    assert(self);
    assert(spriteSheet);

    int spriteWidth = self->spriteWidth;
    int spriteHeight = self->spriteHeight;

    int textureWidth = textureData_GetWidth(spriteSheet);
    int cols = textureWidth / spriteWidth;

    int textureHeight = textureData_GetHeight(spriteSheet);
    int rows = textureHeight / spriteHeight;

    int spriteIdx = self->spritesLen;
    int newSpritesLen = self->spritesLen + (rows * cols);

    TextureData newSprites = (TextureData *)calloc(newSpritesLen, sizeof(TextureData));
    assert(newSprites);
    if (self->sprites != NULL)
    {
        memcpy(newSprites, self->sprites, self->spritesLen * sizeof(TextureData *));
        free(self->sprites);
    }
    self->spritesLen = newSpritesLen;
    self->sprites = newSprites;

    int i = 0;
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            self->sprites[spriteIdx] = textureData_Create(spriteWidth, spriteHeight);
            int tIdx = 0;
            for (int y = (r * spriteHeight); y < (r * spriteHeight) + spriteHeight; y++)
            {
                for (int x = (c * spriteWidth); x < (c * spriteWidth) + spriteWidth; x++)
                {
                    int cIdx = (y * textureWidth) + x;
                    colorId id = textureData_GetPixel(spriteSheet, cIdx);
                    textureData_SetPixelAt(self->sprites[spriteIdx], tIdx, id);
                    tIdx++;
                }
            }
            
            if (map != NULL)
                map[i++] = spriteIdx;

            spriteIdx++;
        }
    }
}