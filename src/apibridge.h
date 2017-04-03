// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#ifndef _apibridge_h_
#define _apibridge_h_

#include <stdbool.h>
#include "chip.h"

typedef struct apiBridge *ApiBridge;

ApiBridge apiBridge_Create(GetChip getChip);

void apiBridge_Destroy(ApiBridge self);

void apiBridge_ChangeBackgroundColor(ApiBridge self,
    int id);

// Deprecated
void apiBridge_RebuildScreenBuffer(ApiBridge self);

// Deprecated
void apiBridge_DrawFontToBuffer(ApiBridge self,
    const char *text, 
    int column, 
    int row, 
    const char *fontName, 
    int letterSpacing, 
    int offset);

void apiBridge_DrawTileText(ApiBridge self,
    const char *text,
    int column,
    int row,
    const char *fontName,
    int colorOffset);

void apiBridge_DrawSprite(ApiBridge self,
    int id,
    int x,
    int y,
    bool flipH,
    bool flipV,
    bool aboveBG,
    int colorOffset);

void apiBridge_DrawSprites(ApiBridge self,
    int ids[],
    int idsLen,
    int x,
    int y,
    int width,
    bool flipH,
    bool flipV,
    bool aboveBG,
    int colorOffset);

void apiBridge_DrawScreenBuffer(ApiBridge self,
    int x, 
    int y, 
    int width, 
    int height, 
    int offsetX, 
    int offsetY);

// Deprecated
void apiBridge_DrawFont(ApiBridge self,
    const char *text, 
    int x, 
    int y, 
    const char *fontName, 
    int letterSpacing, 
    int offset);

void apiBridge_DrawSpriteText(ApiBridge self,
    const char *text, 
    int x, 
    int Y, 
    const char *fontName, 
    int colorOffset);

#endif
