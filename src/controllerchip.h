// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#ifndef _controllerchip_h_
#define _controllerchip_h_

#include <stdbool.h>
#include "pvconf.h"
#include "chip.h"
#include "controllerdevice.h"

typedef struct controllerChip {
    chip base;
    ControllerDevice controllers[MAX_CONTROLLERS];
} controllerChip;

typedef controllerChip *ControllerChip;

ControllerChip controllerChip_Create();

void controllerChip_Destroy(ControllerChip self);

void controllerChip_Init(ControllerChip self);

void controllerChip_InsertController(ControllerChip self, int slot, ControllerDevice controller);

bool controllerChip_ButtonDown(ControllerChip self, int button, int slot);

bool controllerChip_MouseButtonDown(ControllerChip self, int button);

vector controllerChip_GetMousePosition(ControllerChip self);

#endif

