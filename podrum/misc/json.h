/*
                   Podrum R3 Copyright MFDGaming & PodrumTeam
                 This file is licensed under the GPLv2 license.
              To use this file you must own a copy of the license.
                       If you do not you can get it from:
            http://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
 */

#ifndef PODRUM_MISC_JSON_H
#define PODRUM_MISC_JSON_H

#include <stdint.h>

#define JSON_OBJECT 0
#define JSON_ARRAY 1
#define JSON_STRING 2
#define JSON_INT 3
#define JSON_FLOAT 4
#define JSON_BOOL 5
#define JSON_NULL 6

union _multi_json;

typedef union _multi_json multi_json_t;

typedef struct {
	multi_json_t value;
	char type;
} json_member_t;

typedef struct {
	char *name;
	json_member_t member;
} json_key_t;

typedef struct {
	json_key_t *members;
	multi_json_t *size;
} json_object_t;

typedef struct {
	json_member_t *members;
	multi_json_t *size;
} json_array_t;

union _multi_json {
	json_object_t json_object;
	json_array_t json_array;
	char *json_string;
	int64_t json_int;
	double json_float;
	char json_bool;
	void *json_null;
};

#endif