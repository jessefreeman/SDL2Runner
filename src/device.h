// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#ifndef _device_h_
#define _device_h_

#include "types.h"

typedef struct device {
    void(*destroy)(Obj);
    void(*powerOn)(Obj);
    void(*powerOff)(Obj);
} device;

typedef struct device *Device;

void device_Destroy(Device self);

void device_PowerOn(Device self);

void device_PowerOff(Device self);

#endif
