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
#include <ctype.h>

unsigned char hextobin(char hex)
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
	return 0xff; /* Use 0xff as an error code */
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
						unsigned char parts_v1[4] = {
							hextobin(json_input->json[json_input->offset++]),
							hextobin(json_input->json[json_input->offset++]),
							hextobin(json_input->json[json_input->offset++]),
							hextobin(json_input->json[json_input->offset++])
						};
						if (parts_v1[0] != 0xff && parts_v1[1] != 0xff && parts_v1[2] != 0xff && parts_v1[3] != 0xff) {
							uint32_t uni_v1 = (((parts_v1[0] << 4) | parts_v1[1]) << 8) | ((parts_v1[2] << 4) | parts_v1[3]);
							if ((len - json_input->offset) > 5) {
								if (uni_v1 >= 0xd800 && uni_v1 <= 0xdbff && json_input->json[json_input->offset++] == '\\' && json_input->json[json_input->offset++] == 'u') {
									unsigned char parts_v2[4] = {
										hextobin(json_input->json[json_input->offset++]),
										hextobin(json_input->json[json_input->offset++]),
										hextobin(json_input->json[json_input->offset++]),
										hextobin(json_input->json[json_input->offset++])
									};
									if (parts_v2[0] != 0xff && parts_v2[1] != 0xff && parts_v2[2] != 0xff && parts_v2[3] != 0xff) {
										uint16_t uni_v2 = (((parts_v2[0] << 4) | parts_v2[1]) << 8) | ((parts_v2[2] << 4) | parts_v2[3]);
										if (uni_v2 >= 0xdc00 && uni_v2 <= 0xdfff) {
											uni_v1 = 0x10000 + (((uni_v1 - 0xd800) << 10) | (uni_v2 - 0xdc00));
										}
									} else {
										free(out);
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
								free(out);
								perror("Invalid unicode character");
								exit(0);
							}
						} else {
							free(out);
							perror("Invalid hexcode");
							exit(0);
						}
					} else {
						free(out);
						perror("Unexpected EOF");
						exit(0);
					}
					break;
				}
				default:
					free(out);
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
					free(out);
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
		++json_input->offset;
		++out_len;
		out = (char *) realloc(out, out_len);
		out[out_len - 1] = 0;
		return out;
	}
	free(out);
	return NULL;
}

char parse_json_bool(json_input_t *json_input)
{
	int len = strlen(json_input->json);
	if ((len - json_input->offset) > 3) {
		if (
			json_input->json[json_input->offset] == 't' &&
			json_input->json[json_input->offset + 1] == 'r' &&
			json_input->json[json_input->offset + 2] == 'u' &&
			json_input->json[json_input->offset + 3] == 'e'
		) {
			json_input->offset += 4;
			return 1;
		}
	}
	if ((len - json_input->offset) > 4) {
		if (
			json_input->json[json_input->offset] == 'f' &&
			json_input->json[json_input->offset + 1] == 'a' &&
			json_input->json[json_input->offset + 2] == 'l' &&
			json_input->json[json_input->offset + 3] == 's' &&
			json_input->json[json_input->offset + 4] == 'e'
		) {
			json_input->offset += 5;
			return 0;
		}
	}
	return 2;
}

char parse_json_null(json_input_t *json_input)
{
	if ((strlen(json_input->json) - json_input->offset) > 3) {
		if (
			json_input->json[json_input->offset] == 'n' &&
			json_input->json[json_input->offset + 1] == 'u' &&
			json_input->json[json_input->offset + 2] == 'l' &&
			json_input->json[json_input->offset + 3] == 'l'
		) {
			json_input->offset += 4;
			return 1;
		}
	}
	return 2;
}

