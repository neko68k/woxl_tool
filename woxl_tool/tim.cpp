#include "stdafx.h"
//#include <Windows.h>
//#include <stdio.h>
#include "tim.h"
#include "vram.h"
#include "binfile.h"

// need to implement TIM to VRAM auto placement based
// on values used in WOXL

static WORD PalAutoX = 0;
static WORD PalAutoY = 0;
static WORD TimAutoX = 0;
static WORD TimAutoY = 0;
static BYTE HeightFlag = 0;

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

	if(timHdr2 & 7 == 1){
		//do 8-bit
	}
	else{
		//do 4-bit
		binread(&palHdr, sizeof(CHK_HDR), 1, intim);

		palHdr.x=PalAutoX;
		palHdr.y=PalAutoY;
		palHdr.w=0x10;
		palHdr.h=1;

		PalAutoX += 0x10;

		if(PalAutoX == 0x180){
			PalAutoX = 0x140;
			PalAutoY++;
		}

		pal = (BYTE*)calloc(palHdr.w*palHdr.h, 2);
		binread(pal, palHdr.w*palHdr.h, 2, intim);
	}

		if(timHdr2 == 9 || timHdr2 == 8){
			
		}

		binread(&pixHdr, sizeof(CHK_HDR), 1, intim);
		pix = (BYTE*)calloc(pixHdr.w*pixHdr.h, mult);
		binread(pix, pixHdr.w*pixHdr.h, mult, intim);
	if(timHdr2 == 9 || timHdr2 == 8)
		VRAM_LoadTex(pix, pal, pixHdr, pixHdr.w*pixHdr.h*mult, palHdr, palHdr.w*palHdr.h*2);
	else
		VRAM_LoadTex(pix, pixHdr, pixHdr.w*pixHdr.h*mult);
}