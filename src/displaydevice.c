// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#include <assert.h>
#include <stdlib.h>
#include "displaydevice.h"

void displayDevice_Destroy(DisplayDevice self)
{
    assert(self);
    if (self->destroy == NULL) return;
    self->destroy(self);
}

void displayDevice_Init(DisplayDevice self)
{
    assert(self);
    if (self->init == NULL) return;
    self->init(self);
}

void displayDevice_Render(DisplayDevice self, int pixelsLen, colorData pixels[])
{
    assert(self);
    if (self->render == NULL) return;
    self->render(self, pixelsLen, pixels);
}