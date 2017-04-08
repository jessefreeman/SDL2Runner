// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#include <assert.h>
#include <stdlib.h>
#include "util.h"
#include "chip.h"
#include "sprite.h"
#include "colorchip.h"
#include "spritechip.h"

typedef struct spriteChip {
    chip base; // must be first
    int spriteWidth;
    int spriteHeight;
    int numSpritePages;
    int spritesPerPage;
    int maxSprites;
    spriteId nextAvailableSpriteId;
    sprites sprites;
} spriteChip;

static void spriteChip_Destroy(SpriteChip self);

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

    self->spriteWidth = clamp(spriteWidth, 1, MAX_SPRITE_WIDTH);
    self->spriteHeight = clamp(spriteHeight, 1, MAX_SPRITE_HEIGHT);
    self->numSpritePages = MAX_SPRITE_PAGES;
    self->spritesPerPage = MAX_SPRITES_PER_PAGE;
    self->maxSprites = MAX_SPRITES;
    self->nextAvailableSpriteId = 0;

    spriteChip_AddSpritesFromTexture(self, spriteSheet, NULL);

    return self;
}

static void spriteChip_Destroy(SpriteChip self)
{
    assert(self);
    memset(self, 0, sizeof(spriteChip));
    free(self);
}

Sprite spriteChip_GetSprite(SpriteChip self, spriteId id)
{
    assert(self);
    return &self->sprites[clamp(id, 0, min(self->maxSprites, MAX_SPRITES) - 1)];
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

void spriteChip_AddSpritesFromTexture(SpriteChip self, TextureData spriteSheet, spriteId *map)
{
    assert(self);
    assert(spriteSheet);

    int spriteWidth = self->spriteWidth;
    int spriteHeight = self->spriteHeight;

    int textureWidth = textureData_GetWidth(spriteSheet);
    int cols = textureWidth / spriteWidth;

    int textureHeight = textureData_GetHeight(spriteSheet);
    int rows = textureHeight / spriteHeight;

    spriteId spriteId = self->nextAvailableSpriteId;

    int i = 0;
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            sprite_Init(&self->sprites[spriteId], self->spriteWidth, self->spriteHeight, NULL);
            
            int tIdx = 0;
            for (int y = (r * spriteHeight); y < (r * spriteHeight) + spriteHeight; y++)
            {
                for (int x = (c * spriteWidth); x < (c * spriteWidth) + spriteWidth; x++)
                {
                    int cIdx = (y * textureWidth) + x;
                    colorId id = textureData_GetPixel(spriteSheet, cIdx);
                    sprite_SetPixel(&self->sprites[spriteId], tIdx, id);
                    tIdx++;
                }
            }
            
            if (map != NULL)
                map[i++] = spriteId;

            spriteId++;
        }
    }

    self->nextAvailableSpriteId = spriteId;
}