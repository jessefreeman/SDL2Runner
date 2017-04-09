// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#include <assert.h>
#include <stdlib.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include "types.h"
#include "apibridge.h"
#include "luaapibridge.h"

#define ARRAY_BUFFER_LEN 1024

static ApiBridge _api = NULL;

static int getSpriteWidth(lua_State *L);
static int getSpriteHeight(lua_State *L);
static int getDisplayWidth(lua_State *L);
static int getDisplayWrap(lua_State *L);
static int toggleDisplayWrap(lua_State *L);
static int getDisplayHeight(lua_State *L);
static int getScrollX(lua_State *L);
static int getScrollY(lua_State *L);
static int scrollTo(lua_State *L);
static int getPaused(lua_State *L);
static int togglePause(lua_State *L);
static int getMouseX(lua_State *L);
static int getMouseY(lua_State *L);
static int getInputString(lua_State *L);
static int getMousePosition(lua_State *L);
static int getBackgroundColor(lua_State *L);
static int drawSprite(lua_State *L);
static int drawSprites(lua_State *L);
static int updateTile(lua_State *L);
static int clear(lua_State *L);
static int clearArea(lua_State *L);
static int changeBackgroundColor(lua_State *L);
static int playSound(lua_State *L);
static int buttonDown(lua_State *L);
static int buttonReleased(lua_State *L);
static int getMouseButtonDown(lua_State *L);
static int getMouseButtonUp(lua_State *L);
static int getMouseButton(lua_State *L);
static int drawSpriteText(lua_State *L);
static int drawTileText(lua_State *L);
static int drawTileTextBox(lua_State *L);
static int calculateTextBoxHeight(lua_State *L);
static int drawTile(lua_State *L);
static int drawTiles(lua_State *L);
static int drawFont(lua_State *L);
static int drawTextBox(lua_State *L);
static int formatWordWrap(lua_State *L);
static int drawPixelData(lua_State *L);
static int readFlagAt(lua_State *L);
static int getKey(lua_State *L);
static int getKeyDown(lua_State *L);
static int getKeyUp(lua_State *L);
static int spritesToRawData(lua_State *L);
static int saveData(lua_State *L);
static int readData(lua_State *L);
static int loadSong(lua_State *L);
static int playSong(lua_State *L);
static int pauseSong(lua_State *L);
static int stopSong(lua_State *L);
static int rewindSong(lua_State *L);
static int replaceColorID(lua_State *L);
static int replaceColorIDs(lua_State *L);
static int drawScreenBuffer(lua_State *L);
static int drawTilemap(lua_State *L);
static int drawFontTiles(lua_State *L);
static int rebuildScreenBuffer(lua_State *L);
static int drawTileToBuffer(lua_State *L);
static int drawTilesToBuffer(lua_State *L);
static int drawTextBoxToBuffer(lua_State *L);
static int drawBufferData(lua_State *L);
static int drawFontToBuffer(lua_State *L);

static const luaL_Reg meta[] = {
    { NULL, NULL }
};

static const luaL_Reg methods[] = {
    { "GetSpriteWidth", getSpriteWidth },
    { "GetSpriteHeight", getSpriteHeight },
    { "GetDisplayWidth", getDisplayWidth },
    { "GetDisplayWrap", getDisplayWrap },
    { "ToggleDisplayWrap", toggleDisplayWrap },
    { "GetDisplayHeight", getDisplayHeight },
    { "GetScrollX", getScrollX },
    { "GetScrollY", getScrollY },
    { "ScrollTo", scrollTo },
    { "GetPaused", getPaused },
    { "TogglePause", togglePause },
    { "GetMouseX", getMouseX },
    { "GetMouseY", getMouseY },
    { "GetInputString", getInputString },
    { "GetMousePosition", getMousePosition },
    { "GetBackgroundColor", getBackgroundColor },
    { "DrawSprite", drawSprite },
    { "DrawSprites", drawSprites },
    { "UpdateTile", updateTile },
    { "Clear", clear },
    { "ClearArea", clearArea },
    { "ChangeBackgroundColor", changeBackgroundColor },
    { "PlaySound", playSound },
    { "ButtonDown", buttonDown },
    { "ButtonReleased", buttonReleased },
    { "GetMouseButtonDown", getMouseButtonDown },
    { "GetMouseButtonUp", getMouseButtonUp },
    { "GetMouseButton", getMouseButton },
    { "DrawSpriteText", drawSpriteText },
    { "DrawTileText", drawTileText },
    { "DrawTileTextBox", drawTileTextBox },
    { "CalculateTextBoxHeight", calculateTextBoxHeight },
    { "DrawTile", drawTile },
    { "DrawTiles", drawTiles },
    { "DrawFont", drawFont },
    { "DrawTextBox", drawTextBox },
    { "FormatWordWrap", formatWordWrap },
    { "DrawPixelData", drawPixelData },
    { "ReadFlagAt", readFlagAt },
    { "GetKey", getKey },
    { "GetKeyDown", getKeyDown },
    { "GetKeyUp", getKeyUp },
    { "SpritesToRawData", spritesToRawData },
    { "SaveData", saveData },
    { "ReadData", readData },
    { "LoadSong", loadSong },
    { "PlaySong", playSong },
    { "PauseSong", pauseSong },
    { "StopSong", stopSong },
    { "RewindSong", rewindSong },
    { "ReplaceColorID", replaceColorID },
    { "ReplaceColorIDs", replaceColorIDs },
    { "DrawScreenBuffer", drawScreenBuffer },
    { "DrawTilemap", drawTilemap },
    { "DrawFontTiles", drawFontTiles },
    { "RebuildScreenBuffer", rebuildScreenBuffer },
    { "DrawTileToBuffer", drawTileToBuffer },
    { "DrawTilesToBuffer", drawTilesToBuffer },
    { "DrawTextBoxToBuffer", drawTextBoxToBuffer },
    { "DrawBufferData", drawBufferData },
    { "DrawFontToBuffer", drawFontToBuffer },
    { NULL, NULL }
};

void loadApiBridge(lua_State *L)
{
    /* newclass = {} */
    lua_createtable(L, 0, 0);
    int libId = lua_gettop(L);

    /* metatable = {} */
    luaL_newmetatable(L, "ApiBridge");
    int metaId = lua_gettop(L);
    luaL_setfuncs(L, meta, 0);

    /* metatable.__index = _methods */
    luaL_newlib(L, methods);
    lua_setfield(L, metaId, "__index");

    /* metatable.__metatable = _meta */
    luaL_newlib(L, meta);
    lua_setfield(L, metaId, "__metatable");

    /* class.__metatable = metatable */
    lua_setmetatable(L, libId);

    /* _G["Foo"] = newclass */
    lua_setglobal(L, "apiBridge");
}

void setApi(ApiBridge api)
{
    _api = api;
}

static ApiBridge getApi(lua_State *L)
{
    assert(_api);
    return _api;
}

