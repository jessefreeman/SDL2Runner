// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "chip.h"
#include "spritechip.h"
#include "textureData.h"
#include "fontchip.h"

#define MAX_FONTS 4

typedef struct font {
    char name[64];
    int map[96];
} *Font;

typedef struct fontChip {
    chip base;
    struct font fonts[MAX_FONTS];
    SpriteChip spriteChip;
} fontChip;
//
static void fontChip_Destroy(FontChip self);
static void fontChip_Init(FontChip self, GetChip getChip);
static Font fontChip_FindFont(FontChip self, const char *fontName);

FontChip fontChip_Create()
{
    FontChip self = NULL;

    self = (FontChip)calloc(1, sizeof(fontChip));
    if (self == NULL)
        return self;

    strncpy(self->base.name, nameof(FontChip), sizeof(self->base.name) - 1);
    self->base.destroy = fontChip_Destroy;
    self->base.init = fontChip_Init;

    return self;
}

static void fontChip_Destroy(FontChip self)
{
    assert(self);
    memset(self, 0, sizeof(struct fontChip));
    free(self);
}

static void fontChip_Init(FontChip self, GetChip getChip)
{
    assert(self);
    self->spriteChip = (SpriteChip)func_Invoke(getChip, nameof(SpriteChip));
}

void fontChip_AddFont(FontChip self, const char *name, int mapLen, int *map)
{
    assert(self);
    Font slot = NULL;
    for (int i = 0; i < MAX_FONTS; i++)
        if (self->fonts[i].name[0] == '\0')
        {
            slot = &self->fonts[i];
            break;
        }
    slot = slot == NULL ? &self->fonts[0] : slot;
    strncpy(slot->name, name, sizeof(slot->name) - 1);
    memset(slot->map, 0, sizeof(slot->map));
    memcpy(slot->map, map, min(mapLen * sizeof(int), sizeof(slot->map)));
}

void fontChip_ConvertTextToSprites(FontChip self, const char *text, const char *fontName, int *spriteRefs)
{
    assert(self);
    assert(text);
    assert(spriteRefs);

    Font font = fontChip_FindFont(self, fontName);
    if (font == NULL)
        return;

    for (int i = 0; i < strlen(text); i++)
        spriteRefs[i] = font->map[text[i] - ' '];
}

TextureData fontChip_ConvertTextToTexture(FontChip self, const char *text, const char *fontName, int letterSpacing)
{
    assert(self);
    assert(self->spriteChip);

    Font font = fontChip_FindFont(self, fontName);
    if (font == NULL)
        return NULL;

    // TODO: assuming good, single line text for now, don't.

    int cWidth = spriteChip_GetSpriteWidth(self->spriteChip);
    int textureWidth = (cWidth + letterSpacing) * strlen(text); // should be len of longest line

    int cHeight = spriteChip_GetSpriteHeight(self->spriteChip);
    int textureHeight = cHeight; // * totalLines;

                                 // TODO: this can fail
    TextureData outputTexture = textureData_Create(textureWidth, textureHeight, false);

    // TODO: should handle multiple lines
    for (int x = 0; x < strlen(text); x++)
    {
        int spriteIdx = font->map[text[x] - ' '];
        TextureData cTexture = spriteChip_GetSprite(self->spriteChip, spriteIdx);
        textureData_CopyToAtPos(cTexture, outputTexture, x * (cWidth + letterSpacing), 0);
    }

    return outputTexture;
}

static Font fontChip_FindFont(FontChip self, const char *fontName)
{
    assert(self);
    for (int i = 0; i < MAX_FONTS; i++)
        if (strcmp(self->fonts[i].name, fontName) == 0)
            return &self->fonts[i];
    return NULL;
}
