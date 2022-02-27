/*
   Copyright Alexander Argentakis
   Repo: https://github.com/MFDGaming/CNBT
   This file is licensed under the GPL v2.0 license
 */

#include "./nbt.h"
#include <string.h>

int8_t get_nbt_byte_tag(binary_stream_t *stream)
{
	return get_byte(stream);
}

int16_t get_nbt_short_tag(uint8_t endianess, binary_stream_t *stream)
{
	switch (endianess) {
		case E_BIG_ENDIAN:
			return get_short_be(stream);
		case E_LITTLE_ENDIAN:
		case E_NETWORK_ENDIAN:
			return get_short_le(stream);
		default:
			return 0;
	}
}

int32_t get_nbt_int_tag(uint8_t endianess, binary_stream_t *stream)
{
	switch (endianess) {
		case E_BIG_ENDIAN:
			return get_int_be(stream);
		case E_LITTLE_ENDIAN:
			return get_int_le(stream);
		case E_NETWORK_ENDIAN:
			return get_signed_var_int(stream);
		default:
			return 0;
	}
}

int64_t get_nbt_long_tag(uint8_t endianess, binary_stream_t *stream)
{
	switch (endianess) {
		case E_BIG_ENDIAN:
			return get_long_be(stream);
		case E_LITTLE_ENDIAN:
			return get_long_le(stream);
		case E_NETWORK_ENDIAN:
			return get_signed_var_long(stream);
		default:
			return 0;
	}
}

float get_nbt_float_tag(uint8_t endianess, binary_stream_t *stream)
{
	switch (endianess) {
		case E_BIG_ENDIAN:
			return get_float_be(stream);
		case E_LITTLE_ENDIAN:
		case E_NETWORK_ENDIAN:
			return get_float_le(stream);
		default:
			return 0;
	}
}

double get_nbt_double_tag(uint8_t endianess, binary_stream_t *stream)
{
	switch (endianess) {
		case E_BIG_ENDIAN:
			return get_double_be(stream);
		case E_LITTLE_ENDIAN:
		case E_NETWORK_ENDIAN:
			return get_double_le(stream);
		default:
			return 0;
	}
}

nbt_byte_array_t get_nbt_byte_array_tag(uint8_t endianess, binary_stream_t *stream)
{
	nbt_byte_array_t byte_array;
	byte_array.size = get_nbt_int_tag(endianess, stream);
	byte_array.data = (int8_t *) malloc(byte_array.size);
	int32_t i;
	for (i = 0; i < byte_array.size; ++i) {
		byte_array.data[i] = get_nbt_byte_tag(stream);
	}
	return byte_array;
}

char *get_nbt_string_tag(uint8_t endianess, binary_stream_t *stream)
{
	uint32_t length;
	switch (endianess) {
                case E_BIG_ENDIAN:
                        length = get_unsigned_short_be(stream);
			break;
                case E_LITTLE_ENDIAN:
			length = get_unsigned_short_le(stream);
			break;
                case E_NETWORK_ENDIAN:
                        length = get_var_int(stream);
			break;
                default:
                        length = 0;
			break;
        }
	char *result = (char *) malloc(length + 1);
	uint32_t i;
	for (i = 0; i < length; ++i) {
		result[i] = get_nbt_byte_tag(stream);
	}
	result[i] = 0;
	return result;
}

nbt_multi_t get_nbt_multi_tag(int8_t tag_id, uint8_t endianess, binary_stream_t *stream)
{
	nbt_multi_t tag;
	switch (tag_id) {
		case BYTE_TAG:
			tag.byte_tag = get_nbt_byte_tag(stream);
			break;
		case SHORT_TAG:
			tag.short_tag = get_nbt_short_tag(endianess, stream);
			break;
		case INT_TAG:
			tag.int_tag = get_nbt_int_tag(endianess, stream);
			break;
		case LONG_TAG:
			tag.long_tag = get_nbt_long_tag(endianess, stream);
			break;
		case FLOAT_TAG:
			tag.float_tag = get_nbt_float_tag(endianess, stream);
			break;
		case DOUBLE_TAG:
			tag.double_tag = get_nbt_double_tag(endianess, stream);
			break;
		case BYTE_ARRAY_TAG:
			tag.byte_array_tag = get_nbt_byte_array_tag(endianess, stream);
			break;
		case STRING_TAG:
			tag.string_tag = get_nbt_string_tag(endianess, stream);
			break;
		case LIST_TAG:
			tag.list_tag = get_nbt_list_tag(endianess, stream);
			break;
		case COMPOUND_TAG:
			tag.compound_tag = get_nbt_compound_tag(endianess, stream);
			break;
		case INT_ARRAY_TAG:
			tag.int_array_tag = get_nbt_int_array_tag(endianess, stream);
			break;
		case LONG_ARRAY_TAG:
			tag.long_array_tag = get_nbt_long_array_tag(endianess, stream);
			break;
	}
	return tag;
}

