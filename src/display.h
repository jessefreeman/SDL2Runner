// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#ifndef _display_h_
#define _display_h_

#include "types.h"

typedef struct display {
    void(*destroy)(Obj);
    void(*init)(Obj);
} display;

typedef struct display *Display;

void display_Destroy(Display self);

void display_Init(Display self);

#endif
