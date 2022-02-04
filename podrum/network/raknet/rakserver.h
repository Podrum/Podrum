/*
                   Podrum R3 Copyright MFDGaming & PodrumTeam
                 This file is licensed under the GPLv2 license.
              To use this file you must own a copy of the license.
                       If you do not you can get it from:
            http://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
 */

#ifndef PODRUM_NETWORK_RAKNET_RAKSERVER_H
#define PODRUM_NETWORK_RAKNET_RAKSERVER_H

#include <stdint.h>
#include <stdlib.h>

#include "./rakmisc.h"
#include "./rakpacket.h"
#include "./socket.h"

typedef struct {
	uint16_t mtu_size;
	misc_address_t address;
	unsigned guid;
	uint32_t sender_sequence_number;
	uint32_t receiver_sequence_number;
	uint32_t sender_reliable_frame_index;
	uint32_t receiver_reliable_frame_index;
	uint8_t sender_order_channels[32];
	uint8_t sender_sequence_channels[32];
	packet_frame_set_t queue;
	uint32_t *ack_queue;
	uint16_t ack_queue_size;
	uint32_t *nack_queue;
	uint16_t nack_queue_size;
	misc_frame_t *frame_holder;
	size_t frame_holder_size;
	packet_frame_set_t *recovery_queue;
	size_t recovery_queue_size;
	uint32_t compound_id;
	double ms;
	double last_receive_time;
	double last_ping_time;
} connection_t;

struct _raknet_server;

typedef struct _raknet_server raknet_server_t;

typedef void (*on_frame_executor_t)(misc_frame_t frame, connection_t *connection, raknet_server_t *server);

typedef void (*on_new_incoming_connection_executor_t)(connection_t *connection);

typedef void (*on_disconnect_notification_executor_t)(misc_address_t address);

struct _raknet_server {
	uint64_t guid;
	misc_address_t address;
	on_frame_executor_t on_frame_executor;
	on_new_incoming_connection_executor_t on_new_incoming_connection_executor;
	on_disconnect_notification_executor_t on_disconnect_notification_executor;
	connection_t *connections;
	size_t connections_count;
	char *message;
	int sock;
	double epoch;
};

double get_raknet_timestamp(raknet_server_t *server);

uint8_t has_raknet_connection(misc_address_t address, raknet_server_t *server);

void add_raknet_connection(misc_address_t address, uint16_t mtu_size, uint64_t guid, raknet_server_t *server);

void remove_raknet_connection(misc_address_t address, raknet_server_t *server);

connection_t *get_raknet_connection(misc_address_t address, raknet_server_t *server);

uint8_t is_in_raknet_recovery_queue(uint32_t sequence_number, connection_t *connection);

void append_raknet_recovery_queue(packet_frame_set_t frame_set, connection_t *connection);

void deduct_raknet_recovery_queue(uint32_t sequence_number, connection_t *connection);

packet_frame_set_t pop_raknet_recovery_queue(uint32_t sequence_number, connection_t *connection);

uint8_t is_in_raknet_ack_queue(uint32_t sequence_number, connection_t *connection);

void append_raknet_ack_queue(uint32_t sequence_number, connection_t *connection);

uint8_t is_in_raknet_nack_queue(uint32_t sequence_number, connection_t *connection);

void deduct_raknet_nack_queue(uint32_t sequence_number, connection_t *connection);

void append_raknet_nack_queue(uint32_t sequence_number, connection_t *connection);

void send_raknet_ack_queue(connection_t *connection, raknet_server_t *server);

void send_raknet_nack_queue(connection_t *connection, raknet_server_t *server);

void send_raknet_queue(connection_t *connection, raknet_server_t *server);

void append_raknet_frame(misc_frame_t frame, int opts, connection_t *connection, raknet_server_t *server);

void add_to_raknet_queue(misc_frame_t frame, connection_t *connection, raknet_server_t *server);

uint8_t is_in_raknet_frame_holder(uint16_t compound_id, uint32_t index, connection_t *connection);

void append_raknet_frame_holder(misc_frame_t frame, connection_t *connection);

size_t get_raknet_compound_size(uint16_t compound_id, connection_t *connection);

misc_frame_t pop_raknet_compound_entry(uint16_t compound_id, uint32_t index, connection_t *connection);

void disconnect_raknet_client(connection_t *connection, raknet_server_t *server);

void handle_raknet_packet(raknet_server_t *server);

#endif
