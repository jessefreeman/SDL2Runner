// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#ifndef _gameconsole_h_
#define _gameconsole_h_

#include "chip.h"
#include "controllerdevice.h"
#include "displaydevice.h"

typedef struct gameConsole *GameConsole;

GameConsole gameConsole_Create();

void gameConsole_Destroy(GameConsole self);

void gameConsole_InsertChip(GameConsole self, Chip chip);

void gameConsole_InsertController(GameConsole self, int slotIdx, ControllerDevice controller);

void gameConsole_InsertDisplayDevice(GameConsole self, DisplayDevice display);

void gameConsole_PowerOn(GameConsole self);

void gameConsole_Tick(GameConsole self, float timeDelta);

void gameConsole_Render(GameConsole self);

void gameConsole_PowerOff(GameConsole self);

#endif

