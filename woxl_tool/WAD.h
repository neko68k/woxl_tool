#ifndef __WAD_H__
#define __WAD_H__

#include "binfile.h"

bool WAD_Load(char *fn);
BINFILE *WAD_GetFile(char * fn);
#endif