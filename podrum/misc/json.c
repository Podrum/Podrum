/*
                   Podrum R3 Copyright MFDGaming & PodrumTeam
                 This file is licensed under the GPLv2 license.
              To use this file you must own a copy of the license.
                       If you do not you can get it from:
            http://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
 */

#include "./json.h"
#include <string.h>
#include <stdio.h>

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

char *parse_json_string(json_input_t *json_input)
{
	char *out = (char *) malloc(0);
	int out_len = 0;
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
				{
					++json_input->offset;
					int len = strlen(json_input->json);
					if ((len - json_input->offset) > 3) {
						char parts_v1[4] = {
							hextobin(json_input->json[json_input->offset++]),
							hextobin(json_input->json[json_input->offset++]),
							hextobin(json_input->json[json_input->offset++]),
							hextobin(json_input->json[json_input->offset++])
						};
						if (parts_v1[0] != -1 && parts_v1[1] != -1 && parts_v1[2] != -1 && parts_v1[3] != -1) {
							uint32_t uni_v1 = (((parts_v1[0] << 4) | parts_v1[1]) << 8) | ((parts_v1[2] << 4) | parts_v1[3]);
							if ((len - json_input->offset) > 5) {
								if (uni_v1 >= 0xd800 && uni_v1 <= 0xdbff && json_input->json[json_input->offset++] == '\\' && json_input->json[json_input->offset++] == 'u') {
									char parts_v2[4] = {
										hextobin(json_input->json[json_input->offset++]),
										hextobin(json_input->json[json_input->offset++]),
										hextobin(json_input->json[json_input->offset++]),
										hextobin(json_input->json[json_input->offset++])
									};
									if (parts_v2[0] != -1 && parts_v2[1] != -1 && parts_v2[2] != -1 && parts_v2[3] != -1) {
										uint16_t uni_v2 = (((parts_v2[0] << 4) | parts_v2[1]) << 8) | ((parts_v2[2] << 4) | parts_v2[3]);
										if (uni_v2 >= 0xdc00 && uni_v2 <= 0xdfff) {
											uni_v1 = 0x10000 + (((uni_v1 - 0xd800) << 10) | (uni_v2 - 0xdc00));
										}
									} else {
										perror("Invalid hexcode");
										exit(0);
									}
								}
							}
							if (uni_v1 <= 0x7F) {
								// Plain ASCII
								++out_len;
								out = (char *) realloc(out, out_len);
								out[out_len - 1] = (char) uni_v1;
							} else if (uni_v1 <= 0x07FF) {
								// 2-byte unicode
								out_len += 2;
								out = (char *) realloc(out, out_len);
								out[out_len - 2] = (char) (((uni_v1 >> 6) & 0x1F) | 0xC0);
								out[out_len - 1] = (char) (((uni_v1 >> 0) & 0x3F) | 0x80);
							} else if (uni_v1 <= 0xFFFF) {
								// 3-byte unicode
								out_len += 3;
								out = (char *) realloc(out, out_len);
								out[out_len - 3] = (char) (((uni_v1 >> 12) & 0x0F) | 0xE0);
								out[out_len - 2] = (char) (((uni_v1 >> 6) & 0x3F) | 0x80);
								out[out_len - 1] = (char) (((uni_v1 >> 0) & 0x3F) | 0x80);
							} else if (uni_v1 <= 0x10FFFF) {
								// 4-byte unicode
								out_len += 4;
								out = (char *) realloc(out, out_len);
								out[out_len - 4] = (char) (((uni_v1 >> 18) & 0x07) | 0xF0);
								out[out_len - 3] = (char) (((uni_v1 >> 12) & 0x3F) | 0x80);
								out[out_len - 2] = (char) (((uni_v1 >> 6) & 0x3F) | 0x80);
								out[out_len - 1] = (char) (((uni_v1 >> 0) & 0x3F) | 0x80);
							} else { 
								perror("Invalid unicode character");
								exit(0);
							}
						} else {
							perror("Invalid hexcode");
							exit(0);
						}
					} else {
						perror("Unexpected EOF");
						exit(0);
					}
					break;
				}
				default:
					perror("Invalid escape code");
					exit(0);
					break;
				}
				is_esc_code = 0;
			} else {
				switch (json_input->json[json_input->offset]) {
				case '\a':
				case '\b':
				case '\f':
				case '\n':
				case '\r':
				case '\t':
				case '\v':
				case '\?':
					perror("Invalid escape code");
					exit(0);
				case '\\':
					is_esc_code = 1;
					break;
				case '"':
					break;
				default:
					++out_len;
					out = (char *) realloc(out, out_len);
					out[out_len - 1] = json_input->json[json_input->offset];
				}
				++json_input->offset;
			}
		}
	}
	++out_len;
	out = (char *) realloc(out, out_len);
	out[out_len - 1] = 0;
	return out;
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
