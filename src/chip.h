// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#ifndef _chip_h_
#define _chip_h_

#include "types.h"
#include "func.h"

typedef Func1(const char *, Obj) GetChip;

typedef struct chip {
    char name[64];
    void(*destroy)(Obj);
    void(*init)(Obj, GetChip);
    void(*update)(Obj, float);
    void(*draw)(Obj);
} chip;

typedef struct chip *Chip;

const char *chip_GetName(Chip self);

void chip_Destroy(Chip self);

void chip_Init(Chip self,  GetChip getService);

void chip_Update(Chip self, float timeDelta);

void chip_Draw(Chip self);

#endif

