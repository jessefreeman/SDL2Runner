// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "cartridge.h"
#include "demoscartridge.h"

typedef struct demosCartridge {
    cartridge base; // must be first
    const char *scriptFileName;
} demosCartridge;

static const char *demosCartridge_GetScript(DemosCartridge self, int *scriptLen);

DemosCartridge demosCartridge_Create()
{
    DemosCartridge self = NULL;

    self = (DemosCartridge)calloc(1, sizeof(demosCartridge));
    if (self == NULL)
        return NULL;

    self->base.destroy = demosCartridge_Destroy;
    self->base.getScript = demosCartridge_GetScript;

    return self;
}

void demosCartridge_Destroy(DemosCartridge self)
{
    assert(self);
    memset(self, 0, sizeof(demosCartridge));
    free(self);
}

void demosCartridge_AddColors(DemosCartridge self, const char *fileName)
{

}

void demosCartridge_AddSprites(DemosCartridge self, const char *fileName)
{

}

void demosCartridge_AddFont(DemosCartridge self, const char *fileName, const char *fontName)
{

}

void demosCartridge_SetScript(DemosCartridge self, const char *fileName)
{
    assert(self);
    self->scriptFileName = fileName;
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