nbt_list_t get_nbt_list_tag(uint8_t endianess, binary_stream_t *stream)
{
	nbt_list_t list;
	list.tag_id = get_nbt_byte_tag(stream);
	list.size = get_nbt_int_tag(endianess, stream);
	list.data = (nbt_multi_t *) malloc(list.size * sizeof(nbt_multi_t));
	int32_t i;
	for (i = 0; i < list.size; ++i) {
		list.data[i] = get_nbt_multi_tag(list.tag_id, endianess, stream);
	}
	return list;
}

nbt_compound_t get_nbt_compound_tag(uint8_t endianess, binary_stream_t *stream)
{
	nbt_compound_t compound;
	compound.size = 0;
	compound.tag_ids = (int8_t *) malloc(0);
	compound.names = (char **) malloc(0);
	compound.data = (nbt_multi_t *) malloc(0);
	while (stream->offset < stream->size) {
		int8_t tag_id = get_nbt_byte_tag(stream);
		if (tag_id == 0) {
			break;
		}
		++compound.size;
		compound.tag_ids = (int8_t *) realloc(compound.tag_ids, compound.size);
		compound.names = (char **) realloc(compound.names, compound.size * sizeof(char *));
		compound.data = (nbt_multi_t *) realloc(compound.data, compound.size * sizeof(nbt_multi_t));
		compound.tag_ids[compound.size - 1] = tag_id;
		compound.names[compound.size - 1] = get_nbt_string_tag(endianess, stream);
		compound.data[compound.size - 1] = get_nbt_multi_tag(tag_id, endianess, stream);
	}
	return compound;
}

nbt_int_array_t get_nbt_int_array_tag(uint8_t endianess, binary_stream_t *stream)
{
	nbt_int_array_t int_array;
	int_array.size = get_nbt_int_tag(endianess, stream);
	int_array.data = (int32_t *) malloc(int_array.size * sizeof(int32_t));
	int32_t i;
	for (i = 0; i < int_array.size; ++i) {
		int_array.data[i] = get_nbt_int_tag(endianess, stream);
	}
	return int_array;
}

nbt_long_array_t get_nbt_long_array_tag(uint8_t endianess, binary_stream_t *stream)
{
	nbt_long_array_t long_array;
	long_array.size = get_nbt_int_tag(endianess, stream);
	long_array.data = (int64_t *) malloc(long_array.size * sizeof(int64_t));
	int32_t i;
	for (i = 0; i < long_array.size; ++i) {
		long_array.data[i] = get_nbt_long_tag(endianess, stream);
	}
	return long_array;
}

nbt_named_t get_nbt_named_tag(uint8_t endianess, binary_stream_t *stream)
{
	nbt_named_t tag;
	tag.tag_id = get_nbt_byte_tag(stream);
	if (tag.tag_id != END_TAG) {
		tag.name = get_nbt_string_tag(endianess, stream);
		tag.data = get_nbt_multi_tag(tag.tag_id, endianess, stream);
	}
	return tag;
}

void put_nbt_byte_tag(int8_t value, binary_stream_t *stream)
{
	put_byte(value, stream);
}

void put_nbt_short_tag(int16_t value, uint8_t endianess, binary_stream_t *stream)
{
	switch (endianess) {
		case E_BIG_ENDIAN:
			put_short_be(value, stream);
			break;
		case E_LITTLE_ENDIAN:
		case E_NETWORK_ENDIAN:
			put_short_le(value, stream);
			break;
	}
}

