#ifndef __TRV_H__
#define __TRV_H__

typedef struct {
    long x;
    long y;
    long z;
    long unk;	// not always zero, when fixing endian this is ignored
} trv_vertex_t;

void TRV_Load(BINFILE *file);
trv_vertex_t TRV_GetVert(DWORD i);

#endif //__TRV_H__