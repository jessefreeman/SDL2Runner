// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <SDL.h>
#include "util.h"
#include "controllerdevice.h"
#include "sdlcontrollerdevice.h"

static buttonState sdlControllerDevice_GetButtonState(SDLControllerDevice self, button button);
static vector sdlControllerDevice_GetMousePosition(SDLControllerDevice self);

void sdlButtonMap_Init(SDLButtonMap self)
{
    assert(self);
    memset(self, 0, sizeof(sdlButtonMap));
}

void sdlButtonMap_MapButton(SDLButtonMap self, button button, SDL_Keycode keycode)
{
    assert(self);
    self->map[(int)button] = keycode;
}

void sdlControllerDevice_Init(SDLControllerDevice self, sdlButtonMap buttonMap)
{
    assert(self);
    memset(self, 0, sizeof(sdlControllerDevice));
    self->base.getButtonState = sdlControllerDevice_GetButtonState;
    self->base.getMousePosition = sdlControllerDevice_GetMousePosition;
    self->buttonMap = buttonMap;
    self->mousePos.x = -1;
    self->mousePos.y = -1;
}

void sdlController_KeyStateChanged(SDLControllerDevice self,
    SDL_Keycode keycode, buttonState state)
{
    assert(self);
    for (int i = 0; i < arraylen(self->buttonMap.map); i++)
    {
        if (self->buttonMap.map[i] != keycode) continue;

        self->buttonStates.map[i] = state;
    }
}

void sdlController_SetMousePosition(SDLControllerDevice self,
    int x, int y)
{
    assert(self);
    self->mousePos.x = x;
    self->mousePos.y = y;
}

static buttonState sdlControllerDevice_GetButtonState(SDLControllerDevice self, button button)
{
    assert(self);
    return self->buttonStates.map[button];
}

static vector sdlControllerDevice_GetMousePosition(SDLControllerDevice self)
{
    assert(self);
    return self->mousePos;
}