json_number_t parse_json_number(json_input_t *json_input)
{
	json_number_t number;
	number.type = JSON_NUMBER_INT;
	size_t size = 0;
	char *sr_number = (char *) malloc(0);
	char is_first = 1;
	char expect_digit = 0;
	char once = 0;
	while (json_input->offset < strlen(json_input->json)) {
		if (is_first == 1) {
			if (isdigit(json_input->json[json_input->offset])) {
				++size;
				sr_number = (char *) realloc(sr_number, size);
				sr_number[size - 1] = json_input->json[json_input->offset];
				++json_input->offset;
				is_first = 0;
			} else if (json_input->json[json_input->offset] == '-') {
				++size;
				sr_number = (char *) realloc(sr_number, size);
				sr_number[size - 1] = json_input->json[json_input->offset];
				++json_input->offset;
				is_first = 0;
				expect_digit = 1;
			} else {
				number.type = JSON_NUMBER_NAN;
				free(sr_number);
				return number;
			}
		} else {
			if (isdigit(json_input->json[json_input->offset])) {
				++size;
				sr_number = (char *) realloc(sr_number, size);
				sr_number[size - 1] = json_input->json[json_input->offset];
				++json_input->offset;
				if (expect_digit == 1) {
					expect_digit = 0;
				}
			} else {
				if (expect_digit == 1) {
					free(sr_number);
					perror("Expects digit");
					exit(0);
				}
				if (json_input->json[json_input->offset] == '.' && once == 0) {
					expect_digit = 1;
					once = 1;
					number.type = JSON_NUMBER_FLOAT;
					++size;
					sr_number = (char *) realloc(sr_number, size);
					sr_number[size - 1] = json_input->json[json_input->offset];
					++json_input->offset;
				} else if (
					json_input->json[json_input->offset] == '\x20' ||
					json_input->json[json_input->offset] == ',' ||
					json_input->json[json_input->offset] == ']' ||
					json_input->json[json_input->offset] == '}'
				) {
					++size;
					sr_number = (char *) realloc(sr_number, size);
					sr_number[size - 1] = 0;
					if (number.type == JSON_NUMBER_INT) {
						number.number.int_number = atoll(sr_number);
					} else if (number.type == JSON_NUMBER_FLOAT) {
						number.number.float_number = atof(sr_number);
					}
					free(sr_number);
					return number;
				} else {
					free(sr_number);
					perror("Unexpected character");
					exit(0);
				}
			}
		}
	}
	free(sr_number);
	perror("Unexpected EOF");
	exit(0);
}

json_array_t parse_json_array(json_input_t *json_input)
{
	json_array_t json_array;
	json_array.members = (json_multi_t *) malloc(0);
	json_array.types = (char *) malloc(0);
	json_array.size = 0;
	json_array.noret = 0;
	if (json_input->json[json_input->offset] == '[') {
		++json_input->offset;
		char expect_member = 1;
		char expect_seporator = 0;
		while (json_input->json[json_input->offset] != ']') {
			if (json_input->json[json_input->offset] != '\x20' && json_input->json[json_input->offset] != '\n' && json_input->json[json_input->offset] != '\t' && json_input->json[json_input->offset] != '\r') {
				if (expect_member == 1) {
					char *json_string = parse_json_string(json_input);
					if (json_string == NULL) {
						char json_null = parse_json_null(json_input);
						if (json_null == 2) {
							char json_bool = parse_json_bool(json_input);
							if (json_bool == 2) {
								json_number_t json_number = parse_json_number(json_input);
								if (json_number.type == JSON_NUMBER_NAN) {
									json_object_t nested_json_object = parse_json_object(json_input);
									if (nested_json_object.noret == 1) {
										json_array_t nested_json_array = parse_json_array(json_input);
										if (nested_json_array.noret == 1) {
											destroy_json_array(json_array);
											perror("Invalid member");
											exit(0);
										} else {
											++json_array.size;
											json_multi_t json_multi;
											json_multi.json_array = nested_json_array;
											json_array.members = (json_multi_t *) realloc(json_array.members, json_array.size * sizeof(json_multi_t));
											json_array.types = (char *) realloc(json_array.types, json_array.size);
											json_array.members[json_array.size - 1] = json_multi;
											json_array.types[json_array.size - 1] = JSON_ARRAY;
										}
									} else {
										++json_array.size;
										json_multi_t json_multi;
										json_multi.json_object = nested_json_object;
										json_array.members = (json_multi_t *) realloc(json_array.members, json_array.size * sizeof(json_multi_t));
										json_array.types = (char *) realloc(json_array.types, json_array.size);
										json_array.members[json_array.size - 1] = json_multi;
										json_array.types[json_array.size - 1] = JSON_OBJECT;
									}
								} else {
									++json_array.size;
									json_multi_t json_multi;
									json_multi.json_number = json_number;
									json_array.members = (json_multi_t *) realloc(json_array.members, json_array.size * sizeof(json_multi_t));
									json_array.types = (char *) realloc(json_array.types, json_array.size);
									json_array.members[json_array.size - 1] = json_multi;
									json_array.types[json_array.size - 1] = JSON_NUMBER;
								}
							} else {
								++json_array.size;
								json_multi_t json_multi;
								json_multi.json_bool = json_bool;
								json_array.members = (json_multi_t *) realloc(json_array.members, json_array.size * sizeof(json_multi_t));
								json_array.types = (char *) realloc(json_array.types, json_array.size);
								json_array.members[json_array.size - 1] = json_multi;
								json_array.types[json_array.size - 1] = JSON_BOOL;
							}
						} else {
							++json_array.size;
							json_multi_t json_multi;
							json_multi.json_null = NULL;
							json_array.members = (json_multi_t *) realloc(json_array.members, json_array.size * sizeof(json_multi_t));
							json_array.types = (char *) realloc(json_array.types, json_array.size);
							json_array.members[json_array.size - 1] = json_multi;
							json_array.types[json_array.size - 1] = JSON_NULL;
						}
					} else {
						++json_array.size;
						json_multi_t json_multi;
						json_multi.json_string = json_string;
						json_array.members = (json_multi_t *) realloc(json_array.members, json_array.size * sizeof(json_multi_t));
						json_array.types = (char *) realloc(json_array.types, json_array.size);
						json_array.members[json_array.size - 1] = json_multi;
						json_array.types[json_array.size - 1] = JSON_STRING;
					}
					expect_member = 0;
					expect_seporator = 1;
				} else if (json_input->json[json_input->offset] == ',' && expect_seporator == 1) {
					expect_seporator = 0;
					expect_member = 1;
					++json_input->offset;
				} else {
					destroy_json_array(json_array);
					perror("Unexpected character");
					exit(0);
				}
			} else {
				++json_input->offset;
			}
		}
		++json_input->offset;
		return json_array;
	} else {
		destroy_json_array(json_array);
		json_array.noret = 1;
		return json_array;
	}
}

