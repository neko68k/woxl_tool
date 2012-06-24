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


void TIM_Load(BINFILE *intim, bool isLibrary){
	CHK_HDR *palHdr;
	CHK_HDR *pixHdr;

	BYTE *TIM = intim->base;

	BYTE *pix = NULL;
	BYTE *pal = NULL;
	
	DWORD *timHdr2 = (DWORD*)TIM+4;

	BYTE mult = 2;

	DWORD magic;
	//binread(&magic, 4, 1, intim);

	//binread(&timHdr2, 4, 1, intim);	

	if(*timHdr2 & 7 == 1){
		//do 8-bit
	}
	else{
		//do 4-bit
		binread(&palHdr, sizeof(CHK_HDR), 1, intim);
		//palHdr = (CHK_HDR*)TIM+
		if(TIM+56==0&&TIM+58==0){		// if img x/y == 0
			*(WORD*)(TIM+12)=PalAutoX;	// pal x
			*(WORD*)(TIM+14)=PalAutoY;	// pal y
			*(WORD*)(TIM+16)=0x10;		// pal w
			*(WORD*)(TIM+18)=1;			// pal h
			*(WORD*)(TIM+56)=TimAutoX;	// img x
			*(WORD*)(TIM+58)=TimAutoY;	// img y

			PalAutoX += 16;

			if(PalAutoX >= 384){
				PalAutoX = 320;
				++PalAutoY;
			}
			if(isLibrary)
				TimAutoY += 32;
			else
				TimAutoY += 128;

			if(TimAutoY >= 256){
				if(isLibrary)
					TimAutoX += 8;
				else
					TimAutoX += 32;
				TimAutoY = 0;
			}
		}
	}

		//pal = (BYTE*)calloc(palHdr.w*palHdr.h, 2);
		//binread(pal, palHdr.w*palHdr.h, 2, intim);

		//binread(&pixHdr, sizeof(CHK_HDR), 1, intim);

		//if(timHdr2 == 9 || timHdr2 == 8){
			
		//}

		
		//pix = (BYTE*)calloc(pixHdr.w*pixHdr.h, mult);
		//binread(pix, pixHdr.w*pixHdr.h, mult, intim);
	//if(timHdr2 == 9 || timHdr2 == 8)
		//VRAM_LoadTex(pix, pal, pixHdr, pixHdr.w*pixHdr.h*mult, palHdr, palHdr.w*palHdr.h*2);
	//else
		//VRAM_LoadTex(pix, pixHdr, pixHdr.w*pixHdr.h*mult);
}