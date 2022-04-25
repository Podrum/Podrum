/*
                   Podrum R3 Copyright MFDGaming & PodrumTeam
                 This file is licensed under the GPLv2 license.
              To use this file you must own a copy of the license.
                       If you do not you can get it from:
            http://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
 */

#include <podrum/debug.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <podrum/misc/logger.h>
#include <podrum/command/commandmanager.h>
#include <podrum/network/raknet/rakserver.h>
#include <podrum/worker.h>
#include <podrum/network/minecraft/mcpackets.h>
#include <podrum/misc/json.h>
#include <podrum/misc/base64.h>
#include <podrum/misc/jwt.h>
#include <podrum/misc/resourcemanager.h>
#include <podrum/network/minecraft/mcplayermanager.h>
#include <podrum/network/minecraft/mchandler.h>
#include <podrum/network/minecraft/mcplayer.h>
#include <podrum/world/chunk/chunk.h>
#include <cnbt/nbt.h>

#ifdef _WIN32

#include <windows.h>

#else

#include <unistd.h>

#endif

#define CODE_NAME "Titanium"
#define API_VERSION "1.0.0-alpha1"

static int PODRUM_TPS = 1000 / 20;

minecraft_player_manager_t player_manager;

resources_t resources;

raknet_server_t raknet_server;

bool is_running = false;

void cmd1executor(int argc, char **argv)
{
	log_info("Function called!");
}

RETURN_WORKER_EXECUTOR command_task(ARGS_WORKER_EXECUTOR argvp)
{
	char *word = malloc(0);
	size_t size = 0;
	while (is_running) {
		char letter;
		scanf("%c", &letter);
		if (letter == '\r') continue;
		if (letter == '\n') {
			letter = 0;
		}
		++size;
		word = (char *) realloc(word, size);
		word[size - 1] = letter;
		if (letter == 0) {
			if (strcmp(word, "help") == 0) {
				log_info("help - Displays all available commands");
				log_info("stop - Stops the server");
			} else if (strcmp(word, "stop") == 0) {
				log_info("Stopping server...");
				is_running = false;
			} else if (size > 1) {
				log_error("Invalid command!");
			}
			size = 0;
			word = realloc(word, 0);
		}
	}
	return 0;
}

void on_nic(connection_t *connection) {
	int size = snprintf(NULL, 0, "%s:%d connected!", connection->address.address, connection->address.port);
	char *out = (char *) malloc(size + 1);
	sprintf(out, "%s:%d connected!", connection->address.address, connection->address.port);
	out[size] = 0x00;
	log_info(out);
	free(out);
}

void on_dn(misc_address_t address) {
	remove_minecraft_player(address, &player_manager);
	int size = snprintf(NULL, 0, "%s:%d disconnected.", address.address, address.port);
	char *out = (char *) malloc(size + 1);
	sprintf(out, "%s:%d disconnected.", address.address, address.port);
	out[size] = 0x00;
	log_info(out);
	free(out);
	/* exit(0); */
}

