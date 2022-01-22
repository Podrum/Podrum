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

#ifdef _WIN32

#include <windows.h>

#endif

#define CODE_NAME (char *) "Titanium"
#define API_VERSION (char *) "1.0.0-alpha1"

void cmd1executor(int argc, char **argv)
{
	log_info((char *) "Function called!");
}

RETURN_WORKER_EXECUTOR test(ARGS_WORKER_EXECUTOR argvp)
{
	while (1) {
		printf("SPAM!\n");
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
	int size = snprintf(NULL, 0, "%s:%d disconnected.", address.address, address.port);
	char *out = (char *) malloc(size + 1);
	sprintf(out, "%s:%d disconnected.", address.address, address.port);
	out[size] = 0x00;
	log_info(out);
	free(out);
	exit(0);
}

void on_f(misc_frame_t frame, connection_t *connection, raknet_server_t *server) {
	printf("0x%X\n", frame.stream.buffer[0] & 0xFF);
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
				packet_game_t out_game;
				out_game.streams_count = 2;
				out_game.streams = (binary_stream_t *) malloc(out_game.streams_count * sizeof(binary_stream_t));
				out_game.streams[0].buffer = (char *) malloc(0);
				out_game.streams[0].size = 0;
				out_game.streams[0].offset = 0;
				out_game.streams[1].buffer = (char *) malloc(0);
				out_game.streams[1].size = 0;
				out_game.streams[1].offset = 0;
				packet_play_status_t play_status;
				play_status.status = PLAY_STATUS_LOGIN_SUCCESS;
				put_packet_play_status(play_status, ((&(out_game.streams[0]))));
				packet_resource_packs_info_t resource_packs_info;
				resource_packs_info.must_accept = 0;
				resource_packs_info.has_scripts = 0;
				resource_packs_info.force_server_packs = 0;
				resource_packs_info.behavior_packs.size = 0;
				resource_packs_info.texture_packs.size = 0;
				put_packet_resource_packs_info(resource_packs_info, ((&(out_game.streams[1]))));
				misc_frame_t out_frame;
				out_frame.is_fragmented = 0;
				out_frame.reliability = RELIABILITY_UNRELIABLE;
				out_frame.stream.buffer = (char *) malloc(0);
				out_frame.stream.offset = 0;
				out_frame.stream.size = 0;
				put_packet_game(out_game, ((&(out_frame.stream))));
				free(out_game.streams[0].buffer);
				free(out_game.streams[1].buffer);
				free(out_game.streams);
				add_to_raknet_queue(out_frame, connection, server);
			} else if ((game.streams[i].buffer[0] & 0xFF) == ID_RESOURCE_PACK_CLIENT_RESPONSE) {
				packet_resource_pack_client_response_t resource_pack_client_response = get_packet_resource_pack_client_response(((&(game.streams[i]))));
				printf("RESOURCE PACKS RESONSE: %d\n", resource_pack_client_response.response_status);
				if (resource_pack_client_response.response_status == RESOURCE_PACK_CLIENT_RESPONSE_NONE || resource_pack_client_response.response_status == RESOURCE_PACK_CLIENT_RESPONSE_HAVE_ALL_PACKS) {
					packet_game_t out_game;
					out_game.streams_count = 1;
					out_game.streams = (binary_stream_t *) malloc(sizeof(binary_stream_t));
					out_game.streams[0].buffer = (char *) malloc(0);
					out_game.streams[0].size = 0;
					out_game.streams[0].offset = 0;
					packet_resource_pack_stack_t resource_pack_stack;
					resource_pack_stack.must_accept = 0;
					resource_pack_stack.behavior_packs.size = 0;
					resource_pack_stack.resource_packs.size = 0;
					resource_pack_stack.game_version = GAME_VERSION;
					resource_pack_stack.experiments.size = 0;
					resource_pack_stack.experiments_previously_used = 0;
					put_packet_resource_pack_stack(resource_pack_stack, ((&(out_game.streams[0]))));
					misc_frame_t out_frame;
					out_frame.is_fragmented = 0;
					out_frame.reliability = RELIABILITY_UNRELIABLE;
					out_frame.stream.buffer = (char *) malloc(0);
					out_frame.stream.offset = 0;
					out_frame.stream.size = 0;
					put_packet_game(out_game, ((&(out_frame.stream))));
					free(out_game.streams[0].buffer);
					free(out_game.streams);
					add_to_raknet_queue(out_frame, connection, server);
				}
				int16_t ii;
				for (ii = 0; ii < resource_pack_client_response.resource_pack_ids.size; ++ii) {
					free(resource_pack_client_response.resource_pack_ids.ids[i]);
				}
				free(resource_pack_client_response.resource_pack_ids.ids);
			}
			free(game.streams[i].buffer);
		}
		free(game.streams);
	}
}

int main(int argc, char **argv)
{
	#ifdef _WIN32

	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dw_mode = 0;
	GetConsoleMode(handle, &dw_mode);
	dw_mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(handle, dw_mode);

	#endif
	raknet_server_t raknet_server;
	raknet_server.address.version = 4;
	raknet_server.address.address = (char *) "0.0.0.0";
	raknet_server.address.port = 19132;
	raknet_server.sock = create_socket(raknet_server.address);
	raknet_server.connections = (connection_t *) malloc(0);
	raknet_server.connections_count = 0;
	raknet_server.guid = 1325386089232893086;
	raknet_server.message = (char *) "MCPE;Dedicated Server;440;1.17.0;0;10;13253860892328930865;Bedrock level;Survival;1;19132;19133;";
	raknet_server.epoch = time(NULL) * 1000;
	raknet_server.on_disconnect_notification_executor = on_dn;
	raknet_server.on_frame_executor = on_f;
	raknet_server.on_new_incoming_connection_executor = on_nic;
	command_manager_t command_manager;
	command_manager.commands = (command_t *) malloc(0);
	command_manager.commands_count = 0;
	log_info((char *) "Podrum started up!");
	command_t cmd1;
	cmd1.name = (char *) "help";
	cmd1.description = (char *) "help command";
	cmd1.flags = 0;
	cmd1.prefix = (char *) "/help";
	cmd1.usage = (char *) "<page>";
	cmd1.executor = cmd1executor;
	register_command(cmd1, &command_manager);
	char **args = (char **) malloc(0);
	execute((char *) "help", 0, args, &command_manager);
	/* worker_t worker = create_worker(test); */
	while (1) {
		handle_raknet_packet(&raknet_server);
	}
	return 0;
}
