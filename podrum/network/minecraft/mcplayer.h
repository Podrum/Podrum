/*
                   Podrum R3 Copyright MFDGaming & PodrumTeam
                 This file is licensed under the GPLv2 license.
              To use this file you must own a copy of the license.
                       If you do not you can get it from:
            http://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
 */

#ifndef PODRUM_NETWORK_MINECRAFT_MCPLAYER_H
#define PODRUM_NETWORK_MINECRAFT_MCPLAYER_H

#include <podrum/network/raknet/rakserver.h>
#include <podrum/network/minecraft/mcplayermanager.h>
#include <podrum/network/minecraft/mcmapping.h>
#include <stdlib.h>

void send_minecraft_packet(binary_stream_t *streams, size_t streams_count, connection_t *connection, raknet_server_t *server);

void send_network_chunk_publisher_update(minecraft_player_t *player, connection_t *connection, raknet_server_t *server);

void send_chunk(chunk_t *chunk, minecraft_player_t *player, connection_t *connection, raknet_server_t *server);

void send_play_status(int32_t status, connection_t *connection, raknet_server_t *server);

void send_chunks(mapping_block_states_t block_states, minecraft_player_t *player, connection_t *connection, raknet_server_t *server);

#endif