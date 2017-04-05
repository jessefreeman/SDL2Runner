// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#ifndef _controllerdevice_h_
#define _controllerdevice_h_

typedef struct controllerDevice {
    void(*action)(Obj);
} controllerDevice;

typedef struct controllerDevice *ControllerDevice;

#endif
