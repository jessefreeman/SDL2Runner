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
    TilemapChip tilemapChip;
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
    self->tilemapChip = (TilemapChip)func_Invoke(getChip, nameof(TilemapChip));

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
    colorChip_SetBackgroundColorId(self->colorChip, id);
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
    int total = strlen(text);
    // TODO: dont alloc this.
    int *spriteRefs = (int *)calloc(strlen(text), sizeof(int));
    fontChip_ConvertTextToSprites(self->fontChip, text, fontName, spriteRefs);
    for (int i = 0; i < total; i++)
        apiBridge_DrawTile(self, spriteRefs[i], column + i, row, colorOffset);
    free(spriteRefs);
}

void apiBridge_DrawSprite(ApiBridge self,
    spriteId id,
    int x,
    int y,
    bool flipH,
    bool flipV,
    bool aboveBG,
    int colorOffset)
{
    assert(self);
    assert(self->spriteChip);
    assert(self->displayChip);

    Sprite sprite = spriteChip_GetSprite(self->spriteChip, id);
    if (sprite != NULL)
    {
        // TODO: implement all the features of draw api
        displayChip_Draw(self->displayChip, sprite, x, y);
    }
}

void apiBridge_DrawSprites(ApiBridge self,
    spriteId ids[],
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
    int spriteWidth = spriteChip_GetSpriteWidth(self->spriteChip);
    for (int i = 0; i < idsLen; i++)
    {
        Sprite sprite = spriteChip_GetSprite(self->spriteChip, ids[i]);
        if (sprite != NULL)
        {
            int newX = (i % width) * spriteWidth + x;
            int newY = (i / width) * spriteWidth + y;
            displayChip_Draw(self->displayChip, sprite, newX, newY);
        }
    }
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
    // TODO: these other parameters probably are supposed to do things :p
    displayChip_Clear(self->displayChip);
    // TODO: this probably doesn't work like this, and needs more params. To come!
    displayChip_DrawTilemap(self->displayChip);
}

void apiBridge_DrawTile(ApiBridge self,
    int tileID, 
    int column, 
    int row, 
    int colorOffset)
{
    assert(self);
    tilemapChip_UpdateSpriteAt(self->tilemapChip, column, row, tileID);
    tilemapChip_UpdatePaletteAt(self->tilemapChip, column, row, colorOffset);
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
    apiBridge_DrawSpriteText(self, text, x, y, fontName, offset);
}

void apiBridge_DrawSpriteText(ApiBridge self,
    const char *text,
    int x,
    int y,
    const char *fontName,
    int colorOffset)
{
    assert(self);
    // TODO: working again
    //TextureData pixelData = fontChip_ConvertTextToTexture(self->fontChip, text, fontName, 0);
    /*if (pixelData != NULL)
    {
        displayChip_Draw(self->displayChip, pixelData, x, y);
        textureData_Destroy(pixelData);
    }*/
}