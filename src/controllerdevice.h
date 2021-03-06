// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#ifndef _controllerdevice_h_
#define _controllerdevice_h_

#include "types.h"
#include "types.h"
#include "device.h"

typedef enum buttonState {
    RELEASED = 0,
    PRESSED
} buttonState;

typedef enum button {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    A,
    B,
    SELECT,
    START
} button;

typedef enum mouseButton {
    MOUSE_LEFT,
    MOUSE_RIGHT,
    MOUSE_MIDDLE
} mouseButton;

typedef struct controllerDevice {
    device base; // must be first
    buttonState (*getButtonState)(Obj, button);
    buttonState(*getMouseButtonState)(Obj, mouseButton);
    vector (*getMousePosition)(Obj);
} controllerDevice;

typedef struct controllerDevice *ControllerDevice;

buttonState controllerDevice_GetButtonState(ControllerDevice self, button button);

buttonState controllerDevice_GetMouseButtonState(ControllerDevice self, mouseButton button);

vector controllerDevice_GetMousePosition(ControllerDevice self);

#endif
