// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#include <assert.h>
#include <stdlib.h>
#include "chip.h"

const char *chip_GetName(Chip self)
{
    assert(self);
    return self->name;
}

void chip_Destroy(Chip self)
{
    assert(self);
    if (self->destroy == NULL) return;
    self->destroy(self);
}

void chip_Init(Chip self, GetChip getChip)
{
    assert(self);
    if (self->init == NULL) return;
    self->init(self, getChip);
}

void chip_Update(Chip self, float timeDelta)
{
    assert(self);
    if (self->update == NULL) return;
    self->update(self, timeDelta);
}

void chip_Draw(Chip self)
{
    assert(self);
    if (self->draw == NULL) return;
    self->draw(self);
}