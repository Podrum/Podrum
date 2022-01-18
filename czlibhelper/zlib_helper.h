/*
   Copyright Alexander Argentakis
   Repo: https://github.com/MFDGaming/CZlibHelper
   This file is licensed under the MIT license
 */

#ifndef ZLIB_HELPER
#define ZLIB_HELPER

#include <miniz/miniz.h> /* Use miniz cause its more portable than zlib */

#define ZLIB_DEFLATE_MODE 0
#define ZLIB_RAW_MODE 1
#define ZLIB_GZIP_MODE 2

typedef struct {
	Bytef *data;
	uLong size;
} zlib_buf_t;

int zlib_decode(zlib_buf_t in, zlib_buf_t *out, int mode);

int zlib_encode(zlib_buf_t in, zlib_buf_t *out, int level, int mode);

#endif
