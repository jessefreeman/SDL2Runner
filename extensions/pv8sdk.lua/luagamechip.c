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
#include "luagamechip.h"

typedef struct luaGameChip {
    chip base; // must be first
    ApiBridge api;
    int scriptLen;
    char *script;
    lua_State *L;
} luaGameChip;

// funky singleton because I don't get passing back closures in the lua api
// TODO: fix this crap fest
static LuaGameChip instance = NULL;

static void luaGameChip_Destroy(LuaGameChip self);
static void luaGameChip_Init(LuaGameChip self, GetChip getChip);
static void luaGameChip_Dispose(LuaGameChip self);
static void luaGameChip_Update(LuaGameChip self, float timeDelta);
static void luaGameChip_Draw(LuaGameChip self);
static void luaGameChip_LoadApiBridge(LuaGameChip self);

static void changeBackgroundColor(lua_State* L);
static void rebuildScreenBuffer(lua_State *L);
static void drawFontToBuffer(lua_State *L);
static void drawTileText(lua_State *L);
static void drawSprite(lua_State *L);
static void drawSprites(lua_State *L);
static void drawScreenBuffer(lua_State *L);
static void drawFont(lua_State *L);
static void drawSpriteText(lua_State *L);

static const luaL_Reg _meta[] = {
    { NULL, NULL }
};

static const luaL_Reg _methods[] = {
    { "ChangeBackgroundColor", changeBackgroundColor },
    { "RebuildScreenBuffer", rebuildScreenBuffer },
    { "DrawFontToBuffer", drawFontToBuffer },
    { "DrawTileText", drawTileText },
    { "DrawSprite", drawSprite },
    { "DrawSprites", drawSprites },
    { "DrawScreenBuffer", drawScreenBuffer },
    { "DrawFont", drawFont },
    { "DrawSpriteText", drawSpriteText },
    { NULL, NULL }
};


LuaGameChip luaGameChip_Create(const char *script, int len)
{
    // TODO: fix this crap fest
    assert(instance == NULL);

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


    instance = self;
    return self;
}

static void luaGameChip_Destroy(LuaGameChip self)
{
    assert(self);
    assert(self == instance);
    luaGameChip_Dispose(self);
    memset(self, 0, sizeof(luaGameChip));
    free(self);
    instance = NULL;
}

static void luaGameChip_Init(LuaGameChip self, GetChip getChip)
{
    assert(self);
    assert(getChip);
    luaGameChip_Dispose(self);
    self->api = apiBridge_Create(getChip);

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
    if (self->api != NULL)
    {
        apiBridge_Destroy(self->api);
        self->api;
    }
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

    /* newclass = {} */
    lua_createtable(self->L, 0, 0);
    int libId = lua_gettop(self->L);

    /* metatable = {} */
    luaL_newmetatable(self->L, "ApiBridge");
    int metaId = lua_gettop(self->L);
    luaL_setfuncs(self->L, _meta, 0);

    /* metatable.__index = _methods */
    luaL_newlib(self->L, _methods);
    lua_setfield(self->L, metaId, "__index");

    /* metatable.__metatable = _meta */
    luaL_newlib(self->L, _meta);
    lua_setfield(self->L, metaId, "__metatable");

    /* class.__metatable = metatable */
    lua_setmetatable(self->L, libId);

    /* _G["Foo"] = newclass */
    lua_setglobal(self->L, "apiBridge");
}

// TODO: look into automating generating bindings here
// note to self, maybe use lua_State pointer value and lua registry to store luagamechip instance?

static void changeBackgroundColor(lua_State* L)
{
    assert(instance);
    int colorId = lua_tointeger(L, -1);
    apiBridge_ChangeBackgroundColor(instance->api, colorId);
}

// Deprecated
static void rebuildScreenBuffer(lua_State *L)
{
}

// Deprecated
static void drawFontToBuffer(lua_State *L)
{
}

static void drawTileText(lua_State *L)
{
    const char *text = lua_tostring(L, -5);
    int column = lua_tointeger(L, -4);
    int row = lua_tointeger(L, -3);
    const char *fontName = lua_tostring(L, -2);
    int colorOffset = lua_tointeger(L, -1);
    apiBridge_DrawTileText(instance->api, text, column, row, fontName, colorOffset);
}

static void drawSprite(lua_State *L)
{
    assert(instance);
    spriteId id = (spriteId)lua_tointeger(L, -7);
    int x = lua_tointeger(L, -6);
    int y = lua_tointeger(L, -5);
    bool flipH = lua_toboolean(L, -4);
    bool flipV = lua_toboolean(L, -3);
    bool aboveBG = lua_toboolean(L, -2);
    int colorOffset = lua_tointeger(L, -1);
    apiBridge_DrawSprite(instance->api, id, x, y, flipH, flipV, aboveBG, colorOffset);
}

static void drawSprites(lua_State *L)
{
    int len = lua_rawlen(L, -8);
    // TODO: remove alloc
    spriteId *ids = (spriteId *)calloc(len, sizeof(spriteId));
    lua_pushnil(L);
    int i = 0;
    while (lua_next(L, -9) != 0)
    {
        ids[i] = (spriteId)lua_tointeger(L, -1);
        lua_pop(L, 1);
        i++;
    }
    int x = lua_tointeger(L, -7);
    int y = lua_tointeger(L, -6);
    int width = lua_tointeger(L, -5);
    bool flipH = lua_toboolean(L, -4);
    bool flipV = lua_toboolean(L, -3);
    bool aboveBG = lua_toboolean(L, -2);
    int colorOffset = lua_tointeger(L, -1);
    apiBridge_DrawSprites(instance->api, ids, len, x, y, width, flipH, flipV, aboveBG, colorOffset);
    free(ids);
}

static void drawScreenBuffer(lua_State *L)
{
    // TODO: howabout these optional args? derp!
    apiBridge_DrawScreenBuffer(instance->api,
        0, 0, -1, -1, 0, 0);
}

// Deprecated
static void drawFont(lua_State *L)
{
    const char *text = lua_tostring(L, -5);
    int x = lua_tointeger(L, -4);
    int y = lua_tointeger(L, -3);
    const char *fontName = lua_tostring(L, -2);
    int letterSpacing = lua_tointeger(L, -1);
    // missing offset?
    apiBridge_DrawFont(instance->api, text, x, y, fontName, letterSpacing, 0);
}

static void drawSpriteText(lua_State *L)
{
    const char *text = lua_tostring(L, -5);
    int x = lua_tointeger(L, -4);
    int y = lua_tointeger(L, -3);
    const char *fontName = lua_tostring(L, -2);
    int colorOffset = lua_tointeger(L, -1);
    apiBridge_DrawSpriteText(instance->api, text, x, y, fontName, colorOffset);
}