static int getSpriteWidth(lua_State *L)
{
    assert(L);
    int argc = lua_gettop(L);
    if (argc != 1)
    {
        lua_pushstring(L, "invalid number of arguments");
        lua_error(L);
    }
    
    // invoke api call
    ApiBridge api = getApi(L);
    int returnValue = apiBridge_GetSpriteWidth(api);
    
    lua_pushnumber(L, returnValue);

    return 1;
}

static int getSpriteHeight(lua_State *L)
{
    assert(L);
    int argc = lua_gettop(L);
    if (argc != 1)
    {
        lua_pushstring(L, "invalid number of arguments");
        lua_error(L);
    }
    
    // invoke api call
    ApiBridge api = getApi(L);
    int returnValue = apiBridge_GetSpriteHeight(api);
    
    lua_pushnumber(L, returnValue);

    return 1;
}

static int getDisplayWidth(lua_State *L)
{
    assert(L);
    int argc = lua_gettop(L);
    if (argc != 1)
    {
        lua_pushstring(L, "invalid number of arguments");
        lua_error(L);
    }
    
    // invoke api call
    ApiBridge api = getApi(L);
    int returnValue = apiBridge_GetDisplayWidth(api);
    
    lua_pushnumber(L, returnValue);

    return 1;
}

static int getDisplayWrap(lua_State *L)
{
    assert(L);
    int argc = lua_gettop(L);
    if (argc != 1)
    {
        lua_pushstring(L, "invalid number of arguments");
        lua_error(L);
    }
    
    // invoke api call
    ApiBridge api = getApi(L);
    bool returnValue = apiBridge_GetDisplayWrap(api);
    
    lua_pushboolean(L, returnValue);

    return 1;
}

static int toggleDisplayWrap(lua_State *L)
{
    assert(L);
    int argc = lua_gettop(L);
    if (argc != 2)
    {
        lua_pushstring(L, "invalid number of arguments");
        lua_error(L);
    }

    // required parameters
    bool value = lua_toboolean(L, 2);
    
    // invoke api call
    ApiBridge api = getApi(L);
    apiBridge_ToggleDisplayWrap(api,
       value);
    
    return 0;
}

static int getDisplayHeight(lua_State *L)
{
    assert(L);
    int argc = lua_gettop(L);
    if (argc != 1)
    {
        lua_pushstring(L, "invalid number of arguments");
        lua_error(L);
    }
    
    // invoke api call
    ApiBridge api = getApi(L);
    int returnValue = apiBridge_GetDisplayHeight(api);
    
    lua_pushnumber(L, returnValue);

    return 1;
}

static int getScrollX(lua_State *L)
{
    assert(L);
    int argc = lua_gettop(L);
    if (argc != 1)
    {
        lua_pushstring(L, "invalid number of arguments");
        lua_error(L);
    }
    
    // invoke api call
    ApiBridge api = getApi(L);
    int returnValue = apiBridge_GetScrollX(api);
    
    lua_pushnumber(L, returnValue);

    return 1;
}

static int getScrollY(lua_State *L)
{
    assert(L);
    int argc = lua_gettop(L);
    if (argc != 1)
    {
        lua_pushstring(L, "invalid number of arguments");
        lua_error(L);
    }
    
    // invoke api call
    ApiBridge api = getApi(L);
    int returnValue = apiBridge_GetScrollY(api);
    
    lua_pushnumber(L, returnValue);

    return 1;
}

static int scrollTo(lua_State *L)
{
    assert(L);
    int argc = lua_gettop(L);
    if (argc != 3)
    {
        lua_pushstring(L, "invalid number of arguments");
        lua_error(L);
    }

    // required parameters
    int x = luaL_checkinteger(L, 2);
    int y = luaL_checkinteger(L, 3);
    
    // invoke api call
    ApiBridge api = getApi(L);
    apiBridge_ScrollTo(api,
       x,
       y);
    
    return 0;
}

static int getPaused(lua_State *L)
{
    assert(L);
    int argc = lua_gettop(L);
    if (argc != 1)
    {
        lua_pushstring(L, "invalid number of arguments");
        lua_error(L);
    }
    
    // invoke api call
    ApiBridge api = getApi(L);
    bool returnValue = apiBridge_GetPaused(api);
    
    lua_pushboolean(L, returnValue);

    return 1;
}

static int togglePause(lua_State *L)
{
    assert(L);
    int argc = lua_gettop(L);
    if (argc != 2)
    {
        lua_pushstring(L, "invalid number of arguments");
        lua_error(L);
    }

    // required parameters
    bool value = lua_toboolean(L, 2);
    
    // invoke api call
    ApiBridge api = getApi(L);
    apiBridge_TogglePause(api,
       value);
    
    return 0;
}

static int getMouseX(lua_State *L)
{
    assert(L);
    int argc = lua_gettop(L);
    if (argc != 1)
    {
        lua_pushstring(L, "invalid number of arguments");
        lua_error(L);
    }
    
    // invoke api call
    ApiBridge api = getApi(L);
    int returnValue = apiBridge_GetMouseX(api);
    
    lua_pushnumber(L, returnValue);

    return 1;
}

static int getMouseY(lua_State *L)
{
    assert(L);
    int argc = lua_gettop(L);
    if (argc != 1)
    {
        lua_pushstring(L, "invalid number of arguments");
        lua_error(L);
    }
    
    // invoke api call
    ApiBridge api = getApi(L);
    int returnValue = apiBridge_GetMouseY(api);
    
    lua_pushnumber(L, returnValue);

    return 1;
}

static int getInputString(lua_State *L)
{
    assert(L);
    int argc = lua_gettop(L);
    if (argc != 1)
    {
        lua_pushstring(L, "invalid number of arguments");
        lua_error(L);
    }
    
    int returnLen = 0;
    // invoke api call
    ApiBridge api = getApi(L);
    char *returnValue = apiBridge_GetInputString(api,
       &returnLen);
    
    lua_pushstring(L, returnValue);

    return 1;
}

static int getMousePosition(lua_State *L)
{
    assert(L);
    int argc = lua_gettop(L);
    if (argc != 1)
    {
        lua_pushstring(L, "invalid number of arguments");
        lua_error(L);
    }
    
    // invoke api call
    ApiBridge api = getApi(L);
    vector returnValue = apiBridge_GetMousePosition(api);
    
    assert(false);
    //TODO: lua_pushsomething(L, returnValue);

    return 1;
}

static int getBackgroundColor(lua_State *L)
{
    assert(L);
    int argc = lua_gettop(L);
    if (argc != 1)
    {
        lua_pushstring(L, "invalid number of arguments");
        lua_error(L);
    }
    
    // invoke api call
    ApiBridge api = getApi(L);
    int returnValue = apiBridge_GetBackgroundColor(api);
    
    lua_pushnumber(L, returnValue);

    return 1;
}

