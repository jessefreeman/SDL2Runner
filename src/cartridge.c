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

colorData *cartridge_GetColorData(Cartridge self, int *len)
{
    assert(self);
    assert(len);
    *len = 0;
    if (self->getColorsData == NULL) return NULL;
    return self->getColorsData(self, len);
}

colorData *cartridge_GetSpritesData(Cartridge self, int *width, int *height)
{
    assert(self);
    assert(width);
    assert(height);
    *width = 0;
    *height = 0;
    if (self->getSpritesData == NULL) return NULL;
    return self->getSpritesData(self, width, height);
}

char *cartridge_GetScriptData(Cartridge self, int *len)
{
    assert(self);
    *len = 0;
    if (self->getScriptData == NULL) return NULL;
    return self->getScriptData(self, len);
}
