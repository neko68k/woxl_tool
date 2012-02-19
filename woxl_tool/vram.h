#ifndef __VRAM_H__
#define __VRAM_H__

#include <Windows.h>
#include "tim.h"

void VRAM_Save();
//void VRAM_LoadTexture(BINFILE *inFile, bool draw);
void VRAM_LoadTex(BYTE *pix, BYTE *pal, CHK_HDR pixHdr, WORD pixsize, CHK_HDR palHdr, WORD palsize);
void VRAM_LoadTex(BYTE *pix, CHK_HDR pixHdr, WORD pixsize);
void VRAM_SetTPage(WORD x, WORD y);
void VRAM_GetTexture(WORD x, WORD y);


#endif //__VRAM_H__