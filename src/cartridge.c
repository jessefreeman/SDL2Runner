// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#include <assert.h>
#include "cartridge.h"

void cartridge_Destroy(Cartridge self)
{
    assert(self);
    if (self->destroy == NULL) return;
    self->destroy(self);
}

const char *cartridge_GetScript(Cartridge self, int *scriptLen)
{
    assert(self);
    *scriptLen = 0;
    if (self->getScript == NULL) return NULL;
    return self->getScript(self, scriptLen);
}
