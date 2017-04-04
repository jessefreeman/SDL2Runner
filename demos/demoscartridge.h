// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#ifndef _demoscartridge_h_
#define _demoscartridge_h_

typedef struct demosCartridge *DemosCartridge;

DemosCartridge demosCartridge_Create();

void demosCartridge_Destroy(DemosCartridge self);

void demosCartridge_SetColorsFile(DemosCartridge self, const char *fileName);

void demosCartridge_SetSpritesFile(DemosCartridge self, const char *fileName);

void demosCartridge_AddFontFile(DemosCartridge self, const char *fileName, const char *fontName);

void demosCartridge_SetScriptFile(DemosCartridge self, const char *fileName);

#endif

