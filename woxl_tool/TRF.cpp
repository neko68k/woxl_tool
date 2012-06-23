#include "stdafx.h"

typedef struct {
    short vInd[4];    /* Vertex indices in [[.TRV]] file */
	short unk[4];
    

    BYTE color[3];	// ?
	BYTE pad;	// always 0x00
} trv_face_t;