json_object_t parse_json_object(json_input_t *json_input)
{
	json_object_t json_object;
	json_object.keys = (char **) malloc(0);
	json_object.members = (json_multi_t *) malloc(0);
	json_object.types = (char *) malloc(0);
	json_object.size = 0;
	json_object.noret = 0;
	if (json_input->json[json_input->offset] == '{') {
		++json_input->offset;
		char expect_key = 1;
		char expect_colon = 0;
		char expect_member = 0;
		char expect_seporator = 0;
		while (json_input->json[json_input->offset] != '}') {
			if (json_input->json[json_input->offset] != '\x20' && json_input->json[json_input->offset] != '\n' && json_input->json[json_input->offset] != '\t' && json_input->json[json_input->offset] != '\r') {
				if (expect_key == 1) {
					char *key = parse_json_string(json_input);
					if (key == NULL) {
						destroy_json_object(json_object);
						perror("Expected string");
						exit(0);
					}
					++json_object.size;
					json_object.keys = (char **) realloc(json_object.keys, json_object.size * sizeof(char *));
					json_object.keys[json_object.size - 1] = key;
					expect_key = 0;
					expect_colon = 1;
				} else if (json_input->json[json_input->offset] == ':' && expect_colon == 1) {
					expect_colon = 0;
					expect_member = 1;
					++json_input->offset;
				} else if (expect_member == 1) {
					char *json_string = parse_json_string(json_input);
					if (json_string == NULL) {
						char json_null = parse_json_null(json_input);
						if (json_null == 2) {
							char json_bool = parse_json_bool(json_input);
							if (json_bool == 2) {
								json_number_t json_number = parse_json_number(json_input);
								if (json_number.type == JSON_NUMBER_NAN) {
									json_object_t nested_json_object = parse_json_object(json_input);
									if (nested_json_object.noret == 1) {
										json_array_t nested_json_array = parse_json_array(json_input);
										if (nested_json_array.noret == 1) {
											destroy_json_object(json_object);
											perror("Invalid member");
											exit(0);
										} else {
											json_multi_t json_multi;
											json_multi.json_array = nested_json_array;
											json_object.members = (json_multi_t *) realloc(json_object.members, json_object.size * sizeof(json_multi_t));
											json_object.types = (char *) realloc(json_object.types, json_object.size);
											json_object.members[json_object.size - 1] = json_multi;
											json_object.types[json_object.size - 1] = JSON_ARRAY;
										}
									} else {
										json_multi_t json_multi;
										json_multi.json_object = nested_json_object;
										json_object.members = (json_multi_t *) realloc(json_object.members, json_object.size * sizeof(json_multi_t));
										json_object.types = (char *) realloc(json_object.types, json_object.size);
										json_object.members[json_object.size - 1] = json_multi;
										json_object.types[json_object.size - 1] = JSON_OBJECT;
									}
								} else {
									json_multi_t json_multi;
									json_multi.json_number = json_number;
									json_object.members = (json_multi_t *) realloc(json_object.members, json_object.size * sizeof(json_multi_t));
									json_object.types = (char *) realloc(json_object.types, json_object.size);
									json_object.members[json_object.size - 1] = json_multi;
									json_object.types[json_object.size - 1] = JSON_NUMBER;
								}
							} else {
								json_multi_t json_multi;
								json_multi.json_bool = json_bool;
								json_object.members = (json_multi_t *) realloc(json_object.members, json_object.size * sizeof(json_multi_t));
								json_object.types = (char *) realloc(json_object.types, json_object.size);
								json_object.members[json_object.size - 1] = json_multi;
								json_object.types[json_object.size - 1] = JSON_BOOL;
							}
						} else {
							json_multi_t json_multi;
							json_multi.json_null = NULL;
							json_object.members = (json_multi_t *) realloc(json_object.members, json_object.size * sizeof(json_multi_t));
							json_object.types = (char *) realloc(json_object.types, json_object.size);
							json_object.members[json_object.size - 1] = json_multi;
							json_object.types[json_object.size - 1] = JSON_NULL;
						}
					} else {
						json_multi_t json_multi;
						json_multi.json_string = json_string;
						json_object.members = (json_multi_t *) realloc(json_object.members, json_object.size * sizeof(json_multi_t));
						json_object.types = (char *) realloc(json_object.types, json_object.size);
						json_object.members[json_object.size - 1] = json_multi;
						json_object.types[json_object.size - 1] = JSON_STRING;
					}
					expect_member = 0;
					expect_seporator = 1;
				} else if (json_input->json[json_input->offset] == ',' && expect_seporator == 1) {
					expect_seporator = 0;
					expect_key = 1;
					++json_input->offset;
				} else {
					destroy_json_object(json_object);
					perror("Unexpected character");
					exit(0);
				}
			} else {
				++json_input->offset;
			}
		}
		++json_input->offset;
		return json_object;
	} else {
		destroy_json_object(json_object);
		json_object.noret = 1;
		return json_object;
	}
}

