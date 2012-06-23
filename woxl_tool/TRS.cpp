#include "stdafx.h"

// 0x9c bytes total
// first 0x1C bytes are DWORD swapped
// 0x60 thru 0x7D are WORD swapped
// 0x7E thru 0x8D are WORD swapped
// 0x8E thru 0x99 are WORD swapped
// last word is 0x0000 and is ignored
typedef struct {
    DWORD blockMarker;     /* Constant = -1 */
    DWORD previous;     /* Previous section index \ these indices are replaced at run time */
    DWORD next;         /* Next section index     / by pointers directly to the data */
    DWORD unk[3];		// unk, looks like probably translation x,y,z		
	DWORD unkflag;		// checks this while loading, if != 8 then prints error "Convert track with track10"
	
	DWORD unk4[2];		// unk, first is a PSX RAM address
	DWORD VEWOfs[0xF];	// the data that exists in these fields are hard coded 
						// PSX RAM addresses and are replaced with new PC RAM addresses!
						// these addresses point straight at VEW file data

						// I think the View List determines what sectors are visible
						// from a given sector
						
	// the rest of this undefined data is probably texture related			
	WORD unk5[15];		// swapped
	WORD unk6[8];		// swapped
	WORD unk7[6];		// swapped
	WORD unk8;			// always 0?
	
} trs_section_t;
