#pragma once
#include "../zlib.h"

class ZlibWrapper {
public:
	void zerr(int ret);
	int zlibDeflate(char *src, char *dst, int bytes_to_compress, int *bytes_after_compressed);
	int zlibInflate(char *src, char *dst, int bytes_to_decompress, int maximum_after_decompress, int* outbytes, int type);
};