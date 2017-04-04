// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#ifndef _cartridge_h_
#define _cartridge_h_

#include "types.h"
#include "texturedata.h"
#include "cartridge.h"
#include "func.h"

typedef Func1(colorData, int) GetColorRef;

typedef struct cartridge {
    void(*destroy)(Obj);
    colorData *(*getColorsData)(Obj, int *);
    TextureData *(*getSpritesData)(Obj, int *, int *);
    char *(*getScriptData)(Obj, int *);
} cartridge;

typedef struct cartridge *Cartridge;

void cartridge_Destroy(Cartridge self);

colorData *cartridge_GetColorData(Cartridge self, int *len);

colorData *cartridge_GetSpritesData(Cartridge self, int *width, int *height);

char *cartridge_GetScriptData(Cartridge self, int *scriptLen);

#endif

