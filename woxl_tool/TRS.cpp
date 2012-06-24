#include "stdafx.h"
#include "binfile.h"
#include "endian_tools.h"
#include "TRS.h"

trs_section_t *TRS = NULL;
DWORD numsections = 0;

void TRS_Load(BINFILE *file){
	DWORD filesize = 0;
	binseek(file, 0, BIN_END);
	filesize = bintell(file);
	binseek(file, 0, SEEK_SET);

	TRS = (trs_section_t*)file->base;

	numsections = filesize/0x9c;
	for(int i = 0;i<numsections; i++){
		// need to swap endian on each of these
		TRS->previous = readDWORD(TRS->previous);
		TRS->next = readDWORD(TRS->next);
		TRS->unk[0] = readDWORD(TRS->unk[0]);
		TRS->unk[1] = readDWORD(TRS->unk[1]);
		TRS->unk[2] = readDWORD(TRS->unk[2]);
		TRS->unkflag = readDWORD(TRS->unkflag);

		for(int i = 0;i<15;i++){
			TRS->unk5[i] = readWORD(TRS->unk5[i]);
		}
		for(int i = 0;i<8;i++){
			TRS->unk6[i] = readWORD(TRS->unk6[i]);
		}
		for(int i = 0;i<6;i++){
			TRS->unk7[i] = readWORD(TRS->unk7[i]);
		}
	}
}

trs_section_t TRS_GetFace(DWORD i){
	return(TRS[i]);
}

