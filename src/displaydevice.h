// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#ifndef _displaydevice_h_
#define _displaydevice_h_

#include "types.h"
#include "device.h"
#include "displaychip.h"

typedef struct displayDevice {
    device base; // must be first
    void(*refresh)(Obj,DisplayChip);
} displayDevice;

typedef struct displayDevice *DisplayDevice;

void displayDevice_Refresh(DisplayDevice self, DisplayChip displayChip);

#endif
