#include <stdio.h>
#include <stdlib.h>
#include "./misc/logger.h"
#include "./network/raknet/socket.h"

int main(int argc, char *argv[])
{
	misc_address_t address;
	address.version = 4;
	address.address = "0.0.0.0";
	address.port = 19132;
	int sock = create_socket(address);
	socket_data_t socket_data;
	log_info("Podrum started up!");
	while (1) {
		socket_data = receive_data(sock);
		if (socket_data.stream.size > 0) {
			int i;
			for (i = 0; i < socket_data.stream.size; ++i) {
				printf("0x%X ", socket_data.stream.buffer[i]);
			}
			free(socket_data.stream.buffer);
			printf("\n");
		}
	}
	return 0;
}