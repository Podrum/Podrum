/*
                   Podrum R3 Copyright MFDGaming & PodrumTeam
                 This file is licensed under the GPLv2 license.
              To use this file you must own a copy of the license.
                       If you do not you can get it from:
            http://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
 */

#ifndef PODRUM_NETWORK_RAKNET_RAKSERVER_H
#define PODRUM_NETWORK_RAKNET_RAKSERVER_H

#include "./misc.h"
#include "./packet.h"
#include "./socket.h"

typedef struct {
	unsigned short mtu_size;
	misc_address_t address;
	unsigned guid;
	unsigned long sender_sequence_number;
	unsigned long receiver_sequence_number;
	unsigned long sender_reliable_frame_index;
	unsigned long receiver_reliable_frame_index;
	unsigned char sender_order_channels[32];
	unsigned char sender_sequence_channels[32];
	packet_frame_set_t queue;
	unsigned long *ack_queue;
	int ack_queue_size;
	unsigned long *nack_queue;
	int nack_queue_size;
	misc_frame_t *frame_holder;
	int frame_holder_size;
	packet_frame_set_t *recovery_queue;
	int recovery_queue_size;
	unsigned long compound_id;
	double ms;
	double last_receive_time;
	double last_ping_time;
} connection_t;

typedef void (*on_frame_executor_t)(misc_frame_t frame, connection_t *connection);

typedef void (*on_new_incoming_connection_executor_t)(connection_t *connection);

typedef void (*on_disconnect_notification_executor_t)(misc_address_t address);

typedef struct {
	unsigned long long guid;
	misc_address_t address;
	on_frame_executor_t on_frame_executor;
	on_new_incoming_connection_executor_t on_new_incoming_connection_executor;
	on_disconnect_notification_executor_t on_disconnect_notification_executor;
	connection_t *connections;
	int connections_count;
	char *message;
	int sock;
	double epoch;
} raknet_server_t;

double get_raknet_timestamp(raknet_server_t *server);

char has_raknet_connection(misc_address_t address, raknet_server_t *server);

void add_raknet_connection(misc_address_t address, unsigned short mtu_size, unsigned long long guid, raknet_server_t *server);

void remove_raknet_connection(misc_address_t address, raknet_server_t *server);

connection_t *get_raknet_connection(misc_address_t address, raknet_server_t *server);

char is_in_raknet_recovery_queue(unsigned long sequence_number, connection_t *connection);

void append_raknet_recovery_queue(packet_frame_set_t frame_set, connection_t *connection);

void deduct_raknet_recovery_queue(unsigned long sequence_number, connection_t *connection);

packet_frame_set_t pop_raknet_recovery_queue(unsigned long sequence_number, connection_t *connection);

char is_in_raknet_ack_queue(unsigned long sequence_number, connection_t *connection);

void append_raknet_ack_queue(unsigned long sequence_number, connection_t *connection);

char is_in_raknet_nack_queue(unsigned long sequence_number, connection_t *connection);

void deduct_raknet_nack_queue(unsigned long sequence_number, connection_t *connection);

void append_raknet_nack_queue(unsigned long sequence_number, connection_t *connection);

void send_raknet_ack_queue(connection_t *connection, raknet_server_t *server);

void send_raknet_nack_queue(connection_t *connection, raknet_server_t *server);

void send_raknet_queue(connection_t *connection, raknet_server_t *server);

void append_raknet_frame(misc_frame_t frame, int opts, connection_t *connection, raknet_server_t *server);

void add_to_raknet_queue(misc_frame_t frame, connection_t *connection, raknet_server_t *server);

char is_in_raknet_frame_holder(unsigned short compound_id, unsigned long index, connection_t *connection);

void append_raknet_frame_holder(misc_frame_t frame, connection_t *connection);

int get_raknet_compound_size(unsigned short compound_id, connection_t *connection);

misc_frame_t pop_raknet_compound_entry(unsigned short compound_id, unsigned long index, connection_t *connection);

void disconnect_raknet_client(connection_t *connection, raknet_server_t *server);

void handle_raknet_packet(raknet_server_t *server);

#endif
