// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#ifndef _cartridge_h_
#define _cartridge_h_

#include "types.h"
#include "cartridge.h"

typedef struct cartridge {
    void(*destroy)(Obj);
    const char *(*getScript)(Obj, int *);
} cartridge;

typedef struct cartridge *Cartridge;

void cartridge_Destroy(Cartridge self);

const char *cartridge_GetScript(Cartridge self, int *scriptLen);

#endif

