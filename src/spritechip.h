// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#ifndef _spritechip_h_
#define _spritechip_h_

#include "pvconf.h"
#include "types.h"
#include "chip.h"
#include "sprite.h"
#include "texturedata.h"

typedef sprite sprites[MAX_SPRITES];

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

typedef spriteChip *SpriteChip;

SpriteChip spriteChip_Create(int spriteWidth, int spriteHeight, TextureData spriteSheet);

void spriteChip_Destroy(SpriteChip self);

void spriteChip_Init(SpriteChip self, int spriteWidth, int spriteHeight, TextureData spriteSheet);

Sprite spriteChip_GetSprite(SpriteChip self, spriteId id);

int spriteChip_GetSpriteWidth(SpriteChip self);

int spriteChip_GetSpriteHeight(SpriteChip self);

// TODO: maybe not assume map is big enough?
void spriteChip_AddSpritesFromTexture(SpriteChip self, TextureData spriteSheet, spriteId *map);

#endif
