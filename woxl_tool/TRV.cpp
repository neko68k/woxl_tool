#include "stdafx.h"
#include "binfile.h"
#include "endian_tools.h"

#include "TRV.h"

trv_vertex_t *TRV = NULL;
DWORD numverts = 0;

void TRV_Load(BINFILE *file){
	DWORD filesize = 0;
	binseek(file, 0, BIN_END);
	filesize = bintell(file);
	binseek(file, 0, SEEK_SET);

	TRV = (trv_vertex_t*)file->base;
	//binread(TRV, filesize, 1, file);
	//binclose(file);

	numverts = filesize>>4;
	for(int i = 0;i<numverts; i++){
		// need to swap endian on each of these
		TRV->x = readDWORD(TRV->x);
		TRV->y = readDWORD(TRV->y);
		TRV->z = readDWORD(TRV->z);
	}
}

trv_vertex_t TRV_GetVert(DWORD i){
	return(TRV[i]);
}