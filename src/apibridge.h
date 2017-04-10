// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#ifndef _apibridge_h_
#define _apibridge_h_

#include <stdbool.h>
#include "func.h"
#include "chips.h"
#include "types.h"

typedef struct apiBridge {
    ColorChip colorChip;
    DisplayChip displayChip;
    SpriteChip spriteChip;
    FontChip fontChip;
    TilemapChip tilemapChip;
    ControllerChip controllerChip;
} apiBridge;

typedef apiBridge *ApiBridge;

void apiBridge_Init(
    ApiBridge self, 
    GetChip getChip);

int apiBridge_GetSpriteWidth(ApiBridge self);

int apiBridge_GetSpriteHeight(ApiBridge self);

int apiBridge_GetDisplayWidth(ApiBridge self);

bool apiBridge_GetDisplayWrap(ApiBridge self);

void apiBridge_ToggleDisplayWrap(ApiBridge self,
    bool value);

int apiBridge_GetDisplayHeight(ApiBridge self);

int apiBridge_GetScrollX(ApiBridge self);

int apiBridge_GetScrollY(ApiBridge self);

void apiBridge_ScrollTo(ApiBridge self,
    int x,
    int y);

bool apiBridge_GetPaused(ApiBridge self);

void apiBridge_TogglePause(ApiBridge self,
    bool value);

int apiBridge_GetMouseX(ApiBridge self);

int apiBridge_GetMouseY(ApiBridge self);

char *apiBridge_GetInputString(ApiBridge self);

vector apiBridge_GetMousePosition(ApiBridge self);

int apiBridge_GetBackgroundColor(ApiBridge self);

void apiBridge_DrawSprite(ApiBridge self,
    int id,
    int x,
    int y,
    bool *flipH /* = False */,
    bool *flipV /* = False */,
    bool *aboveBG /* = True */,
    int *colorOffset /* = 0 */);

void apiBridge_DrawSprites(ApiBridge self,
    int *ids,
    int idsLen,
    int x,
    int y,
    int width,
    bool *flipH /* = False */,
    bool *flipV /* = False */,
    bool *aboveBG /* = True */,
    int *colorOffset /* = 0 */);

void apiBridge_UpdateTile(ApiBridge self,
    int spriteID,
    int column,
    int row,
    int *flag /* = -1 */,
    int *colorOffset /* = 0 */);

void apiBridge_Clear(ApiBridge self);

void apiBridge_ClearArea(ApiBridge self,
    int x,
    int y,
    int width,
    int height,
    int *color /* = -1 */);

void apiBridge_ChangeBackgroundColor(ApiBridge self,
    int id);

void apiBridge_PlaySound(ApiBridge self,
    int id,
    int *channel /* = 0 */);

bool apiBridge_ButtonDown(ApiBridge self,
    int button,
    int *player /* = 0 */);

bool apiBridge_ButtonReleased(ApiBridge self,
    int button,
    int *player /* = 0 */);

bool apiBridge_GetMouseButtonDown(ApiBridge self,
    int *id /* = 0 */);

bool apiBridge_GetMouseButtonUp(ApiBridge self,
    int *id /* = 0 */);

bool apiBridge_GetMouseButton(ApiBridge self,
    int *id /* = 0 */);

void apiBridge_DrawSpriteText(ApiBridge self,
    char *text,
    int x,
    int y,
    char **fontName /* = "Default" */,
    int *colorOffset /* = 0 */,
    int *spacing /* = 0 */);

void apiBridge_DrawTileText(ApiBridge self,
    char *text,
    int column,
    int row,
    char **fontName /* = "Default" */,
    int *colorOffset /* = 0 */);

void apiBridge_DrawTileTextBox(ApiBridge self,
    char *text,
    int column,
    int row,
    int characterWidth,
    char **fontName /* = "Default" */,
    int *colorOffset /* = 0 */);

int apiBridge_CalculateTextBoxHeight(ApiBridge self,
    char *text,
    int characterWidth);

void apiBridge_DrawTile(ApiBridge self,
    int tileID,
    int column,
    int row,
    int *colorOffset /* = 0 */);

void apiBridge_DrawTiles(ApiBridge self,
    int *ids,
    int idsLen,
    int column,
    int row,
    int columns,
    int *colorOffset /* = 0 */);

