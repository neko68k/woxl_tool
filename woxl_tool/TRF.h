#ifndef __TRF_H__
#define __TRF_H__

#include "stdafx.h"

typedef struct {
    short vInd[4];    /* Vertex indices in [[.TRV]] file */
	short unk[4];
    

    BYTE color[3];	// ?
	BYTE pad;	// always 0x00
} trf_face_t;

#endif //__TRF_H__