void on_f(misc_frame_t frame, connection_t *connection, raknet_server_t *server) {
	if ((frame.stream.buffer[0] & 0xFF) == ID_GAME) {
		packet_game_t game = get_packet_game(((&(frame.stream))));
		size_t i;
		for (i = 0; i < game.streams_count; ++i) {
			/* printf("MINECRAFT: 0x%X\n", game.streams[i].buffer[0] & 0xff); */
			if ((game.streams[i].buffer[0] & 0xFF) == ID_LOGIN) {
				handle_packet_login((&(game.streams[i])), connection, server, &player_manager);
			} else {
				minecraft_player_t *player = get_minecraft_player_address(connection->address, &player_manager);
				if (player == NULL) {
					break;
				}
				if ((game.streams[i].buffer[0] & 0xFF) == 0x9C) {
					int ii;
					for (ii = 0; ii < game.streams[i].size; ++ii) {
						printf("\\x%X", game.streams[i].buffer[ii] & 0xFF);
					}
					printf("\n");
				} else if ((game.streams[i].buffer[0] & 0xFF) == ID_RESOURCE_PACK_CLIENT_RESPONSE) {
					packet_resource_pack_client_response_t resource_pack_client_response = get_packet_resource_pack_client_response(((&(game.streams[i]))));
					if (resource_pack_client_response.response_status == RESOURCE_PACK_CLIENT_RESPONSE_NONE || resource_pack_client_response.response_status == RESOURCE_PACK_CLIENT_RESPONSE_HAVE_ALL_PACKS) {
						size_t streams_count = 1;
						binary_stream_t *streams = (binary_stream_t *) malloc(sizeof(binary_stream_t));
						streams[0].buffer = (int8_t *) malloc(0);
						streams[0].size = 0;
						streams[0].offset = 0;
						packet_resource_pack_stack_t resource_pack_stack;
						resource_pack_stack.must_accept = 0;
						resource_pack_stack.behavior_packs.size = 0;
						resource_pack_stack.resource_packs.size = 0;
						resource_pack_stack.game_version = GAME_VERSION;
						resource_pack_stack.experiments.size = 0;
						resource_pack_stack.experiments_previously_used = 0;
						put_packet_resource_pack_stack(resource_pack_stack, (&(streams[0])));
						send_minecraft_packet(streams, streams_count, connection, server);
						free(streams[0].buffer);
						free(streams);
					} else if (resource_pack_client_response.response_status == RESOURCE_PACK_CLIENT_RESPONSE_COMPLETED) {
						size_t streams_count = 4;
						binary_stream_t *streams = (binary_stream_t *) malloc(streams_count * sizeof(binary_stream_t));
						streams[0].buffer = (int8_t *) malloc(0);
						streams[0].size = 0;
						streams[0].offset = 0;
						streams[1].buffer = (int8_t *) malloc(0);
						streams[1].size = 0;
						streams[1].offset = 0;
						streams[2].buffer = (int8_t *) malloc(0);
						streams[2].size = 0;
						streams[2].offset = 0;
						streams[3].buffer = (int8_t *) malloc(0);
						streams[3].size = 0;
						streams[3].offset = 0;
						packet_start_game_t start_game;
						start_game.entity_id = 0;
						start_game.runtime_entity_id = 0;
						start_game.player_gamemode = 1;
						start_game.player_x = 0.0;
						start_game.player_y = 9.0;
						start_game.player_z = 0.0;
						start_game.pitch = 0.0;
						start_game.yaw = 0.0;
						start_game.seed = 0;
						start_game.biome_type = 0;
						start_game.biome_name = "";
						start_game.dimension = 0;
						start_game.generator = 1;
						start_game.world_gamemode = 1;
						start_game.difficulty = 0;
						start_game.spawn_x = 0;
						start_game.spawn_y = 9;
						start_game.spawn_z = 0;
						start_game.achievements_disabled = 0;
						start_game.day_cycle_stop_time = 0;
						start_game.edu_offer = 0;
						start_game.edu_features_enabled = 0;
						start_game.edu_product_uuid = "";
						start_game.rain_level = 0.0;
						start_game.lightning_level = 0.0;
						start_game.has_confirmed_platform_locked_content = 0;
						start_game.is_multiplayer = 1;
						start_game.broadcast_to_lan = 1;
						start_game.xbox_live_broadcast_mode = 4;
						start_game.platform_broadcast_mode = 4;
						start_game.enable_commands = 1;
						start_game.are_texture_packs_required = 0;
						start_game.gamerules.size = 0;
						start_game.experiments.size = 0;
						start_game.experiments_previously_used = 0;
						start_game.bonus_chest = 0;
						start_game.map_enabled = 0;
						start_game.permission_level = 1;
						start_game.server_chunk_tick_range = 0;
						start_game.has_locked_behavior_pack = 0;
						start_game.has_locked_texture_pack = 0;
						start_game.is_from_locked_world_template = 0;
						start_game.msa_gamertags_only = 0;
						start_game.is_from_world_template = 0;
						start_game.is_world_template_option_locked = 0;
						start_game.only_spawn_v1_villagers = 0;
						start_game.game_version = GAME_VERSION;
						start_game.limited_world_width = 0;
						start_game.limited_world_length = 0;
						start_game.is_new_nether = 1;
						start_game.edu_resource_uri.button_name = "";
						start_game.edu_resource_uri.link_uri = "";
						start_game.experimental_gameplay_override = 0;
						start_game.level_id = "";
						start_game.world_name = "";
						start_game.premium_world_template_id = "";
						start_game.is_trial = 0;
						start_game.movement_authority = 0;
						start_game.rewind_history_size = 0;
						start_game.server_authoritative_block_breaking = 0;
						start_game.current_tick = 0;
						start_game.enchantment_seed = 0;
						start_game.block_properties.size = 0;
						start_game.item_states = resources.item_states;
						start_game.multiplayer_correlation_id = "";
						start_game.server_authoritative_inventory = 0;
						start_game.engine = GAME_ENGINE;
						start_game.block_pallete_checksum = 0;
						put_packet_start_game(start_game, (&(streams[0])));
						packet_creative_content_t creative_content;
						creative_content.size = resources.creative_items.size;
						creative_content.entry_ids = (uint32_t *) malloc(creative_content.size * sizeof(uint32_t));
						size_t ii;
							for (ii = 0; ii < creative_content.size; ++ii) {
							creative_content.entry_ids[i] = i + 1;
						}
						creative_content.items = resources.creative_items.entries;
						put_packet_creative_content(creative_content, (&(streams[1])));
						free(creative_content.entry_ids);
						packet_biome_definition_list_t biome_definition_list;
						biome_definition_list.nbt = resources.biome_definitions;
						put_packet_biome_definition_list(biome_definition_list, (&(streams[2])));
						packet_available_entity_identifiers_t available_entity_identifiers;
						available_entity_identifiers.nbt = resources.entity_identifiers;
						put_packet_available_entity_identifiers(available_entity_identifiers, (&(streams[3])));
						send_minecraft_packet(streams, streams_count, connection, server);
						free(streams[0].buffer);
						free(streams[1].buffer);
						free(streams[2].buffer);
						free(streams[3].buffer);
						free(streams);
					}
					int16_t ii;
					for (ii = 0; ii < resource_pack_client_response.resource_pack_ids.size; ++ii) {
						free(resource_pack_client_response.resource_pack_ids.ids[i]);
					}
					free(resource_pack_client_response.resource_pack_ids.ids);
				} else if ((game.streams[i].buffer[0] & 0xFF) == ID_REQUEST_CHUNK_RADIUS) {
					handle_packet_request_chunk_radius((&(game.streams[i])), connection, server, player, &resources);
				} else if ((game.streams[i].buffer[0] & 0xFF) == ID_INTERACT) {
					handle_packet_interact((&(game.streams[i])), connection, server, player, &resources);
				} else if ((game.streams[i].buffer[0] & 0xFF) == 0x09) {
					/* Just for debugging
					minecraft_player_t *player = get_minecraft_player_address(connection->address, &player_manager);
					if (player == NULL) continue;
					if (player->spawned == 0) continue;
					send_raknet_shutdown(server);
					*/
				} else if ((game.streams[i].buffer[0] & 0xFF) == ID_CONTAINER_CLOSE) {
					handle_packet_window_close((&(game.streams[i])), connection, server);
				} else if ((game.streams[i].buffer[0] & 0xFF) == ID_MOVE_PLAYER) {
					handle_packet_move_player((&(game.streams[i])), connection, server, player, &resources);
				}
			}
			free(game.streams[i].buffer);
		}
		free(game.streams);
	}
}

