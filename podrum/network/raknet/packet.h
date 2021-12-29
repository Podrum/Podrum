/*
                   Podrum R3 Copyright MFDGaming & PodrumTeam
                 This file is licensed under the GPLv2 license.
              To use this file you must own a copy of the license.
                       If you do not you can get it from:
            http://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
 */

#ifndef PODRUM_NETWORK_RAKNET_PACKET_H
#define PODRUM_NETWORK_RAKNET_PACKET_H

#include "./misc.h"

#define ID_UNCONNECTED_PING 0x01
#define ID_UNCONNECTED_PING_OPEN_CONNECTIONS 0x02
#define ID_UNCONNECTED_PONG 0x1C
#define ID_INCOMPATIBLE_PROTOCOL_VERSION 0x19
#define ID_OPEN_CONNECTION_REQUEST_1 0x05
#define ID_OPEN_CONNECTION_REPLY_1 0x06
#define ID_OPEN_CONNECTION_REQUEST_2 0x07
#define ID_OPEN_CONNECTION_REPLY_2 0x08
#define ID_ACK 0xC0
#define ID_NACK 0xA0
#define ID_FRAME_SET 0x80
#define ID_CONNECTION_REQUEST 0x09
#define ID_CONNECTION_REQUEST_ACCEPTED 0x10
#define ID_NEW_INCOMING_CONNECTION 0x13
#define ID_CONNECTED_PING 0x01
#define ID_CONNECTED_PONG 0x03

char MAGIC[16] = {
	0x00, 0xff, 0xff, 0x00,
	0xfe, 0xfe, 0xfe, 0xfe,
	0xfd, 0xfd, 0xfd, 0xfd,
	0x12, 0x34, 0x56, 0x78
};

typedef struct {
	unsigned long long timestamp;
	unsigned long long guid;
} packet_unconnected_ping_t;

typedef struct {
	unsigned long long timestamp;
	unsigned long long guid;
	char *message;
} packet_unconnected_pong_t;

typedef struct {
	unsigned char protocol_version;
	unsigned long long guid;
} packet_incompatible_protocol_version_t;

typedef struct {
	unsigned char protocol_version;
	unsigned short mtu_size;
} packet_open_connection_request_1_t;

typedef struct {
	unsigned long long guid;
	unsigned char use_security;
	unsigned short mtu_size;
} packet_open_connection_reply_1_t;

typedef struct {
	misc_address_t address;
	unsigned short mtu_size;
	unsigned long long guid;
} packet_open_connection_request_2_t;

typedef struct {
	unsigned long long guid;
	misc_address_t address;
	unsigned short mtu_size;
	unsigned char use_encryption;
} packet_open_connection_reply_2_t;

typedef struct {
	unsigned long *sequence_numbers;
	unsigned short sequence_numbers_count;
} packet_acknowledge_t;

typedef struct {
	unsigned long sequence_number;
	misc_frame_t *frames;
	int frames_count;
} packet_frame_set_t;

typedef struct {
	unsigned long long guid;
	unsigned long long timestamp;
} packet_connection_request_t;

typedef struct {
	misc_address_t address;
	unsigned short system_index;
	misc_address_t system_addresses[20];
	unsigned long long request_timestamp;
	unsigned long long reply_timestamp;
} packet_connection_request_accepted_t;

typedef struct {
	misc_address_t address;
	misc_address_t system_addresses[20];
	unsigned long long request_timestamp;
	unsigned long long reply_timestamp;
} packet_new_incoming_connection_t;

typedef struct {
	unsigned long long timestamp;
} packet_connected_ping_t;

typedef struct {
	unsigned long long request_timestamp;
	unsigned long long reply_timestamp;
} packet_connected_pong_t;

packet_unconnected_ping_t get_packet_unconnected_ping(binary_stream_t *stream);

packet_unconnected_pong_t get_packet_unconnected_pong(binary_stream_t *stream);

packet_incompatible_protocol_version_t get_packet_incompatible_protocol_version(binary_stream_t *stream);

packet_open_connection_request_1_t get_packet_open_connection_request_1(binary_stream_t *stream);

packet_open_connection_reply_1_t get_packet_open_connection_reply_1(binary_stream_t *stream);

packet_open_connection_request_2_t get_packet_open_connection_request_2(binary_stream_t *stream);

packet_open_connection_reply_2_t get_packet_open_connection_reply_2(binary_stream_t *stream);

packet_acknowledge_t get_packet_acknowledge(binary_stream_t *stream);

packet_frame_set_t get_packet_frame_set(binary_stream_t *stream);

packet_connection_request_t get_packet_connection_request(binary_stream_t *stream);

packet_connection_request_accepted_t get_packet_connection_request_accepted(binary_stream_t *stream);

packet_new_incoming_connection_t get_packet_new_incoming_connection(binary_stream_t *stream);

packet_connected_ping_t get_packet_connected_ping(binary_stream_t *stream);

packet_connected_pong_t get_packet_connected_pong(binary_stream_t *stream);

void put_packet_unconnected_ping(packet_unconnected_ping_t packet, int opts, binary_stream_t *stream);

void put_packet_unconnected_pong(packet_unconnected_pong_t packet, binary_stream_t *stream);

void put_packet_incompatible_protocol_version(packet_incompatible_protocol_version_t packet, binary_stream_t *stream);

void put_packet_open_connection_request_1(packet_open_connection_request_1_t packet, binary_stream_t *stream);

void put_packet_open_connection_reply_1(packet_open_connection_reply_1_t packet, binary_stream_t *stream);

void put_packet_open_connection_request_2(packet_open_connection_request_2_t packet, binary_stream_t *stream);

void put_packet_open_connection_reply_2(packet_open_connection_reply_2_t packet, binary_stream_t *stream);

void put_packet_acknowledge(packet_acknowledge_t packet, int opts, binary_stream_t *stream);

void put_packet_frame_set(packet_frame_set_t packet, binary_stream_t *stream);

void put_packet_connection_request(packet_connection_request_t packet, binary_stream_t *stream);

void put_packet_connection_request_accepted(packet_connection_request_accepted_t packet, binary_stream_t *stream);

void put_packet_new_incoming_connection(packet_new_incoming_connection_t packet, binary_stream_t *stream);

void put_packet_connected_ping(packet_connected_ping_t packet, binary_stream_t *stream);

void put_packet_connected_pong(packet_connected_pong_t packet, binary_stream_t *stream);

#endif