// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#include <assert.h>
#include <stdlib.h>
#include "util.h"
#include "chip.h"
#include "cartridgechip.h"

typedef struct cartridgeChip {
    chip base; // must be first
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

colorData *cartridgeChip_ReadColors(CartridgeChip self, int *colorsLen)
{
    assert(self);
    assert(colorsLen);
    *colorsLen = 0;
    return NULL;
}