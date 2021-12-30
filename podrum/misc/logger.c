/*
                   Podrum R3 Copyright MFDGaming & PodrumTeam
                 This file is licensed under the GPLv2 license.
              To use this file you must own a copy of the license.
                       If you do not you can get it from:
            http://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
 */

#include "./textformat.h"
#include <stdio.h>
#include <time.h>

void log_generic(char *message, char *type_name, char *type_color)
{
	time_t current_time;
	struct tm *time_info;
	char time_string[9];
	time(&current_time);
	time_info = localtime(&current_time);
	strftime(time_string, sizeof(time_string), "%H:%M:%S", time_info);
	printf("%s[%s: %s]%s %s%s\n", type_color, type_name, time_string, TEXTFORMAT_RESET, message, TEXTFORMAT_RESET);
}

void log_info(char *message)
{
	log_generic(message, "INFO", TEXTFORMAT_BLUE);
}

void log_warning(char *message)
{
	log_generic(message, "WARNING", TEXTFORMAT_YELLOW);
}

void log_error(char *message)
{
	log_generic(message, "ERROR", TEXTFORMAT_RED);
}

void log_success(char *message)
{
	log_generic(message, "SUCCESS", TEXTFORMAT_GREEN);
}

void log_emergency(char *message)
{
	log_generic(message, "EMERGENCY", TEXTFORMAT_GOLD);
}

void log_notice(char *message)
{
	log_generic(message, "NOTICE", TEXTFORMAT_AQUA);
}

void log_critical(char *message)
{
	log_generic(message, "CRITICAL", TEXTFORMAT_DARK_RED);
}

void log_debug(char *message)
{
	log_generic(message, "DEBUG", TEXTFORMAT_GRAY);
}