#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <iostream>
#include <fstream>
#include "ZlibWrapper.h"


#if defined(MSDOS) || defined(OS2) || defined(WIN32) || defined(__CYGWIN__)
#  include <fcntl.h>
#  include <io.h>
#  define SET_BINARY_MODE(file) setmode(fileno(file), O_BINARY)
#else
#  define SET_BINARY_MODE(file)
#endif


//#define CHUNK 16271376
#define CHUNK 22917472
#ifdef WIN32
#define ZLIB_WINAPI
#endif

/* report a zlib or i/o error */
void ZlibWrapper::zerr(int ret)
{
	fputs("zpipe: ", stderr);
	switch (ret) {
	case Z_ERRNO:
		if (ferror(stdin))
			fputs("error reading stdin\n", stderr);
		if (ferror(stdout))
			fputs("error writing stdout\n", stderr);
		break;
	case Z_STREAM_ERROR:
		fputs("invalid compression level\n", stderr);
		break;
	case Z_DATA_ERROR:
		fputs("invalid or incomplete deflate data\n", stderr);
		break;
	case Z_MEM_ERROR:
		fputs("out of memory\n", stderr);
		break;
	case Z_VERSION_ERROR:
		fputs("zlib version mismatch!\n", stderr);
	}
}

/* Compress from file source to file dest until EOF on source.
def() returns Z_OK on success, Z_MEM_ERROR if memory could not be
allocated for processing, Z_STREAM_ERROR if an invalid compression
level is supplied, Z_VERSION_ERROR if the version of zlib.h and the
version of the library linked do not match, or Z_ERRNO if there is
an error reading or writing the files. */
int ZlibWrapper::zlibDeflate(char *src, char *dst, int bytes_to_compress, int *bytes_after_compressed)
{
	z_stream strm;

	int ret;
	unsigned have;

	/* allocate deflate state */
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;
	//ret = deflateInit(&strm, Z_DEFAULT_COMPRESSION);
	ret = deflateInit2(&strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED, -15, 9, Z_DEFAULT_STRATEGY);

	if (ret != Z_OK)
		return ret;

	/* compress */
	strm.avail_in = bytes_to_compress;
	strm.avail_out = bytes_to_compress;

	strm.next_in = (Bytef *)src;
	strm.next_out = (Bytef *)dst;

	ret = deflate(&strm, Z_FINISH);

	have = bytes_to_compress - strm.avail_out;

	*bytes_after_compressed = have;

	assert(ret == Z_STREAM_END);		/* stream will be complete */

	/* clean up and return */
	(void)deflateEnd(&strm);

	return Z_OK;
}

/* Decompress from file source to file dest until stream ends or EOF.
inf() returns Z_OK on success, Z_MEM_ERROR if memory could not be
allocated for processing, Z_DATA_ERROR if the deflate data is
invalid or incomplete, Z_VERSION_ERROR if the version of zlib.h and
the version of the library linked do not match, or Z_ERRNO if there
is an error reading or writing the files. */
int ZlibWrapper::zlibInflate(char *src, char *dst, int bytes_to_decompress, int maximum_after_decompress, int* outbytes, int type)
{
	z_stream strm;

	int ret = 0;

	/* allocate inflate state */
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;
	strm.avail_in = 0;
	strm.next_in = Z_NULL;
	if (type == 1)
		ret = inflateInit(&strm);
	else
		ret = inflateInit2(&strm, -15);
	if (ret != Z_OK) return ret;

	/* decompress */
	strm.avail_in = bytes_to_decompress;
	strm.next_in = (Bytef *)src;

	strm.next_out = (Bytef *)dst;
	strm.avail_out = maximum_after_decompress;

	ret = inflate(&strm, Z_NO_FLUSH);

	assert(ret != Z_STREAM_ERROR);  /* state not clobbered */

	switch (ret)
	{

	case Z_NEED_DICT:
		ret = Z_DATA_ERROR;	 /* and fall through */

	case Z_DATA_ERROR:

	case Z_MEM_ERROR:
		(void)inflateEnd(&strm);
		return ret;
	}

	assert(strm.avail_in == 0);	 /* all input will be used */

	if (outbytes != NULL) *outbytes = strm.total_out;

	/* clean up and return */
	(void)inflateEnd(&strm);

	return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
}