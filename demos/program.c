// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#include <stdlib.h>
#include "pv8sdk.h"
#include "pv8sdk.lua.h"
#include "pv8sdk.sdl2.h"
#include "util.h"
#include "demoscartridge.h"

#define WINDOW_WIDTH    1024
#define WINDOW_HEIGHT   720
#define DISPLAY_WIDTH   256
#define DISPLAY_HEIGHT  240
#define SPRITE_WIDTH    8
#define SPRITE_HEIGHT   8

int main(int argc, char **argv)
{
    // Build the cartridge.
    DemosCartridge cartridge = demosCartridge_Create();
    demosCartridge_SetColorsFile(cartridge, ".\\resources\\colors.png");
    demosCartridge_SetSpritesFile(cartridge, ".\\resources\\sprites.png");
    demosCartridge_AddFontFile(cartridge, ".\\resources\\large-font.png", "large-font");
    demosCartridge_SetScriptFile(cartridge, ".\\resources\\DrawSpriteDemo.lua");

    // Build the game console.
    GameConsole console = gameConsole_Create();

    // Insert chips.
    // These are standard chips that should be always inserted.
    gameConsole_InsertChip(console, (Chip)colorChip_Create());
    gameConsole_InsertChip(console, (Chip)displayChip_Create(DISPLAY_WIDTH, DISPLAY_HEIGHT));
    gameConsole_InsertChip(console, (Chip)spriteChip_Create(SPRITE_WIDTH, SPRITE_HEIGHT));
    // This console will use a cartridge to load content so the cartidge chip is needed.
    gameConsole_InsertChip(console, (Chip)cartridgeChip_Create());
    // This console will use Lua for code so the Lua game chip is needed.
    gameConsole_InsertChip(console, (Chip)luaGameChip_Create());

    Sdl sdl = sdl_GetInstance();

    // Insert display.
    gameConsole_InsertDisplay(console, 
        (Display)sdl_CreateDisplay(sdl,
            WINDOW_WIDTH, WINDOW_HEIGHT, 
            DISPLAY_WIDTH, DISPLAY_HEIGHT));

    // Insert controller.
    // gameConsole_InsertController(console, (Controller)sdl2Controller_Create());

    // Insert cartridge.
    gameConsole_InsertCartridge(console, (Cartridge)cartridge);

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