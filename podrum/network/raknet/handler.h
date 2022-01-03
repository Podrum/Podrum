/*
                   Podrum R3 Copyright MFDGaming & PodrumTeam
                 This file is licensed under the GPLv2 license.
              To use this file you must own a copy of the license.
                       If you do not you can get it from:
            http://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
 */

#ifndef PODRUM_NETWORK_RAKNET_HANDLER_H
#define PODRUM_NETWORK_RAKNET_HANDLER_H

#include "./packet.h"
#include "./rakserver.h"

binary_stream_t handle_unconneted_ping(binary_stream_t *stream, raknet_server_t *server);

#endif
