// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#include <assert.h>
#include <stdlib.h>
#include "util.h"
#include "chip.h"
#include "texturedata.h"
#include "colorchip.h"
#include "spritechip.h"
#include "tilemapchip.h"
#include "displaychip.h"

typedef struct displayChip {
    chip base; // must be first
    TextureData texture;
    ColorChip colorChip;
    SpriteChip spriteChip;
    TilemapChip tilemapChip;
    TextureData tilemapBuffer;
    int width;
    int height;
    int tilemapRows;
    int tilemapCols;
    int sizeX;
    int sizeY;
} displayChip;

static void displayChip_Destroy(DisplayChip self);
static void displayChip_Init(DisplayChip self, GetChip getChip);

DisplayChip displayChip_Create(int width, int height)
{
    DisplayChip self = NULL;

    self = (DisplayChip)calloc(1, sizeof(displayChip));
    if (self == NULL)
        return NULL;

    strncpy(self->base.name, nameof(DisplayChip), sizeof(self->base.name) - 1);
    self->base.destroy = displayChip_Destroy;
    self->base.init = displayChip_Init;
    self->texture = textureData_Create(width, height);
    if (self->texture == NULL)
    {
        free(self);
        return NULL;
    }
    self->width = width;
    self->height = height;

    self->tilemapBuffer = textureData_Create(width, height);
    if (self->tilemapBuffer == NULL)
    {
        textureData_Destroy(self->texture);
        free(self);
        return NULL;
    }

    return self;
}

static void displayChip_Destroy(DisplayChip self)
{
    assert(self);
    textureData_Destroy(self->texture);
    textureData_Destroy(self->tilemapBuffer);
    memset(self, 0, sizeof(displayChip));
    free(self);
}

static void displayChip_Init(DisplayChip self, GetChip getChip)
{
    assert(self);
    self->colorChip = (ColorChip)func_Invoke(getChip, nameof(ColorChip));
    self->spriteChip = (SpriteChip)func_Invoke(getChip, nameof(SpriteChip));
    self->tilemapChip = (TilemapChip)func_Invoke(getChip, nameof(TilemapChip));
    self->tilemapRows = tilemapChip_GetRows(self->tilemapChip);
    self->tilemapCols = tilemapChip_GetColumns(self->tilemapChip);
    self->sizeX = self->width / self->tilemapCols;
    self->sizeY = self->height / self->tilemapRows;
}

int displayChip_GetDisplayWidth(DisplayChip self)
{
    assert(self);
    return self->width;
}

int displayChip_GetDisplayHeight(DisplayChip self)
{
    assert(self);
    return self->height;
}

int displayChip_GetPixelCount(DisplayChip self)
{
    assert(self);
    return textureData_GetPixelCount(self->texture);
}

colorId displayChip_GetPixel(DisplayChip self, int idx)
{
    assert(self);
    return textureData_GetPixel(self->texture, idx);
}

void displayChip_Clear(DisplayChip self)
{
    assert(self);

    int backgroundColor = self->colorChip != NULL
        ? colorChip_GetBackgroundColorId(self->colorChip)
        : -1;

    textureData_Clear(self->texture, backgroundColor);
}

void displayChip_DrawSprite(DisplayChip self, Sprite sprite, int x, int y)
{
    assert(self);
    sprite_CopyToTextureAtPos(sprite, self->texture, x, y);
}

void displayChip_DrawTexture(DisplayChip self, TextureData texture, int x, int y)
{
    assert(self);
    textureData_CopyToAtPos(texture, self->texture, x, y);
}

void displayChip_DrawTilemap(DisplayChip self)
{
    assert(self);    
    if (tilemapChip_IsInvalidated(self->tilemapChip))
    {
        textureData_Clear(self->tilemapBuffer, colorChip_GetBackgroundColorId(self->colorChip));
        for (int r = 0; r < self->tilemapRows; r++)
        {
            for (int c = 0; c < self->tilemapCols; c++)
            {
                int ref = tilemapChip_GetSpriteRefAt(self->tilemapChip, c, r);
                if (ref >= 0)
                {
                    Sprite sprite = spriteChip_GetSprite(self->spriteChip, ref);
                    sprite_CopyToTextureAtPos(sprite, self->tilemapBuffer, c * self->sizeX, r * self->sizeY);
                }
            }
        }
        tilemapChip_ResetInvalidated(self->tilemapChip);
    }
    textureData_CopyTo(self->tilemapBuffer, self->texture);
}