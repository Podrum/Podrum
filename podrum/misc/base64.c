/*
                   Podrum R3 Copyright MFDGaming & PodrumTeam
                 This file is licensed under the GPLv2 license.
              To use this file you must own a copy of the license.
                       If you do not you can get it from:
            http://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
 */

#include "./base64.h"
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

binary_stream_t base64_decode(char *base64_string)
{
	binary_stream_t stream;
	stream.buffer = (char *) malloc(0);
	stream.offset = 0;
	stream.size = 0;
	size_t len = strlen(base64_string);
	size_t i;
	char padding = 0;
	for (i = 0; i < len; i += 4) {
		uint8_t i1 = 0;
		uint8_t i2 = 0;
		uint8_t i3 = 0;
		uint8_t i4 = 0;
		char c1 = 0;
		char c2 = 0;
		char c3 = 0;
		char c4 = 0;
		if (i < len) {
			if (base64_string[i] == '=') {
				++padding;
			} else {
				c1 = base64_string[i];
			}
		} else {
			break;
		}
		if (i + 1 < len) {
			if (base64_string[i + 1] == '=') {
				++padding;
			} else {
				c2 = base64_string[i + 1];
			}
		} else {
			break;
		}
		if (i + 2 < len) {
			if (base64_string[i + 2] == '=') {
				++padding;
			} else {
				c3 = base64_string[i + 2];
			}
		} else {
			++padding;
		}
		if (i + 3 < len) {
			if (base64_string[i + 3] == '=') {
				++padding;
			} else {
				c4 = base64_string[i + 3];
			}
		} else {
			++padding;
		}
		if (padding > 2) {
			break;
		}
		int step = 0;
		int ii;
		for (ii = 0; ii < 64; ++ii) {
			if (BASE64_TABLE[ii] == c1) {
				i1 = ii;
				++step;
			} else if (BASE64_TABLE[ii] == c2) {
				i2 = ii;
				++step;
			} else if (BASE64_TABLE[ii] == c3) {
				i3 = ii;
				++step;
			} else if (BASE64_TABLE[ii] == c4) {
				i4 = ii;
				++step;
			}
			if (step == 4) {
				break;
			}
		}
		uint32_t bit_array = (i1 << 18) | (i2 << 12) | (i3 << 6) | i4;
		put_unsigned_byte((bit_array >> 16) & 0xff, &stream);
		if (padding < 2) {
			put_unsigned_byte((bit_array >> 8) & 0xff, &stream);
		}
		if (padding < 1) {
			put_unsigned_byte(bit_array & 0xff, &stream);
		}
	}
	return stream;
}

char *base64_encode(binary_stream_t stream)
{
	char *out = (char *) malloc(1);
	out[0] = 0;
	size_t i;
	char padding = 0;
	for (i = 0; i < stream.size; i += 3) {
		char b1 = 0;
		char b2 = 0;
		char b3 = 0;
		if (i < stream.size) {
			b1 = stream.buffer[i];
		}
		if (i + 1 < stream.size) {
			b2 = stream.buffer[i + 1];
		} else {
			++padding;
		}
		if (i + 2 < stream.size) {
			b3 = stream.buffer[i + 2];
		} else {
			++padding;
		}
		uint32_t bit_array = (b1 << 16) | (b2 << 8) | b3;
		uint8_t i1 = (bit_array >> 18) & 0x3f;
		uint8_t i2 = (bit_array >> 12) & 0x3f;
		uint8_t i3 = (bit_array >> 6) & 0x3f;
		uint8_t i4 = bit_array & 0x3f;
		char *indexes = (char *) malloc(5);
		indexes[0] = BASE64_TABLE[i1];
		indexes[1] = BASE64_TABLE[i2];
		if (padding < 2) {
			indexes[2] = BASE64_TABLE[i3];
		} else {
			indexes[2] = '=';
		}
		if (padding < 1) {
			indexes[3] = BASE64_TABLE[i4];
		} else {
			indexes[3] = '=';
		}
		indexes[4] = 0;
		strcat(out, indexes);
	}
	return out;
}