// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#ifndef _fontchip_h_
#define _fontchip_h_

#include "texturedata.h"

typedef struct fontChip *FontChip;

void fontChip_AddFont(FontChip self, const char *name, int mapLen, int *map);

TextureData fontChip_ConvertTextToTexture(FontChip self, const char *text, const char *fontName, int letterSpacing);

#endif

