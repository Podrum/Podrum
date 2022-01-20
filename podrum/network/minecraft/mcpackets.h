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

#define ID_GAME 0xFE
#define ID_LOGIN 0x01
#define ID_PLAY_STATUS 0x02

#define PLAY_STATUS_LOGIN_SUCCESS 0
#define PLAY_STATUS_FAILED_CLIENT 1
#define PLAY_STATUS_FAILED_SPAWN 2
#define PLAY_STATUS_PLAYER_SPAWN 3
#define PLAY_STATUS_FAILED_INVALID_TENANT 4
#define PLAY_STATUS_FAILED_VANILLA_EDU 5
#define PLAY_STATUS_FAILED_EDU_VANILLA 6
#define PLAY_STATUS_FAILED_SERVER_FULL 7

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

packet_game_t get_packet_game(binary_stream_t *stream);

packet_login_t get_packet_login(binary_stream_t *stream);

packet_play_status_t get_packet_play_status(binary_stream_t *stream);

void put_packet_game(packet_game_t packet, binary_stream_t *stream);

void put_packet_login(packet_login_t packet, binary_stream_t *stream);

void put_packet_play_status(packet_play_status_t packet, binary_stream_t *stream);

#endif
