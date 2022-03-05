/*
                   Podrum R3 Copyright MFDGaming & PodrumTeam
                 This file is licensed under the GPLv2 license.
              To use this file you must own a copy of the license.
                       If you do not you can get it from:
            http://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
 */

#include <podrum/network/minecraft/mchandler.h>
#include <podrum/network/minecraft/mcpackets.h>
#include <podrum/misc/logger.h>
#include <podrum/network/minecraft/mcmisc.h>
#include <podrum/network/minecraft/mcplayer.h>
#include <podrum/misc/json.h>
#include <podrum/misc/jwt.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <math.h>

void handle_packet_login(binary_stream_t *stream, connection_t *connection, raknet_server_t *server, minecraft_player_manager_t *player_manager)
{
	packet_login_t login = get_packet_login(stream);
	minecraft_player_t player;
	player.display_name = (char *) malloc(1);
	player.display_name[0] = 0;
	player.identity = (char *) malloc(1);
	player.identity[0] = 0;
	player.xuid = (char *) malloc(1);
	player.xuid[0] = 0;
	player.gamemode = 1;
	player.view_distance = 8;
	player.address = connection->address;
	player.x = 0.0;
	player.y = 9.0;
	player.z = 0.0;
	player.pitch = 0.0;
	player.yaw = 0.0;
	player.spawned = 0;
	srand(time(NULL));
	player.entity_id = rand();
	while (has_minecraft_player_entity_id(player.entity_id, player_manager)) {
		srand(time(NULL));
		player.entity_id = rand();
	}
	json_input_t json_input;
	json_input.json = login.tokens.identity;
	json_input.offset = 0;
	json_root_t json_root = parse_json_root(&json_input);
	json_root_t chain = get_json_object_value("chain", json_root.entry.json_object);
	size_t i;
	uint8_t got_identity = 0;
	uint8_t got_display_name = 0;
	uint8_t got_xuid = 0;
	for (i = 0; i < chain.entry.json_array.size; ++i) {
		json_object_t entry = jwt_decode(get_json_array_value(i, chain.entry.json_array).entry.json_string).entry.json_object;
		json_root_t value = get_json_object_value("extraData", entry);
		if (value.type == JSON_OBJECT) {
			if (got_display_name == 0) {
				json_root_t display_name = get_json_object_value("displayName", value.entry.json_object);
				if (display_name.type == JSON_STRING) {
					size_t size = strlen(display_name.entry.json_string) + 1;
					player.display_name = (char *) realloc(player.display_name, size);
					memcpy(player.display_name, display_name.entry.json_string, size);
					got_display_name = 1;
				}
			}
			if (got_identity == 0) {
				json_root_t identity = get_json_object_value("identity", value.entry.json_object);
				if (identity.type == JSON_STRING) {
					size_t size = strlen(identity.entry.json_string) + 1;
					player.identity = (char *) realloc(player.identity, size);
					memcpy(player.identity, identity.entry.json_string, size);
					got_identity = 1;
				}
			}
			if (got_xuid == 0) {
				json_root_t xuid = get_json_object_value("XUID", value.entry.json_object);
				if (xuid.type == JSON_STRING) {
					size_t size = strlen(xuid.entry.json_string) + 1;
					player.xuid = (char *) realloc(player.xuid, size);
					memcpy(player.xuid, xuid.entry.json_string, size);
					got_xuid = 1;
				}
			}
		}
		destroy_json_object(entry);
	}
	destroy_json_root(json_root);
	free(login.tokens.client);
	free(login.tokens.identity);
	add_minecraft_player(player, player_manager);
	send_play_status(PLAY_STATUS_LOGIN_SUCCESS, connection, server);
	binary_stream_t *streams = (binary_stream_t *) malloc(sizeof(binary_stream_t));
	streams[0].buffer = (int8_t *) malloc(0);
	streams[0].size = 0;
	streams[0].offset = 0;
	packet_resource_packs_info_t resource_packs_info;
	resource_packs_info.must_accept = 0;
	resource_packs_info.has_scripts = 0;
	resource_packs_info.force_server_packs = 0;
	resource_packs_info.behavior_packs.size = 0;
	resource_packs_info.texture_packs.size = 0;
	put_packet_resource_packs_info(resource_packs_info, (&(streams[0])));
	send_minecraft_packet(streams, 1, connection, server);
	free(streams[0].buffer);
	free(streams);
	printf("%s logged in with entity id %lld\n", player.display_name, player.entity_id);
	if (got_xuid) {
		printf("%s is your xuid\n", player.xuid);
	}
	printf("%s is your identity\n", player.identity);
}

