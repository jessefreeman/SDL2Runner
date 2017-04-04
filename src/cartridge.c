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

colorData *cartridge_GetColors(Cartridge self, int *colorsLen)
{
    assert(self);
    *colorsLen = 0;
    if (self->getScript == NULL) return NULL;
    return self->getColors(self, colorsLen);
}

TextureData *cartridge_GetSprites(Cartridge self, 
    int spriteWidth, int spriteHeight, GetColorRef getColorRef, int *spritesLen)
{
    assert(self);
    *spritesLen = 0;
    if (self->getScript == NULL) return NULL;
    TextureData *result = self->getSprites(self, spriteWidth, spriteHeight, getColorRef, spritesLen);
    return result;
}

const char *cartridge_GetScript(Cartridge self, int *scriptLen)
{
    assert(self);
    *scriptLen = 0;
    if (self->getScript == NULL) return NULL;
    return self->getScript(self, scriptLen);
}
