// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#ifndef _importutil_h_
#define _importutil_h_

#include "types.h"

colorData *importImageFromFile(const char *filePath, int *width, int *height);

char *importTextFromFile(const char *filePath, int *len);

TextureData *importSpriteSheetFromFile(const char *filePath, ColorChip colorChip);

#endif
