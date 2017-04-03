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

struct font {
    char name[64];
    int map[96];
};

//typedef struct font font;
//typedef font *Font;
//
typedef struct fontChip {
    chip base;
    //font fonts[MAX_FONTS];
} fontChip;
//
static void fontChip_Destroy(FontChip self);
//static Font fontChip_FindFont(FontChip self, const char *fontName);
//
FontChip fontChip_Create()
{
    FontChip self = NULL;

    self = (FontChip)calloc(1, sizeof(fontChip));
    if (self == NULL)
        return self;

    strncpy(self->base.name, nameof(FontChip), sizeof(self->base.name) - 1);
    self->base.destroy = fontChip_Destroy;

    return self;
}

static void fontChip_Destroy(FontChip self)
{
    assert(self);
    memset(self, 0, sizeof(struct fontChip));
    free(self);
}

void fontChip_AddFont(FontChip self, const char *name, int mapLen, int *map)
{
}
//    assert(self);
//    Font slot = NULL;
//    for (int i = 0; i < MAX_FONTS; i++)
//        if (self->fonts[i].name[0] == '\0')
//        {
//            slot = &self->fonts[i];
//            break;
//        }
//    slot = slot == NULL ? &self->fonts[0] : slot;
//    strncpy(slot->name, name, sizeof(slot->name) - 1);
//    memset(slot->map, 0, sizeof(slot->map));
//    memcpy(slot->map, map, MIN(mapLen * sizeof(int), sizeof(slot->map)));
//}
//
TextureData fontChip_ConvertTextToTexture(FontChip self, const char *text, const char *fontName, int letterSpacing)
{
    return NULL;
}

//// TODO: break hard dependency on SpriteChip?
//TextureData fontChip_ConvertTextToTexture(FontChip self, SpriteChip spriteChip, const char *text, const char *fontName, int letterSpacing)
//{
//    assert(self);
//    Font font = fontChip_FindFont(self, fontName);
//    if (font == NULL)
//        return NULL;
//
//    // TODO: assuming good, single line text for now, don't.
//
//    int cWidth = spriteChip_GetSpriteWidth(spriteChip);
//    int textureWidth = (cWidth + letterSpacing) * strlen(text); // should be len of longest line
//
//    int cHeight = spriteChip_GetSpriteHeight(spriteChip);
//    int textureHeight = cHeight; // * totalLines;
//
//                                 // TODO: this can fail
//    TextureData outputTexture = textureData_Create(textureWidth, textureHeight, false);
//
//    // TODO: should handle multiple lines
//    for (int x = 0; x < strlen(text); x++)
//    {
//        int spriteIdx = font->map[text[x] - ' '];
//        TextureData cTexture = spriteChip_GetSpriteAt(spriteChip, spriteIdx);
//        textureData_CopyToPos(cTexture, outputTexture, x * (cWidth + letterSpacing), 0);
//    }
//
//    return outputTexture;
//}
//
//static Font fontChip_FindFont(FontChip self, const char *fontName)
//{
//    assert(self);
//    for (int i = 0; i < MAX_FONTS; i++)
//        if (strcmp(self->fonts[i].name, fontName) == 0)
//            return &self->fonts[i];
//    return NULL;
//}
//
//void FontChipLoader(PixelVisionEngine engine)
//{
//    FontChip chip = fontChip_Create();
//    pixelVisionEngine_RegisterChip(engine,
//        NAMEOF(FontChip),
//        chip,
//        fontChip_Destroy,
//        NULL,
//        NULL,
//        NULL);
//}
