/*
                   Podrum R3 Copyright MFDGaming & PodrumTeam
                 This file is licensed under the GPLv2 license.
              To use this file you must own a copy of the license.
                       If you do not you can get it from:
            http://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
 */

#include "./mcmisc.h"
#include <stdlib.h>

char *get_misc_string_var_int(binary_stream_t *stream)
{
	uint32_t size = get_var_int(stream);
	char *output = (char *) malloc(size + 1);
	uint32_t i;
	for (i = 0; i < size; ++i) {
		output[i] = (char) get_unsigned_byte(stream);
	}
	output[size] = 0x00;
	return output;
}

char *get_misc_string_int_le(binary_stream_t *stream)
{
	int32_t size = get_int_le(stream);
	char *output = (char *) malloc(size + 1);
	int32_t i;
	for (i = 0; i < size; ++i) {
		output[i] = (char) get_unsigned_byte(stream);
	}
	output[size] = 0x00;
	return output;
}

binary_stream_t get_misc_byte_array_var_int(binary_stream_t *stream)
{
	binary_stream_t output;
	output.offset = 0;
	output.size = (size_t) get_var_int(stream);
	output.buffer = get_bytes(output.size, stream);
	return output;
}

binary_stream_t get_misc_byte_array_signed_var_int(binary_stream_t *stream)
{
	binary_stream_t output;
	output.offset = 0;
	output.size = (size_t) get_signed_var_int(stream);
	output.buffer = get_bytes(output.size, stream);
	return output;
}

binary_stream_t get_misc_byte_array_short_le(binary_stream_t *stream)
{
	binary_stream_t output;
	output.offset = 0;
	output.size = (size_t) get_short_le(stream);
	output.buffer = get_bytes(output.size, stream);
	return output;
}

misc_login_tokens_t get_misc_login_tokens(binary_stream_t *stream)
{
	misc_login_tokens_t login_tokens;
	login_tokens.identity = get_misc_string_int_le(stream);
	login_tokens.client = get_misc_string_int_le(stream);
	return login_tokens;
}

void put_misc_string_var_int(char *value, binary_stream_t *stream)
{}

void put_misc_string_int_le(char *value, binary_stream_t *stream)
{}

void put_misc_byte_array_var_int(binary_stream_t value, binary_stream_t *stream)
{}

void put_misc_byte_array_signed_var_int(binary_stream_t value, binary_stream_t *stream)
{}

void put_misc_byte_array_short_le(binary_stream_t value, binary_stream_t *stream)
{}

void put_misc_login_tokens(misc_login_tokens_t value, binary_stream_t *stream)
{}