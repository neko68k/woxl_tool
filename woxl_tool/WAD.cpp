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

bool WAD_Load(char *fn){
	WORD numfiles;	// first in file, followed by ENT's
	ENT *entries = NULL;
	BINFILE *infile = binopen(fn);

	binread(&numfiles, 2, 1, infile);
	entries = (ENT*)calloc(numfiles-1, sizeof(ENT));
	
	binread(entries, sizeof(ENT), numfiles-1, infile);

	
	return true;

}