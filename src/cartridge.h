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
    colorData *(*getColors)(Obj, int *);
    TextureData *(*getSprites)(Obj, int, int, GetColorRef, int *);
    const char *(*getScript)(Obj, int *);
} cartridge;

typedef struct cartridge *Cartridge;

void cartridge_Destroy(Cartridge self);

colorData *cartridge_GetColors(Cartridge self, int *scriptLen);

TextureData *cartridge_GetSprites(Cartridge self, 
    int spriteWidth, int spriteHeight, GetColorRef getColorRef, int *spritesLen);

const char *cartridge_GetScript(Cartridge self, int *scriptLen);

#endif

