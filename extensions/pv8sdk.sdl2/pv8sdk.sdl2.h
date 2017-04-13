// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#ifndef _pv8sdk_sdl2_h_
#define _pv8sdk_sdl2_h_

#define FRAMES_PER_SECOND 1

#include <SDL.h>
#include "sdldisplaydevice.h"
#include "sdlcontrollerdevice.h"
#include "gameconsole.h"

// TODO: this isn't at all elegeant, need to rethink how SDL interacts with PV8
void sdl_runGame(GameConsole console,
    SDLControllerDevice controller1,
    SDLControllerDevice controller2);

#endif