static int drawSprite(lua_State *L)
{
    assert(L);
    int argc = lua_gettop(L);
    if (argc < 4 || argc > 8)
    {
        lua_pushstring(L, "invalid number of arguments");
        lua_error(L);
    }

    // required parameters
    int id = luaL_checkinteger(L, 2);
    int x = luaL_checkinteger(L, 3);
    int y = luaL_checkinteger(L, 4);

    // optional parameters
    bool *flipH = NULL;
    bool flipHValue;
    if (argc >= 5)
    {
        flipHValue = lua_toboolean(L, 5);
        flipH = &flipHValue;
    }

    bool *flipV = NULL;
    bool flipVValue;
    if (argc >= 6)
    {
        flipVValue = lua_toboolean(L, 6);
        flipV = &flipVValue;
    }

    bool *aboveBG = NULL;
    bool aboveBGValue;
    if (argc >= 7)
    {
        aboveBGValue = lua_toboolean(L, 7);
        aboveBG = &aboveBGValue;
    }

    int *colorOffset = NULL;
    int colorOffsetValue;
    if (argc >= 8)
    {
        colorOffsetValue = luaL_checkinteger(L, 8);
        colorOffset = &colorOffsetValue;
    }

    
    // invoke api call
    ApiBridge api = getApi(L);
    apiBridge_DrawSprite(api,
       id,
       x,
       y,
       flipH,
       flipV,
       aboveBG,
       colorOffset);
    
    return 0;
}

static int drawSprites(lua_State *L)
{
    assert(L);
    int argc = lua_gettop(L);
    if (argc < 5 || argc > 9)
    {
        lua_pushstring(L, "invalid number of arguments");
        lua_error(L);
    }

    // required parameters
    luaL_checktype(L, 2, LUA_TTABLE);
    int idsLen = lua_rawlen(L, 2);
    int ids[ARRAY_BUFFER_LEN] = { 0 };
    lua_pushnil(L);
    int idsIdx = 0;
    while (lua_next(L, 2) != 0)
    {
        ids[idsIdx] = luaL_checkinteger(L, -1);
        lua_pop(L, 1);
        idsIdx++;
    }
    int x = luaL_checkinteger(L, 3);
    int y = luaL_checkinteger(L, 4);
    int width = luaL_checkinteger(L, 5);

    // optional parameters
    bool *flipH = NULL;
    bool flipHValue;
    if (argc >= 6)
    {
        flipHValue = lua_toboolean(L, 6);
        flipH = &flipHValue;
    }

    bool *flipV = NULL;
    bool flipVValue;
    if (argc >= 7)
    {
        flipVValue = lua_toboolean(L, 7);
        flipV = &flipVValue;
    }

    bool *aboveBG = NULL;
    bool aboveBGValue;
    if (argc >= 8)
    {
        aboveBGValue = lua_toboolean(L, 8);
        aboveBG = &aboveBGValue;
    }

    int *colorOffset = NULL;
    int colorOffsetValue;
    if (argc >= 9)
    {
        colorOffsetValue = luaL_checkinteger(L, 9);
        colorOffset = &colorOffsetValue;
    }

    
    // invoke api call
    ApiBridge api = getApi(L);
    apiBridge_DrawSprites(api,
       ids,
       idsLen,
       x,
       y,
       width,
       flipH,
       flipV,
       aboveBG,
       colorOffset);
    
    return 0;
}

static int updateTile(lua_State *L)
{
    assert(L);
    int argc = lua_gettop(L);
    if (argc < 4 || argc > 6)
    {
        lua_pushstring(L, "invalid number of arguments");
        lua_error(L);
    }

    // required parameters
    int spriteID = luaL_checkinteger(L, 2);
    int column = luaL_checkinteger(L, 3);
    int row = luaL_checkinteger(L, 4);

    // optional parameters
    int *flag = NULL;
    int flagValue;
    if (argc >= 5)
    {
        flagValue = luaL_checkinteger(L, 5);
        flag = &flagValue;
    }

    int *colorOffset = NULL;
    int colorOffsetValue;
    if (argc >= 6)
    {
        colorOffsetValue = luaL_checkinteger(L, 6);
        colorOffset = &colorOffsetValue;
    }

    
    // invoke api call
    ApiBridge api = getApi(L);
    apiBridge_UpdateTile(api,
       spriteID,
       column,
       row,
       flag,
       colorOffset);
    
    return 0;
}

static int clear(lua_State *L)
{
    assert(L);
    int argc = lua_gettop(L);
    if (argc != 1)
    {
        lua_pushstring(L, "invalid number of arguments");
        lua_error(L);
    }
    
    // invoke api call
    ApiBridge api = getApi(L);
    apiBridge_Clear(api);
    
    return 0;
}

static int clearArea(lua_State *L)
{
    assert(L);
    int argc = lua_gettop(L);
    if (argc < 5 || argc > 6)
    {
        lua_pushstring(L, "invalid number of arguments");
        lua_error(L);
    }

    // required parameters
    int x = luaL_checkinteger(L, 2);
    int y = luaL_checkinteger(L, 3);
    int width = luaL_checkinteger(L, 4);
    int height = luaL_checkinteger(L, 5);

    // optional parameters
    int *color = NULL;
    int colorValue;
    if (argc >= 6)
    {
        colorValue = luaL_checkinteger(L, 6);
        color = &colorValue;
    }

    
    // invoke api call
    ApiBridge api = getApi(L);
    apiBridge_ClearArea(api,
       x,
       y,
       width,
       height,
       color);
    
    return 0;
}

static int changeBackgroundColor(lua_State *L)
{
    assert(L);
    int argc = lua_gettop(L);
    if (argc != 2)
    {
        lua_pushstring(L, "invalid number of arguments");
        lua_error(L);
    }

    // required parameters
    int id = luaL_checkinteger(L, 2);
    
    // invoke api call
    ApiBridge api = getApi(L);
    apiBridge_ChangeBackgroundColor(api,
       id);
    
    return 0;
}

static int playSound(lua_State *L)
{
    assert(L);
    int argc = lua_gettop(L);
    if (argc < 2 || argc > 3)
    {
        lua_pushstring(L, "invalid number of arguments");
        lua_error(L);
    }

    // required parameters
    int id = luaL_checkinteger(L, 2);

    // optional parameters
    int *channel = NULL;
    int channelValue;
    if (argc >= 3)
    {
        channelValue = luaL_checkinteger(L, 3);
        channel = &channelValue;
    }

    
    // invoke api call
    ApiBridge api = getApi(L);
    apiBridge_PlaySound(api,
       id,
       channel);
    
    return 0;
}

static int buttonDown(lua_State *L)
{
    assert(L);
    int argc = lua_gettop(L);
    if (argc < 2 || argc > 3)
    {
        lua_pushstring(L, "invalid number of arguments");
        lua_error(L);
    }

    // required parameters
    int button = luaL_checkinteger(L, 2);

    // optional parameters
    int *player = NULL;
    int playerValue;
    if (argc >= 3)
    {
        playerValue = luaL_checkinteger(L, 3);
        player = &playerValue;
    }

    
    // invoke api call
    ApiBridge api = getApi(L);
    bool returnValue = apiBridge_ButtonDown(api,
       button,
       player);
    
    lua_pushboolean(L, returnValue);

    return 1;
}

