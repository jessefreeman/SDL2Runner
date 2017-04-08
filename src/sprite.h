// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#ifndef _sprite_h_
#define _sprite_h_

#include "pvconf.h"
#include "types.h"
#include "texturedata.h"

typedef colorId spritePixels[MAX_PIXELS_PER_SPRITE];

typedef struct sprite {
    int width;
    int height;
    int maxPixels;
    spritePixels pixels;
} sprite;

typedef sprite *Sprite;

void sprite_Init(Sprite self, int width, int height, colorId *spritePixels);

void sprite_SetPixel(Sprite self, int idx, colorId);

void sprite_CopyToTextureAtPos(Sprite self, TextureData target, int x, int y);

#endif

