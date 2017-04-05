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
    ColorChip colorChip = colorChip_Create(colors, width * height);
    free(colors);
    gameConsole_InsertChip(console, (Chip)colorChip);

    // Create SpriteChip.
    width = 0;
    height = 0;
    colorData *spritePixels = importImageFromFile(".\\resources\\sprites.png", &width, &height);
    TextureData *spriteSheet = textureData_Create(width, height);
    spriteSheet = colorChip_MapPixelDataToTexture(colorChip, width, height, spritePixels, spriteSheet);
    SpriteChip spriteChip = spriteChip_Create(SPRITE_WIDTH, SPRITE_HEIGHT, spriteSheet);
    textureData_Destroy(spriteSheet);
    free(spritePixels);
    gameConsole_InsertChip(console, (Chip)spriteChip);

    // Create LuaGameChip
    int len = 0;
    char *gameCode = importTextFromFile(".\\resources\\DrawSpriteDemo.lua", &len);
    LuaGameChip gameChip = luaGameChip_Create(gameCode, len);
    free(gameCode);
    gameConsole_InsertChip(console, (Chip)gameChip);

    Sdl sdl = sdl_GetInstance();

    // Insert display.
    gameConsole_InsertDisplay(console, 
        (Display)sdl_CreateDisplay(sdl,
            WINDOW_WIDTH, WINDOW_HEIGHT, 
            DISPLAY_WIDTH, DISPLAY_HEIGHT));

    // Insert controller.
    // gameConsole_InsertController(console, (Controller)sdl2Controller_Create());

    // We need to provide a function to resolve time delta (for now)
    GetElapsedTime getElapsedTime = func_Create(sdl, sdl_GetElapsedTime);
    
    // Run the game.
    gameConsole_Run(console, getElapsedTime);

    func_Destroy(getElapsedTime);

    // Note, any resource inserted into the console delegates the responsibility
    // of resource cleanup to the console. i.e. no need to Destroy a chips or devices
    // inserted into the console, the console will do so when it is destroyed.
    gameConsole_Destroy(console);
}