// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#ifndef _pvconf_h_
#define _pvconf_h_

#include <limits.h>

typedef unsigned char colorId;
#define COLORID_MAX UCHAR_MAX

#define MAX_SPRITE_WIDTH        32
#define MAX_SPRITE_HEIGHT       32
#define MAX_PIXELS_PER_SPRITE   (MAX_SPRITE_WIDTH * MAX_SPRITE_HEIGHT)

#define MAX_SPRITE_PAGES        16
#define MAX_SPRITES_PER_PAGE    UCHAR_MAX
#define MAX_SPRITES             (MAX_SPRITE_PAGES * MAX_SPRITES_PER_PAGE)

typedef unsigned short spriteId;

#endif