static int buttonReleased(lua_State *L)
{
    assert(L);
    int argc = lua_gettop(L);
    if (argc < 2 || argc > 3)
    {
        lua_pushstring(L, "invalid number of arguments");
        lua_error(L);
    }

    // required parameters
    int button = luaL_checkinteger(L, 2);

    // optional parameters
    int *player = NULL;
    int playerValue;
    if (argc >= 3)
    {
        playerValue = luaL_checkinteger(L, 3);
        player = &playerValue;
    }

    
    // invoke api call
    ApiBridge api = getApi(L);
    bool returnValue = apiBridge_ButtonReleased(api,
       button,
       player);
    
    lua_pushboolean(L, returnValue);

    return 1;
}

static int getMouseButtonDown(lua_State *L)
{
    assert(L);
    int argc = lua_gettop(L);
    if (argc < 1 || argc > 2)
    {
        lua_pushstring(L, "invalid number of arguments");
        lua_error(L);
    }

    // optional parameters
    int *id = NULL;
    int idValue;
    if (argc >= 2)
    {
        idValue = luaL_checkinteger(L, 2);
        id = &idValue;
    }

    
    // invoke api call
    ApiBridge api = getApi(L);
    bool returnValue = apiBridge_GetMouseButtonDown(api,
       id);
    
    lua_pushboolean(L, returnValue);

    return 1;
}

static int getMouseButtonUp(lua_State *L)
{
    assert(L);
    int argc = lua_gettop(L);
    if (argc < 1 || argc > 2)
    {
        lua_pushstring(L, "invalid number of arguments");
        lua_error(L);
    }

    // optional parameters
    int *id = NULL;
    int idValue;
    if (argc >= 2)
    {
        idValue = luaL_checkinteger(L, 2);
        id = &idValue;
    }

    
    // invoke api call
    ApiBridge api = getApi(L);
    bool returnValue = apiBridge_GetMouseButtonUp(api,
       id);
    
    lua_pushboolean(L, returnValue);

    return 1;
}

static int getMouseButton(lua_State *L)
{
    assert(L);
    int argc = lua_gettop(L);
    if (argc < 1 || argc > 2)
    {
        lua_pushstring(L, "invalid number of arguments");
        lua_error(L);
    }

    // optional parameters
    int *id = NULL;
    int idValue;
    if (argc >= 2)
    {
        idValue = luaL_checkinteger(L, 2);
        id = &idValue;
    }

    
    // invoke api call
    ApiBridge api = getApi(L);
    bool returnValue = apiBridge_GetMouseButton(api,
       id);
    
    lua_pushboolean(L, returnValue);

    return 1;
}

static int drawSpriteText(lua_State *L)
{
    assert(L);
    int argc = lua_gettop(L);
    if (argc < 4 || argc > 7)
    {
        lua_pushstring(L, "invalid number of arguments");
        lua_error(L);
    }

    // required parameters
    char *text = luaL_checkstring(L, 2);
    int x = luaL_checkinteger(L, 3);
    int y = luaL_checkinteger(L, 4);

    // optional parameters
    char **fontName = NULL;
    char *fontNameValue;
    if (argc >= 5)
    {
        fontNameValue = luaL_checkstring(L, 5);
        fontName = &fontNameValue;
    }

    int *colorOffset = NULL;
    int colorOffsetValue;
    if (argc >= 6)
    {
        colorOffsetValue = luaL_checkinteger(L, 6);
        colorOffset = &colorOffsetValue;
    }

    int *spacing = NULL;
    int spacingValue;
    if (argc >= 7)
    {
        spacingValue = luaL_checkinteger(L, 7);
        spacing = &spacingValue;
    }

    
    // invoke api call
    ApiBridge api = getApi(L);
    apiBridge_DrawSpriteText(api,
       text,
       x,
       y,
       fontName,
       colorOffset,
       spacing);
    
    return 0;
}

static int drawTileText(lua_State *L)
{
    assert(L);
    int argc = lua_gettop(L);
    if (argc < 4 || argc > 6)
    {
        lua_pushstring(L, "invalid number of arguments");
        lua_error(L);
    }

    // required parameters
    char *text = luaL_checkstring(L, 2);
    int column = luaL_checkinteger(L, 3);
    int row = luaL_checkinteger(L, 4);

    // optional parameters
    char **fontName = NULL;
    char *fontNameValue;
    if (argc >= 5)
    {
        fontNameValue = luaL_checkstring(L, 5);
        fontName = &fontNameValue;
    }

    int *colorOffset = NULL;
    int colorOffsetValue;
    if (argc >= 6)
    {
        colorOffsetValue = luaL_checkinteger(L, 6);
        colorOffset = &colorOffsetValue;
    }

    
    // invoke api call
    ApiBridge api = getApi(L);
    apiBridge_DrawTileText(api,
       text,
       column,
       row,
       fontName,
       colorOffset);
    
    return 0;
}

static int drawTileTextBox(lua_State *L)
{
    assert(L);
    int argc = lua_gettop(L);
    if (argc < 5 || argc > 7)
    {
        lua_pushstring(L, "invalid number of arguments");
        lua_error(L);
    }

    // required parameters
    char *text = luaL_checkstring(L, 2);
    int column = luaL_checkinteger(L, 3);
    int row = luaL_checkinteger(L, 4);
    int characterWidth = luaL_checkinteger(L, 5);

    // optional parameters
    char **fontName = NULL;
    char *fontNameValue;
    if (argc >= 6)
    {
        fontNameValue = luaL_checkstring(L, 6);
        fontName = &fontNameValue;
    }

    int *colorOffset = NULL;
    int colorOffsetValue;
    if (argc >= 7)
    {
        colorOffsetValue = luaL_checkinteger(L, 7);
        colorOffset = &colorOffsetValue;
    }

    
    // invoke api call
    ApiBridge api = getApi(L);
    apiBridge_DrawTileTextBox(api,
       text,
       column,
       row,
       characterWidth,
       fontName,
       colorOffset);
    
    return 0;
}

static int calculateTextBoxHeight(lua_State *L)
{
    assert(L);
    int argc = lua_gettop(L);
    if (argc != 3)
    {
        lua_pushstring(L, "invalid number of arguments");
        lua_error(L);
    }

    // required parameters
    char *text = luaL_checkstring(L, 2);
    int characterWidth = luaL_checkinteger(L, 3);
    
    // invoke api call
    ApiBridge api = getApi(L);
    int returnValue = apiBridge_CalculateTextBoxHeight(api,
       text,
       characterWidth);
    
    lua_pushnumber(L, returnValue);

    return 1;
}

