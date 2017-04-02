// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#ifndef _demoscartridge_h_
#define _demoscartridge_h_

typedef struct demosCartridge *DemosCartridge;

DemosCartridge demosCartridge_Create();

void demosCartridge_Destroy(DemosCartridge self);

void demosCartridge_AddColors(DemosCartridge self, const char *fileName);

void demosCartridge_AddSprites(DemosCartridge self, const char *fileName);

void demosCartridge_AddFont(DemosCartridge self, const char *fileName, const char *fontName);

void demosCartridge_AddCode(DemosCartridge self, const char *fileName);

#endif

