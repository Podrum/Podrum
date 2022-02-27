/*
   Copyright Alexander Argentakis
   Repo: https://github.com/MFDGaming/CNBT
   This file is licensed under the GPL v2.0 license
 */

#ifndef NBT_H
#define NBT_H

#include <cbinarystream/binary_stream.h>
#include <stdint.h>
#include <stdlib.h>

#define END_TAG 0
#define BYTE_TAG 1
#define SHORT_TAG 2
#define INT_TAG 3
#define LONG_TAG 4
#define FLOAT_TAG 5
#define DOUBLE_TAG 6
#define BYTE_ARRAY_TAG 7
#define STRING_TAG 8
#define LIST_TAG 9
#define COMPOUND_TAG 10
#define INT_ARRAY_TAG 11
#define LONG_ARRAY_TAG 12

#define E_BIG_ENDIAN 0
#define E_LITTLE_ENDIAN 1
#define E_NETWORK_ENDIAN 2

union _nbt_multi;
typedef union _nbt_multi nbt_multi_t;

typedef struct {
	int8_t *data;
	int32_t size;
} nbt_byte_array_t;

typedef struct {
	int32_t *data;
	int32_t size;
} nbt_int_array_t;

typedef struct {
	int64_t *data;
	int32_t size;
} nbt_long_array_t;

typedef struct {
	int8_t tag_id;
	int32_t size;
	nbt_multi_t *data;
} nbt_list_t;

typedef struct {
	size_t size;
	int8_t *tag_ids;
	char **names;
	nbt_multi_t *data;
} nbt_compound_t;

union _nbt_multi {
	int8_t byte_tag;
	int16_t short_tag;
	int32_t int_tag;
	int64_t long_tag;
	float float_tag;
	double double_tag;
	nbt_byte_array_t byte_array_tag;
	char *string_tag;
	nbt_list_t list_tag;
	nbt_int_array_t int_array_tag;
	nbt_long_array_t long_array_tag;
	nbt_compound_t compound_tag;
};

typedef struct {
	char *name;
	int8_t tag_id;
	nbt_multi_t data;
} nbt_named_t;

int8_t get_nbt_byte_tag(binary_stream_t *stream);

int16_t get_nbt_short_tag(uint8_t endianess, binary_stream_t *stream);

int32_t get_nbt_int_tag(uint8_t endianess, binary_stream_t *stream);

int64_t get_nbt_long_tag(uint8_t endianess, binary_stream_t *stream);

float get_nbt_float_tag(uint8_t endianess, binary_stream_t *stream);

double get_nbt_double_tag(uint8_t endianess, binary_stream_t *stream);

nbt_byte_array_t get_nbt_byte_array_tag(uint8_t endianess, binary_stream_t *stream);

char *get_nbt_string_tag(uint8_t endianess, binary_stream_t *stream);

nbt_multi_t get_nbt_multi_tag(int8_t tag_id, uint8_t endianess, binary_stream_t *stream);

nbt_list_t get_nbt_list_tag(uint8_t endianess, binary_stream_t *stream);

nbt_compound_t get_nbt_compound_tag(uint8_t endianess, binary_stream_t *stream);

nbt_int_array_t get_nbt_int_array_tag(uint8_t endianess, binary_stream_t *stream);

nbt_long_array_t get_nbt_long_array_tag(uint8_t endianess, binary_stream_t *stream);

nbt_named_t get_nbt_named_tag(uint8_t endianess, binary_stream_t *stream);

void put_nbt_byte_tag(int8_t value, binary_stream_t *stream);

void put_nbt_short_tag(int16_t value, uint8_t endianess, binary_stream_t *stream);

void put_nbt_int_tag(int32_t value, uint8_t endianess, binary_stream_t *stream);

void put_nbt_long_tag(int64_t value, uint8_t endianess, binary_stream_t *stream);

void put_nbt_float_tag(float value, uint8_t endianess, binary_stream_t *stream);

void put_nbt_double_tag(double value, uint8_t endianess, binary_stream_t *stream);

void put_nbt_byte_array_tag(nbt_byte_array_t value, uint8_t endianess, binary_stream_t *stream);

void put_nbt_string_tag(char *value, uint8_t endianess, binary_stream_t *stream);

void put_nbt_multi_tag(nbt_multi_t value, int8_t tag_id, uint8_t endianess, binary_stream_t *stream);

void put_nbt_list_tag(nbt_list_t value, uint8_t endianess, binary_stream_t *stream);

void put_nbt_compound_tag(nbt_compound_t value, uint8_t endianess, binary_stream_t *stream);

void put_nbt_int_array_tag(nbt_int_array_t value, uint8_t endianess, binary_stream_t *stream);

void put_nbt_long_array_tag(nbt_long_array_t value, uint8_t endianess, binary_stream_t *stream);

void put_nbt_named_tag(nbt_named_t value, uint8_t endianess, binary_stream_t *stream);

void destroy_nbt_list(nbt_list_t value);

void destroy_nbt_compound(nbt_compound_t value);

void destroy_nbt_named(nbt_named_t value);

#endif