static int drawTile(lua_State *L)
{
    assert(L);
    int argc = lua_gettop(L);
    if (argc < 4 || argc > 5)
    {
        lua_pushstring(L, "invalid number of arguments");
        lua_error(L);
    }

    // required parameters
    int tileID = luaL_checkinteger(L, 2);
    int column = luaL_checkinteger(L, 3);
    int row = luaL_checkinteger(L, 4);

    // optional parameters
    int *colorOffset = NULL;
    int colorOffsetValue;
    if (argc >= 5)
    {
        colorOffsetValue = luaL_checkinteger(L, 5);
        colorOffset = &colorOffsetValue;
    }

    
    // invoke api call
    ApiBridge api = getApi(L);
    apiBridge_DrawTile(api,
       tileID,
       column,
       row,
       colorOffset);
    
    return 0;
}

static int drawTiles(lua_State *L)
{
    assert(L);
    int argc = lua_gettop(L);
    if (argc < 5 || argc > 6)
    {
        lua_pushstring(L, "invalid number of arguments");
        lua_error(L);
    }

    // required parameters
    luaL_checktype(L, 2, LUA_TTABLE);
    int idsLen = lua_rawlen(L, 2);
    int ids[ARRAY_BUFFER_LEN] = { 0 };
    lua_pushnil(L);
    int idsIdx = 0;
    while (lua_next(L, 2) != 0)
    {
        ids[idsIdx] = luaL_checkinteger(L, -1);
        lua_pop(L, 1);
        idsIdx++;
    }
    int column = luaL_checkinteger(L, 3);
    int row = luaL_checkinteger(L, 4);
    int columns = luaL_checkinteger(L, 5);

    // optional parameters
    int *colorOffset = NULL;
    int colorOffsetValue;
    if (argc >= 6)
    {
        colorOffsetValue = luaL_checkinteger(L, 6);
        colorOffset = &colorOffsetValue;
    }

    
    // invoke api call
    ApiBridge api = getApi(L);
    apiBridge_DrawTiles(api,
       ids,
       idsLen,
       column,
       row,
       columns,
       colorOffset);
    
    return 0;
}

static int drawFont(lua_State *L)
{
    assert(L);
    int argc = lua_gettop(L);
    if (argc < 4 || argc > 7)
    {
        lua_pushstring(L, "invalid number of arguments");
        lua_error(L);
    }

    // required parameters
    char *text = luaL_checkstring(L, 2);
    int x = luaL_checkinteger(L, 3);
    int y = luaL_checkinteger(L, 4);

    // optional parameters
    char **fontName = NULL;
    char *fontNameValue;
    if (argc >= 5)
    {
        fontNameValue = luaL_checkstring(L, 5);
        fontName = &fontNameValue;
    }

    int *letterSpacing = NULL;
    int letterSpacingValue;
    if (argc >= 6)
    {
        letterSpacingValue = luaL_checkinteger(L, 6);
        letterSpacing = &letterSpacingValue;
    }

    int *offset = NULL;
    int offsetValue;
    if (argc >= 7)
    {
        offsetValue = luaL_checkinteger(L, 7);
        offset = &offsetValue;
    }

    
    // invoke api call
    ApiBridge api = getApi(L);
    apiBridge_DrawFont(api,
       text,
       x,
       y,
       fontName,
       letterSpacing,
       offset);
    
    return 0;
}

static int drawTextBox(lua_State *L)
{
    assert(L);
    int argc = lua_gettop(L);
    if (argc < 5 || argc > 8)
    {
        lua_pushstring(L, "invalid number of arguments");
        lua_error(L);
    }

    // required parameters
    char *text = luaL_checkstring(L, 2);
    int witdh = luaL_checkinteger(L, 3);
    int x = luaL_checkinteger(L, 4);
    int y = luaL_checkinteger(L, 5);

    // optional parameters
    char **fontName = NULL;
    char *fontNameValue;
    if (argc >= 6)
    {
        fontNameValue = luaL_checkstring(L, 6);
        fontName = &fontNameValue;
    }

    int *letterSpacing = NULL;
    int letterSpacingValue;
    if (argc >= 7)
    {
        letterSpacingValue = luaL_checkinteger(L, 7);
        letterSpacing = &letterSpacingValue;
    }

    bool *wholeWords = NULL;
    bool wholeWordsValue;
    if (argc >= 8)
    {
        wholeWordsValue = lua_toboolean(L, 8);
        wholeWords = &wholeWordsValue;
    }

    
    // invoke api call
    ApiBridge api = getApi(L);
    apiBridge_DrawTextBox(api,
       text,
       witdh,
       x,
       y,
       fontName,
       letterSpacing,
       wholeWords);
    
    return 0;
}

static int formatWordWrap(lua_State *L)
{
    assert(L);
    int argc = lua_gettop(L);
    if (argc < 3 || argc > 4)
    {
        lua_pushstring(L, "invalid number of arguments");
        lua_error(L);
    }

    // required parameters
    char *text = luaL_checkstring(L, 2);
    int witdh = luaL_checkinteger(L, 3);

    // optional parameters
    bool *wholeWords = NULL;
    bool wholeWordsValue;
    if (argc >= 4)
    {
        wholeWordsValue = lua_toboolean(L, 4);
        wholeWords = &wholeWordsValue;
    }

    
    int returnLen = 0;
    // invoke api call
    ApiBridge api = getApi(L);
    char *returnValue = apiBridge_FormatWordWrap(api,
       text,
       witdh,
       wholeWords,
       &returnLen);
    
    lua_pushstring(L, returnValue);

    return 1;
}

static int drawPixelData(lua_State *L)
{
    assert(L);
    int argc = lua_gettop(L);
    if (argc < 9 || argc > 12)
    {
        lua_pushstring(L, "invalid number of arguments");
        lua_error(L);
    }

    // required parameters
    luaL_checktype(L, 2, LUA_TTABLE);
    int pixelDataLen = lua_rawlen(L, 2);
    int pixelData[ARRAY_BUFFER_LEN] = { 0 };
    lua_pushnil(L);
    int pixelDataIdx = 0;
    while (lua_next(L, 2) != 0)
    {
        pixelData[pixelDataIdx] = luaL_checkinteger(L, -1);
        lua_pop(L, 1);
        pixelDataIdx++;
    }
    int x = luaL_checkinteger(L, 3);
    int y = luaL_checkinteger(L, 4);
    int width = luaL_checkinteger(L, 5);
    int height = luaL_checkinteger(L, 6);
    bool flipH = lua_toboolean(L, 7);
    bool flipV = lua_toboolean(L, 8);
    bool flipY = lua_toboolean(L, 9);

    // optional parameters
    int *layerOrder = NULL;
    int layerOrderValue;
    if (argc >= 10)
    {
        layerOrderValue = luaL_checkinteger(L, 10);
        layerOrder = &layerOrderValue;
    }

    bool *masked = NULL;
    bool maskedValue;
    if (argc >= 11)
    {
        maskedValue = lua_toboolean(L, 11);
        masked = &maskedValue;
    }

    int *colorOffset = NULL;
    int colorOffsetValue;
    if (argc >= 12)
    {
        colorOffsetValue = luaL_checkinteger(L, 12);
        colorOffset = &colorOffsetValue;
    }

    
    // invoke api call
    ApiBridge api = getApi(L);
    apiBridge_DrawPixelData(api,
       pixelData,
       pixelDataLen,
       x,
       y,
       width,
       height,
       flipH,
       flipV,
       flipY,
       layerOrder,
       masked,
       colorOffset);
    
    return 0;
}

