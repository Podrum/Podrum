/*
                   Podrum R3 Copyright MFDGaming & PodrumTeam
                 This file is licensed under the GPLv2 license.
              To use this file you must own a copy of the license.
                       If you do not you can get it from:
            http://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "./misc/logger.h"
#include "command/commandmanager.h"
#include "./network/raknet/rakserver.h"
#include "./worker.h"
#include "./network/minecraft/mcpackets.h"
#include "./misc/json.h"
#include "./misc/base64.h"
#include <cnbt/nbt.h>

#ifdef _WIN32

#include <windows.h>

#endif

#define CODE_NAME "Titanium"
#define API_VERSION "1.0.0-alpha1"

void cmd1executor(int argc, char **argv)
{
	log_info("Function called!");
}

RETURN_WORKER_EXECUTOR test(ARGS_WORKER_EXECUTOR argvp)
{
	printf("Worker Test\n");
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

void send_minecraft_packet(binary_stream_t *streams, size_t streams_count, connection_t *connection, raknet_server_t *server)
{
	packet_game_t game;
	game.streams = streams;
	game.streams_count = streams_count;
	misc_frame_t frame;
	frame.is_fragmented = 0;
	frame.reliability = RELIABILITY_RELIABLE_ORDERED;
	frame.order_channel = 0;
	frame.stream.buffer = (int8_t *) malloc(0);
	frame.stream.offset = 0;
	frame.stream.size = 0;
	put_packet_game(game, (&(frame.stream)));
	add_to_raknet_queue(frame, connection, server);
}

void on_dn(misc_address_t address) {
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
			printf("MINECRAFT: 0x%X\n", game.streams[i].buffer[0] & 0xff);
			if ((game.streams[i].buffer[0] & 0xFF) == ID_LOGIN) {
				packet_login_t login = get_packet_login(((&(game.streams[i]))));
				printf("Tryed to connect with protocol %d\n", login.protocol_version);
				free(login.tokens.client);
				free(login.tokens.identity);
				size_t streams_count = 2;
				binary_stream_t *streams = (binary_stream_t *) malloc(streams_count * sizeof(binary_stream_t));
				streams[0].buffer = (int8_t *) malloc(0);
				streams[0].size = 0;
				streams[0].offset = 0;
				streams[1].buffer = (int8_t *) malloc(0);
				streams[1].size = 0;
				streams[1].offset = 0;
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
				free(streams[0].buffer);
				free(streams[1].buffer);
				free(streams);
			} else if ((game.streams[i].buffer[0] & 0xFF) == 0x9C) {
				int ii;
				for (ii = 0; ii < game.streams[i].size; ++ii) {
					printf("\\x%X", game.streams[i].buffer[ii] & 0xFF);
				}
				printf("\n");
			} else if ((game.streams[i].buffer[0] & 0xFF) == ID_RESOURCE_PACK_CLIENT_RESPONSE) {
				packet_resource_pack_client_response_t resource_pack_client_response = get_packet_resource_pack_client_response(((&(game.streams[i]))));
				printf("RESOURCE PACKS RESONSE: %d\n", resource_pack_client_response.response_status);
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
					start_game.item_states.size = 0;
					start_game.item_states.entries = (misc_item_state_t *) malloc(0);
					FILE *item_states_file = fopen("resource/item_states.json", "rb");
					fseek(item_states_file, 0, SEEK_END);
					binary_stream_t item_states_stream;
					item_states_stream.size = ftell(item_states_file);
					item_states_stream.offset = 0;
					item_states_stream.buffer = (int8_t *) malloc(item_states_stream.size);
					fseek(item_states_file, 0, SEEK_SET);
					fread(item_states_stream.buffer, 1, item_states_stream.size, item_states_file);
					fclose(item_states_file);
					put_unsigned_byte(0, &item_states_stream);
					json_input_t item_states_input;
					item_states_input.json = (char *) item_states_stream.buffer;
					item_states_input.offset = 0;
					json_array_t item_states_array = parse_json_array(&item_states_input);
					free(item_states_stream.buffer);
					size_t ii;
					for (ii = 0; ii < item_states_array.size; ++ii) {
						misc_item_state_t item_state;
						item_state.component_based = 0;
						item_state.runtime_id = 0;
						size_t iii;
						for (iii = 0; iii < item_states_array.members[ii].json_object.size; ++iii) {
							if (strcmp(item_states_array.members[ii].json_object.keys[iii], "name") == 0) {
								item_state.name = item_states_array.members[ii].json_object.members[iii].json_string;
							} else if (strcmp(item_states_array.members[ii].json_object.keys[iii], "runtime_id") == 0) {
								item_state.runtime_id = item_states_array.members[ii].json_object.members[iii].json_number.number.int_number;
							} else if (strcmp(item_states_array.members[ii].json_object.keys[iii], "component_based") == 0) {
								item_state.component_based = item_states_array.members[ii].json_object.members[iii].json_bool;
							}
						}
						++start_game.item_states.size;
						start_game.item_states.entries = (misc_item_state_t *) realloc(start_game.item_states.entries, start_game.item_states.size * sizeof(misc_item_state_t));
						start_game.item_states.entries[start_game.item_states.size - 1] = item_state;
					}
					start_game.multiplayer_correlation_id = "";
					start_game.server_authoritative_inventory = 0;
					start_game.engine = GAME_ENGINE;
					start_game.block_pallete_checksum = 0;
					put_packet_start_game(start_game, (&(streams[0])));
					destroy_json_array(item_states_array);
					free(start_game.item_states.entries);
					packet_creative_content_t creative_content;
					creative_content.size = 0;
					put_packet_creative_content(creative_content, (&(streams[1])));
					packet_biome_definition_list_t biome_definition_list;
					FILE *file = fopen("resource/biome_definitions.nbt", "rb");
					fseek(file, 0, SEEK_END);
					binary_stream_t biome_definition_list_stream;
					biome_definition_list_stream.size = ftell(file);
					biome_definition_list_stream.offset = 0;
					biome_definition_list_stream.buffer = (int8_t *) malloc(biome_definition_list_stream.size);
					fseek(file, 0, SEEK_SET);
					fread(biome_definition_list_stream.buffer, 1, biome_definition_list_stream.size, file);
					fclose(file);
					biome_definition_list.nbt = get_misc_nbt_tag(&biome_definition_list_stream);
					put_packet_biome_definition_list(biome_definition_list, (&(streams[2])));
					free(biome_definition_list_stream.buffer);
					destroy_nbt_compound(biome_definition_list.nbt);
					packet_available_entity_identifiers_t available_entity_identifiers;
					file = fopen("resource/entity_identifiers.nbt", "rb");
					fseek(file, 0, SEEK_END);
					binary_stream_t available_entity_identifiers_stream;
					available_entity_identifiers_stream.size = ftell(file);
					available_entity_identifiers_stream.offset = 0;
					available_entity_identifiers_stream.buffer = (int8_t *) malloc(available_entity_identifiers_stream.size);
					fseek(file, 0, SEEK_SET);
					fread(available_entity_identifiers_stream.buffer, 1, available_entity_identifiers_stream.size, file);
					fclose(file);
					available_entity_identifiers.nbt = get_misc_nbt_tag(&available_entity_identifiers_stream);
					put_packet_available_entity_identifiers(available_entity_identifiers, (&(streams[3])));
					free(available_entity_identifiers_stream.buffer);
					destroy_nbt_compound(available_entity_identifiers.nbt);
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
			} else if ((game.streams[i].buffer[0] & 0xFF) == 0x45) {
				binary_stream_t *streams = (binary_stream_t *) malloc(sizeof(binary_stream_t));
				streams[0].buffer = (int8_t *) malloc(0);
				streams[0].size = 0;
				streams[0].offset = 0;
				packet_play_status_t play_status;
				play_status.status = PLAY_STATUS_PLAYER_SPAWN;
				put_packet_play_status(play_status, (&(streams[0])));
				send_minecraft_packet(streams, 1, connection, server);
				free(streams[0].buffer);
				free(streams);
			}
			free(game.streams[i].buffer);
		}
		free(game.streams);
	}
}

int main(int argc, char **argv)
{
	/* Some Tests
	binary_stream_t stream;
	stream.buffer = "Man";
	stream.size = 3;
	char *b64 = base64_encode(stream);
	printf("%s\n", b64);
	binary_stream_t stream2 = base64_decode("TWFu");
	int i;
	for (i = 0; i < stream2.size; ++i) {
		printf("%c", stream2.buffer[i]);
	}
	printf("\n");
	printf("%ld\n", stream2.size);
	json_input_t my_json_object;
	my_json_object.json = "{	\":)\": 1234, \":o\": 12.5, \":]\": \"a string\", \":(\": null, \":[\": false, \";)\": true, \"test\": {\"hi\": \"worked\", \"ha\": [12, 13, 14, [		{\"ayy\": 8}, 3]]}}";
	my_json_object.offset = 0;
	json_object_t json_object = parse_json_object(&my_json_object);
	log_info(json_object.keys[0]);
	printf("%ld\n", json_object.members[0].json_number.number.int_number);
	log_info(json_object.keys[1]);
	printf("%lf\n", json_object.members[1].json_number.number.float_number);
	log_info(json_object.keys[2]);
	printf("%s\n", json_object.members[2].json_string);
	log_info(json_object.keys[3]);
	printf("%p\n", json_object.members[3].json_null);
	log_info(json_object.keys[4]);
	printf("%d\n", json_object.members[4].json_bool);
	log_info(json_object.keys[5]);
	printf("%d\n", json_object.members[5].json_bool);
	log_info(json_object.keys[6]);
	json_object_t json_object2 = json_object.members[6].json_object;
	log_info(json_object2.keys[0]);
	log_info(json_object2.members[0].json_string);
	log_info(json_object2.keys[1]);
	json_array_t json_array = json_object2.members[1].json_array;
	printf("%ld\n", json_array.members[0].json_number.number.int_number);
	printf("%ld\n", json_array.members[1].json_number.number.int_number);
	printf("%ld\n", json_array.members[2].json_number.number.int_number);
	json_array_t json_array2 = json_array.members[3].json_array;
	json_object_t json_object3 = json_array2.members[0].json_object;
	log_info(json_object3.keys[0]);
	printf("%ld\n", json_object3.members[0].json_number.number.int_number);
	printf("%ld\n", json_array2.members[1].json_number.number.int_number);

	destroy_json_object(json_object);
	*/

	#ifdef _WIN32

	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dw_mode = 0;
	GetConsoleMode(handle, &dw_mode);
	dw_mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(handle, dw_mode);

	#endif
	raknet_server_t raknet_server;
	raknet_server.address.version = 4;
	raknet_server.address.address = "0.0.0.0";
	raknet_server.address.port = 19132;
	raknet_server.sock = create_socket(raknet_server.address);
	raknet_server.connections = (connection_t *) malloc(0);
	raknet_server.connections_count = 0;
	raknet_server.guid = 1325386089232893086;
	raknet_server.message = "MCPE;Dedicated Server;440;1.17.0;0;10;13253860892328930865;Bedrock level;Survival;1;19132;19133;";
	raknet_server.epoch = time(NULL) * 1000;
	raknet_server.on_disconnect_notification_executor = on_dn;
	raknet_server.on_frame_executor = on_f;
	raknet_server.on_new_incoming_connection_executor = on_nic;
	command_manager_t command_manager;
	command_manager.commands = (command_t *) malloc(0);
	command_manager.commands_count = 0;
	log_info("Podrum started up!");
	/* Some other Tests
	command_t cmd1;
	cmd1.name = "help";
	cmd1.description = "help command";
	cmd1.flags = 0;
	cmd1.prefix = "/help";
	cmd1.usage = "<page>";
	cmd1.executor = cmd1executor;
	register_command(cmd1, &command_manager);
	char **args = (char **) malloc(0);
	execute("help", 0, args, &command_manager);
	worker_t worker = create_worker(test);
	*/
	while (1) {
		handle_raknet_packet(&raknet_server);
	}
	return 0;
}
