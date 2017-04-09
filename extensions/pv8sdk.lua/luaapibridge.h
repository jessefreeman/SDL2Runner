// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#ifndef _luaapibridge_h_
#define _luaapibridge_h_

#include "apibridge.h"
#include "lua.h"

void loadApiBridge(lua_State *L);

void setApi(ApiBridge api);

#endif

