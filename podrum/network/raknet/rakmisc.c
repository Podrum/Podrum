/*
                   Podrum R3 Copyright MFDGaming & PodrumTeam
                 This file is licensed under the GPLv2 license.
              To use this file you must own a copy of the license.
                       If you do not you can get it from:
            http://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
 */

#include "./rakmisc.h"
#include <stdlib.h>
#include <stdio.h>

#ifdef _WIN32

#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")

#else

#include <arpa/inet.h>

#endif

int is_reliable(int reliability)
{
	if (reliability == RELIABILITY_RELIABLE ||
	    reliability == RELIABILITY_RELIABLE_ORDERED ||
	    reliability == RELIABILITY_RELIABLE_SEQUENCED ||
	    reliability == RELIABILITY_RELIABLE_WITH_ACK_RECEIPT ||
	    reliability == RELIABILITY_RELIABLE_ORDERED_WITH_ACK_RECEIPT
	) {
		return 1;
	}
	return 0;
}

int is_sequenced(int reliability)
{
	if (reliability == RELIABILITY_UNRELIABLE_SEQUENCED ||
	    reliability == RELIABILITY_RELIABLE_SEQUENCED
	) {
		return 1;
	}
	return 0;
}

int is_ordered(int reliability)
{
	if (reliability == RELIABILITY_RELIABLE_ORDERED ||
	    reliability == RELIABILITY_RELIABLE_ORDERED_WITH_ACK_RECEIPT
	) {
		return 1;
	}
	return 0;
}

int is_sequenced_or_ordered(int reliability)
{
	if (reliability == RELIABILITY_UNRELIABLE_SEQUENCED ||
	    reliability == RELIABILITY_RELIABLE_ORDERED ||
	    reliability == RELIABILITY_RELIABLE_SEQUENCED ||
	    reliability == RELIABILITY_RELIABLE_ORDERED_WITH_ACK_RECEIPT
	) {
		return 1;
	}
	return 0;
}

misc_frame_t get_misc_frame(binary_stream_t *stream)
{
	misc_frame_t frame;
	uint8_t flags = get_unsigned_byte(stream);
	frame.reliability = (flags & 0xf4) >> 5;
	frame.is_fragmented = (flags & 0x10) > 0;
	frame.stream.offset = 0;
	frame.stream.size = get_unsigned_short_be(stream) >> 3;
	frame.stream.buffer = (int8_t *) malloc(frame.stream.size);
	if (is_reliable(frame.reliability) == 1) {
		frame.reliable_frame_index = get_unsigned_triad_le(stream);
	}
	if (is_sequenced(frame.reliability) == 1) {
		frame.sequenced_frame_index = get_unsigned_triad_le(stream);
	}
	if (is_sequenced_or_ordered(frame.reliability) == 1) {
		frame.ordered_frame_index = get_unsigned_triad_le(stream);
		frame.order_channel = get_unsigned_byte(stream);
	}
	if (frame.is_fragmented != 0) {
		frame.compound_size = get_unsigned_int_be(stream);
		frame.compound_id = get_unsigned_short_be(stream);
		frame.index = get_unsigned_int_be(stream);
	}
	size_t i;
	for (i = 0; i < frame.stream.size; ++i)
	{
		frame.stream.buffer[i] = get_unsigned_byte(stream);
	}
	return frame;
}

misc_address_t get_misc_address(binary_stream_t *stream)
{
	misc_address_t address;
	address.version = get_unsigned_byte(stream);
	if (address.version == 4) {
		uint8_t part_1 = ~get_unsigned_byte(stream) & 0xff;
		uint8_t part_2 = ~get_unsigned_byte(stream) & 0xff;
		uint8_t part_3 = ~get_unsigned_byte(stream) & 0xff;
		uint8_t part_4 = ~get_unsigned_byte(stream) & 0xff;
		int size = snprintf(NULL, 0, "%d.%d.%d.%d", part_1, part_2, part_3, part_4);
		address.address = (char *) malloc(size + 1);
		address.address[size] = 0x00;
		sprintf(address.address, "%u.%u.%u.%u", part_1, part_2, part_3, part_4);
		address.port = get_unsigned_short_be(stream);
	}
	return address;
}

void put_misc_frame(misc_frame_t frame, binary_stream_t *stream)
{
	put_unsigned_byte((frame.reliability << 5) | (frame.is_fragmented != 0 ? 0x10 : 0x00), stream);
	put_unsigned_short_be(frame.stream.size << 3, stream);
	if (is_reliable(frame.reliability) == 1) {
		put_unsigned_triad_le(frame.reliable_frame_index, stream);
	}
	if (is_sequenced(frame.reliability) == 1) {
		put_unsigned_triad_le(frame.sequenced_frame_index, stream);
	}
	if (is_sequenced_or_ordered(frame.reliability) == 1) {
		put_unsigned_triad_le(frame.ordered_frame_index, stream);
		put_unsigned_byte(frame.order_channel, stream);
	}
	if (frame.is_fragmented != 0) {
		put_unsigned_int_be(frame.compound_size, stream);
		put_unsigned_short_be(frame.compound_id, stream);
		put_unsigned_int_be(frame.index, stream);
	}
	size_t i;
	for (i = 0; i < frame.stream.size; ++i)
	{
		put_unsigned_byte(frame.stream.buffer[i], stream);
	}
}

void put_misc_address(misc_address_t address, binary_stream_t *stream)
{
	put_unsigned_byte(address.version, stream);
	if (address.version == 4) {
		int packed_address = ~inet_addr(address.address);
		put_int_le(packed_address, stream);
		put_unsigned_short_be(address.port, stream);
	}
}

size_t get_frame_size(misc_frame_t frame)
{
	size_t size = 3 + frame.stream.size;
	if (is_reliable(frame.reliability) == 1) {
		size += 3;
	}
	if (is_sequenced(frame.reliability) == 1) {
		size += 3;
	}
	if (is_sequenced_or_ordered(frame.reliability) == 1) {
		size += 4;
	}
	if (frame.is_fragmented != 0) {
		size += 10;
	}
	return size;
}