void put_nbt_int_tag(int32_t value, uint8_t endianess, binary_stream_t *stream)
{
	switch (endianess) {
		case E_BIG_ENDIAN:
			put_int_be(value, stream);
			break;
		case E_LITTLE_ENDIAN:
			put_int_le(value, stream);
			break;
		case E_NETWORK_ENDIAN:
			put_signed_var_int(value, stream);
			break;
	}
}

void put_nbt_long_tag(int64_t value, uint8_t endianess, binary_stream_t *stream)
{
	switch (endianess) {
		case E_BIG_ENDIAN:
			put_long_be(value, stream);
			break;
		case E_LITTLE_ENDIAN:
			put_long_le(value, stream);
			break;
		case E_NETWORK_ENDIAN:
			put_signed_var_long(value, stream);
			break;
	}
}

void put_nbt_float_tag(float value, uint8_t endianess, binary_stream_t *stream)
{
	switch (endianess) {
		case E_BIG_ENDIAN:
			put_float_be(value, stream);
			break;
		case E_LITTLE_ENDIAN:
		case E_NETWORK_ENDIAN:
			put_float_le(value, stream);
			break;
	}
}

void put_nbt_double_tag(double value, uint8_t endianess, binary_stream_t *stream)
{
	switch (endianess) {
		case E_BIG_ENDIAN:
			put_double_be(value, stream);
			break;
		case E_LITTLE_ENDIAN:
		case E_NETWORK_ENDIAN:
			put_double_le(value, stream);
			break;
	}
}

void put_nbt_byte_array_tag(nbt_byte_array_t value, uint8_t endianess, binary_stream_t *stream)
{
	put_nbt_int_tag(value.size, endianess, stream);
	int32_t i;
	for (i = 0; i < value.size; ++i) {
		put_nbt_byte_tag(value.data[i], stream);
	}
}

void put_nbt_string_tag(char *value, uint8_t endianess, binary_stream_t *stream)
{
	switch (endianess) {
		case E_BIG_ENDIAN:
			put_unsigned_short_be(strlen(value), stream);
			break;
		case E_LITTLE_ENDIAN:
			put_unsigned_short_le(strlen(value), stream);
			break;
		case E_NETWORK_ENDIAN:
			put_var_int(strlen(value), stream);
			break;
	}
	uint32_t i;
	for (i = 0; i < strlen(value); ++i) {
		put_nbt_byte_tag(value[i], stream);
	}
}

void put_nbt_multi_tag(nbt_multi_t value, int8_t tag_id, uint8_t endianess, binary_stream_t *stream)
{
	switch (tag_id) {
		case BYTE_TAG:
			put_nbt_byte_tag(value.byte_tag, stream);
			break;
		case SHORT_TAG:
			put_nbt_short_tag(value.short_tag, endianess, stream);
			break;
		case INT_TAG:
			put_nbt_int_tag(value.int_tag, endianess, stream);
			break;
		case LONG_TAG:
			put_nbt_long_tag(value.long_tag, endianess, stream);
			break;
		case FLOAT_TAG:
			put_nbt_float_tag(value.float_tag, endianess, stream);
			break;
		case DOUBLE_TAG:
			put_nbt_double_tag(value.double_tag, endianess, stream);
			break;
		case BYTE_ARRAY_TAG:
			put_nbt_byte_array_tag(value.byte_array_tag, endianess, stream);
			break;
		case STRING_TAG:
			put_nbt_string_tag(value.string_tag, endianess, stream);
			break;
		case LIST_TAG:
			put_nbt_list_tag(value.list_tag, endianess, stream);
			break;
		case COMPOUND_TAG:
			put_nbt_compound_tag(value.compound_tag, endianess, stream);
			break;
		case INT_ARRAY_TAG:
			put_nbt_int_array_tag(value.int_array_tag, endianess, stream);
			break;
		case LONG_ARRAY_TAG:
			put_nbt_long_array_tag(value.long_array_tag, endianess, stream);
			break;
	}
}

