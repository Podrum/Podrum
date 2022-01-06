/*
                   Podrum R3 Copyright MFDGaming & PodrumTeam
                 This file is licensed under the GPLv2 license.
              To use this file you must own a copy of the license.
                       If you do not you can get it from:
            http://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
 */

#include "./handler.h"
#include <stdlib.h>
#include <stdio.h>
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
	open_connection_reply_1.mtu_size = 28 + open_connection_request_1.mtu_size; // IP header (20) + UDP header (8) + Payload (x)
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
	system_address.address = "255.255.255.255";
	system_address.port = 19132;
	system_address.version = 4;
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

void handle_ack(binary_stream_t *stream, raknet_server_t *server, connection_t *connection)
{
	packet_acknowledge_t acknowledge = get_packet_acknowledge(stream);
	int i;
	for (i = 0; i < acknowledge.sequence_numbers_count; ++i) {
		deduct_raknet_recovery_queue(acknowledge.sequence_numbers[i], connection);
	}
	free(acknowledge.sequence_numbers);
}

void handle_nack(binary_stream_t *stream, raknet_server_t *server, connection_t *connection)
{
	packet_acknowledge_t acknowledge = get_packet_acknowledge(stream);
	int i;
	for (i = 0; i < acknowledge.sequence_numbers_count; ++i) {
		packet_frame_set_t frame_set = pop_raknet_recovery_queue(acknowledge.sequence_numbers[i], connection);
		if (frame_set.sequence_number != 0 && frame_set.frames_count != 0 && frame_set.frames != NULL) {
			frame_set.sequence_number = connection->sender_sequence_number;
			socket_data_t output_socket_data;
			output_socket_data.stream.buffer = malloc(0);
			output_socket_data.stream.offset = 0;
			output_socket_data.stream.size = 0;
			put_packet_frame_set(frame_set, ((&(output_socket_data.stream))));
			output_socket_data.address = connection->address;
			send_data(server->sock, output_socket_data);
			free(output_socket_data.stream.buffer);
			memset(&output_socket_data, 0, sizeof(socket_data_t));
			free(frame_set.frames);
			memset(&frame_set, 0, sizeof(packet_frame_set_t));
		}
	}
	free(acknowledge.sequence_numbers);
}

void handle_frame(misc_frame_t frame, raknet_server_t *server, connection_t *connection);

void handle_fragmented_frame(misc_frame_t frame, raknet_server_t *server, connection_t *connection)
{
	append_raknet_frame_holder(frame, connection);
	if (get_raknet_compound_size(frame.compound_id, connection) == frame.compound_size) {
		misc_frame_t output_frame;
		output_frame.is_fragmented = 0;
		output_frame.stream.buffer = malloc(0);
		output_frame.stream.offset = 0;
		output_frame.stream.size = 0;
		int i;
		for (i = 0; i < frame.compound_size; ++i) {
			misc_frame_t compound_entry = pop_raknet_compound_entry(frame.compound_id, i, connection);
			put_bytes(compound_entry.stream.buffer, compound_entry.stream.size, ((&(output_frame.stream))));
			free(compound_entry.stream.buffer);
			memset(&compound_entry, 0, sizeof(misc_frame_t));
		}
		handle_frame(output_frame, server, connection);
	}
}

void handle_frame(misc_frame_t frame, raknet_server_t *server, connection_t *connection)
{
	if (frame.is_fragmented != 0) {
		handle_fragmented_frame(frame, server, connection);
		return;
	}
	printf("-> 0x%X\n", frame.stream.buffer[0] & 0xff);
	if ((frame.stream.buffer[0] & 0xff) == ID_CONNECTION_REQUEST) {
		misc_frame_t output_frame;
		output_frame.is_fragmented = 0;
		output_frame.reliability = RELIABILITY_UNRELIABLE;
		output_frame.stream = handle_connection_request(((&(frame.stream))), server, connection->address);
		add_to_raknet_queue(output_frame, connection, server);
	} else if ((frame.stream.buffer[0] & 0xff) == ID_CONNECTED_PING) {
		misc_frame_t output_frame;
		output_frame.is_fragmented = 0;
		output_frame.reliability = RELIABILITY_UNRELIABLE;
		output_frame.stream = handle_connected_ping(((&(frame.stream))), server);
		add_to_raknet_queue(output_frame, connection, server);
	} else if ((frame.stream.buffer[0] & 0xff) == ID_DISCONNECT_NOTIFICATION) {
		disconnect_raknet_client(connection, server);
	}
	free(frame.stream.buffer);
	memset(&frame, 0, sizeof(misc_frame_t));
}

void handle_frame_set(binary_stream_t *stream, raknet_server_t *server, connection_t *connection)
{
	packet_frame_set_t frame_set = get_packet_frame_set(stream);
	deduct_raknet_nack_queue(frame_set.sequence_number, connection);
	append_raknet_ack_queue(frame_set.sequence_number, connection);
	unsigned long hole_size = frame_set.sequence_number - connection->receiver_sequence_number;
	if (hole_size > 0) {
		unsigned long sequence_number;
		for (sequence_number = connection->receiver_sequence_number + 1; sequence_number < hole_size; ++sequence_number) {
			append_raknet_nack_queue(sequence_number, connection);
		}
	}
	connection->receiver_sequence_number = frame_set.sequence_number;
	int i;
	for (i = 0; i < frame_set.frames_count; ++i) {
		if (is_reliable(frame_set.frames[i].reliability) == 0) {
			handle_frame(frame_set.frames[i], server, connection);
		} else {
			hole_size = frame_set.frames[i].reliable_frame_index - connection->receiver_reliable_frame_index;
			if (hole_size == 0) {
				handle_frame(frame_set.frames[i], server, connection);
				++connection->receiver_reliable_frame_index;
			}
		}
	}
	free(frame_set.frames);
}