// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#include <assert.h>
#include <stdlib.h>
#include "displaydevice.h"

void displayDevice_Refresh(DisplayDevice self, DisplayChip displayChip)
{
    assert(self);
    if (self->refresh == NULL) return;
    self->refresh(self, displayChip);
}