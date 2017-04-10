// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#ifndef _sdlcontrollerdevice_h_
#define _sdlcontrollerdevice_h_

#include "controllerdevice.h"

typedef union sdlButtonMap {
    struct {
        SDL_Keycode up;
        SDL_Keycode down;
        SDL_Keycode left;
        SDL_Keycode right;
        SDL_Keycode a;
        SDL_Keycode b;
        SDL_Keycode select;
        SDL_Keycode start;
    } keys;
    SDL_Keycode map[8];
} sdlButtonMap;

typedef sdlButtonMap *SDLButtonMap;

void sdlButtonMap_Init(SDLButtonMap self);

void sdlButtonMap_MapButton(SDLButtonMap self, button button, SDL_Keycode keycode);

typedef struct sdlControllerDevice {
    controllerDevice base; // must be first
    sdlButtonMap buttonMap;
    union {
        struct {
            buttonState up;
            buttonState down;
            buttonState left;
            buttonState right;
            buttonState a;
            buttonState b;
            buttonState select;
            buttonState start;
        } buttons;
        buttonState map[8];
    } buttonStates;
} sdlControllerDevice;

typedef sdlControllerDevice *SDLControllerDevice;

void sdlControllerDevice_Init(SDLControllerDevice self, sdlButtonMap);

#endif