static int readFlagAt(lua_State *L)
{
    assert(L);
    int argc = lua_gettop(L);
    if (argc != 3)
    {
        lua_pushstring(L, "invalid number of arguments");
        lua_error(L);
    }

    // required parameters
    int column = luaL_checkinteger(L, 2);
    int row = luaL_checkinteger(L, 3);
    
    // invoke api call
    ApiBridge api = getApi(L);
    int returnValue = apiBridge_ReadFlagAt(api,
       column,
       row);
    
    lua_pushnumber(L, returnValue);

    return 1;
}

static int getKey(lua_State *L)
{
    assert(L);
    int argc = lua_gettop(L);
    if (argc != 2)
    {
        lua_pushstring(L, "invalid number of arguments");
        lua_error(L);
    }

    // required parameters
    int key = luaL_checkinteger(L, 2);
    
    // invoke api call
    ApiBridge api = getApi(L);
    bool returnValue = apiBridge_GetKey(api,
       key);
    
    lua_pushboolean(L, returnValue);

    return 1;
}

static int getKeyDown(lua_State *L)
{
    assert(L);
    int argc = lua_gettop(L);
    if (argc != 2)
    {
        lua_pushstring(L, "invalid number of arguments");
        lua_error(L);
    }

    // required parameters
    int key = luaL_checkinteger(L, 2);
    
    // invoke api call
    ApiBridge api = getApi(L);
    bool returnValue = apiBridge_GetKeyDown(api,
       key);
    
    lua_pushboolean(L, returnValue);

    return 1;
}

static int getKeyUp(lua_State *L)
{
    assert(L);
    int argc = lua_gettop(L);
    if (argc != 2)
    {
        lua_pushstring(L, "invalid number of arguments");
        lua_error(L);
    }

    // required parameters
    int key = luaL_checkinteger(L, 2);
    
    // invoke api call
    ApiBridge api = getApi(L);
    bool returnValue = apiBridge_GetKeyUp(api,
       key);
    
    lua_pushboolean(L, returnValue);

    return 1;
}

static int spritesToRawData(lua_State *L)
{
    assert(L);
    int argc = lua_gettop(L);
    if (argc != 3)
    {
        lua_pushstring(L, "invalid number of arguments");
        lua_error(L);
    }

    // required parameters
    luaL_checktype(L, 2, LUA_TTABLE);
    int idsLen = lua_rawlen(L, 2);
    int ids[ARRAY_BUFFER_LEN] = { 0 };
    lua_pushnil(L);
    int idsIdx = 0;
    while (lua_next(L, 2) != 0)
    {
        ids[idsIdx] = luaL_checkinteger(L, -1);
        lua_pop(L, 1);
        idsIdx++;
    }
    int width = luaL_checkinteger(L, 3);
    
    int returnLen = 0;
    // invoke api call
    ApiBridge api = getApi(L);
    int *returnValue = apiBridge_SpritesToRawData(api,
       ids,
       idsLen,
       width,
       &returnLen);
    
    lua_newtable(L);
    for (int idx = 0; idx < returnLen; idx++)
    {
        lua_pushnumber(L, idx);
        lua_pushnumber(L, returnValue[idx]);
        lua_settable(L, -3);
    }

    return 1;
}

static int saveData(lua_State *L)
{
    assert(L);
    int argc = lua_gettop(L);
    if (argc != 3)
    {
        lua_pushstring(L, "invalid number of arguments");
        lua_error(L);
    }

    // required parameters
    char *key = luaL_checkstring(L, 2);
    char *value = luaL_checkstring(L, 3);
    
    // invoke api call
    ApiBridge api = getApi(L);
    apiBridge_SaveData(api,
       key,
       value);
    
    return 0;
}

static int readData(lua_State *L)
{
    assert(L);
    int argc = lua_gettop(L);
    if (argc < 2 || argc > 3)
    {
        lua_pushstring(L, "invalid number of arguments");
        lua_error(L);
    }

    // required parameters
    char *key = luaL_checkstring(L, 2);

    // optional parameters
    char **defaultValue = NULL;
    char *defaultValueValue;
    if (argc >= 3)
    {
        defaultValueValue = luaL_checkstring(L, 3);
        defaultValue = &defaultValueValue;
    }

    
    int returnLen = 0;
    // invoke api call
    ApiBridge api = getApi(L);
    char *returnValue = apiBridge_ReadData(api,
       key,
       defaultValue,
       &returnLen);
    
    lua_pushstring(L, returnValue);

    return 1;
}

static int loadSong(lua_State *L)
{
    assert(L);
    int argc = lua_gettop(L);
    if (argc != 2)
    {
        lua_pushstring(L, "invalid number of arguments");
        lua_error(L);
    }

    // required parameters
    int id = luaL_checkinteger(L, 2);
    
    // invoke api call
    ApiBridge api = getApi(L);
    apiBridge_LoadSong(api,
       id);
    
    return 0;
}

static int playSong(lua_State *L)
{
    assert(L);
    int argc = lua_gettop(L);
    if (argc < 1 || argc > 2)
    {
        lua_pushstring(L, "invalid number of arguments");
        lua_error(L);
    }

    // optional parameters
    bool *loop = NULL;
    bool loopValue;
    if (argc >= 2)
    {
        loopValue = lua_toboolean(L, 2);
        loop = &loopValue;
    }

    
    // invoke api call
    ApiBridge api = getApi(L);
    apiBridge_PlaySong(api,
       loop);
    
    return 0;
}

static int pauseSong(lua_State *L)
{
    assert(L);
    int argc = lua_gettop(L);
    if (argc != 1)
    {
        lua_pushstring(L, "invalid number of arguments");
        lua_error(L);
    }
    
    // invoke api call
    ApiBridge api = getApi(L);
    apiBridge_PauseSong(api);
    
    return 0;
}

static int stopSong(lua_State *L)
{
    assert(L);
    int argc = lua_gettop(L);
    if (argc < 1 || argc > 2)
    {
        lua_pushstring(L, "invalid number of arguments");
        lua_error(L);
    }

    // optional parameters
    bool *autoRewind = NULL;
    bool autoRewindValue;
    if (argc >= 2)
    {
        autoRewindValue = lua_toboolean(L, 2);
        autoRewind = &autoRewindValue;
    }

    
    // invoke api call
    ApiBridge api = getApi(L);
    apiBridge_StopSong(api,
       autoRewind);
    
    return 0;
}

static int rewindSong(lua_State *L)
{
    assert(L);
    int argc = lua_gettop(L);
    if (argc != 1)
    {
        lua_pushstring(L, "invalid number of arguments");
        lua_error(L);
    }
    
    // invoke api call
    ApiBridge api = getApi(L);
    apiBridge_RewindSong(api);
    
    return 0;
}

