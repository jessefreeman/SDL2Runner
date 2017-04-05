// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#ifndef _luagamechip_h_
#define _luagamechip_h_

typedef struct luaGameChip *LuaGameChip;

LuaGameChip luaGameChip_Create(const char *script, int len);

#endif