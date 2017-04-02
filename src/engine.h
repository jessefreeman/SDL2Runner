// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#ifndef _engine_h_
#define _engine_h_

#include "chip.h"

typedef struct pixelVisionEngine *PixelVisionEngine;

PixelVisionEngine pixelVisionEngine_Create();

void pixelVisionEngine_Destroy(PixelVisionEngine self); 

void pixelVisionEngine_InsertChip(PixelVisionEngine self, Chip chip);

Chip pixelVisionEngine_GetChip(PixelVisionEngine self, const char *chipName);

void pixelVisionEngine_Init(PixelVisionEngine self);

void pixelVisionEngine_Update(PixelVisionEngine self, float timeDelta);

void pixelVisionEngine_Draw(PixelVisionEngine self);

#endif
