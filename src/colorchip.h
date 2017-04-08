// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#ifndef _colorchip_h_
#define _colorchip_h_

#include "types.h"
#include "texturedata.h"

typedef struct colorChip *ColorChip;

ColorChip colorChip_Create(colorData colors[], int len);

colorData colorChip_GetColor(ColorChip self, colorId id);

void colorChip_SetColor(ColorChip self, colorId, colorData value);

colorId colorChip_FindColorId(ColorChip self, colorData color);

void colorChip_SetBackgroundColorId(ColorChip self, colorId id);

colorId colorChip_GetBackgroundColorId(ColorChip self);

TextureData colorChip_MapPixelDataToTexture(ColorChip self,
    int width, int height, colorData pixels[], TextureData textureData);

#endif
