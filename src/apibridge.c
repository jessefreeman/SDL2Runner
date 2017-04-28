// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include "func.h"
#include "util.h"
#include "chips.h"
#include "apibridge.h"

void apiBridge_Init(
    ApiBridge self,
    GetChip getChip)
{
    assert(self);
    self->colorChip = (ColorChip)func_Invoke(getChip, nameof(ColorChip));
    self->displayChip = (DisplayChip)func_Invoke(getChip, nameof(DisplayChip));
    self->spriteChip = (SpriteChip)func_Invoke(getChip, nameof(SpriteChip));
    self->fontChip = (FontChip)func_Invoke(getChip, nameof(FontChip));
    self->tilemapChip = (TilemapChip)func_Invoke(getChip, nameof(TilemapChip));
    self->controllerChip = (ControllerChip)func_Invoke(getChip, nameof(ControllerChip));
}

int apiBridge_GetSpriteWidth(ApiBridge self)
{
    assert(self);
    assert(false);
    // TODO: implement this
    return 0;
}

int apiBridge_GetSpriteHeight(ApiBridge self)
{
    assert(self);
    assert(false);
    // TODO: implement this
    return 0;
}

int apiBridge_GetDisplayWidth(ApiBridge self)
{
    assert(self);

    return displayChip_GetDisplayWidth(self->displayChip);
}

bool apiBridge_GetDisplayWrap(ApiBridge self)
{
    assert(self);
    assert(false);
    // TODO: implement this
    return false;
}

void apiBridge_ToggleDisplayWrap(ApiBridge self,
    bool value)
{
    assert(self);
    displayChip_SetWrapMode(self->displayChip, value);
}

int apiBridge_GetDisplayHeight(ApiBridge self)
{
    assert(self);

    return displayChip_GetDisplayHeight(self->displayChip);
}

int apiBridge_GetScrollX(ApiBridge self)
{
    assert(self);
    assert(false);
    // TODO: implement this
    return 0;
}

int apiBridge_GetScrollY(ApiBridge self)
{
    assert(self);
    assert(false);
    // TODO: implement this
    return 0;
}

void apiBridge_ScrollTo(ApiBridge self,
    int x,
    int y)
{
    assert(self);
    assert(false);
    // TODO: implement this
}

bool apiBridge_GetPaused(ApiBridge self)
{
    assert(self);
    assert(false);
    // TODO: implement this
    return false;
}

void apiBridge_TogglePause(ApiBridge self,
    bool value)
{
    assert(self);
    assert(false);
    // TODO: implement this
}

int apiBridge_GetMouseX(ApiBridge self)
{
    assert(self);
    return controllerChip_GetMousePosition(self->controllerChip).x;
}

int apiBridge_GetMouseY(ApiBridge self)
{
    assert(self);
    assert(self);
    return controllerChip_GetMousePosition(self->controllerChip).y;
}

char *apiBridge_GetInputString(ApiBridge self)
{
    assert(self);
    assert(false);
    // TODO: implement this
    return NULL;
}

vector apiBridge_GetMousePosition(ApiBridge self)
{
    assert(self);
    assert(false);
    // TODO: implement this
    vector result = { 0 };
    return result;
}

int apiBridge_GetBackgroundColor(ApiBridge self)
{
    assert(self);
    assert(false);
    // TODO: implement this
    return 0;
}

void apiBridge_DrawSprite(ApiBridge self,
    int id,
    int x,
    int y,
    bool *flipH /* = False */,
    bool *flipV /* = False */,
    bool *aboveBG /* = True */,
    int *colorOffset /* = 0 */)
{
    bool flipHValue = flipH == NULL ? false : *flipH;
    bool flipVValue = flipV == NULL ? false : *flipV;
    bool aboveBGValue = aboveBG == NULL ? true : *aboveBG;
    int colorOffsetValue = colorOffset == NULL ? 0 : *colorOffset;
    assert(self);

    Sprite sprite = spriteChip_GetSprite(self->spriteChip, id);
    if (sprite != NULL)
    {
        // TODO: implement all the features of draw api
        displayChip_DrawSprite(self->displayChip, sprite, x, y);
    }
}

void apiBridge_DrawSprites(ApiBridge self,
    int *ids,
    int idsLen,
    int x,
    int y,
    int width,
    bool *flipH /* = False */,
    bool *flipV /* = False */,
    bool *aboveBG /* = True */,
    int *colorOffset /* = 0 */)
{
    bool flipHValue = flipH == NULL ? false : *flipH;
    bool flipVValue = flipV == NULL ? false : *flipV;
    bool aboveBGValue = aboveBG == NULL ? true : *aboveBG;
    int colorOffsetValue = colorOffset == NULL ? 0 : *colorOffset;
    assert(self);

    int spriteWidth = spriteChip_GetSpriteWidth(self->spriteChip);
    for (int i = 0; i < idsLen; i++)
    {
        Sprite sprite = spriteChip_GetSprite(self->spriteChip, ids[i]);
        if (sprite != NULL)
        {
            int newX = (i % width) * spriteWidth + x;
            int newY = (i / width) * spriteWidth + y;
            displayChip_DrawSprite(self->displayChip, sprite, newX, newY);
        }
    }
}

