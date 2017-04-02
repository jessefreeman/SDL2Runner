// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#include "util.h"

Obj nvl(Obj a, Obj b)
{
    return a == NULL ? b : a;
}

Obj nullifsame(Obj a, Obj b)
{
    return a == b ? NULL : a;
}