/*
                   Podrum R3 Copyright MFDGaming & PodrumTeam
                 This file is licensed under the GPLv2 license.
              To use this file you must own a copy of the license.
                       If you do not you can get it from:
            http://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
 */

#include <podrum/network/raknet/rakinternal.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

internal_frame_t get_internal_frame(binary_stream_t *stream)
{
	internal_frame_t internal_frame;
	get_unsigned_byte(stream);
	uint32_t size = get_unsigned_int_le(stream);
	internal_frame.address.address = (char *) malloc(size + 1);
	uint32_t i;
	for (i = 0; i < size; ++i) {
		internal_frame.address.address[i] = (char) get_unsigned_byte(stream);
	}
	internal_frame.address.address[size] = 0x00;
	internal_frame.address.port = get_unsigned_short_le(stream);
	internal_frame.address.version = get_unsigned_byte(stream);
	internal_frame.frame.reliability = get_unsigned_byte(stream);
	if (is_reliable(internal_frame.frame.reliability) == 1) {
		internal_frame.frame.reliable_frame_index = get_unsigned_triad_le(stream);
	}
	if (is_sequenced(internal_frame.frame.reliability) == 1) {
		internal_frame.frame.sequenced_frame_index = get_unsigned_triad_le(stream);
	}
	if (is_sequenced_or_ordered(internal_frame.frame.reliability) == 1) {
		internal_frame.frame.ordered_frame_index = get_unsigned_triad_le(stream);
		internal_frame.frame.order_channel = get_unsigned_byte(stream);
	}
	internal_frame.frame.stream.size = get_unsigned_int_le(stream);
	internal_frame.frame.stream.buffer = get_bytes(internal_frame.frame.stream.size, stream);
	internal_frame.frame.stream.offset = 0;
	internal_frame.frame.is_fragmented = 0;
	return internal_frame;
}

misc_address_t get_internal_new_incoming_connection(binary_stream_t *stream)
{
	misc_address_t address;
	get_unsigned_byte(stream);
	uint32_t size = get_unsigned_int_le(stream);
	address.address = (char *) malloc(size + 1);
	uint32_t i;
	for (i = 0; i < size; ++i) {
		address.address[i] = (char) get_unsigned_byte(stream);
	}
	address.address[size] = 0x00;
	address.port = get_unsigned_short_le(stream);
	address.version = get_unsigned_byte(stream);
	return address;
}

misc_address_t get_internal_disconnect_notification(binary_stream_t *stream)
{
	misc_address_t address;
	get_unsigned_byte(stream);
	uint32_t size = get_unsigned_int_le(stream);
	address.address = (char *) malloc(size + 1);
	uint32_t i;
	for (i = 0; i < size; ++i) {
		address.address[i] = (char) get_unsigned_byte(stream);
	}
	address.address[size] = 0x00;
	address.port = get_unsigned_short_le(stream);
	address.version = get_unsigned_byte(stream);
	return address;
}

internal_set_option_t get_internal_set_option(binary_stream_t *stream)
{
	internal_set_option_t internal_set_option;
	uint32_t i;
	get_unsigned_byte(stream);
	uint32_t name_size = get_unsigned_int_le(stream);
	internal_set_option.name = (char *) malloc(name_size + 1);
	for (i = 0; i < name_size; ++i) {
		internal_set_option.name[i] = (char) get_unsigned_byte(stream);
	}
	internal_set_option.name[name_size] = 0x00;
	uint32_t option_size = get_unsigned_int_le(stream);
	internal_set_option.option = (char *) malloc(option_size + 1);
	for (i = 0; i < option_size; ++i) {
		internal_set_option.option[i] = (char) get_unsigned_byte(stream);
	}
	internal_set_option.option[option_size] = 0x00;
	return internal_set_option;
}

void put_internal_frame(internal_frame_t internal_frame, binary_stream_t *stream)
{
	put_unsigned_byte(INTERNAL_FRAME, stream);
	uint32_t size = strlen(internal_frame.address.address);
	put_unsigned_int_le(size, stream);
	put_bytes((int8_t *) internal_frame.address.address, size, stream);
	put_unsigned_short_le(internal_frame.address.port, stream);
	put_unsigned_byte(internal_frame.address.version, stream);
	put_unsigned_byte(internal_frame.frame.reliability, stream);
	if (is_reliable(internal_frame.frame.reliability) == 1) {
		put_unsigned_triad_le(internal_frame.frame.reliable_frame_index, stream);
	}
	if (is_sequenced(internal_frame.frame.reliability) == 1) {
		put_unsigned_triad_le(internal_frame.frame.sequenced_frame_index, stream);
	}
	if (is_sequenced_or_ordered(internal_frame.frame.reliability) == 1) {
		put_unsigned_triad_le(internal_frame.frame.ordered_frame_index, stream);
		put_unsigned_byte(internal_frame.frame.order_channel, stream);
	}
	put_unsigned_int_le(internal_frame.frame.stream.size, stream);
	put_bytes((int8_t *) internal_frame.frame.stream.buffer, internal_frame.frame.stream.size, stream);
}

void put_internal_new_incoming_connection(misc_address_t address, binary_stream_t *stream)
{
	put_unsigned_byte(INTERNAL_NEW_INCOMING_CONNECTION, stream);
	uint32_t size = strlen(address.address);
	put_unsigned_int_le(size, stream);
	put_bytes((int8_t *) address.address, size, stream);
	put_unsigned_short_le(address.port, stream);
	put_unsigned_byte(address.version, stream);
}

void put_internal_disconnect_notification(misc_address_t address, binary_stream_t *stream)
{
	put_unsigned_byte(INTERNAL_DISCONNECT_NOTIFICATION, stream);
	uint32_t size = strlen(address.address);
	put_unsigned_int_le(size, stream);
	put_bytes((int8_t *) address.address, size, stream);
	put_unsigned_short_le(address.port, stream);
	put_unsigned_byte(address.version, stream);
}

void put_internal_set_option(internal_set_option_t internal_set_option, binary_stream_t *stream)
{
	put_unsigned_byte(INTERNAL_SET_OPTION, stream);
	uint32_t name_size = strlen(internal_set_option.name);
	put_unsigned_int_le(name_size, stream);
	put_bytes((int8_t *) internal_set_option.name, name_size, stream);
	uint32_t option_size = strlen(internal_set_option.option);
	put_unsigned_int_le(option_size, stream);
	put_bytes((int8_t *) internal_set_option.option, option_size, stream);
}
