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
static buttonState sdlControllerDevice_GetMouseButtonState(SDLControllerDevice self, mouseButton mouseButton);
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
    self->base.getMouseButtonState = sdlControllerDevice_GetMouseButtonState;
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

static buttonState sdlControllerDevice_GetMouseButtonState(SDLControllerDevice self, mouseButton mouseButton)
{
    assert(self);
    unsigned int map = SDL_GetMouseState(NULL, NULL);

    buttonState state = RELEASED;
    switch (mouseButton)
    {
    case MOUSE_LEFT:
        state = map & SDL_BUTTON_LEFT;
        break;

    case MOUSE_RIGHT:
        // wtf? my mouse maps to x1?
        state = ((map & SDL_BUTTON_RMASK) || (map & SDL_BUTTON_X1MASK)) ? PRESSED : RELEASED; 
        break;

    case MOUSE_MIDDLE:
        state = (map & SDL_BUTTON_MIDDLE) ? PRESSED : RELEASED;
        break;

    default:
        break;
    }

    return state;
}

static vector sdlControllerDevice_GetMousePosition(SDLControllerDevice self)
{
    assert(self);
    return self->mousePos;
}