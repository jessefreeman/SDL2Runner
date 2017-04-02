// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#include <assert.h>
#include <stdlib.h>
#include "display.h"

void display_Destroy(Display self)
{
    assert(self);
    if (self->destroy == NULL) return;
    self->destroy(self);
}

void display_Init(Display self)
{
    assert(self);
    if (self->init == NULL) return;
    self->init(self);
}