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
	unsigned long sender_sequenced_frame_index;
	unsigned long receiver_sequenced_frame_index;
	unsigned long sender_ordered_frame_index;
	unsigned long receiver_ordered_frame_index;
	unsigned char sender_order_channels[32];
	unsigned char sender_sequence_channels[32];
	packet_frame_set_t queue;
	unsigned int *ack_queue;
	int ack_queue_size;
	unsigned int *nack_queue;
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

typedef void (*on_frame_executor_t)(misc_frame_t frame, connection_t connection);

typedef void (*on_new_incomming_connection_executor_t)(connection_t connection);

typedef void (*on_diconnect_notification_executor_t)(connection_t connection);

typedef struct {
	unsigned long long guid;
	misc_address_t address;
	on_frame_executor_t on_frame_executor;
	on_new_incomming_connection_executor_t on_new_incomming_connection_executor;
	on_diconnect_notification_executor_t on_diconnect_notification_executor;
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

void handle_raknet_packet(raknet_server_t *server);

#endif
