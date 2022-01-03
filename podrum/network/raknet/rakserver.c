/*
                   Podrum R3 Copyright MFDGaming & PodrumTeam
                 This file is licensed under the GPLv2 license.
              To use this file you must own a copy of the license.
                       If you do not you can get it from:
            http://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
 */

#include "./rakserver.h"
#include "./handler.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>

char has_connection(misc_address_t address, raknet_server_t *server)
{
	int i;
	for (i = 0; i < server->connections_count; ++i) {
		if ((server->connections[i].address.port == address.port) && (strcmp(server->connections[i].address.address, address.address) == 0)) {
			return 1;
		}
	}
	return 0;
}

void add_connection(misc_address_t address, unsigned short mtu_size, unsigned long long guid, raknet_server_t *server)
{
	if (has_connection(address, server) == 0) {
		printf("%s:%d connected!\n", address.address, address.port);
		connection_t connection;
		connection.ack_queue = malloc(0);
		connection.ack_queue_size = 0;
		connection.address = address;
		connection.compound_id = 0;
		connection.frame_holder = malloc(0);
		connection.frame_holder_size = 0;
		connection.guid = guid;
		connection.last_ping_time = time(NULL);
		connection.last_receive_time = time(NULL);
		connection.ms = 0;
		connection.mtu_size = mtu_size;
		connection.nack_queue = malloc(0);
		connection.nack_queue_size = 0;
		connection.queue.frames = malloc(0);
		connection.queue.frames_count = 0;
		connection.queue.sequence_number = 0;
		connection.receiver_ordered_frame_index = 0;
		connection.receiver_reliable_frame_index = 0;
		connection.receiver_sequence_number = 0;
		connection.receiver_sequenced_frame_index = 0;
		connection.recovery_queue = malloc(0);
		connection.recovery_queue_size = 0;
		memset(connection.sender_order_channels, 0, sizeof(connection.sender_order_channels));
		connection.sender_ordered_frame_index = 0;
		connection.sender_reliable_frame_index = 0;
		memset(connection.sender_sequence_channels, 0, sizeof(connection.sender_order_channels));
		connection.sender_sequence_number = 0;
		connection.sender_sequenced_frame_index = 0;
		++server->connections_count;
		server->connections = realloc(server->connections, server->connections_count * sizeof(connection_t));
		server->connections[server->connections_count - 1] = connection;
	}
}

void remove_connection(misc_address_t address, raknet_server_t *server)
{
	if (has_connection(address, server) == 1) {
		int i;
		connection_t *connections = malloc((server->connections_count - 1) * sizeof(connection_t));
		int connections_count = 0;
		for (i = 0; i < server->connections_count; ++i) {
			if ((server->connections[i].address.port != address.port) || (strcmp(server->connections[i].address.address, address.address) != 0)) {
				connections[connections_count] = server->connections[i];
				++connections_count;
			}
		}
		free(server->connections);
		server->connections = connections;
		--server->connections_count;
	}
}

void handle_packet(raknet_server_t *server)
{
	socket_data_t socket_data = receive_data(server->sock);
	if (socket_data.stream.size > 0) {
		/* Just a debug thing
			int i;
			for (i = 0; i < socket_data.stream.size; ++i) {
				printf("0x%X ", socket_data.stream.buffer[i] & 0xff);
			}
			printf("\n");
		*/
		if ((socket_data.stream.buffer[0] & 0xff) == ID_UNCONNECTED_PING)
		{
			socket_data_t output_socket_data;
			output_socket_data.stream = handle_unconneted_ping((&(socket_data.stream)), server);
			output_socket_data.address = socket_data.address;
			send_data(server->sock, output_socket_data);
			free(output_socket_data.stream.buffer);
			memset(&output_socket_data, 0, sizeof(socket_data_t));
		} else if ((socket_data.stream.buffer[0] & 0xff) == ID_OPEN_CONNECTION_REQUEST_1) {
			socket_data_t output_socket_data;
			output_socket_data.stream = handle_open_connection_request_1((&(socket_data.stream)), server);
			output_socket_data.address = socket_data.address;
			send_data(server->sock, output_socket_data);
			free(output_socket_data.stream.buffer);
			memset(&output_socket_data, 0, sizeof(socket_data_t));
		} else if ((socket_data.stream.buffer[0] & 0xff) == ID_OPEN_CONNECTION_REQUEST_2) {
			socket_data_t output_socket_data;
			output_socket_data.stream = handle_open_connection_request_2((&(socket_data.stream)), server, socket_data.address);
			output_socket_data.address = socket_data.address;
			send_data(server->sock, output_socket_data);
			free(output_socket_data.stream.buffer);
			memset(&output_socket_data, 0, sizeof(socket_data_t));
		} else {
			printf("0x%X\n", socket_data.stream.buffer[0] & 0xff);
		}
	}
	free(socket_data.stream.buffer);
	memset(&socket_data, 0, sizeof(socket_data_t));
}
