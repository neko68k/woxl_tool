#include "stdafx.h"
#include <Windows.h>
#include <stdio.h>
//#include "binfile.h"
#include "vram.h"
#include "tim.h"


#define LO(x)  ((BYTE) ((BYTE) (x) & (BYTE) 0x0F))
#define HI(x)  ((BYTE) ((BYTE) (x) >> (BYTE) 4))

BYTE VRAM[1024*512*2];


// borrowed from psx_vram by agemo
WORD C8toC5(BYTE r, BYTE g, BYTE b){
	WORD outcolor = 0;
	BYTE or, og, ob;
	
	or = r;
	og = g;
	ob = b;

	or >>= 3;
	og >>= 3;
	ob >>= 3;

	outcolor = (or) | (og<<5) | (ob<<10);

	return outcolor;
}

void VRAM_Save(){
	FILE *outfile;
	outfile = fopen("vram.bin", "wb");
	fwrite(VRAM, 1024*512, 2, outfile);	
	fclose(outfile);
}

void VRAM_LoadTex(BYTE *pix, BYTE *pal, CHK_HDR pixHdr, WORD pixsize, CHK_HDR palHdr, WORD palsize){

	for(int y = 0; y<palHdr.h; y++){
		memcpy(&VRAM[(y+palHdr.y)*2*1024+palHdr.x*2], &pal[y*palHdr.w], palHdr.w*2);
	}

	for(int y = 0; y<pixHdr.h; y++){
		memcpy(&VRAM[(y+pixHdr.y)*2*1024+pixHdr.x*2], &pix[y*(pixHdr.w*2)], pixHdr.w*2);
	}	
}

void VRAM_LoadTex(BYTE *pix, CHK_HDR pixHdr, WORD pixsize){
	for(int y = 0; y<pixHdr.h; y++){
		memcpy(&VRAM[(y+pixHdr.y)*2*1024+pixHdr.x*2], &pix[y*(pixHdr.w*2)], pixHdr.w*2);
	}	
}


void VRAM_SetTPage(WORD x, WORD y);
void VRAM_GetTexture(WORD x, WORD y);