void apiBridge_UpdateTile(ApiBridge self,
    int spriteID,
    int column,
    int row,
    int *flag /* = -1 */,
    int *colorOffset /* = 0 */)
{
    int flagValue = flag == NULL ? -1 : *flag;
    int colorOffsetValue = colorOffset == NULL ? 0 : *colorOffset;
    assert(self);
    assert(false);
    // TODO: implement this
}

void apiBridge_Clear(ApiBridge self)
{
    assert(self);

    displayChip_Clear(self->displayChip);
}

void apiBridge_ClearArea(ApiBridge self,
    int x,
    int y,
    int width,
    int height,
    int *color /* = -1 */)
{
    int colorValue = color == NULL ? -1 : *color;
    assert(self);
    assert(false);
    // TODO: implement this
}

void apiBridge_ChangeBackgroundColor(ApiBridge self,
    int id)
{
    assert(self);

    colorChip_SetBackgroundColorId(self->colorChip, id);
}

void apiBridge_PlaySound(ApiBridge self,
    int id,
    int *channel /* = 0 */)
{
    int channelValue = channel == NULL ? 0 : *channel;
    assert(self);
    assert(false);
    // TODO: implement this
}

bool apiBridge_ButtonDown(ApiBridge self,
    int button,
    int *player /* = 0 */)
{
    int playerValue = player == NULL ? 0 : *player;
    assert(self);

    return controllerChip_ButtonDown(self->controllerChip, button, playerValue);
}

bool apiBridge_ButtonReleased(ApiBridge self,
    int button,
    int *player /* = 0 */)
{
    int playerValue = player == NULL ? 0 : *player;
    assert(self);
    assert(false);
    // TODO: implement this
    return false;
}

bool apiBridge_GetMouseButtonDown(ApiBridge self,
    int *id /* = 0 */)
{
    return apiBridge_GetMouseButtonDown(self, id) == true;
}

bool apiBridge_GetMouseButtonUp(ApiBridge self,
    int *id /* = 0 */)
{
    return apiBridge_GetMouseButtonDown(self, id) == false;
}

bool apiBridge_GetMouseButton(ApiBridge self,
    int *id /* = 0 */)
{
    int idValue = id == NULL ? 0 : *id;
    assert(self);
    return controllerChip_MouseButtonDown(self->controllerChip, idValue);
}

void apiBridge_DrawSpriteText(ApiBridge self,
    char *text,
    int x,
    int y,
    char **fontName /* = "Default" */,
    int *colorOffset /* = 0 */,
    int *spacing /* = 0 */)
{
    char *fontNameValue = fontName == NULL ? "Default" : *fontName;
    int colorOffsetValue = colorOffset == NULL ? 0 : *colorOffset;
    int spacingValue = spacing == NULL ? 0 : *spacing;
    assert(self);

    TextureData pixelData = fontChip_ConvertTextToTexture(self->fontChip, text, fontNameValue, spacingValue);
    if (pixelData != NULL)
    {
        displayChip_DrawTexture(self->displayChip, pixelData, x, y);
        textureData_Destroy(pixelData);
    }
}

void apiBridge_DrawTileText(ApiBridge self,
    char *text,
    int column,
    int row,
    char **fontName /* = "Default" */,
    int *colorOffset /* = 0 */)
{
    char *fontNameValue = fontName == NULL ? "Default" : *fontName;
    int colorOffsetValue = colorOffset == NULL ? 0 : *colorOffset;
    assert(self);

    int total = strlen(text);
    spriteId spriteIds[2048] = { 0 };
    fontChip_ConvertTextToSprites(self->fontChip, text, fontNameValue, spriteIds);
    for (int i = 0; i < total; i++)
        apiBridge_DrawTile(self, spriteIds[i], column + i, row, colorOffset);
}

void apiBridge_DrawTileTextBox(ApiBridge self,
    char *text,
    int column,
    int row,
    int characterWidth,
    char **fontName /* = "Default" */,
    int *colorOffset /* = 0 */)
{
    char *fontNameValue = fontName == NULL ? "Default" : *fontName;
    int colorOffsetValue = colorOffset == NULL ? 0 : *colorOffset;
    assert(self);
    assert(false);
    // TODO: implement this
}

int apiBridge_CalculateTextBoxHeight(ApiBridge self,
    char *text,
    int characterWidth)
{
    assert(self);
    assert(false);
    // TODO: implement this
    return 0;
}

