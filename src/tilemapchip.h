// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#ifndef _tilemapchip_h_
#define _tilemapchip_h_

#include <stdbool.h>

typedef struct tilemapChip *TilemapChip;

TilemapChip tilemapChip_Create(int width, int height);

void tilemapChip_UpdateSpriteAt(TilemapChip self, int column, int row, int tileID);

void tilemapChip_UpdatePaletteAt(TilemapChip self, int column, int row, int colorOffset);

bool tilemapChip_IsInvalidated(TilemapChip self);

void tilemapChip_ResetInvalidated(TilemapChip self);

int tilemapChip_GetSpriteRefAt(TilemapChip self, int col, int row);

int tilemapChip_GetRows(TilemapChip self);

int tilemapChip_GetColumns(TilemapChip self);

#endif
