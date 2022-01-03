/*
                   Podrum R3 Copyright MFDGaming & PodrumTeam
                 This file is licensed under the GPLv2 license.
              To use this file you must own a copy of the license.
                       If you do not you can get it from:
            http://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
 */

#include "./handler.h"
#include <stdlib.h>

binary_stream_t handle_unconneted_ping(binary_stream_t *stream, raknet_server_t *server)
{
	packet_unconnected_ping_t unconnected_ping = get_packet_unconnected_ping(stream);
	packet_unconnected_pong_t unconnected_pong;
	unconnected_pong.timestamp = unconnected_ping.timestamp;
	unconnected_pong.guid = server->guid;
	unconnected_pong.message = server->message;
	binary_stream_t output_stream;
	output_stream.buffer = malloc(0);
	output_stream.offset = 0;
	output_stream.size = 0;
	put_packet_unconnected_pong(unconnected_pong, &output_stream);
	return output_stream;
}