// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#ifndef _gameconsole_h_
#define _gameconsole_h_

#include "chip.h"
#include "cartridge.h"
#include "controller.h"
#include "display.h"

typedef struct gameConsole *GameConsole;

GameConsole gameConsole_Create();

void gameConsole_Destroy(GameConsole self);

void gameConsole_InsertChip(GameConsole self, Chip chip);

void gameConsole_InsertCartridge(GameConsole self, Cartridge cartridge);

void gameConsole_InsertController(GameConsole self, Controller controller);

void gameConsole_InsertDisplay(GameConsole self, Display display);

void gameConsole_Run(GameConsole self);

#endif

