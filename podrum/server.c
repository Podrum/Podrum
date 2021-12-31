#include <stdio.h>
#include <stdlib.h>
#include "./misc/logger.h"
#include "command/commandmanager.h"
#include "./network/raknet/socket.h"

#ifdef _WIN32

#include <windows.h>

#endif

int main(int argc, char *argv[])
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