/*
                   Podrum R3 Copyright MFDGaming & PodrumTeam
                 This file is licensed under the GPLv2 license.
              To use this file you must own a copy of the license.
                       If you do not you can get it from:
            http://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
 */

#ifndef PODRUM_NETWORK_MINECRAFT_MCPACKETS_H
#define PODRUM_NETWORK_MINECRAFT_MCPACKETS_H

#include <cbinarystream/binary_stream.h>
#include <stdlib.h>
#include "./mcmisc.h"

#define GAME_VERSION (char *) "1.18.2"
#define GAME_PROTOCOL_VERSION 475

#define ID_GAME 0xFE
#define ID_LOGIN 0x01
#define ID_PLAY_STATUS 0x02
#define ID_RESOURCE_PACKS_INFO 0x06
#define ID_RESOURCE_PACK_STACK 0x07
#define ID_RESOURCE_PACK_CLIENT_RESPONSE 0x08
#define ID_START_GAME 0x0b

#define PLAY_STATUS_LOGIN_SUCCESS 0
#define PLAY_STATUS_FAILED_CLIENT 1
#define PLAY_STATUS_FAILED_SPAWN 2
#define PLAY_STATUS_PLAYER_SPAWN 3
#define PLAY_STATUS_FAILED_INVALID_TENANT 4
#define PLAY_STATUS_FAILED_VANILLA_EDU 5
#define PLAY_STATUS_FAILED_EDU_VANILLA 6
#define PLAY_STATUS_FAILED_SERVER_FULL 7

#define RESOURCE_PACK_CLIENT_RESPONSE_NONE 0
#define RESOURCE_PACK_CLIENT_RESPONSE_REFUSED 1
#define RESOURCE_PACK_CLIENT_RESPONSE_SEND_PACKS 2
#define RESOURCE_PACK_CLIENT_RESPONSE_HAVE_ALL_PACKS 3
#define RESOURCE_PACK_CLIENT_RESPONSE_COMPLETED 4

typedef struct {
	binary_stream_t *streams;
	size_t streams_count;
} packet_game_t;

typedef struct {
	int32_t protocol_version;
	misc_login_tokens_t tokens;
} packet_login_t;

typedef struct {
	int32_t status;
} packet_play_status_t;

typedef struct {
	char must_accept;
	char has_scripts;
	char force_server_packs;
	misc_behavior_pack_infos_t behavior_packs;
	misc_texture_pack_infos_t texture_packs;
} packet_resource_packs_info_t;

typedef struct {
	char must_accept;
	misc_resource_pack_id_versions_t behavior_packs;
	misc_resource_pack_id_versions_t resource_packs;
	char *game_version;
	misc_experiments_t experiments;
	char experiments_previously_used;
} packet_resource_pack_stack_t;

typedef struct {
	unsigned char response_status;
	misc_resource_pack_ids_t resource_pack_ids;
} packet_resource_pack_client_response_t;

packet_game_t get_packet_game(binary_stream_t *stream);

packet_login_t get_packet_login(binary_stream_t *stream);

packet_play_status_t get_packet_play_status(binary_stream_t *stream);

packet_resource_packs_info_t get_packet_resource_packs_info(binary_stream_t *stream);

packet_resource_pack_stack_t get_packet_resource_pack_stack(binary_stream_t *stream);

packet_resource_pack_client_response_t get_packet_resource_pack_client_response(binary_stream_t *stream);

void put_packet_game(packet_game_t packet, binary_stream_t *stream);

void put_packet_login(packet_login_t packet, binary_stream_t *stream);

void put_packet_play_status(packet_play_status_t packet, binary_stream_t *stream);

void put_packet_resource_packs_info(packet_resource_packs_info_t packet, binary_stream_t *stream);

void put_packet_resource_pack_stack(packet_resource_pack_stack_t packet, binary_stream_t *stream);

void put_packet_resource_pack_client_response(packet_resource_pack_client_response_t packet, binary_stream_t *stream);

#endif
