/*
                   Podrum R3 Copyright MFDGaming & PodrumTeam
                 This file is licensed under the GPLv2 license.
              To use this file you must own a copy of the license.
                       If you do not you can get it from:
            http://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
 */

#ifndef PODRUM_NETWORK_MINECRAFT_MCHANDLER_H
#define PODRUM_NETWORK_MINECRAFT_MCHANDLER_H

#include <podrum/debug.h>
#include <podrum/network/raknet/rakserver.h>
#include <podrum/network/minecraft/mcplayermanager.h>
#include <podrum/misc/resourcemanager.h>

void handle_packet_login(binary_stream_t *stream, connection_t *connection, raknet_server_t *server, minecraft_player_manager_t *player_manager);

void handle_packet_interact(binary_stream_t *stream, connection_t *connection, raknet_server_t *server, minecraft_player_t *player, resources_t *resources);

void handle_packet_window_close(binary_stream_t *stream, connection_t *connection, raknet_server_t *server);

void handle_packet_request_chunk_radius(binary_stream_t *stream, connection_t *connection, raknet_server_t *server, minecraft_player_t *player, resources_t *resources);

void handle_packet_move_player(binary_stream_t *stream, connection_t *connection, raknet_server_t *server, minecraft_player_t *player, resources_t *resources);

#endif