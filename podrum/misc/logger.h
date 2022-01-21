/*
                   Podrum R3 Copyright MFDGaming & PodrumTeam
                 This file is licensed under the GPLv2 license.
              To use this file you must own a copy of the license.
                       If you do not you can get it from:
            http://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
 */

#ifndef PODRUM_MISC_LOGGER_H
#define PODRUM_MISC_LOGGER_H

void log_generic(char *message, char *type_name, char *type_color);

void log_info(char *message);

void log_warning(char *message);

void log_error(char *message);

void log_success(char *message);

void log_emergency(char *message);

void log_notice(char *message);

void log_critical(char *message);

void log_debug(char *message);

#endif