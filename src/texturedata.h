// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#ifndef _texturedata_h_
#define _texturedata_h_

#include "pvconf.h"

typedef struct textureData *TextureData;

TextureData textureData_Create(int width, int height);

void textureData_Destroy(TextureData self);

void textureData_Clear(TextureData self, colorId id);

colorId textureData_GetPixel(TextureData self, int idx);

void textureData_SetPixelAt(TextureData self, int idx, colorId id);

int textureData_GetPixelCount(TextureData self);

void textureData_CopyTo(TextureData self, TextureData target);

void textureData_CopyToAtPos(TextureData self, TextureData target, int x, int y);

int textureData_GetWidth(TextureData self);

int textureData_GetHeight(TextureData self);

#endif