int main(int argc, char **argv)
{
	is_running = true;
	#ifdef _WIN32

	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dw_mode = 0;
	GetConsoleMode(handle, &dw_mode);
	dw_mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(handle, dw_mode);

	#endif
	resources = get_resources();
	player_manager.size = 0;
	player_manager.players = (minecraft_player_t *) malloc(0);
	raknet_server = create_raknet_server(1, "0.0.0.0", 19132, 4, on_f, on_nic, on_dn);
	run_raknet_server(&raknet_server);
	send_set_raknet_option("name", "MCPE;Dedicated Server;486;1.18.11;0;10;13253860892328930865;Bedrock level;Survival;1;19132;19133;", &raknet_server);
	command_manager_t command_manager;
	command_manager.commands = (command_t *) malloc(0);
	command_manager.commands_count = 0;
	create_worker(command_task, NULL);
	log_info("Podrum started up!");
	while (is_running) {
		#ifdef _WIN32

		Sleep(PODRUM_TPS);

		#else

		usleep(PODRUM_TPS * 1000);

		#endif
	}
	send_raknet_shutdown(&raknet_server);
	join_worker(raknet_server.main_worker);
	destroy_resources(&resources);
	free(player_manager.players);
	free(command_manager.commands);
	log_info("Server stopped.");
	return 0;
}