void apiBridge_DrawTile(ApiBridge self,
    int tileID,
    int column,
    int row,
    int *colorOffset /* = 0 */)
{
    int colorOffsetValue = colorOffset == NULL ? 0 : *colorOffset;
    assert(self);

    tilemapChip_UpdateSpriteAt(self->tilemapChip, column, row, tileID);
    tilemapChip_UpdatePaletteAt(self->tilemapChip, column, row, colorOffsetValue);
}

void apiBridge_DrawTiles(ApiBridge self,
    int *ids,
    int idsLen,
    int column,
    int row,
    int columns,
    int *colorOffset /* = 0 */)
{
    int colorOffsetValue = colorOffset == NULL ? 0 : *colorOffset;
    assert(self);
    assert(false);
    // TODO: implement this
}

void apiBridge_DrawFont(ApiBridge self,
    char *text,
    int x,
    int y,
    char **fontName /* = "Default" */,
    int *letterSpacing /* = 0 */,
    int *offset /* = 0 */)
{
    char *fontNameValue = fontName == NULL ? "Default" : *fontName;
    int letterSpacingValue = letterSpacing == NULL ? 0 : *letterSpacing;
    int offsetValue = offset == NULL ? 0 : *offset;
    assert(self);

    apiBridge_DrawSpriteText(self,
        text,
        x,
        y,
        fontName,
        offset,
        letterSpacing);
}

void apiBridge_DrawTextBox(ApiBridge self,
    char *text,
    int witdh,
    int x,
    int y,
    char **fontName /* = "Default" */,
    int *letterSpacing /* = 0 */,
    bool *wholeWords /* = False */)
{
    char *fontNameValue = fontName == NULL ? "Default" : *fontName;
    int letterSpacingValue = letterSpacing == NULL ? 0 : *letterSpacing;
    bool wholeWordsValue = wholeWords == NULL ? false : *wholeWords;
    assert(self);
    assert(false);
    // TODO: implement this
}

char *apiBridge_FormatWordWrap(ApiBridge self,
    char *text,
    int witdh,
    bool *wholeWords /* = False */)
{
    bool wholeWordsValue = wholeWords == NULL ? false : *wholeWords;
    assert(self);
    assert(false);
    // TODO: implement this
    return NULL;
}

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
    int *colorOffset /* = 0 */)
{
    int layerOrderValue = layerOrder == NULL ? 0 : *layerOrder;
    bool maskedValue = masked == NULL ? false : *masked;
    int colorOffsetValue = colorOffset == NULL ? 0 : *colorOffset;
    assert(self);
    assert(false);
    // TODO: implement this
}

int apiBridge_ReadFlagAt(ApiBridge self,
    int column,
    int row)
{
    assert(self);
    assert(false);
    // TODO: implement this
    return 0;
}

bool apiBridge_GetKey(ApiBridge self,
    int key)
{
    assert(self);
    assert(false);
    // TODO: implement this
    return false;
}

bool apiBridge_GetKeyDown(ApiBridge self,
    int key)
{
    assert(self);
    assert(false);
    // TODO: implement this
    return false;
}

bool apiBridge_GetKeyUp(ApiBridge self,
    int key)
{
    assert(self);
    assert(false);
    // TODO: implement this
    return false;
}

int *apiBridge_SpritesToRawData(ApiBridge self,
    int *ids,
    int idsLen,
    int width,
    int *returnLen)
{
    assert(self);
    assert(false);
    // TODO: implement this
    return 0;
}

void apiBridge_SaveData(ApiBridge self,
    char *key,
    char *value)
{
    assert(self);
    assert(false);
    // TODO: implement this
}

char *apiBridge_ReadData(ApiBridge self,
    char *key,
    char **defaultValue /* = "undefined" */)
{
    char *defaultValueValue = defaultValue == NULL ? "undefined" : *defaultValue;
    assert(self);
    assert(false);
    // TODO: implement this
    return NULL;
}

void apiBridge_LoadSong(ApiBridge self,
    int id)
{
    assert(self);
    assert(false);
    // TODO: implement this
}

void apiBridge_PlaySong(ApiBridge self,
    bool *loop /* = False */)
{
    bool loopValue = loop == NULL ? false : *loop;
    assert(self);
    assert(false);
    // TODO: implement this
}

void apiBridge_PauseSong(ApiBridge self)
{
    assert(self);
    assert(false);
    // TODO: implement this
}

void apiBridge_StopSong(ApiBridge self,
    bool *autoRewind /* = True */)
{
    bool autoRewindValue = autoRewind == NULL ? true : *autoRewind;
    assert(self);
    assert(false);
    // TODO: implement this
}

void apiBridge_RewindSong(ApiBridge self)
{
    assert(self);
    assert(false);
    // TODO: implement this
}

