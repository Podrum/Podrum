/*
                   Podrum R3 Copyright MFDGaming & PodrumTeam
                 This file is licensed under the GPLv2 license.
              To use this file you must own a copy of the license.
                       If you do not you can get it from:
            http://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
 */

#ifndef PODRUM_MISC_BASE64_H
#define PODRUM_MISC_BASE64_H

#include <stdlib.h>
#include <cbinarystream/binary_stream.h>

#define BASE64_TABLE "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"

binary_stream_t base64_decode(char *base64_string);

char *base64_encode(binary_stream_t stream);

#endif