// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#ifndef _spritechip_h_
#define _spritechip_h_

#include "texturedata.h"

typedef struct spriteChip *SpriteChip;

SpriteChip spriteChip_Create(int spriteWidth, int spriteHeight);

TextureData spriteChip_GetSprite(SpriteChip self, int id);

int spriteChip_GetSpriteWidth(SpriteChip self);

int spriteChip_GetSpriteHeight(SpriteChip self);

#endif
