/*
                   Podrum R3 Copyright MFDGaming & PodrumTeam
                 This file is licensed under the GPLv2 license.
              To use this file you must own a copy of the license.
                       If you do not you can get it from:
            http://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
 */

#ifndef PODRUM_DEBUG_H
#define PODRUM_DEBUG_H

#ifdef _WIN32

#define USE_LEAK_DETECTOR 0

#if USE_LEAK_DETECTOR 1

#include <vld.h>

#endif

#endif

#endif