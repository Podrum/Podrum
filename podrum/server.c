/*
                   Podrum R3 Copyright MFDGaming & PodrumTeam
                 This file is licensed under the GPLv2 license.
              To use this file you must own a copy of the license.
                       If you do not you can get it from:
            http://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "./misc/logger.h"
#include "command/commandmanager.h"
#include "./network/raknet/rakserver.h"
#include "./worker.h"

#ifdef _WIN32

#include <windows.h>

#endif

void cmd1executor(int argc, char **argv)
{
	log_info("Function called!");
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
	char *out = malloc(size + 1);
	sprintf(out, "%s:%d connected!", connection->address.address, connection->address.port);
	out[size] = 0x00;
	log_info(out);
	free(out);
}


void on_dn(misc_address_t address) {
	int size = snprintf(NULL, 0, "%s:%d disconnected.", address.address, address.port);
	char *out = malloc(size + 1);
	sprintf(out, "%s:%d disconnected.", address.address, address.port);
	out[size] = 0x00;
	log_info(out);
	free(out);
}

void on_f(misc_frame_t frame, connection_t *connection) {
	printf("0x%X\n", frame.stream.buffer[0]);
};

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
	raknet_server.address.address = "0.0.0.0";
	raknet_server.address.port = 19132;
	raknet_server.sock = create_socket(raknet_server.address);
	raknet_server.connections = malloc(0);
	raknet_server.connections_count = 0;
	raknet_server.guid = 13253860892328930865;
	raknet_server.message = "MCPE;Dedicated Server;440;1.17.0;0;10;13253860892328930865;Bedrock level;Survival;1;19132;19133;";
	raknet_server.epoch = time(NULL) * 1000;
	raknet_server.on_disconnect_notification_executor = on_dn;
	raknet_server.on_frame_executor = on_f;
	raknet_server.on_new_incoming_connection_executor = on_nic;
	command_manager_t command_manager;
	command_manager.commands = malloc(0);
	command_manager.commands_count = 0;
	log_info("Podrum started up!");
	command_t cmd1;
	cmd1.name = "help";
	cmd1.description = "help command";
	cmd1.flags = 0;
	cmd1.prefix = "/help";
	cmd1.usage = "<page>";
	cmd1.executor = cmd1executor;
	register_command(cmd1, &command_manager);
	char **args = malloc(0);
	execute("help", 0, args, &command_manager);
	//worker_t worker = create_worker(test);
	while (1) {
		handle_raknet_packet(&raknet_server);
	}
	return 0;
}
