// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#ifndef _displaychip_h_
#define _displaychip_h_

#include "pvconf.h"
#include "sprite.h"
#include "texturedata.h"

typedef struct displayChip *DisplayChip;

DisplayChip displayChip_Create(int width, int height);

int displayChip_GetDisplayWidth(DisplayChip self);

int displayChip_GetDisplayHeight(DisplayChip self);

int displayChip_GetPixelCount(DisplayChip self);

colorId displayChip_GetPixel(DisplayChip self, int idx);

void displayChip_Clear(DisplayChip self);

void displayChip_DrawSprite(DisplayChip self, Sprite sprite, int x, int y);

void displayChip_DrawTexture(DisplayChip self, TextureData texture, int x, int y);

void displayChip_DrawTilemap(DisplayChip self);

void displayChip_BeginRead(DisplayChip self);

int displayChip_Read(DisplayChip self, colorData *buffer, int bufferLen);

void displayChip_EndRead(DisplayChip self);

#endif
