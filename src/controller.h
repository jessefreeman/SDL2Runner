// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#ifndef _controller_h_
#define _controller_h_

typedef struct controller {
    void(*action)(Obj);
} controller;

typedef struct controller *Controller;

#endif
