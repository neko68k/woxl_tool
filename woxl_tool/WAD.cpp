#include "stdafx.h"
#include "binfile.h"




typedef struct{
	char fn[0x10];		// null terminated, 
						// if fn is not 0x10 chars 
						// remaining is padded with garbage after null
	DWORD fileSize;
	DWORD fileSize2;	// ? thought this might be for compressed 
						// files but doesn't seem to be
	BYTE zero;			// last byte is always 0x00
}ENT;

WORD numfiles;
ENT *entries = NULL;
BINFILE *infile;

bool WAD_Load(char *fn){
	DWORD filesize = 0;
		// first in file, followed by ENT's
	
	infile = binopen(fn);

	binread(&numfiles, 2, 1, infile);
	
	entries = (ENT*)calloc(numfiles-1, 0x19);

	binread(entries, sizeof(ENT), numfiles-1, infile);
	
	
	return true;

}

BINFILE *WAD_GetFile(char * fn){
	int ofs = 0;
	for(int i =0;i<numfiles;i++){

		if(!strcmp(fn, entries[i].fn))
			return binopen(infile->base+ofs, entries[i].fileSize);
		ofs+=entries[i].fileSize;
	}

}