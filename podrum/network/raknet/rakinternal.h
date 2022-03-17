/*
                   Podrum R3 Copyright MFDGaming & PodrumTeam
                 This file is licensed under the GPLv2 license.
              To use this file you must own a copy of the license.
                       If you do not you can get it from:
            http://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
 */

#ifndef PODRUM_NETWORK_RAKNET_RAKINTERNAL_H
#define PODRUM_NETWORK_RAKNET_RAKINTERNAL_H

#include <podrum/debug.h>
#include <podrum/network/raknet/rakmisc.h>

#define INTERNAL_FRAME 0x00
#define INTERNAL_NEW_INCOMING_CONNECTION 0x01
#define INTERNAL_DISCONNECT_NOTIFICATION 0x02
#define INTERNAL_SET_OPTION 0x03
#define INTERNAL_SHUTDOWN 0x04

typedef struct {
	misc_address_t address;
	misc_frame_t frame;
} internal_frame_t;

typedef struct {
	char *name;
	char *option;
} internal_set_option_t;

internal_frame_t get_internal_frame(binary_stream_t *stream);

misc_address_t get_internal_new_incoming_connection(binary_stream_t *stream);

misc_address_t get_internal_disconnect_notification(binary_stream_t *stream);

internal_set_option_t get_internal_set_option(binary_stream_t *stream);

void put_internal_frame(internal_frame_t internal_frame, binary_stream_t *stream);

void put_internal_new_incoming_connection(misc_address_t address, binary_stream_t *stream);

void put_internal_disconnect_notification(misc_address_t address, binary_stream_t *stream);

void put_internal_set_option(internal_set_option_t internal_set_option, binary_stream_t *stream);

#endif
