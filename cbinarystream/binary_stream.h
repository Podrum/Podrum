/*
   Copyright Alexander Argentakis
   Repo: https://github.com/MFDGaming/CBinaryStream
   This file is licensed under the GPL v2.0 license
 */

#ifndef BINARY_STREAM_H
#define BINARY_STREAM_H

typedef struct {
	char *buffer;
	int size;
	int offset;
} binary_stream_t;

char *get_bytes(int count, binary_stream_t *stream);

char *get_remaining_bytes(binary_stream_t *stream);

unsigned char get_unsigned_byte(binary_stream_t *stream);

char get_byte(binary_stream_t *stream);

unsigned short get_unsigned_short_le(binary_stream_t *stream);

unsigned short get_unsigned_short_be(binary_stream_t *stream);

short get_short_le(binary_stream_t *stream);

short get_short_be(binary_stream_t *stream);

unsigned long get_unsigned_triad_le(binary_stream_t *stream);

unsigned long get_unsigned_triad_be(binary_stream_t *stream);

long get_triad_le(binary_stream_t *stream);

long get_triad_be(binary_stream_t *stream);

unsigned long get_unsigned_int_le(binary_stream_t *stream);

unsigned long get_unsigned_int_be(binary_stream_t *stream);

long get_int_le(binary_stream_t *stream);

long get_int_be(binary_stream_t *stream);

unsigned long long get_unsigned_long_le(binary_stream_t *stream);

unsigned long long get_unsigned_long_be(binary_stream_t *stream);

long long get_long_le(binary_stream_t *stream);

long long get_long_be(binary_stream_t *stream);

unsigned long get_var_int(binary_stream_t *stream);

long get_signed_var_int(binary_stream_t *stream);

unsigned long long get_var_long(binary_stream_t *stream);

long long get_signed_var_long(binary_stream_t *stream);

float get_float_le(binary_stream_t *stream);

float get_float_be(binary_stream_t *stream);

double get_double_le(binary_stream_t *stream);

double get_double_be(binary_stream_t *stream);

void put_bytes(char *data, int size, binary_stream_t *stream);

void put_unsigned_byte(unsigned char value, binary_stream_t *stream);

void put_byte(char value, binary_stream_t *stream);

void put_unsigned_short_le(unsigned short value, binary_stream_t *stream);

void put_unsigned_short_be(unsigned short value, binary_stream_t *stream);

void put_short_le(short value, binary_stream_t *stream);

void put_short_be(short value, binary_stream_t *stream);

void put_unsigned_triad_le(unsigned long value, binary_stream_t *stream);

void put_unsigned_triad_be(unsigned long value, binary_stream_t *stream);

void put_triad_le(long value, binary_stream_t *stream);

void put_triad_be(long value, binary_stream_t *stream);

void put_unsigned_int_le(unsigned long value, binary_stream_t *stream);

void put_unsigned_int_be(unsigned long value, binary_stream_t *stream);

void put_int_le(long value, binary_stream_t *stream);

void put_int_be(long value, binary_stream_t *stream);

void put_unsigned_long_le(unsigned long long value, binary_stream_t *stream);

void put_unsigned_long_be(unsigned long long value, binary_stream_t *stream);

void put_long_le(long long value, binary_stream_t *stream);

void put_long_be(long long value, binary_stream_t *stream);

void put_var_int(unsigned long value, binary_stream_t *stream);

void put_signed_var_int(long value, binary_stream_t *stream);

void put_var_long(unsigned long long value, binary_stream_t *stream);

void put_signed_var_long(long long value, binary_stream_t *stream);

void put_float_le(float value, binary_stream_t *stream);

void put_float_be(float value, binary_stream_t *stream);

void put_double_le(double value, binary_stream_t *stream);

void put_double_be(double value, binary_stream_t *stream);

#endif
