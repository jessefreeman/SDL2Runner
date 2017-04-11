// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#ifndef _controllerdevice_h_
#define _controllerdevice_h_

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

typedef struct controllerDevice {
    device base; // must be first
    buttonState (*getButtonState)(Obj, button);
} controllerDevice;

typedef struct controllerDevice *ControllerDevice;

buttonState controllerDevice_GetButtonState(ControllerDevice self, button button);

#endif
