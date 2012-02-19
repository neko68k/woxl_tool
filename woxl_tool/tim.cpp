#include "stdafx.h"
#include <Windows.h>
#include <stdio.h>
#include "tim.h"
#include "vram.h"
#include "binfile.h"


void TIM_Load(BINFILE *intim){
	CHK_HDR palHdr;
	CHK_HDR pixHdr;

	BYTE *pix = NULL;
	BYTE *pal = NULL;
	
	DWORD timHdr2 = 0;

	BYTE mult = 2;

	DWORD magic;
	binread(&magic, 4, 1, intim);

	binread(&timHdr2, 4, 1, intim);		

		if(timHdr2 == 9 || timHdr2 == 8){
			binread(&palHdr, sizeof(CHK_HDR), 1, intim);
			pal = (BYTE*)calloc(palHdr.w*palHdr.h, 2);
			binread(pal, palHdr.w*palHdr.h, 2, intim);
		}

		binread(&pixHdr, sizeof(CHK_HDR), 1, intim);
		pix = (BYTE*)calloc(pixHdr.w*pixHdr.h, mult);
		binread(pix, pixHdr.w*pixHdr.h, mult, intim);
	if(timHdr2 == 9 || timHdr2 == 8)
		VRAM_LoadTex(pix, pal, pixHdr, pixHdr.w*pixHdr.h*mult, palHdr, palHdr.w*palHdr.h*2);
	else
		VRAM_LoadTex(pix, pixHdr, pixHdr.w*pixHdr.h*mult);
}