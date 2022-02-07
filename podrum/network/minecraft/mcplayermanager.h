/*
                   Podrum R3 Copyright MFDGaming & PodrumTeam
                 This file is licensed under the GPLv2 license.
              To use this file you must own a copy of the license.
                       If you do not you can get it from:
            http://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
 */

#ifndef PODRUM_NETWORK_MINECRAFT_MCPLAYERMANAGER_H
#define PODRUM_NETWORK_MINECRAFT_MCPLAYERMANAGER_H

#include <stdint.h>
#include <stdlib.h>
#include "../raknet/rakmisc.h"

typedef struct {
	misc_address_t address;
	char *display_name;
	char *identity;
	float x;
	float y;
	float z;
	float pitch;
	float yaw;
	int32_t gamemode;
	int32_t view_distance;
} minecraft_player_t;

typedef struct {
	minecraft_player_t *players;
	size_t size;
} minecraft_player_manager_t;

uint8_t has_minecraft_player_address(misc_address_t address, minecraft_player_manager_t *player_manager);

void add_minecraft_player(misc_address_t address, minecraft_player_manager_t *player_manager);

void remove_minecraft_player(misc_address_t address, minecraft_player_manager_t *player_manager);

minecraft_player_t *get_minecraft_player_address(misc_address_t address, minecraft_player_manager_t *player_manager);

uint8_t has_minecraft_player_display_name(char *display_name, minecraft_player_manager_t *player_manager);

minecraft_player_t *get_minecraft_player_display_name(char *display_name, minecraft_player_manager_t *player_manager);

#endif