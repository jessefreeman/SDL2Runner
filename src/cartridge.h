// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#ifndef _cartridge_h_
#define _cartridge_h_

#include "types.h"

typedef struct cartridge {
    void(*action)(Obj);
} cartridge;

typedef struct cartridge *Cartridge;

#endif

