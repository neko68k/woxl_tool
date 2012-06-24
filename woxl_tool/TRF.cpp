#include "stdafx.h"
#include "binfile.h"
#include "endian_tools.h"

#include "TRF.h"

trf_face_t *TRF = NULL;
DWORD numfaces = 0;

void TRF_Load(BINFILE *file){
	DWORD filesize = 0;
	binseek(file, 0, BIN_END);
	filesize = bintell(file);
	binseek(file, 0, SEEK_SET);

	TRF = (trf_face_t*)file->base;

	numfaces = filesize/0x20;
	for(int i = 0;i<numfaces; i++){
		// need to swap endian on each of these
		TRF->vInd[0] = readWORD(TRF->vInd[0]);
		TRF->vInd[0] = readWORD(TRF->vInd[1]);
		TRF->vInd[0] = readWORD(TRF->vInd[2]);
		TRF->vInd[0] = readWORD(TRF->vInd[3]);


		TRF->unk[0] = readWORD(TRF->unk[0]);
		TRF->unk[1] = readWORD(TRF->unk[1]);
		TRF->unk[2] = readWORD(TRF->unk[2]);
		TRF->unk[3] = readWORD(TRF->unk[3]);
	}


}

trf_face_t TRF_GetFace(DWORD i){
	return(TRF[i]);
}