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

typedef enum
{
    DrawSprite,
    Font,
    Controller,
    Mouse,
    SpriteStressTest,
    Tilemap
} Demos;

static const Demos demo = Mouse;

// TODO: add allocing Create's for these again (keep inits anyways)
static controllerChip pvControllerChip;

int main(int argc, char **argv)
{
    // Build the game console.
    
    GameConsole console = gameConsole_Create();

    // Create DisplayChip.

    gameConsole_InsertChip(console, (Chip)displayChip_Create(DISPLAY_WIDTH, DISPLAY_HEIGHT));

    // Create ColorChip.

    int width = 0;
    int height = 0;
    colorData *colors = importImageFromFile("./resources/colors.png", &width, &height);
    
    ColorChip colorChip = colorChip_Create(colors, width * height);
    free(colors);
    gameConsole_InsertChip(console, (Chip)colorChip);

    // Create SpriteChip

    TextureData spriteSheet = importSpriteSheetFromFile("./resources/sprites.png", colorChip);
    SpriteChip spriteChip = spriteChip_Create(SPRITE_WIDTH, SPRITE_HEIGHT, spriteSheet);
    textureData_Destroy(spriteSheet);
    spriteSheet = NULL;
    gameConsole_InsertChip(console, (Chip)spriteChip);

    // Create FontChip

    FontChip fontChip = fontChip_Create();
    gameConsole_InsertChip(console, (Chip)fontChip);
    spriteId mapBuffer[96] = { 0 };

    spriteSheet = importSpriteSheetFromFile("./resources/large-font.png", colorChip);
    spriteChip_AddSpritesFromTexture(spriteChip, spriteSheet, mapBuffer);
    textureData_Destroy(spriteSheet);
    spriteSheet = NULL;
    fontChip_AddFont(fontChip, "large-font", 96, mapBuffer);

    spriteSheet = importSpriteSheetFromFile("./resources/small-font.png", colorChip);
    spriteChip_AddSpritesFromTexture(spriteChip, spriteSheet, mapBuffer);
    textureData_Destroy(spriteSheet);
    spriteSheet = NULL;
    fontChip_AddFont(fontChip, "small-font", 96, mapBuffer);

    // Create TilemapChip

    TilemapChip tilemapChip = tilemapChip_Create(DISPLAY_WIDTH / SPRITE_WIDTH, DISPLAY_HEIGHT / SPRITE_HEIGHT);
    gameConsole_InsertChip(console, (Chip)tilemapChip);

    // Create ControllerChip

    controllerChip_Init(&pvControllerChip);
    gameConsole_InsertChip(console, (Chip)&pvControllerChip);

    // Create LuaGameChip

    int len = 0;
    char *gameCode = NULL;
    switch (demo)
    {
    case DrawSprite:
        gameCode = importTextFromFile("./resources/DrawSpriteDemo.lua", &len);
        break;

    case Font:
        gameCode = importTextFromFile("./resources/FontDemo.lua", &len);
        break;

    case Controller:
        gameCode = importTextFromFile("./resources/ControllerDemo.lua", &len);
        break;

    case Mouse:
        gameCode = importTextFromFile("./resources/MouseDemo.lua", &len);
        break;

    case SpriteStressTest:
        gameCode = importTextFromFile("./resources/SpriteStressTest.lua", &len);
        break;

    case Tilemap:
        gameCode = importTextFromFile("./resources/TilemapDemo/TilemapDemo.lua", &len);
        break;

    default:
        break;
    }
     
    LuaGameChip gameChip = luaGameChip_Create(gameCode, len);
    free(gameCode);
    gameConsole_InsertChip(console, (Chip)gameChip);

    // Create DisplayDevice.

    sdlDisplayDevice displayDevice;
    sdlDisplayDevice_Init(&displayDevice,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        DISPLAY_WIDTH, DISPLAY_HEIGHT);

    gameConsole_InsertDisplayDevice(console, (DisplayDevice)&displayDevice);

    // Create Controllers

    sdlButtonMap p1ButtonMap;
    sdlButtonMap_Init(&p1ButtonMap);
    sdlButtonMap_MapButton(&p1ButtonMap, UP,    SDLK_UP);
    sdlButtonMap_MapButton(&p1ButtonMap, DOWN,  SDLK_DOWN);
    sdlButtonMap_MapButton(&p1ButtonMap, LEFT,  SDLK_LEFT);
    sdlButtonMap_MapButton(&p1ButtonMap, RIGHT, SDLK_RIGHT);
    sdlButtonMap_MapButton(&p1ButtonMap, A,     SDLK_x);
    sdlButtonMap_MapButton(&p1ButtonMap, B,     SDLK_c);
    sdlButtonMap_MapButton(&p1ButtonMap, SELECT,SDLK_a);
    sdlButtonMap_MapButton(&p1ButtonMap, START, SDLK_s);
    sdlControllerDevice p1Controller;
    sdlControllerDevice_Init(&p1Controller, p1ButtonMap);
    gameConsole_InsertController(console, 0, (ControllerDevice)&p1Controller);

    sdlButtonMap p2ButtonMap;
    sdlButtonMap_Init(&p2ButtonMap);
    sdlButtonMap_MapButton(&p2ButtonMap, UP,    SDLK_i);
    sdlButtonMap_MapButton(&p2ButtonMap, DOWN,  SDLK_k);
    sdlButtonMap_MapButton(&p2ButtonMap, LEFT,  SDLK_j);
    sdlButtonMap_MapButton(&p2ButtonMap, RIGHT, SDLK_l);
    sdlButtonMap_MapButton(&p2ButtonMap, A,     SDLK_QUOTE);
    sdlButtonMap_MapButton(&p2ButtonMap, B,     SDLK_RETURN);
    sdlButtonMap_MapButton(&p2ButtonMap, SELECT,SDLK_y);
    sdlButtonMap_MapButton(&p2ButtonMap, START, SDLK_u);
    sdlControllerDevice p2Controller;
    sdlControllerDevice_Init(&p2Controller, p2ButtonMap);
    gameConsole_InsertController(console, 1, (ControllerDevice)&p2Controller);

    // Run the game.

    sdl_runGame(console,
        &p1Controller,
        &p2Controller);

    // Note, any resource inserted into the console delegates the responsibility
    // of resource cleanup to the console. i.e. no need to Destroy a chips or devices
    // inserted into the console, the console will do so when it is destroyed.
    gameConsole_Destroy(console);
}