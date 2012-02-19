#ifndef __TIM_H__
#define __TIM_H__

#include "binfile.h"

typedef struct{
	DWORD len;	// incl header
	WORD x;
	WORD y;
	WORD w;
	WORD h;
}CHK_HDR;


void TIM_Load(BINFILE *infile);


#endif