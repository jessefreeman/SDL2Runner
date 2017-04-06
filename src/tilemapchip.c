// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include "util.h"
#include "chip.h"
#include "texturedata.h"
#include "layer.h"
#include "tilemapchip.h"

typedef struct tilemapChip {
    chip base; // must be first
    TextureData texture;
    int columns;
    int rows;
    int tilesPerLayer;
    int *layers[MaxLayers]; // layers with sprite references
    bool invalid;
    /*ColorChip colorChip;*/
} tilemapChip;

static void tilemapChip_Destroy(TilemapChip self);
static void tilemapChip_Init(TilemapChip self, GetChip getChip);
static void tilemapChip_UpdateDataAt(TilemapChip self, Layer layer, int column, int row, int value);
static void tilemapChip_Invalidate(TilemapChip self, int idx);

TilemapChip tilemapChip_Create(int columns, int rows)
{
    TilemapChip self = NULL;

    self = (TilemapChip)calloc(1, sizeof(tilemapChip));
    if (self == NULL)
        return NULL;

    strncpy(self->base.name, nameof(TilemapChip), sizeof(self->base.name) - 1);
    self->base.destroy = tilemapChip_Destroy;
    self->base.init = tilemapChip_Init;
    self->columns = columns;
    self->rows = rows;
    self->tilesPerLayer = rows * columns;
    self->invalid = true;

    for (int l = 0; l < MaxLayers; l++)
    {
        self->layers[l] = (int *)calloc(self->tilesPerLayer, sizeof(int));
        assert(self->layers[l]); // TODO: add graceful failure mechanism
        for (int i = 0; i < self->tilesPerLayer; i++)
            self->layers[l][i] = -1;
    }

    return self;
}

static void tilemapChip_Destroy(TilemapChip self)
{
    assert(self);
    for (int l = 0; l < MaxLayers; l++) free(self->layers[l]);
    memset(self, 0, sizeof(tilemapChip));
    free(self);
}

static void tilemapChip_Init(TilemapChip self, GetChip getChip)
{
    assert(self);
    /*self->colorChip = (ColorChip)func_Invoke(getChip, nameof(ColorChip));*/
}

void tilemapChip_UpdateSpriteAt(TilemapChip self, int column, int row, int tileID)
{
    assert(self);
    tilemapChip_UpdateDataAt(self, Sprites, column, row, tileID);
}

void tilemapChip_UpdatePaletteAt(TilemapChip self, int column, int row, int colorOffset)
{
    // TODO: this method?
}

bool tilemapChip_IsInvalidated(TilemapChip self)
{
    assert(self);
    return self->invalid;
}

void tilemapChip_ResetInvalidated(TilemapChip self)
{
    assert(self);
    self->invalid = false;
}

int tilemapChip_GetSpriteRefAt(TilemapChip self, int col, int row)
{
    assert(self);
    return self->layers[Sprites][(row * self->columns) + col];
}

int tilemapChip_GetRows(TilemapChip self)
{
    assert(self);
    return self->rows;
}

int tilemapChip_GetColumns(TilemapChip self)
{
    assert(self);
    return self->columns;
}

static void tilemapChip_UpdateDataAt(TilemapChip self, Layer layer, int column, int row, int value)
{
    assert(self);
    // TODO: some MathUtil.Repeat stuff I've had too much wine to try and understand at this point.
    int idx = column + (row * self->columns);
    self->layers[layer][idx] = value;
    tilemapChip_Invalidate(self, idx);
}

static void tilemapChip_Invalidate(TilemapChip self, int idx)
{
    assert(self);
    self->invalid = true;
}