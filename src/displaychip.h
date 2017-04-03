// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#ifndef _displaychip_h_
#define _displaychip_h_

#include "texturedata.h"

typedef struct displayChip *DisplayChip;

DisplayChip displayChip_Create(int width, int height);

int displayChip_GetPixelCount(DisplayChip self);

int displayChip_GetPixelAt(DisplayChip self, int idx);

void displayChip_Clear(DisplayChip self);

void displayChip_Draw(DisplayChip self, TextureData pixelData, int x, int y);

#endif
