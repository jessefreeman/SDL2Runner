// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#include <assert.h>
#include <stdlib.h>
#include "util.h"
#include "chip.h"
#include "luagamechip.h"

typedef struct luaGameChip {
    chip base;
} luaGameChip;

static void luaGameChip_Destroy(LuaGameChip self);

LuaGameChip luaGameChip_Create()
{
    LuaGameChip self = NULL;

    self = (LuaGameChip)calloc(1, sizeof(luaGameChip));
    if (self == NULL) return NULL;

    strncpy(self->base.name, nameof(LuaGameChip), sizeof(self->base.name) - 1);
    self->base.destroy = luaGameChip_Destroy;

    return self;
}

static void luaGameChip_Destroy(LuaGameChip self)
{
    assert(self);
    memset(self, 0, sizeof(luaGameChip));
    free(self);
}
