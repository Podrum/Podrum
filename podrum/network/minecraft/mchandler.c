/*
                   Podrum R3 Copyright MFDGaming & PodrumTeam
                 This file is licensed under the GPLv2 license.
              To use this file you must own a copy of the license.
                       If you do not you can get it from:
            http://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
 */

#include "./mchandler.h"
#include "./mcpackets.h"
#include "./mcmisc.h"
#include "./mcplayer.h"
#include "../../misc/json.h"
#include "../../misc/jwt.h"
#include <string.h>
#include <time.h>
#include <stdio.h>

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
	}
	destroy_json_root(json_root);
	free(login.tokens.client);
	free(login.tokens.identity);
	add_minecraft_player(player, player_manager);
	size_t streams_count = 2;
	binary_stream_t *streams = (binary_stream_t *) malloc(streams_count * sizeof(binary_stream_t));
	for (i = 0; i < streams_count; ++i) {
		streams[i].buffer = (int8_t *) malloc(0);
		streams[i].size = 0;
		streams[i].offset = 0;
	}
	packet_play_status_t play_status;
	play_status.status = PLAY_STATUS_LOGIN_SUCCESS;
	put_packet_play_status(play_status, (&(streams[0])));
	packet_resource_packs_info_t resource_packs_info;
	resource_packs_info.must_accept = 0;
	resource_packs_info.has_scripts = 0;
	resource_packs_info.force_server_packs = 0;
	resource_packs_info.behavior_packs.size = 0;
	resource_packs_info.texture_packs.size = 0;
	put_packet_resource_packs_info(resource_packs_info, (&(streams[1])));
	send_minecraft_packet(streams, streams_count, connection, server);
	for (i = 0; i < streams_count; ++i) {
		free(streams[i].buffer);
	}
	free(streams);
	printf("%s logged in with entity id %ld\n", player.display_name, player.entity_id);
	if (got_xuid) {
		printf("%s is your xuid\n", player.xuid);
	}
	printf("%s is your identity\n", player.identity);
}