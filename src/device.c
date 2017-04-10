// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#include <assert.h>
#include <stdlib.h>
#include "device.h"

void device_Destroy(Device self)
{
    assert(self);
    if (self->destroy == NULL) return;
    self->destroy(self);
}

void device_PowerOn(Device self)
{
    assert(self);
    if (self->powerOn == NULL) return;
    self->powerOn(self);
}

void device_PowerOff(Device self)
{
    assert(self);
    if (self->powerOff == NULL) return;
    self->powerOff(self);
}