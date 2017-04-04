// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#include <assert.h>
#include <stdlib.h>
#include "util.h"
#include "chip.h"
#include "cartridge.h"
#include "cartridgechip.h"

typedef struct cartridgeChip {
    chip base; // must be first
    Cartridge cartridge;
} cartridgeChip;

static void cartridgeChip_Destroy(CartridgeChip self);

CartridgeChip cartridgeChip_Create()
{
    CartridgeChip self = NULL;

    self = (CartridgeChip)calloc(1, sizeof(cartridgeChip));
    if (self == NULL)
        return NULL;

    strncpy(self->base.name, nameof(CartridgeChip), sizeof(self->base.name) - 1);
    self->base.destroy = cartridgeChip_Destroy;

    return self;
}

static void cartridgeChip_Destroy(CartridgeChip self)
{
    assert(self);
    memset(self, 0, sizeof(cartridgeChip));
    free(self);
}

void cartridgeChip_InsertCartridge(CartridgeChip self, Cartridge cartridge)
{
    assert(self);
    assert(cartridge);
    self->cartridge = cartridge;
}

colorData *cartridgeChip_GetColors(CartridgeChip self, int *colorsLen)
{
    assert(self);
    assert(colorsLen);
    return cartridge_GetColors(self->cartridge, colorsLen);
}

TextureData *cartridgeChip_GetSprites(CartridgeChip self,
    int spriteWidth, int spriteHeight, GetColorRef getColorRef, int *spritesLen)
{
    assert(self);
    assert(spritesLen);
    return cartridge_GetSprites(self->cartridge,
        spriteWidth, spriteHeight, getColorRef, spritesLen);
}

const char *cartridgeChip_GetScript(CartridgeChip self, int *scriptLen)
{
    assert(self);
    *scriptLen = 0;
    if (self->cartridge == NULL)
        return NULL;

    return cartridge_GetScript(self->cartridge, scriptLen);
}