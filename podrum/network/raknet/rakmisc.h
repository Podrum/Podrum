/*
                   Podrum R3 Copyright MFDGaming & PodrumTeam
                 This file is licensed under the GPLv2 license.
              To use this file you must own a copy of the license.
                       If you do not you can get it from:
            http://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
 */

#ifndef PODRUM_NETWORK_RAKNET_RAKMISC_H
#define PODRUM_NETWORK_RAKNET_RAKMISC_H

#include <stdint.h>
#include <cbinarystream/binary_stream.h>

#define RELIABILITY_UNRELIABLE 0x00
#define RELIABILITY_UNRELIABLE_SEQUENCED 0x01
#define RELIABILITY_RELIABLE 0x02
#define RELIABILITY_RELIABLE_ORDERED 0x03
#define RELIABILITY_RELIABLE_SEQUENCED 0x04
#define RELIABILITY_UNRELIABLE_WITH_ACK_RECEIPT 0x05
#define RELIABILITY_RELIABLE_WITH_ACK_RECEIPT 0x06
#define RELIABILITY_RELIABLE_ORDERED_WITH_ACK_RECEIPT 0x07

typedef struct {
	uint8_t version;
	char *address;
	uint16_t port;
} misc_address_t;

typedef struct {
	uint8_t reliability;
	uint8_t is_fragmented;
	uint32_t reliable_frame_index;
	uint32_t sequenced_frame_index;
	uint32_t ordered_frame_index;
	uint8_t order_channel;
	uint32_t compound_size;
	uint16_t compound_id;
	uint32_t index;
	binary_stream_t stream;
} misc_frame_t;

int is_reliable(int reliability);

int is_sequenced(int reliability);

int is_ordered(int reliability);

int is_sequenced_or_ordered(int reliability);

misc_frame_t get_misc_frame(binary_stream_t *stream);

misc_address_t get_misc_address(binary_stream_t *stream);

void put_misc_frame(misc_frame_t frame, binary_stream_t *stream);

void put_misc_address(misc_address_t address, binary_stream_t *stream);

size_t get_frame_size(misc_frame_t frame);

#endif
