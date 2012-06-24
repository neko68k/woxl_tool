#include "stdafx.h"
#include <stdio.h>
#include "WAD.h"
#include "TRV.h"
#include "TRF.h"
#include "TRS.h"
#include "binfile.h"


void TRACK_Load(char *path){
	char wadfile[MAX_PATH];
	sprintf(wadfile, "%s\\TRACK.WAD", path);
	WAD_Load(wadfile);		
	

	TRV_Load(WAD_GetFile("track.trv"));
	TRF_Load(WAD_GetFile("track.trf"));
	TRS_Load(WAD_GetFile("track.trs"));
}

void TRACK_Draw(){

}
