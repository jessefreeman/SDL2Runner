// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#include <assert.h>
#include <stdlib.h>
#include "util.h"
#include "controllerdevice.h"
#include "controllerchip.h"

ControllerChip controllerChip_Create()
{
    ControllerChip self = NULL;
    
    self = (ControllerChip)calloc(1, sizeof(controllerChip));
    if (self == NULL)
        return NULL;
    
    controllerChip_Init(self);
    self->base.destroy = controllerChip_Destroy;

    return self;
}

void controllerChip_Destroy(ControllerChip self)
{
    assert(self);
    memset(self, 0, sizeof(controllerChip));
    free(self);
}

void controllerChip_Init(ControllerChip self)
{
    assert(self);
    memset(self, 0, sizeof(controllerChip));
    strncpy(self->base.name, nameof(ControllerChip), sizeof(self->base.name) - 1);
}

void controllerChip_InsertController(ControllerChip self, int slot, ControllerDevice controller)
{
    assert(self);
    self->controllers[clamp(slot, 0, MAX_CONTROLLERS - 1)] = controller;
}

bool controllerChip_ButtonDown(ControllerChip self, int button, int slot)
{
    assert(self);
    if (slot < 0 || slot >= MAX_CONTROLLERS) return false;
    if (self->controllers[slot] == NULL) return false;
    buttonState buttonState = controllerDevice_GetButtonState(self->controllers[slot], button);
    return buttonState == PRESSED;
}

vector controllerChip_GetMousePosition(ControllerChip self)
{
    assert(self);

}