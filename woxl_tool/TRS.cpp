#include "stdafx.h"

typedef struct {
    DWORD blockMarker;     /* Constant = -1 */
    DWORD previous;     /* Previous section index */
    DWORD next;         /* Next section index */
    DWORD unk[3];
	WORD unk2[2];

	BYTE unk4[43];

	SHORT unk5[60];
	
	/*short unknown1[6];  /* ? Integer coordinates *
    long unknown2[3];
    long unknown3[15]; /* ? Bitmasks *
    short unknown4[15];/* ? Bitmasks *
    short unknown5[7]; /* ? Some section index *
    long n_face;       /* Index of first face of section (see .TRF) *
    long unknown6[3];*/
} trs_section_t;