int *apiBridge_ReplaceColorID(ApiBridge self,
    int *pixelData,
    int pixelDataLen,
    int oldID,
    int newID,
    int *returnLen)
{
    assert(self);
    assert(false);
    // TODO: implement this
    return NULL;
}

int *apiBridge_ReplaceColorIDs(ApiBridge self,
    int *pixelData,
    int pixelDataLen,
    int *oldIDs,
    int oldIDsLen,
    int *newIDs,
    int newIDsLen,
    int *returnLen)
{
    assert(self);
    assert(false);
    // TODO: implement this
    return NULL;
}

void apiBridge_DrawScreenBuffer(ApiBridge self,
    int *x /* = 0 */,
    int *y /* = 0 */,
    int *width /* = -1 */,
    int *height /* = -1 */,
    int *offsetX /* = 0 */,
    int *offsetY /* = 0 */)
{
    int xValue = x == NULL ? 0 : *x;
    int yValue = y == NULL ? 0 : *y;
    int widthValue = width == NULL ? -1 : *width;
    int heightValue = height == NULL ? -1 : *height;
    int offsetXValue = offsetX == NULL ? 0 : *offsetX;
    int offsetYValue = offsetY == NULL ? 0 : *offsetY;
    assert(self);

    // TODO: these other parameters probably are supposed to do things :p
    displayChip_Clear(self->displayChip);
    // TODO: this probably doesn't work like this, and needs more params. To come!
    displayChip_DrawTilemap(self->displayChip);
}

void apiBridge_DrawTilemap(ApiBridge self,
    int *startCol /* = 0 */,
    int *startRow /* = 0 */,
    int *columns /* = -1 */,
    int *rows /* = -1 */,
    int *offsetX /* = 0 */,
    int *offsetY /* = 0 */)
{
    int startColValue = startCol == NULL ? 0 : *startCol;
    int startRowValue = startRow == NULL ? 0 : *startRow;
    int columnsValue = columns == NULL ? -1 : *columns;
    int rowsValue = rows == NULL ? -1 : *rows;
    int offsetXValue = offsetX == NULL ? 0 : *offsetX;
    int offsetYValue = offsetY == NULL ? 0 : *offsetY;
    assert(self);
    assert(false);
    // TODO: implement this
}

void apiBridge_DrawFontTiles(ApiBridge self,
    char *text,
    int column,
    int row,
    char **fontName /* = "Default" */,
    int *offset /* = 0 */)
{
    char *fontNameValue = fontName == NULL ? "Default" : *fontName;
    int offsetValue = offset == NULL ? 0 : *offset;
    assert(self);
    assert(false);
    // TODO: implement this
}

// Deprecated
void apiBridge_RebuildScreenBuffer(ApiBridge self)
{
    assert(self);
    // nothing to do
}

void apiBridge_DrawTileToBuffer(ApiBridge self,
    int spriteID,
    int column,
    int row,
    int *colorOffset /* = 0 */)
{
    int colorOffsetValue = colorOffset == NULL ? 0 : *colorOffset;
    assert(self);
    assert(false);
    // TODO: implement this
}

void apiBridge_DrawTilesToBuffer(ApiBridge self,
    int *ids,
    int idsLen,
    int column,
    int row,
    int columns,
    int *colorOffset /* = 0 */)
{
    int colorOffsetValue = colorOffset == NULL ? 0 : *colorOffset;
    assert(self);
    assert(false);
    // TODO: implement this
}

void apiBridge_DrawTextBoxToBuffer(ApiBridge self,
    char *text,
    int witdh,
    int column,
    int row,
    char **fontName /* = "Default" */,
    int *letterSpacing /* = 0 */,
    bool *wholeWords /* = False */)
{
    char *fontNameValue = fontName == NULL ? "Default" : *fontName;
    int letterSpacingValue = letterSpacing == NULL ? 0 : *letterSpacing;
    bool wholeWordsValue = wholeWords == NULL ? false : *wholeWords;
    assert(self);
    assert(false);
    // TODO: implement this
}

void apiBridge_DrawBufferData(ApiBridge self,
    int *pixelData,
    int pixelDataLen,
    int x,
    int y,
    int width,
    int height)
{
    assert(self);
    assert(false);
    // TODO: implement this
}

void apiBridge_DrawFontToBuffer(ApiBridge self,
    char *text,
    int column,
    int row,
    char **fontName /* = "Default" */,
    int *letterSpacing /* = 0 */,
    int *offset /* = 0 */)
{
    char *fontNameValue = fontName == NULL ? "Default" : *fontName;
    int letterSpacingValue = letterSpacing == NULL ? 0 : *letterSpacing;
    int offsetValue = offset == NULL ? 0 : *offset;
    assert(self);

    apiBridge_DrawTileText(self,
        text,
        column,
        row,
        fontName,
        offset);
}
