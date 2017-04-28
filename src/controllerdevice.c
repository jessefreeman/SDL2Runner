// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#include <assert.h>
#include <stdlib.h>
#include "controllerdevice.h"

buttonState controllerDevice_GetButtonState(ControllerDevice self, button button)
{
    assert(self);
    if (self->getButtonState == NULL) return RELEASED;
    return self->getButtonState(self, button);
}

buttonState controllerDevice_GetMouseButtonState(ControllerDevice self, mouseButton button)
{
    assert(self);
    if (self->getMouseButtonState == NULL) return RELEASED;
    return self->getMouseButtonState(self, button);
}

vector controllerDevice_GetMousePosition(ControllerDevice self)
{
    assert(self);
    if (self->getMousePosition == NULL) return (vector){ -1, -1 };
    return self->getMousePosition(self);
}