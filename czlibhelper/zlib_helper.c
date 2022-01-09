/*
   Copyright Alexander Argentakis
   Repo: https://github.com/MFDGaming/CZlibHelper
   This file is licensed under the MIT license
 */

#include "./zlib_helper.h"
#include <stdlib.h>

int zlib_decode(zlib_buf_t in, zlib_buf_t *out, int mode)
{
	int status;
	int len = in.size;
	Bytef *uncomp = malloc(len);
	uLong half_len = in.size / 2;

	if (in.size == 0) {
		return Z_OK;
	}

	z_stream stream;
	stream.next_in = in.data;
	stream.avail_in = in.size;
	stream.total_out = 0;
	stream.zalloc = Z_NULL;
	stream.zfree = Z_NULL;

	status = inflateInit2(
	  &stream,
	  mode == ZLIB_DEFLATE_MODE ? -MAX_WBITS :
	  mode == ZLIB_RAW_MODE ? MAX_WBITS :
	  mode == ZLIB_GZIP_MODE ? 16 + MAX_WBITS :
	  MAX_WBITS
	);

	if (status != Z_OK) {
		return status;
	}

	do {
		if (stream.total_out >= len) {
			len += half_len;
			uncomp = realloc(uncomp, len);
		}
		stream.next_out = (uncomp + stream.total_out);
		stream.avail_out = len - stream.total_out;
		status = inflate(&stream, Z_SYNC_FLUSH);
	} while (status == Z_OK);

	if (status != Z_STREAM_END) {
		return status;
	}

	out->size = stream.total_out;
	out->data = malloc(stream.total_out);

	int i;
	for (i = 0; i < stream.total_out; ++i) {
		out->data[i] = uncomp[i];
	}

	free(uncomp);

	return inflateEnd(&stream);
}

int zlib_encode(zlib_buf_t in, zlib_buf_t *out, int level, int mode)
{
	int status;
	out->size = compressBound(in.size);
	out->data = malloc(out->size);

	z_stream stream;
	stream.next_in = in.data;
	stream.avail_in = in.size;
	stream.next_out = out->data;
	stream.avail_out = out->size;
	stream.zalloc = Z_NULL;
	stream.zfree = Z_NULL;

	status = deflateInit2(
	  &stream,
	  level,
	  Z_DEFLATED,
	  mode == ZLIB_DEFLATE_MODE ? -MAX_WBITS :
	  mode == ZLIB_RAW_MODE ? MAX_WBITS :
	  mode == ZLIB_GZIP_MODE ? 16 + MAX_WBITS :
	  MAX_WBITS,
	  9,
	  Z_DEFAULT_STRATEGY
	);

	if (status != Z_OK) {
		return status;
	}

	status = deflate(&stream, Z_FINISH);

	if (status != Z_STREAM_END) {
		return status;
	}

	out->size = stream.total_out;

	return deflateEnd(&stream);
}
