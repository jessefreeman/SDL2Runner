// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#ifndef _colorchip_h_
#define _colorchip_h_

#include "types.h"
#include "texturedata.h"

typedef struct colorChip *ColorChip;

ColorChip colorChip_Create(colorData colors[], int len);

colorData colorChip_GetColorAt(ColorChip self, int idx);

void colorChip_SetColorAt(ColorChip self, int idx, colorData value);

int colorChip_FindColorRef(ColorChip self, colorData color);

void colorChip_SetBackgroundColor(ColorChip self, int id);

int colorChip_GetBackgroundColor(ColorChip self);

TextureData colorChip_MapPixelDataToTexture(ColorChip self,
    int width, int height, colorData pixels[], TextureData textureData);

#endif