static int replaceColorID(lua_State *L)
{
    assert(L);
    int argc = lua_gettop(L);
    if (argc != 4)
    {
        lua_pushstring(L, "invalid number of arguments");
        lua_error(L);
    }

    // required parameters
    luaL_checktype(L, 2, LUA_TTABLE);
    int pixelDataLen = lua_rawlen(L, 2);
    int pixelData[ARRAY_BUFFER_LEN] = { 0 };
    lua_pushnil(L);
    int pixelDataIdx = 0;
    while (lua_next(L, 2) != 0)
    {
        pixelData[pixelDataIdx] = luaL_checkinteger(L, -1);
        lua_pop(L, 1);
        pixelDataIdx++;
    }
    int oldID = luaL_checkinteger(L, 3);
    int newID = luaL_checkinteger(L, 4);
    
    int returnLen = 0;
    // invoke api call
    ApiBridge api = getApi(L);
    int *returnValue = apiBridge_ReplaceColorID(api,
       pixelData,
       pixelDataLen,
       oldID,
       newID,
       &returnLen);
    
    lua_newtable(L);
    for (int idx = 0; idx < returnLen; idx++)
    {
        lua_pushnumber(L, idx);
        lua_pushnumber(L, returnValue[idx]);
        lua_settable(L, -3);
    }

    return 1;
}

static int replaceColorIDs(lua_State *L)
{
    assert(L);
    int argc = lua_gettop(L);
    if (argc != 4)
    {
        lua_pushstring(L, "invalid number of arguments");
        lua_error(L);
    }

    // required parameters
    luaL_checktype(L, 2, LUA_TTABLE);
    int pixelDataLen = lua_rawlen(L, 2);
    int pixelData[ARRAY_BUFFER_LEN] = { 0 };
    lua_pushnil(L);
    int pixelDataIdx = 0;
    while (lua_next(L, 2) != 0)
    {
        pixelData[pixelDataIdx] = luaL_checkinteger(L, -1);
        lua_pop(L, 1);
        pixelDataIdx++;
    }
    luaL_checktype(L, 3, LUA_TTABLE);
    int oldIDsLen = lua_rawlen(L, 3);
    int oldIDs[ARRAY_BUFFER_LEN] = { 0 };
    lua_pushnil(L);
    int oldIDsIdx = 0;
    while (lua_next(L, 3) != 0)
    {
        oldIDs[oldIDsIdx] = luaL_checkinteger(L, -1);
        lua_pop(L, 1);
        oldIDsIdx++;
    }
    luaL_checktype(L, 4, LUA_TTABLE);
    int newIDsLen = lua_rawlen(L, 4);
    int newIDs[ARRAY_BUFFER_LEN] = { 0 };
    lua_pushnil(L);
    int newIDsIdx = 0;
    while (lua_next(L, 4) != 0)
    {
        newIDs[newIDsIdx] = luaL_checkinteger(L, -1);
        lua_pop(L, 1);
        newIDsIdx++;
    }
    
    int returnLen = 0;
    // invoke api call
    ApiBridge api = getApi(L);
    int *returnValue = apiBridge_ReplaceColorIDs(api,
       pixelData,
       pixelDataLen,
       oldIDs,
       oldIDsLen,
       newIDs,
       newIDsLen,
       &returnLen);
    
    lua_newtable(L);
    for (int idx = 0; idx < returnLen; idx++)
    {
        lua_pushnumber(L, idx);
        lua_pushnumber(L, returnValue[idx]);
        lua_settable(L, -3);
    }

    return 1;
}

static int drawScreenBuffer(lua_State *L)
{
    assert(L);
    int argc = lua_gettop(L);
    if (argc < 1 || argc > 7)
    {
        lua_pushstring(L, "invalid number of arguments");
        lua_error(L);
    }

    // optional parameters
    int *x = NULL;
    int xValue;
    if (argc >= 2)
    {
        xValue = luaL_checkinteger(L, 2);
        x = &xValue;
    }

    int *y = NULL;
    int yValue;
    if (argc >= 3)
    {
        yValue = luaL_checkinteger(L, 3);
        y = &yValue;
    }

    int *width = NULL;
    int widthValue;
    if (argc >= 4)
    {
        widthValue = luaL_checkinteger(L, 4);
        width = &widthValue;
    }

    int *height = NULL;
    int heightValue;
    if (argc >= 5)
    {
        heightValue = luaL_checkinteger(L, 5);
        height = &heightValue;
    }

    int *offsetX = NULL;
    int offsetXValue;
    if (argc >= 6)
    {
        offsetXValue = luaL_checkinteger(L, 6);
        offsetX = &offsetXValue;
    }

    int *offsetY = NULL;
    int offsetYValue;
    if (argc >= 7)
    {
        offsetYValue = luaL_checkinteger(L, 7);
        offsetY = &offsetYValue;
    }

    
    // invoke api call
    ApiBridge api = getApi(L);
    apiBridge_DrawScreenBuffer(api,
       x,
       y,
       width,
       height,
       offsetX,
       offsetY);
    
    return 0;
}

static int drawTilemap(lua_State *L)
{
    assert(L);
    int argc = lua_gettop(L);
    if (argc < 1 || argc > 7)
    {
        lua_pushstring(L, "invalid number of arguments");
        lua_error(L);
    }

    // optional parameters
    int *startCol = NULL;
    int startColValue;
    if (argc >= 2)
    {
        startColValue = luaL_checkinteger(L, 2);
        startCol = &startColValue;
    }

    int *startRow = NULL;
    int startRowValue;
    if (argc >= 3)
    {
        startRowValue = luaL_checkinteger(L, 3);
        startRow = &startRowValue;
    }

    int *columns = NULL;
    int columnsValue;
    if (argc >= 4)
    {
        columnsValue = luaL_checkinteger(L, 4);
        columns = &columnsValue;
    }

    int *rows = NULL;
    int rowsValue;
    if (argc >= 5)
    {
        rowsValue = luaL_checkinteger(L, 5);
        rows = &rowsValue;
    }

    int *offsetX = NULL;
    int offsetXValue;
    if (argc >= 6)
    {
        offsetXValue = luaL_checkinteger(L, 6);
        offsetX = &offsetXValue;
    }

    int *offsetY = NULL;
    int offsetYValue;
    if (argc >= 7)
    {
        offsetYValue = luaL_checkinteger(L, 7);
        offsetY = &offsetYValue;
    }

    
    // invoke api call
    ApiBridge api = getApi(L);
    apiBridge_DrawTilemap(api,
       startCol,
       startRow,
       columns,
       rows,
       offsetX,
       offsetY);
    
    return 0;
}

