// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "cartridge.h"
#include "demoscartridge.h"

// these are all arbitrary limits
#define MAX_FONT_NAME 64
#define MAX_NUM_FONTS 4
#define MAX_FILE_PATH_LEN   2048

typedef struct pixel {
    colorData color;
    byte a;
} pixel;

typedef struct font {
    char name[MAX_FONT_NAME];
    char *fileName[MAX_FILE_PATH_LEN];
} font;

typedef struct demosCartridge {
    cartridge base; // must be first
    char scriptFileName[MAX_FILE_PATH_LEN];
    char colorsFileName[MAX_FILE_PATH_LEN];
    char spriteFileName[MAX_FILE_PATH_LEN];
    font fonts[MAX_NUM_FONTS];
} demosCartridge;

static colorData *demosCartridge_GetColors(DemosCartridge self, int *colorsLen);
static TextureData *demosCartridge_GetSprites(DemosCartridge self, 
    int spriteWidth, int spriteHeight, GetColorRef getColorRef, int *spriteLen);
static const char *demosCartridge_GetScript(DemosCartridge self, int *scriptLen);

DemosCartridge demosCartridge_Create()
{
    DemosCartridge self = NULL;

    self = (DemosCartridge)calloc(1, sizeof(demosCartridge));
    if (self == NULL)
        return NULL;

    self->base.destroy = demosCartridge_Destroy;
    self->base.getColors = demosCartridge_GetColors;
    self->base.getSprites = demosCartridge_GetSprites;
    self->base.getScript = demosCartridge_GetScript;

    return self;
}

void demosCartridge_Destroy(DemosCartridge self)
{
    assert(self);
    memset(self, 0, sizeof(demosCartridge));
    free(self);
}

void demosCartridge_SetColors(DemosCartridge self, const char *fileName)
{
    assert(self);
    assert(fileName);
    strncpy(self->colorsFileName, fileName, sizeof(self->colorsFileName) - 1);
}

// TODO: this is named to support a single sprite file, should support more?
void demosCartridge_SetSprites(DemosCartridge self, const char *fileName)
{
    assert(self);
    assert(fileName);
    strncpy(self->spriteFileName, fileName, sizeof(self->colorsFileName) - 1);
}

void demosCartridge_AddFont(DemosCartridge self, const char *fileName, const char *fontName)
{
    assert(self);
}

void demosCartridge_SetScript(DemosCartridge self, const char *fileName)
{
    assert(self);
    assert(fileName);
    strncpy(self->scriptFileName, fileName, sizeof(self->colorsFileName) - 1);
}

static colorData *demosCartridge_GetColors(DemosCartridge self, int *colorsLen)
{
    assert(self);
    assert(colorsLen);
    *colorsLen = 0;
    if (self->colorsFileName == NULL)
        return NULL;
    int x = 0, y = 0, n = 0;
    unsigned char *data = stbi_load(self->colorsFileName, &x, &y, &n, 4);
    if (data == NULL)
        return NULL;

    int pixelsLen = x * y;
    int dataLen = pixelsLen * 4;
    colorData *colors = calloc(pixelsLen, sizeof(colorData));
    for (int p = 0, i = 0; p < dataLen; p += 4, i++) // cause 4 pixels duh
    {
        pixel *currentPixel = &data[p];
        colors[i] = currentPixel->color;
    }

    stbi_image_free(data);
    *colorsLen = pixelsLen;
    return colors;
}

static TextureData *demosCartridge_GetSprites(DemosCartridge self, 
    int spriteWidth, int spriteHeight, GetColorRef getColorRef, int *spritesLen)
{
    assert(self);
    assert(spriteWidth > 0);
    assert(spriteHeight > 0);
    assert(getColorRef);
    assert(spritesLen);

    *spritesLen = 0;
    if (self->spriteFileName == NULL)
        return NULL;

    int imgWidth = 0, imageHeight = 0, n = 0;
    unsigned char *data = stbi_load(self->spriteFileName, &imgWidth, &imageHeight, &n, 4);
    if (data == NULL)
        return NULL;

    int pixelsLen = imgWidth * imageHeight;
    int dataLen = pixelsLen * 4;
    int *colorRefs = calloc(pixelsLen, sizeof(pixelsLen));
    for (int p = 0, i = 0; p < dataLen; p += 4, i++) // cause 4 pixels duh
    {
        pixel *currentPixel = &data[p];
        colorRefs[i] = currentPixel->a < 255 
            ? -1
            : func_Invoke(getColorRef, currentPixel->color);
    }

    stbi_image_free(data);

    int cols = imgWidth / spriteWidth;
    int rows = imageHeight / spriteHeight;
    int numSprites = cols * rows;

    TextureData *sprites = (TextureData *)calloc(numSprites, sizeof(TextureData));
    int spriteIdx = 0;
    for (int r = 0; r < rows; r++)
        for (int c = 0; c < cols; c++)
        {
            sprites[spriteIdx] = textureData_Create(spriteWidth, spriteHeight);
            int tIdx = 0;
            for (int y = (r * spriteHeight); y < (r * spriteHeight) + spriteHeight; y++)
                for (int x = (c * spriteWidth); x < (c * spriteWidth) + spriteWidth; x++)
                {
                    int cIdx = (y * imgWidth) + x;
                    int c = colorRefs[cIdx];
                    textureData_SetPixelAt(sprites[spriteIdx], tIdx, c);
                    tIdx++;
                }
            spriteIdx++;
        }

    *spritesLen = numSprites;
    return sprites;
}

static const char *demosCartridge_GetScript(DemosCartridge self, int *scriptLen)
{
    assert(self);
    assert(scriptLen);
    *scriptLen = 0;
    if (self->scriptFileName == NULL)
        return NULL;
    FILE *fp = fopen(self->scriptFileName, "r");
    fseek(fp, 0, SEEK_END);
    long fsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char *data = calloc(1, fsize + 1);
    fread(data, fsize, 1, fp);
    fclose(fp);
    *scriptLen = fsize;
    return data;
}