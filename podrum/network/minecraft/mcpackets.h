#ifndef PODRUM_NETWORK_MINECRAFT_PACKETS_H
#define PODRUM_NETWORK_MINECRAFT_PACKETS_H

#include <cbinarystream/binary_stream.h>

#define ID_GAME 0xFE

typedef struct {
	binary_stream_t *streams;
	int streams_count;
} packet_game_t;

packet_game_t get_packet_game(binary_stream_t *stream);

void put_packet_game(packet_game_t packet, binary_stream_t *stream);

#endif
