// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#ifndef _util_h_
#define _util_h_

#include <string.h>
#include "types.h"

#define nameof(x) #x

#define strequals(a, b) (strcmp((a), (b)) == 0)

#define coordsToIdx(x, y, width) (((y) * (width)) + ((x) % (width)))

#ifndef min
#define min(x,y) ((x) < (y) ? (x) : (y))
#endif

#ifndef max
#define max(x,y) ((x) > (y) ? (x) : (y))
#endif

// returns first Obj if not NULL, otherwise returns second
Obj nvl(Obj, Obj);

// returns NULL if the Objs are the same, otherwise returns first param
Obj nullifsame(Obj, Obj);

#endif

