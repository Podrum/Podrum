/*
                   Podrum R3 Copyright MFDGaming & PodrumTeam
                 This file is licensed under the GPLv2 license.
              To use this file you must own a copy of the license.
                       If you do not you can get it from:
            http://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
 */

#include "./rakserver.h"
#include "./handler.h"
#include <string.h>
#include <stdlib.h>

void handle_packet(raknet_server_t *server)
{
	socket_data_t socket_data = receive_data(server->sock);
	if (socket_data.stream.size > 0) {
		/* Just a debug thing
			int i;
			for (i = 0; i < socket_data.stream.size; ++i) {
				printf("0x%X ", socket_data.stream.buffer[i] & 0xff);
			}
			printf("\n");
		*/
		if ((socket_data.stream.buffer[0] & 0xff) == ID_UNCONNECTED_PING)
		{
			socket_data_t output_socket_data;
			output_socket_data.stream = handle_unconneted_ping((&(socket_data.stream)), server);
			output_socket_data.address = socket_data.address;
			send_data(server->sock, output_socket_data);
			free(output_socket_data.stream.buffer);
			memset(&output_socket_data, 0, sizeof(socket_data_t));
		}
	}
	free(socket_data.stream.buffer);
	memset(&socket_data, 0, sizeof(socket_data_t));
}