json_root_t parse_json_root(json_input_t *json_input)
{
	json_root_t json_root;
	json_root.type = JSON_EMPTY;
	while (json_input->offset < strlen(json_input->json)) {
		if (json_input->json[json_input->offset] != '\x20' && json_input->json[json_input->offset] != '\n' && json_input->json[json_input->offset] != '\t' && json_input->json[json_input->offset] != '\r') {
			json_object_t nested_json_object = parse_json_object(json_input);
			if (nested_json_object.noret == 1) {
				json_array_t nested_json_array = parse_json_array(json_input);
				if (nested_json_array.noret == 1) {
					perror("Invalid member");
					exit(0);
				} else {
					json_root.type = JSON_ARRAY;
					json_root.entry.json_array = nested_json_array;
					return json_root;
				}
			} else {
				json_root.type = JSON_OBJECT;
				json_root.entry.json_object = nested_json_object;
				return json_root;
			}
		} else {
			++json_input->offset;
		}
	}
}

void destroy_json_array(json_array_t json_array)
{
	size_t i;
	for (i = 0; i < json_array.size; ++i) {
		switch (json_array.types[i]) {
		case JSON_STRING:
			free(json_array.members[i].json_string);
			break;
		case JSON_OBJECT:
			destroy_json_object(json_array.members[i].json_object);
			break;
		case JSON_ARRAY:
			destroy_json_array(json_array.members[i].json_array);
			break;
		}
	}
	free(json_array.types);
	free(json_array.members);
}

void destroy_json_object(json_object_t json_object)
{
	size_t i;
	for (i = 0; i < json_object.size; ++i) {
		switch (json_object.types[i]) {
		case JSON_STRING:
			free(json_object.members[i].json_string);
			break;
		case JSON_OBJECT:
			destroy_json_object(json_object.members[i].json_object);
			break;
		case JSON_ARRAY:
			destroy_json_array(json_object.members[i].json_array);
			break;
		}
		free(json_object.keys[i]);
	}
	free(json_object.keys);
	free(json_object.types);
	free(json_object.members);
}

void destroy_json_root(json_root_t json_root)
{
	if (json_root.type == JSON_ARRAY) {
		destroy_json_array(json_root.entry.json_array);
	} else if (json_root.type == JSON_OBJECT) {
		destroy_json_object(json_root.entry.json_object);
	}
}