void put_nbt_list_tag(nbt_list_t value, uint8_t endianess, binary_stream_t *stream)
{
	put_nbt_byte_tag(value.tag_id, stream);
	put_nbt_int_tag(value.size, endianess, stream);
	int32_t i;
	for (i = 0; i < value.size; ++i) {
		put_nbt_multi_tag(value.data[i], value.tag_id, endianess, stream);
	}
}

void put_nbt_compound_tag(nbt_compound_t value, uint8_t endianess, binary_stream_t *stream)
{
	size_t i;
	for (i = 0; i < value.size; ++i) {
		put_nbt_byte_tag(value.tag_ids[i], stream);
		if (value.tag_ids[i] == END_TAG) {
			break;
		}
		put_nbt_string_tag(value.names[i], endianess, stream);
		put_nbt_multi_tag(value.data[i], value.tag_ids[i], endianess, stream);
	}
}

void put_nbt_int_array_tag(nbt_int_array_t value, uint8_t endianess, binary_stream_t *stream)
{
	put_nbt_int_tag(value.size, endianess, stream);
	int32_t i;
	for (i = 0; i < value.size; ++i) {
		put_nbt_int_tag(value.data[i], endianess, stream);
	}
}

void put_nbt_long_array_tag(nbt_long_array_t value, uint8_t endianess, binary_stream_t *stream)
{
	put_nbt_int_tag(value.size, endianess, stream);
	int32_t i;
	for (i = 0; i < value.size; ++i) {
		put_nbt_long_tag(value.data[i], endianess, stream);
	}
}

void put_nbt_named_tag(nbt_named_t value, uint8_t endianess, binary_stream_t *stream)
{
	put_nbt_byte_tag(value.tag_id, stream);
	if (value.tag_id != END_TAG) {
		put_nbt_string_tag(value.name, endianess, stream);
		put_nbt_multi_tag(value.data, value.tag_id, endianess, stream);
	}
}

void destroy_nbt_list(nbt_list_t value)
{
	int32_t i;
	for (i = 0; i < value.size; ++i) {
		switch (value.tag_id) {
		case STRING_TAG:
			free(value.data[i].string_tag);
			break;
		case BYTE_ARRAY_TAG:
			free(value.data[i].byte_array_tag.data);
			break;
		case INT_ARRAY_TAG:
			free(value.data[i].int_array_tag.data);
			break;
		case LONG_ARRAY_TAG:
			free(value.data[i].long_array_tag.data);
			break;
		case LIST_TAG:
			destroy_nbt_list(value.data[i].list_tag);
			break;
		case COMPOUND_TAG:
			destroy_nbt_compound(value.data[i].compound_tag);
			break;
		}
	}
	free(value.data);
}

void destroy_nbt_compound(nbt_compound_t value)
{
	int32_t i;
	for (i = 0; i < value.size; ++i) {
		free(value.names[i]);
		switch (value.tag_ids[i]) {
		case STRING_TAG:
			free(value.data[i].string_tag);
			break;
		case BYTE_ARRAY_TAG:
			free(value.data[i].byte_array_tag.data);
			break;
		case INT_ARRAY_TAG:
			free(value.data[i].int_array_tag.data);
			break;
		case LONG_ARRAY_TAG:
			free(value.data[i].long_array_tag.data);
			break;
		case LIST_TAG:
			destroy_nbt_list(value.data[i].list_tag);
			break;
		case COMPOUND_TAG:
			destroy_nbt_compound(value.data[i].compound_tag);
			break;
		}
	}
	free(value.data);
	free(value.names);
	free(value.tag_ids);
}

void destroy_nbt_named(nbt_named_t value)
{
	free(value.name);
	switch (value.tag_id) {
	case STRING_TAG:
		free(value.data.string_tag);
		break;
	case BYTE_ARRAY_TAG:
		free(value.data.byte_array_tag.data);
		break;
	case INT_ARRAY_TAG:
		free(value.data.int_array_tag.data);
		break;
	case LONG_ARRAY_TAG:
		free(value.data.long_array_tag.data);
		break;
	case LIST_TAG:
		destroy_nbt_list(value.data.list_tag);
		break;
	case COMPOUND_TAG:
		destroy_nbt_compound(value.data.compound_tag);
		break;
	}
}
