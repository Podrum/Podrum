/*
                   Podrum R3 Copyright MFDGaming & PodrumTeam
                 This file is licensed under the GPLv2 license.
              To use this file you must own a copy of the license.
                       If you do not you can get it from:
            http://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
 */

#include "./handler.h"
#include <stdlib.h>
#include <string.h>

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

binary_stream_t handle_open_connection_request_1(binary_stream_t *stream, raknet_server_t *server)
{
	packet_open_connection_request_1_t open_connection_request_1 = get_packet_open_connection_request_1(stream);
	packet_open_connection_reply_1_t open_connection_reply_1;
	open_connection_reply_1.mtu_size = open_connection_request_1.mtu_size;
	open_connection_reply_1.use_security = 0;
	open_connection_reply_1.guid = server->guid;
	binary_stream_t output_stream;
	output_stream.buffer = malloc(0);
	output_stream.offset = 0;
	output_stream.size = 0;
	put_packet_open_connection_reply_1(open_connection_reply_1, &output_stream);
	return output_stream;
}

binary_stream_t handle_open_connection_request_2(binary_stream_t *stream, raknet_server_t *server, misc_address_t address)
{
	packet_open_connection_request_2_t open_connection_request_2 = get_packet_open_connection_request_2(stream);
	packet_open_connection_reply_2_t open_connection_reply_2;
	open_connection_reply_2.address = address;
	open_connection_reply_2.mtu_size = open_connection_request_2.mtu_size;
	open_connection_reply_2.guid = server->guid;
	open_connection_reply_2.use_encryption = 0;
	binary_stream_t output_stream;
	output_stream.buffer = malloc(0);
	output_stream.offset = 0;
	output_stream.size = 0;
	put_packet_open_connection_reply_2(open_connection_reply_2, &output_stream);
	add_raknet_connection(address, open_connection_request_2.mtu_size, open_connection_request_2.guid, server);
	return output_stream;
}

binary_stream_t handle_connection_request(binary_stream_t *stream, raknet_server_t *server, misc_address_t address)
{
	packet_connection_request_t connection_request = get_packet_connection_request(stream);
	packet_connection_request_accepted_t connection_request_accepted;
	connection_request_accepted.address = address;
	connection_request_accepted.system_index = 0;
	misc_address_t system_address;
	address.address = "255.255.255.255";
	address.port = 19132;
	address.version = 4;
	int i;
	for (i = 0; i < 20; ++i) {
		connection_request_accepted.system_addresses[i] = system_address;
	}
	connection_request_accepted.request_timestamp = connection_request.timestamp;
	connection_request_accepted.reply_timestamp = get_raknet_timestamp(server);
	binary_stream_t output_stream;
	output_stream.buffer = malloc(0);
	output_stream.offset = 0;
	output_stream.size = 0;
	put_packet_connection_request_accepted(connection_request_accepted, &output_stream);
	return output_stream;
}

binary_stream_t handle_connected_ping(binary_stream_t *stream, raknet_server_t *server)
{
	packet_connected_ping_t connected_ping = get_packet_connected_ping(stream);
	packet_connected_pong_t connected_pong;
	connected_pong.request_timestamp = connected_ping.timestamp;
	connected_pong.reply_timestamp = get_raknet_timestamp(server);
	binary_stream_t output_stream;
	output_stream.buffer = malloc(0);
	output_stream.offset = 0;
	output_stream.size = 0;
	put_packet_connected_pong(connected_pong, &output_stream);
	return output_stream;
}