// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

// As a side note, this is a 'func'y (get it?) experiement of mine and probably doesn't belong here.

#ifndef _func_h_
#define _func_h_

#include "types.h"

typedef struct { Obj closure; Obj invoke; } _func;

#define _funcSize sizeof(_func)

static _func _defaultFunc;

#define Func(TReturn) struct { Obj closure; TReturn (*invoke)(Obj); } *

#define Func1(TParam1, TReturn) struct { Obj closure; TReturn (*invoke)(Obj, TParam1); } *

#define func_Create(closure, func) nullifsame(memcpy(nvl(calloc(1, _funcSize), &_defaultFunc), &((_func){ closure, func }), _funcSize), &_defaultFunc)

#define func_Destroy(func) free(func)

#define func_Invoke(func, ...) (func)->invoke((func)->closure, __VA_ARGS__)

#endif
