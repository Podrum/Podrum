/*
                   Podrum R3 Copyright MFDGaming & PodrumTeam
                 This file is licensed under the GPLv2 license.
              To use this file you must own a copy of the license.
                       If you do not you can get it from:
            http://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
 */

#ifdef _WIN32

#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib,"ws2_32.lib")

#else

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <fcntl.h>

#endif

#include "./socket.h"
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

int set_nb_socket(int fd, int is_nb)
{
	if (fd < 0) {
		return 0;
	}
	#ifdef _WIN32

	unsigned long mode = is_nb;
	return (ioctlsocket(fd, FIONBIO, &mode) == 0);

	#else

	int flags = fcntl(fd, F_GETFL, 0);
	if (flags == -1) {
		return 0;
	}
	flags = is_nb == 0 ? (flags & ~O_NONBLOCK) : (flags | O_NONBLOCK);
	return (fcntl(fd, F_SETFL, flags) == 0);

	#endif
}

int create_socket(misc_address_t address)
{
	#ifdef _WIN32

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		return -1;
	}

	#endif

	int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	set_nb_socket(sock, 1);
	struct sockaddr_in s_address;
	memset(&s_address, 0, sizeof(s_address));
	s_address.sin_family = AF_INET;
	s_address.sin_addr.s_addr = inet_addr(address.address);
	s_address.sin_port = htons(address.port);
	if (bind(sock, (struct sockaddr *) &s_address, sizeof(s_address)) < 0) {
		return -1;
	}
	return sock;
}

socket_data_t receive_data(int sock)
{
	socket_data_t result;
	result.stream.offset = 0;
	result.stream.size = 65535;
	result.stream.buffer = malloc(result.stream.size);
	struct sockaddr_in s_address;
	memset(&s_address, 0, sizeof(s_address));
	socklen_t s_address_length = sizeof(s_address);
	int length = recvfrom(sock, result.stream.buffer, result.stream.size, 0, (struct sockaddr *) &s_address, &s_address_length);
	if (length < result.stream.size) {
		result.stream.size = length;
		result.stream.buffer = realloc(result.stream.buffer, result.stream.size);
	}
	result.address.version = 4;
	result.address.address = inet_ntoa(s_address.sin_addr);
	result.address.port = htons(s_address.sin_port);
	return result;
}

void send_data(int sock, socket_data_t socket_data)
{
	struct sockaddr_in s_address;
	memset(&s_address, 0, sizeof(s_address));
	s_address.sin_family = AF_INET;
	s_address.sin_addr.s_addr = inet_addr(socket_data.address.address);
	s_address.sin_port = htons(socket_data.address.port);
	sendto(sock, socket_data.stream.buffer, socket_data.stream.size, 0, (struct sockaddr *) &s_address, sizeof(s_address));
}