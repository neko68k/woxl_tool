#include "stdafx.h"
#include <stdio.h>
#include "WAD.h"
#include "TRV.h"
#include "binfile.h"


void TRACK_Load(char *path){
	char wadfile[MAX_PATH];
	sprintf(wadfile, "%s\\TRACK.WAD", path);
	WAD_Load(wadfile);	
	BINFILE *trf = WAD_GetFile("track.trf");
	BINFILE *trs = WAD_GetFile("track.trs");

	TRV_Load(WAD_GetFile("track.trv"));
}
