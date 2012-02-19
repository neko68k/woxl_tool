#include <Windows.h>
#include "endian_tools.h"
#include "stdafx.h"

WORD	readWORD(WORD in)
{

	in = (in>>8) | 
        (in<<8);


	return in;
}
DWORD	readDWORD(DWORD in)
{


	    in = (in>>24) | 
        ((in<<8) & 0x00FF0000) |
        ((in>>8) & 0x0000FF00) |
        (in<<24);


	return in;
}