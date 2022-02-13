/*
                   Podrum R3 Copyright MFDGaming & PodrumTeam
                 This file is licensed under the GPLv2 license.
              To use this file you must own a copy of the license.
                       If you do not you can get it from:
            http://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
 */

#include "./mcplayer.h"
#include "./mcpackets.h"

void send_minecraft_packet(binary_stream_t *streams, size_t streams_count, connection_t *connection, raknet_server_t *server)
{
	packet_game_t game;
	game.streams = streams;
	game.streams_count = streams_count;
	misc_frame_t frame;
	frame.is_fragmented = 0;
	frame.reliability = RELIABILITY_RELIABLE_ORDERED;
	frame.order_channel = 0;
	frame.stream.buffer = (int8_t *) malloc(0);
	frame.stream.offset = 0;
	frame.stream.size = 0;
	put_packet_game(game, (&(frame.stream)));
	add_to_raknet_queue(frame, connection, server);
}