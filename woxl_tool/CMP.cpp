#include "stdafx.h"
#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include "tim.h"
#include "vram.h"
#include "binfile.h"

/* Load file in mem from filename, return buffer, update length */

unsigned char *loadFile(char *filename, int *length)
{
	int handle;
	unsigned char *buffer;

	/* Load file */
	handle = open(filename, O_RDONLY);
	if (handle<0) {
		fprintf(stderr, "Unable to open %s\n", filename);	
		return NULL;
	}

	*length = lseek(handle, 0, SEEK_END);
	lseek(handle, 0, SEEK_SET); 	

	buffer = (unsigned char *)malloc(*length);
	if (buffer==NULL) {
		fprintf(stderr, "Unable to allocate %d bytes\n", *length);
		return NULL;
	}

	read(handle, buffer, *length);
	close(handle);

	return buffer;
}

int srcPos, dstPos;
unsigned char tmp8K[0x2000];
int srcBitMask, srcCurByte;

int readSrcBitfield(unsigned char *src, int bitfieldSize)
{
	int curValue = 0;

	while (bitfieldSize > 0) {
		if (srcBitMask == 0x80) {
			srcCurByte = src[srcPos++];
		}

		if (srcCurByte & srcBitMask) {
			curValue |= bitfieldSize;
		}

		bitfieldSize >>= 1;

		srcBitMask >>= 1;
		if (srcBitMask == 0) {
			srcBitMask = 0x80;
		}
	}

	return curValue;
}

void depackFile(unsigned char *src, int srcLength, unsigned char *dst, int dstLength)
{
	srcPos = dstPos = 0;

	int srcCurBit;
	int curValue, curValue2;
	int tmp8Kpos;

	srcBitMask = 0x80;
	srcCurByte = 0;
	tmp8Kpos = 1;

	for (;;) {
		/* Just to be safe */
		if ((srcPos>srcLength) || (dstPos>dstLength)) {
			break;
		}

		if (srcBitMask == 0x80) {
			srcCurByte = src[srcPos++];
		}

		srcCurBit = (srcCurByte & srcBitMask);

		srcBitMask >>= 1;
		if (srcBitMask == 0) {
			srcBitMask = 0x80;
		}

		if (srcCurBit) {
			curValue = readSrcBitfield(src, 0x80);
			tmp8K[(tmp8Kpos++) & 0x1fff] = dst[dstPos++] = curValue;
		} else {
			int position, length, i;

			position = readSrcBitfield(src, 0x1000);
			if (position == 0) {
				break;
			}

			length = readSrcBitfield(src, 0x08)+2;			

			for (i=0; i<=length; i++) {
				curValue = tmp8K[(i + position) & 0x1fff];
				tmp8K[(tmp8Kpos++) & 0x1fff] = dst[dstPos++] = curValue;
			}
		}
	}
}

void *parseCmp(unsigned char *filePtr, int length, int *dstLength)
{
	int i, len;
	unsigned char *dstFile;
	unsigned long *file = (unsigned long *)filePtr;

	

	/* Sum all length */
	len = 0;
	for (i=0; i<file[0]; i++) {
		len += file[i+1];
	}

	length -= (file[0]+1)*4;
	dstFile = (unsigned char *)malloc(len);
	if (dstFile) {
		depackFile((unsigned char *) &file[file[0]+1], length, dstFile, len);

		*dstLength = len;
	}	

	return dstFile;
}

bool CMP_Extract(char *fn){
	int length, dstLength;
	unsigned char *fileInMem;
	unsigned char *dstFile;

	fileInMem = loadFile(fn, &length);
	if (fileInMem==NULL) {
		return false;
	}

	unsigned long *file = (unsigned long *)fileInMem;
	DWORD numFiles = file[0];

	dstFile = (unsigned char*)parseCmp(fileInMem, length, &dstLength);
	if (dstFile) {

		/* Now we can save the file from dstFile, dstLength bytes */
		printf("%d bytes depacked\n", dstLength);
		FILE *test = fopen("test.dcmp", "wb");
		fwrite(dstFile, dstLength, 1, test);
		fclose(test);
		/*BINFILE *infile = binopen(dstFile, dstLength);
		for(int i = 0;i<numFiles-2;i++){
			TIM_Load(infile);
		}
		VRAM_Save();*/
		free(dstFile);
	}

	free(fileInMem);

	return 0;
}