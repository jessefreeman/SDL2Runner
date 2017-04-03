// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#include <assert.h>
#include <stdlib.h>
#include "util.h"
#include "chips.h"
#include "apibridge.h"

typedef struct apiBridge {
    ColorChip colorChip;
    DisplayChip displayChip;
    SpriteChip spriteChip;
    FontChip fontChip;
} apiBridge;

ApiBridge apiBridge_Create(GetChip getChip)
{
    ApiBridge self = NULL;

    self = (ApiBridge)calloc(1, sizeof(apiBridge));
    if (self == NULL) return NULL;

    self->colorChip = (ColorChip)func_Invoke(getChip, nameof(ColorChip));
    self->displayChip = (DisplayChip)func_Invoke(getChip, nameof(DisplayChip));
    self->spriteChip = (SpriteChip)func_Invoke(getChip, nameof(SpriteChip));
    self->fontChip = (FontChip)func_Invoke(getChip, nameof(FontChip));

    return self;
}

void apiBridge_Destroy(ApiBridge self)
{
    assert(self);
    memset(self, 0, sizeof(apiBridge));
    free(self);
}

void apiBridge_ChangeBackgroundColor(ApiBridge self,
    int id)
{
    assert(self);
}

// Deprecated
void apiBridge_RebuildScreenBuffer(ApiBridge self)
{
    assert(self);
}

// Deprecated
void apiBridge_DrawFontToBuffer(ApiBridge self,
    const char *text,
    int column,
    int row,
    const char *fontName,
    int letterSpacing,
    int offset)
{
    assert(self);
}

void apiBridge_DrawTileText(ApiBridge self,
    const char *text,
    int column,
    int row,
    const char *fontName,
    int colorOffset)
{
    assert(self);
}

void apiBridge_DrawSprite(ApiBridge self,
    int id,
    int x,
    int y,
    bool flipH,
    bool flipV,
    bool aboveBG,
    int colorOffset)
{
    assert(self);
}

void apiBridge_DrawSprites(ApiBridge self,
    int ids[],
    int idsLen,
    int x,
    int y,
    int width,
    bool flipH,
    bool flipV,
    bool aboveBG,
    int colorOffset)
{
    assert(self);
}

void apiBridge_DrawScreenBuffer(ApiBridge self,
    int x,
    int y,
    int width,
    int height,
    int offsetX,
    int offsetY)
{
    assert(self);
}

// Deprecated
void apiBridge_DrawFont(ApiBridge self,
    const char *text,
    int x,
    int y,
    const char *fontName,
    int letterSpacing,
    int offset)
{
    assert(self);
}

void apiBridge_DrawSpriteText(ApiBridge self,
    const char *text,
    int x,
    int Y,
    const char *fontName,
    int colorOffset)
{
    assert(self);
}