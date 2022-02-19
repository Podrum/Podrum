/*
                   Podrum R3 Copyright MFDGaming & PodrumTeam
                 This file is licensed under the GPLv2 license.
              To use this file you must own a copy of the license.
                       If you do not you can get it from:
            http://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
 */

#ifndef PODRUM_NETWORK_RAKNET_RAKPACKET_H
#define PODRUM_NETWORK_RAKNET_RAKPACKET_H

#include <stdint.h>
#include <podrum/network/raknet/rakmisc.h>

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
#define ID_CONNECTED_PING 0x00
#define ID_CONNECTED_PONG 0x03
#define ID_DISCONNECT_NOTIFICATION 0x15

#define MAGIC "\x00\xff\xff\x00\xfe\xfe\xfe\xfe\xfd\xfd\xfd\xfd\x12\x34\x56\x78"

typedef struct {
	uint64_t timestamp;
	uint64_t guid;
} packet_unconnected_ping_t;

typedef struct {
	uint64_t timestamp;
	uint64_t guid;
	char *message;
} packet_unconnected_pong_t;

typedef struct {
	uint8_t protocol_version;
	uint64_t guid;
} packet_incompatible_protocol_version_t;

typedef struct {
	uint8_t protocol_version;
	uint16_t mtu_size;
} packet_open_connection_request_1_t;

typedef struct {
	uint64_t guid;
	uint8_t use_security;
	uint16_t mtu_size;
} packet_open_connection_reply_1_t;

typedef struct {
	misc_address_t address;
	uint16_t mtu_size;
	uint64_t guid;
} packet_open_connection_request_2_t;

typedef struct {
	uint64_t guid;
	misc_address_t address;
	uint16_t mtu_size;
	uint8_t use_encryption;
} packet_open_connection_reply_2_t;

typedef struct {
	uint32_t *sequence_numbers;
	uint16_t sequence_numbers_count;
} packet_acknowledge_t;

typedef struct {
	uint32_t sequence_number;
	misc_frame_t *frames;
	size_t frames_count;
} packet_frame_set_t;

typedef struct {
	uint64_t guid;
	uint64_t timestamp;
} packet_connection_request_t;

typedef struct {
	misc_address_t address;
	uint16_t system_index;
	misc_address_t system_addresses[20];
	uint64_t request_timestamp;
	uint64_t reply_timestamp;
} packet_connection_request_accepted_t;

typedef struct {
	misc_address_t address;
	misc_address_t system_addresses[20];
	uint64_t request_timestamp;
	uint64_t reply_timestamp;
} packet_new_incoming_connection_t;

typedef struct {
	uint64_t timestamp;
} packet_connected_ping_t;

typedef struct {
	uint64_t request_timestamp;
	uint64_t reply_timestamp;
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