/*
                   Podrum R3 Copyright MFDGaming & PodrumTeam
                 This file is licensed under the GPLv2 license.
              To use this file you must own a copy of the license.
                       If you do not you can get it from:
            http://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
 */

#ifndef PODRUM_NETWORK_RAKNET_SOCKET_H
#define PODRUM_NETWORK_RAKNET_SOCKET_H

#include <podrum/network/raknet/rakmisc.h>
#include <cbinarystream/binary_stream.h>

typedef struct {
	binary_stream_t stream;
	misc_address_t address;
} socket_data_t;

int set_nb_socket(int fd, int is_nb);

int create_socket(misc_address_t address);

socket_data_t receive_data(int sock);

void send_data(int sock, socket_data_t socket_data);

#endif