static int drawFontTiles(lua_State *L)
{
    assert(L);
    int argc = lua_gettop(L);
    if (argc < 4 || argc > 6)
    {
        lua_pushstring(L, "invalid number of arguments");
        lua_error(L);
    }

    // required parameters
    char *text = luaL_checkstring(L, 2);
    int column = luaL_checkinteger(L, 3);
    int row = luaL_checkinteger(L, 4);

    // optional parameters
    char **fontName = NULL;
    char *fontNameValue;
    if (argc >= 5)
    {
        fontNameValue = luaL_checkstring(L, 5);
        fontName = &fontNameValue;
    }

    int *offset = NULL;
    int offsetValue;
    if (argc >= 6)
    {
        offsetValue = luaL_checkinteger(L, 6);
        offset = &offsetValue;
    }

    
    // invoke api call
    ApiBridge api = getApi(L);
    apiBridge_DrawFontTiles(api,
       text,
       column,
       row,
       fontName,
       offset);
    
    return 0;
}

static int rebuildScreenBuffer(lua_State *L)
{
    assert(L);
    int argc = lua_gettop(L);
    if (argc != 1)
    {
        lua_pushstring(L, "invalid number of arguments");
        lua_error(L);
    }
    
    // invoke api call
    ApiBridge api = getApi(L);
    apiBridge_RebuildScreenBuffer(api);
    
    return 0;
}

static int drawTileToBuffer(lua_State *L)
{
    assert(L);
    int argc = lua_gettop(L);
    if (argc < 4 || argc > 5)
    {
        lua_pushstring(L, "invalid number of arguments");
        lua_error(L);
    }

    // required parameters
    int spriteID = luaL_checkinteger(L, 2);
    int column = luaL_checkinteger(L, 3);
    int row = luaL_checkinteger(L, 4);

    // optional parameters
    int *colorOffset = NULL;
    int colorOffsetValue;
    if (argc >= 5)
    {
        colorOffsetValue = luaL_checkinteger(L, 5);
        colorOffset = &colorOffsetValue;
    }

    
    // invoke api call
    ApiBridge api = getApi(L);
    apiBridge_DrawTileToBuffer(api,
       spriteID,
       column,
       row,
       colorOffset);
    
    return 0;
}

static int drawTilesToBuffer(lua_State *L)
{
    assert(L);
    int argc = lua_gettop(L);
    if (argc < 5 || argc > 6)
    {
        lua_pushstring(L, "invalid number of arguments");
        lua_error(L);
    }

    // required parameters
    luaL_checktype(L, 2, LUA_TTABLE);
    int idsLen = lua_rawlen(L, 2);
    int ids[ARRAY_BUFFER_LEN] = { 0 };
    lua_pushnil(L);
    int idsIdx = 0;
    while (lua_next(L, 2) != 0)
    {
        ids[idsIdx] = luaL_checkinteger(L, -1);
        lua_pop(L, 1);
        idsIdx++;
    }
    int column = luaL_checkinteger(L, 3);
    int row = luaL_checkinteger(L, 4);
    int columns = luaL_checkinteger(L, 5);

    // optional parameters
    int *colorOffset = NULL;
    int colorOffsetValue;
    if (argc >= 6)
    {
        colorOffsetValue = luaL_checkinteger(L, 6);
        colorOffset = &colorOffsetValue;
    }

    
    // invoke api call
    ApiBridge api = getApi(L);
    apiBridge_DrawTilesToBuffer(api,
       ids,
       idsLen,
       column,
       row,
       columns,
       colorOffset);
    
    return 0;
}

static int drawTextBoxToBuffer(lua_State *L)
{
    assert(L);
    int argc = lua_gettop(L);
    if (argc < 5 || argc > 8)
    {
        lua_pushstring(L, "invalid number of arguments");
        lua_error(L);
    }

    // required parameters
    char *text = luaL_checkstring(L, 2);
    int witdh = luaL_checkinteger(L, 3);
    int column = luaL_checkinteger(L, 4);
    int row = luaL_checkinteger(L, 5);

    // optional parameters
    char **fontName = NULL;
    char *fontNameValue;
    if (argc >= 6)
    {
        fontNameValue = luaL_checkstring(L, 6);
        fontName = &fontNameValue;
    }

    int *letterSpacing = NULL;
    int letterSpacingValue;
    if (argc >= 7)
    {
        letterSpacingValue = luaL_checkinteger(L, 7);
        letterSpacing = &letterSpacingValue;
    }

    bool *wholeWords = NULL;
    bool wholeWordsValue;
    if (argc >= 8)
    {
        wholeWordsValue = lua_toboolean(L, 8);
        wholeWords = &wholeWordsValue;
    }

    
    // invoke api call
    ApiBridge api = getApi(L);
    apiBridge_DrawTextBoxToBuffer(api,
       text,
       witdh,
       column,
       row,
       fontName,
       letterSpacing,
       wholeWords);
    
    return 0;
}

static int drawBufferData(lua_State *L)
{
    assert(L);
    int argc = lua_gettop(L);
    if (argc != 6)
    {
        lua_pushstring(L, "invalid number of arguments");
        lua_error(L);
    }

    // required parameters
    luaL_checktype(L, 2, LUA_TTABLE);
    int pixelDataLen = lua_rawlen(L, 2);
    int pixelData[ARRAY_BUFFER_LEN] = { 0 };
    lua_pushnil(L);
    int pixelDataIdx = 0;
    while (lua_next(L, 2) != 0)
    {
        pixelData[pixelDataIdx] = luaL_checkinteger(L, -1);
        lua_pop(L, 1);
        pixelDataIdx++;
    }
    int x = luaL_checkinteger(L, 3);
    int y = luaL_checkinteger(L, 4);
    int width = luaL_checkinteger(L, 5);
    int height = luaL_checkinteger(L, 6);
    
    // invoke api call
    ApiBridge api = getApi(L);
    apiBridge_DrawBufferData(api,
       pixelData,
       pixelDataLen,
       x,
       y,
       width,
       height);
    
    return 0;
}

static int drawFontToBuffer(lua_State *L)
{
    assert(L);
    int argc = lua_gettop(L);
    if (argc < 4 || argc > 7)
    {
        lua_pushstring(L, "invalid number of arguments");
        lua_error(L);
    }

    // required parameters
    char *text = luaL_checkstring(L, 2);
    int column = luaL_checkinteger(L, 3);
    int row = luaL_checkinteger(L, 4);

    // optional parameters
    char **fontName = NULL;
    char *fontNameValue;
    if (argc >= 5)
    {
        fontNameValue = luaL_checkstring(L, 5);
        fontName = &fontNameValue;
    }

    int *letterSpacing = NULL;
    int letterSpacingValue;
    if (argc >= 6)
    {
        letterSpacingValue = luaL_checkinteger(L, 6);
        letterSpacing = &letterSpacingValue;
    }

    int *offset = NULL;
    int offsetValue;
    if (argc >= 7)
    {
        offsetValue = luaL_checkinteger(L, 7);
        offset = &offsetValue;
    }

    
    // invoke api call
    ApiBridge api = getApi(L);
    apiBridge_DrawFontToBuffer(api,
       text,
       column,
       row,
       fontName,
       letterSpacing,
       offset);
    
    return 0;
}


