// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#ifndef _displaydevice_h_
#define _displaydevice_h_

#include "types.h"

typedef struct displayDevice {
    void(*destroy)(Obj);
    void(*init)(Obj);
    void(*render)(Obj,int,colorData*);
} displayDevice;

typedef struct displayDevice *DisplayDevice;

void displayDevice_Destroy(DisplayDevice self);

void displayDevice_Init(DisplayDevice self);

void displayDevice_Render(DisplayDevice self, int pixelsLen, colorData pixels[]);

#endif
