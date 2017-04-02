// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#include <assert.h>
#include <stdlib.h>
#include "util.h"
#include "chip.h"
#include "spritechip.h"

typedef struct spriteChip {
    chip base; // must be first
    int pass;
} spriteChip;

static void spriteChip_Destroy(SpriteChip self);

SpriteChip spriteChip_Create()
{
    SpriteChip self = NULL;

    self = (SpriteChip)calloc(1, sizeof(spriteChip));
    if (self == NULL)
        return NULL;

    strncpy(self->base.name, nameof(SpriteChip), sizeof(self->base.name) - 1);
    self->base.destroy = spriteChip_Destroy;

    return self;
}

static void spriteChip_Destroy(SpriteChip self)
{
    assert(self);
    memset(self, 0, sizeof(spriteChip));
    free(self);
}
