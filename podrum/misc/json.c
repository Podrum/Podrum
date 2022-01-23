/*
                   Podrum R3 Copyright MFDGaming & PodrumTeam
                 This file is licensed under the GPLv2 license.
              To use this file you must own a copy of the license.
                       If you do not you can get it from:
            http://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
 */

#include "./json.h"
#include <string.h>

char hextobin(char hex)
{
	switch (hex) {
	case '0':
		return 0x00;
	case '1':
		return 0x01;
	case '2':
		return 0x02;
	case '3':
		return 0x03;
	case '4':
		return 0x04;
	case '5':
		return 0x05;
	case '6':
		return 0x06;
	case '7':
		return 0x07;
	case '8':
		return 0x08;
	case '9':
		return 0x09;
	case 'A':
	case 'a':
		return 0x0A;
	case 'B':
	case 'b':
		return 0x0B;
	case 'C':
	case 'c':
		return 0x0C;
	case 'D':
	case 'd':
		return 0x0D;
	case 'E':
	case 'e':
		return 0x0E;
	case 'F':
	case 'f':
		return 0x0F;
	}
	return -1;
}

/* Part of python's json implementation
	uni = _decode_uXXXX(s, end)
	end += 5
	if 0xd800 <= uni <= 0xdbff and s[end:end + 2] == '\\u':
		uni2 = _decode_uXXXX(s, end + 1)
		if 0xdc00 <= uni2 <= 0xdfff:
			uni = 0x10000 + (((uni - 0xd800) << 10) | (uni2 - 0xdc00))
			end += 6
	char = chr(uni)
 */

void parse_json_string(json_input_t *json_input)
{
	char *out = (char *) malloc(1);
	int out_len = 1;
	char is_esc_code = 0;
	if (json_input->json[json_input->offset] == '"') {
		++json_input->offset;
		while (json_input->json[json_input->offset] != '"') {
			if (is_esc_code) {
				switch (json_input->json[json_input->offset]) {
				case 'n':
					++out_len;
					out = (char *) realloc(out, out_len);
					out[out_len - 1] = '\n';
					++json_input->offset;
					break;
				case '\\':
					++out_len;
					out = (char *) realloc(out, out_len);
					out[out_len - 1] = '\\';
					++json_input->offset;
					break;
				case '"':
					++out_len;
					out = (char *) realloc(out, out_len);
					out[out_len - 1] = '"';
					++json_input->offset;
					break;
				case '/':
					++out_len;
					out = (char *) realloc(out, out_len);
					out[out_len - 1] = '/';
					++json_input->offset;
					break;
				case 'b':
					++out_len;
					out = (char *) realloc(out, out_len);
					out[out_len - 1] = '\b';
					++json_input->offset;
					break;
				case 'r':
					++out_len;
					out = (char *) realloc(out, out_len);
					out[out_len - 1] = '\r';
					++json_input->offset;
					break;
				case 't':
					++out_len;
					out = (char *) realloc(out, out_len);
					out[out_len - 1] = '\t';
					++json_input->offset;
					break;
				case 'f':
					++out_len;
					out = (char *) realloc(out, out_len);
					out[out_len - 1] = '\f';
					++json_input->offset;
					break;
				case 'u':
					break;
				}
			}
		}
	}
}

void parse_json_object(json_input_t *json_input)
{
	if (json_input->json[json_input->offset] == '{') {
		++json_input->offset;
		while (json_input->json[json_input->offset] != '}') {
			if (json_input->json[json_input->offset] == '{') {
				parse_json_object(json_input);
			} else {
				++json_input->offset;
			}
		}
	}
}
