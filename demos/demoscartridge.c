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

static colorData *demosCartridge_GetColorsData(DemosCartridge self, int *len);
static colorData *demosCartridge_GetSpritesData(DemosCartridge self, int *width, int *height);
static char *demosCartridge_GetScriptData(DemosCartridge self, int *len);

DemosCartridge demosCartridge_Create()
{
    DemosCartridge self = NULL;

    self = (DemosCartridge)calloc(1, sizeof(demosCartridge));
    if (self == NULL)
        return NULL;

    self->base.destroy = demosCartridge_Destroy;
    self->base.getColorsData = demosCartridge_GetColorsData;
    self->base.getSpritesData = demosCartridge_GetSpritesData;
    self->base.getScriptData = demosCartridge_GetScriptData;

    return self;
}

void demosCartridge_Destroy(DemosCartridge self)
{
    assert(self);
    memset(self, 0, sizeof(demosCartridge));
    free(self);
}

void demosCartridge_SetColorsFile(DemosCartridge self, const char *fileName)
{
    assert(self);
    assert(fileName);
    strncpy(self->colorsFileName, fileName, sizeof(self->colorsFileName) - 1);
}

// TODO: this is named to support a single sprite file, should support more?
void demosCartridge_SetSpritesFile(DemosCartridge self, const char *fileName)
{
    assert(self);
    assert(fileName);
    strncpy(self->spriteFileName, fileName, sizeof(self->colorsFileName) - 1);
}

void demosCartridge_AddFontFile(DemosCartridge self, const char *fileName, const char *fontName)
{
    assert(self);
}

void demosCartridge_SetScriptFile(DemosCartridge self, const char *fileName)
{
    assert(self);
    assert(fileName);
    strncpy(self->scriptFileName, fileName, sizeof(self->colorsFileName) - 1);
}

static colorData *demosCartridge_GetColorsData(DemosCartridge self, int *len)
{
    assert(self);
    assert(len);
    *len = 0;
    if (self->colorsFileName == NULL)
        return NULL;
    int x = 0, y = 0, n = 0;
    unsigned char *data = stbi_load(self->colorsFileName, &x, &y, &n, 4);
    if (data == NULL)
        return NULL;

    int pixelsLen = x * y;
    int dataLen = pixelsLen * 4;
    colorData *colors = (colorData *)calloc(pixelsLen, sizeof(colorData));
    for (int p = 0, i = 0; p < dataLen; p += 4, i++) // cause 4 pixels duh
    {
        pixel *currentPixel = &data[p];
        colors[i] = currentPixel->a == 255
            ? currentPixel->color
            : (colorData) { 255, 0, 255 };
    }

    stbi_image_free(data);
    *len = pixelsLen;
    return colors;
}

static colorData *demosCartridge_GetSpritesData(DemosCartridge self, int *width, int *height)
{
    assert(self);
    assert(width);
    assert(height);

    *width = 0;
    *height = 0;

    if (self->spriteFileName == NULL)
        return NULL;
    
    int x = 0, y = 0, n = 0;
    unsigned char *data = stbi_load(self->spriteFileName, &x, &y, &n, 4);
    if (data == NULL)
        return NULL;

    int pixelsLen = x * y;
    int dataLen = pixelsLen * 4;
    colorData *colors = calloc(pixelsLen, sizeof(colorData));
    for (int p = 0, i = 0; p < dataLen; p += 4, i++) // cause 4 pixels duh
    {
        pixel *currentPixel = &data[p];
        colors[i] = currentPixel->a == 255
            ? currentPixel->color
            : (colorData) { 255, 0, 255 };
    }

    stbi_image_free(data);

    *width = x;
    *height = y;
    return colors;
}

static char *demosCartridge_GetScriptData(DemosCartridge self, int *len)
{
    assert(self);
    assert(len);
    *len = 0;
    if (self->scriptFileName == NULL)
        return NULL;
    FILE *fp = fopen(self->scriptFileName, "r");
    fseek(fp, 0, SEEK_END);
    long fsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char *data = calloc(1, fsize + 1);
    fread(data, fsize, 1, fp);
    fclose(fp);
    *len = fsize;
    return data;
}