void handle_packet_interact(binary_stream_t *stream, connection_t *connection, raknet_server_t *server, minecraft_player_manager_t *player_manager)
{
	packet_interact_t interact = get_packet_interact(stream);
	if (interact.action_id == INTERACT_OPEN_INVENTORY) {
		minecraft_player_t *player = get_minecraft_player_address(connection->address, player_manager);
		binary_stream_t *streams = (binary_stream_t *) malloc(sizeof(binary_stream_t));
		streams[0].buffer = (int8_t *) malloc(0);
		streams[0].size = 0;
		streams[0].offset = 0;
		packet_container_open_t container_open;
		switch (player->gamemode) {
		case GAMEMODE_SURVIVAL:
		case GAMEMODE_ADVENTURE:
		case GAMEMODE_SURVIVAL_SPECTATOR:
		case GAMEMODE_FALLBACK:
			container_open.window_id = WINDOW_ID_INVENTORY;
			break;
		case GAMEMODE_CREATIVE:
		case GAMEMODE_CREATIVE_SPECTATOR:
			container_open.window_id = WINDOW_ID_CREATIVE;
			break;
		default:
			log_warning("Invalid Gamemode");
			disconnect_raknet_client(connection, server);
		}
		container_open.window_type = WINDOW_TYPE_INVENTORY;
		container_open.coordinates_x = (int32_t) player->x;
		container_open.coordinates_y = (uint32_t) (((int32_t) player->y) & 0xffffffff);
		container_open.coordinates_z = (int32_t) player->z;
		container_open.runtime_entity_id = player->entity_id;
		put_packet_container_open(container_open, (&(streams[0])));
		send_minecraft_packet(streams, 1, connection, server);
		free(streams[0].buffer);
		free(streams);
	}
}

void handle_packet_window_close(binary_stream_t *stream, connection_t *connection, raknet_server_t *server)
{
	packet_container_close_t container_close_in = get_packet_container_close(stream);
	binary_stream_t *streams = (binary_stream_t *) malloc(sizeof(binary_stream_t));
	streams[0].buffer = (int8_t *) malloc(0);
	streams[0].size = 0;
	streams[0].offset = 0;
	packet_container_close_t container_close_out;
	container_close_out.window_id = container_close_in.window_id;
	container_close_out.server = 0;
	put_packet_container_close(container_close_out, (&(streams[0])));
	send_minecraft_packet(streams, 1, connection, server);
	free(streams[0].buffer);
	free(streams);
}

void handle_packet_request_chunk_radius(binary_stream_t *stream, connection_t *connection, raknet_server_t *server, minecraft_player_manager_t *player_manager, resources_t *resources)
{
	minecraft_player_t *player = get_minecraft_player_address(connection->address, player_manager);
	if (player->spawned == 1) return;
	binary_stream_t *streams = (binary_stream_t *) malloc(sizeof(binary_stream_t));
	streams[0].buffer = (int8_t *) malloc(0);
	streams[0].size = 0;
	streams[0].offset = 0;
	packet_request_chunk_radius_t request_chunk_radius = get_packet_request_chunk_radius(stream);
	packet_chunk_radius_updated_t chunk_radius_updated;
	chunk_radius_updated.chunk_radius = (int32_t) fmin((double) request_chunk_radius.chunk_radius, 4.0); /* server_chunk_radius = 2 */
	put_packet_chunk_radius_updated(chunk_radius_updated, (&(streams[0])));
	player->view_distance = chunk_radius_updated.chunk_radius;
	send_minecraft_packet(streams, 1, connection, server);
	free(streams[0].buffer);
	free(streams);
	send_chunks(resources->block_states, player, connection, server);
	send_play_status(PLAY_STATUS_PLAYER_SPAWN, connection, server);
	player->spawned = 1;
}

void handle_packet_move_player(binary_stream_t *stream, connection_t *connection, raknet_server_t *server, minecraft_player_manager_t *player_manager, resources_t *resources)
{
	minecraft_player_t *player = get_minecraft_player_address(connection->address, player_manager);
	packet_move_player_t move_player = get_packet_move_player(stream);
	if (player->spawned == 1) {
		if (floor(floor(player->x) / 16.0) != floor(floor(move_player.position_x) / 16.0) || floor(floor(player->z) / 16.0) != floor(floor(move_player.position_z) / 16)) {
			send_chunks(resources->block_states, player, connection, server);
		}
	}
	player->x = move_player.position_x;
	player->y = move_player.position_y;
	player->z = move_player.position_z;
	player->pitch = move_player.pitch;
	player->yaw = move_player.yaw;
}
