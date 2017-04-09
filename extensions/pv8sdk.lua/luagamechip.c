// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#include <assert.h>
#include <stdlib.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include "pvconf.h"
#include "types.h"
#include "util.h"
#include "chip.h"
#include "apibridge.h"
#include "luaapibridge.h"
#include "luagamechip.h"

typedef struct luaGameChip {
    chip base; // must be first
    apiBridge api;
    int scriptLen;
    char *script;
    lua_State *L;
} luaGameChip;

static void luaGameChip_Destroy(LuaGameChip self);
static void luaGameChip_Init(LuaGameChip self, GetChip getChip);
static void luaGameChip_Dispose(LuaGameChip self);
static void luaGameChip_Update(LuaGameChip self, float timeDelta);
static void luaGameChip_Draw(LuaGameChip self);
static void luaGameChip_LoadApiBridge(LuaGameChip self);

LuaGameChip luaGameChip_Create(const char *script, int len)
{
    LuaGameChip self = NULL;

    self = (LuaGameChip)calloc(1, sizeof(luaGameChip));
    if (self == NULL) return NULL;

    strncpy(self->base.name, nameof(LuaGameChip), sizeof(self->base.name) - 1);
    self->base.destroy = luaGameChip_Destroy;
    self->base.init = luaGameChip_Init;
    self->base.update = luaGameChip_Update;
    self->base.draw = luaGameChip_Draw;
    self->script = (char *)calloc(len, sizeof(char));
    if (self->script == NULL)
    {
        free(self);
        return NULL;
    }
    memcpy(self->script, script, len * sizeof(char));
    self->scriptLen = len;

    return self;
}

static void luaGameChip_Destroy(LuaGameChip self)
{
    assert(self);
    luaGameChip_Dispose(self);
    memset(self, 0, sizeof(luaGameChip));
    free(self);
}

static void luaGameChip_Init(LuaGameChip self, GetChip getChip)
{
    assert(self);
    assert(getChip);
    luaGameChip_Dispose(self);
    
    apiBridge_Init(&self->api, getChip);
    setApi(&self->api);
    
    self->L = luaL_newstate();
    luaL_openlibs(self->L);
    luaGameChip_LoadApiBridge(self);

    luaL_loadstring(self->L, self->script);
    lua_pcall(self->L, 0, LUA_MULTRET, 0);

    // call init in lua script
    lua_getglobal(self->L, "Init");
    lua_pcall(self->L, 0, 0, 0);
}

static void luaGameChip_Dispose(LuaGameChip self)
{
    assert(self);
    setApi(NULL);
    if (self->L != NULL)
    {
        lua_close(self->L);
        self->L = NULL;
    }
}

static void luaGameChip_Update(LuaGameChip self, float timeDelta)
{
    assert(self);
    lua_getglobal(self->L, "Update");
    lua_pushnumber(self->L, timeDelta);
    lua_pcall(self->L, 1, 0, 0);
}

static void luaGameChip_Draw(LuaGameChip self)
{
    assert(self);
    lua_getglobal(self->L, "Draw");
    lua_pcall(self->L, 0, 0, 0);
}

static void luaGameChip_LoadApiBridge(LuaGameChip self)
{
    assert(self);
    loadApiBridge(self->L);
}
