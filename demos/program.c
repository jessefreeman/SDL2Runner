// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#include <stdlib.h>
#include "pv8sdk.h"
#include "pv8sdk.lua.h"
#include "pv8sdk.sdl2.h"
#include "util.h"
#include "importutil.h"

#define WINDOW_WIDTH    1024
#define WINDOW_HEIGHT   720
#define DISPLAY_WIDTH   256
#define DISPLAY_HEIGHT  240
#define SPRITE_WIDTH    8
#define SPRITE_HEIGHT   8

int main(int argc, char **argv)
{
    // Build the game console.
    GameConsole console = gameConsole_Create();

    // Create DisplayChip.
    
    gameConsole_InsertChip(console, (Chip)displayChip_Create(DISPLAY_WIDTH, DISPLAY_HEIGHT));

    // Create ColorChip.
    
    int width = 0;
    int height = 0;
    colorData *colors = importImageFromFile(".\\resources\\colors.png", &width, &height);
    colorChip colorChip;
    colorChip_Init(&colorChip, colors, width * height);
    free(colors);
    gameConsole_InsertChip(console, (Chip)&colorChip);

    // Create SpriteChip.
    
    TextureData spriteSheet = importSpriteSheetFromFile(".\\resources\\sprites.png", &colorChip);
    SpriteChip spriteChip = spriteChip_Create(SPRITE_WIDTH, SPRITE_HEIGHT, spriteSheet);
    textureData_Destroy(spriteSheet);
    spriteSheet = NULL;
    gameConsole_InsertChip(console, (Chip)spriteChip);

    // Create FontChip

    FontChip fontChip = fontChip_Create();
    gameConsole_InsertChip(console, (Chip)fontChip);

    spriteId mapBuffer[96] = { 0 };

    spriteSheet = importSpriteSheetFromFile(".\\resources\\large-font.png", &colorChip);
    spriteChip_AddSpritesFromTexture(spriteChip, spriteSheet, mapBuffer);
    textureData_Destroy(spriteSheet);
    spriteSheet = NULL;
    fontChip_AddFont(fontChip, "large-font", 96, mapBuffer);

    spriteSheet = importSpriteSheetFromFile(".\\resources\\small-font.png", &colorChip);
    spriteChip_AddSpritesFromTexture(spriteChip, spriteSheet, mapBuffer);
    textureData_Destroy(spriteSheet);
    spriteSheet = NULL;
    fontChip_AddFont(fontChip, "small-font", 96, mapBuffer);

    // Create TilemapChip
    TilemapChip tilemapChip = tilemapChip_Create(DISPLAY_WIDTH / SPRITE_WIDTH, DISPLAY_HEIGHT / SPRITE_HEIGHT);
    gameConsole_InsertChip(console, (Chip)tilemapChip);

    // Create LuaGameChip
    
    int len = 0;
    char *gameCode = importTextFromFile(".\\resources\\DrawSpriteDemo.lua", &len);
    LuaGameChip gameChip = luaGameChip_Create(gameCode, len);
    free(gameCode);
    gameConsole_InsertChip(console, (Chip)gameChip);

    // Insert DisplayDevice.

    SDL sdl = sdl_GetInstance();

    gameConsole_InsertDisplayDevice(console, 
        (DisplayDevice)sdl_CreateDisplay(sdl,
            WINDOW_WIDTH, WINDOW_HEIGHT, 
            DISPLAY_WIDTH, DISPLAY_HEIGHT));

    // Insert controller.

    // gameConsole_InsertController(console, (Controller)sdl2Controller_Create());

    // Run the game.
    gameConsole_PowerOn(console);
    sdl_RunGame(sdl, console);
    gameConsole_PowerOff(console);

    // Note, any resource inserted into the console delegates the responsibility
    // of resource cleanup to the console. i.e. no need to Destroy a chips or devices
    // inserted into the console, the console will do so when it is destroyed.
    gameConsole_Destroy(console);
}