void apiBridge_DrawFont(ApiBridge self,
    char *text,
    int x,
    int y,
    char **fontName /* = "Default" */,
    int *letterSpacing /* = 0 */,
    int *offset /* = 0 */);

void apiBridge_DrawTextBox(ApiBridge self,
    char *text,
    int witdh,
    int x,
    int y,
    char **fontName /* = "Default" */,
    int *letterSpacing /* = 0 */,
    bool *wholeWords /* = False */);

char *apiBridge_FormatWordWrap(ApiBridge self,
    char *text,
    int witdh,
    bool *wholeWords /* = False */);

void apiBridge_DrawPixelData(ApiBridge self,
    int *pixelData,
    int pixelDataLen,
    int x,
    int y,
    int width,
    int height,
    bool flipH,
    bool flipV,
    bool flipY,
    int *layerOrder /* = 0 */,
    bool *masked /* = False */,
    int *colorOffset /* = 0 */);

int apiBridge_ReadFlagAt(ApiBridge self,
    int column,
    int row);

bool apiBridge_GetKey(ApiBridge self,
    int key);

bool apiBridge_GetKeyDown(ApiBridge self,
    int key);

bool apiBridge_GetKeyUp(ApiBridge self,
    int key);

int *apiBridge_SpritesToRawData(ApiBridge self,
    int *ids,
    int idsLen,
    int width,
    int *returnLen);

void apiBridge_SaveData(ApiBridge self,
    char *key,
    char *value);

char *apiBridge_ReadData(ApiBridge self,
    char *key,
    char **defaultValue /* = "undefined" */);

void apiBridge_LoadSong(ApiBridge self,
    int id);

void apiBridge_PlaySong(ApiBridge self,
    bool *loop /* = False */);

void apiBridge_PauseSong(ApiBridge self);

void apiBridge_StopSong(ApiBridge self,
    bool *autoRewind /* = True */);

void apiBridge_RewindSong(ApiBridge self);

int *apiBridge_ReplaceColorID(ApiBridge self,
    int *pixelData,
    int pixelDataLen,
    int oldID,
    int newID,
    int *returnLen);

int *apiBridge_ReplaceColorIDs(ApiBridge self,
    int *pixelData,
    int pixelDataLen,
    int *oldIDs,
    int oldIDsLen,
    int *newIDs,
    int newIDsLen,
    int *returnLen);

void apiBridge_DrawScreenBuffer(ApiBridge self,
    int *x /* = 0 */,
    int *y /* = 0 */,
    int *width /* = -1 */,
    int *height /* = -1 */,
    int *offsetX /* = 0 */,
    int *offsetY /* = 0 */);

void apiBridge_DrawTilemap(ApiBridge self,
    int *startCol /* = 0 */,
    int *startRow /* = 0 */,
    int *columns /* = -1 */,
    int *rows /* = -1 */,
    int *offsetX /* = 0 */,
    int *offsetY /* = 0 */);

void apiBridge_DrawFontTiles(ApiBridge self,
    char *text,
    int column,
    int row,
    char **fontName /* = "Default" */,
    int *offset /* = 0 */);

void apiBridge_RebuildScreenBuffer(ApiBridge self);

void apiBridge_DrawTileToBuffer(ApiBridge self,
    int spriteID,
    int column,
    int row,
    int *colorOffset /* = 0 */);

void apiBridge_DrawTilesToBuffer(ApiBridge self,
    int *ids,
    int idsLen,
    int column,
    int row,
    int columns,
    int *colorOffset /* = 0 */);

void apiBridge_DrawTextBoxToBuffer(ApiBridge self,
    char *text,
    int witdh,
    int column,
    int row,
    char **fontName /* = "Default" */,
    int *letterSpacing /* = 0 */,
    bool *wholeWords /* = False */);

void apiBridge_DrawBufferData(ApiBridge self,
    int *pixelData,
    int pixelDataLen,
    int x,
    int y,
    int width,
    int height);

void apiBridge_DrawFontToBuffer(ApiBridge self,
    char *text,
    int column,
    int row,
    char **fontName /* = "Default" */,
    int *letterSpacing /* = 0 */,
    int *offset /* = 0 */);

#endif
