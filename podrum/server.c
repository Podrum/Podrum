/*
                   Podrum R3 Copyright MFDGaming & PodrumTeam
                 This file is licensed under the GPLv2 license.
              To use this file you must own a copy of the license.
                       If you do not you can get it from:
            http://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
 */

#include <stdio.h>
#include <stdlib.h>
#include "./misc/logger.h"
#include "command/commandmanager.h"
#include "./network/raknet/socket.h"
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
	misc_address_t address;
	address.version = 4;
	address.address = "0.0.0.0";
	address.port = 19132;
	int sock = create_socket(address);
	socket_data_t socket_data;
	init_commands();
	log_info("Podrum started up!");
	command_t cmd1;
	cmd1.name = "help";
	cmd1.executor = cmd1executor;
	register_command(cmd1);
	char **args = malloc(0);
	execute("help", 0, args);
	worker_t worker = create_worker(test);
	while (1) {
		socket_data = receive_data(sock);
		if (socket_data.stream.size > 0) {
			int i;
			for (i = 0; i < socket_data.stream.size; ++i) {
				printf("0x%X ", socket_data.stream.buffer[i] & 0xff);
			}
			free(socket_data.stream.buffer);
			printf("\n");
		}
	}
	return 0;
}