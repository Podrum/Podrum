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

char *base64_encode(char *data, size_t size)
{
	char *out = (char *) malloc(1);
	out[0] = 0;
	int i;
	char padding = 0;
	for (i = 0; i < size; i += 3) {
		char b1, b2, b3 = 0;
		if (i < size) {
			b1 = data[i];
		}
		if (i + 1 < size) {
			b2 = data[i + 1];
		} else {
			padding = 2;
		}
		if (i + 2 < size) {
			b3 = data[i + 2];
		} else if (padding < 1) {
			padding = 1;
		}
		uint32_t bit_array = (((b1 << 8) | b2) << 8) | b3;
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