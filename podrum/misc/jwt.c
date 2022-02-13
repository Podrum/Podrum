/*
                   Podrum R3 Copyright MFDGaming & PodrumTeam
                 This file is licensed under the GPLv2 license.
              To use this file you must own a copy of the license.
                       If you do not you can get it from:
            http://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
 */

#include "./jwt.h"
#include "./base64.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

json_root_t jwt_decode(char *token)
{
	char *payload = (char *) malloc(0);
	size_t size = 0;
	uint8_t step = 0;
	size_t i;
	for (i = 0; i < strlen(token); ++i)
	{
		if (token[i] == '.') {
			++step;
		} else if (step == 1) {
			++size;
			payload = (char *) realloc(payload, size);
			if (token[i] == '-') {
				payload[size - 1] = '+';
			} else if (token[i] == '_') {
				payload[size - 1] = '/';
			} else {
				payload[size - 1] = token[i];
			}
		} else if (step > 1) {
			break;
		}
	}
	++size;
	payload = (char *) realloc(payload, size);
	payload[size - 1] = 0;
	binary_stream_t stream = base64_decode(payload);
	put_unsigned_byte(0, &stream);
	json_input_t json_input;
	json_input.json = (char *) stream.buffer;
	json_input.offset = 0;
	return parse_json_root(&json_input);
}