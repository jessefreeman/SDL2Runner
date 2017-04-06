// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#include <assert.h>
#include <stdio.h>
#include "stb_image.h"
#include "types.h"
#include "colorchip.h"
#include "importutil.h"

typedef struct pixel {
    colorData color;
    byte a;
} pixel;

colorData *importImageFromFile(const char *filePath, int *width, int *height)
{
    assert(filePath);
    assert(width);
    assert(height);

    *width = 0;
    *height = 0;

    int x = 0, y = 0, n = 0;
    unsigned char *data = stbi_load(filePath, &x, &y, &n, 4);
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

char *importTextFromFile(const char *filePath, int *len)
{
    assert(filePath);
    assert(len);
    *len = 0;
    FILE *fp = fopen(filePath, "r");
    fseek(fp, 0, SEEK_END);
    long fsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char *data = calloc(1, fsize + 1);
    fread(data, fsize, 1, fp);
    fclose(fp);
    *len = fsize;
    return data;
}

TextureData *importSpriteSheetFromFile(const char *filePath, ColorChip colorChip)
{
    int width = 0;
    int height = 0;
    colorData *pixels = importImageFromFile(filePath, &width, &height);
    TextureData spriteSheet = textureData_Create(width, height);
    spriteSheet = colorChip_MapPixelDataToTexture(colorChip, width, height, pixels, spriteSheet);
    free(pixels);
    pixels = NULL;
    return spriteSheet;
}