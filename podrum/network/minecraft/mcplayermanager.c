/*
                   Podrum R3 Copyright MFDGaming & PodrumTeam
                 This file is licensed under the GPLv2 license.
              To use this file you must own a copy of the license.
                       If you do not you can get it from:
            http://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
 */

#include <podrum/network/minecraft/mcplayermanager.h>

#include <string.h>

uint8_t has_minecraft_player_address(misc_address_t address, minecraft_player_manager_t *player_manager)
{
	size_t i;
	for (i = 0; i < player_manager->size; ++i) {
		if ((player_manager->players[i].address.port == address.port) && (strcmp(player_manager->players[i].address.address, address.address) == 0)) {
			return 1;
		}
	}
	return 0;
}

void add_minecraft_player(minecraft_player_t player, minecraft_player_manager_t *player_manager)
{
	if (has_minecraft_player_address(player.address, player_manager) == 0) {
		++player_manager->size;
		player_manager->players = (minecraft_player_t *) realloc(player_manager->players, player_manager->size * sizeof(minecraft_player_t));
		player_manager->players[player_manager->size - 1] = player;
	}
}

void remove_minecraft_player(misc_address_t address, minecraft_player_manager_t *player_manager)
{
	if (has_minecraft_player_address(address, player_manager) == 1) {
		size_t i;
		minecraft_player_t *players = (minecraft_player_t *) malloc((player_manager->size - 1) * sizeof(minecraft_player_t));
		size_t size = 0;
		for (i = 0; i < player_manager->size; ++i) {
			if ((player_manager->players[i].address.port != address.port) || (strcmp(player_manager->players[i].address.address, address.address) != 0)) {
				players[size] = player_manager->players[i];
				++size;
			} else {
				free(player_manager->players[i].display_name);
				free(player_manager->players[i].identity);
				free(player_manager->players[i].xuid);
				free(player_manager->players[i].address.address);
			}
		}
		free(player_manager->players);
		player_manager->players = players;
		--player_manager->size;
	}
}

minecraft_player_t *get_minecraft_player_address(misc_address_t address, minecraft_player_manager_t *player_manager)
{
	size_t i;
	for (i = 0; i < player_manager->size; ++i) {
		if ((player_manager->players[i].address.port == address.port) && (strcmp(player_manager->players[i].address.address, address.address) == 0)) {
			return (&(player_manager->players[i]));
		}
	}
	return NULL;
}

uint8_t has_minecraft_player_display_name(char *display_name, minecraft_player_manager_t *player_manager)
{
	size_t i;
	for (i = 0; i < player_manager->size; ++i) {
		if (strcmp(player_manager->players[i].display_name, display_name) == 0) {
			return 1;
		}
	}
	return 0;
}

minecraft_player_t *get_minecraft_player_display_name(char *display_name, minecraft_player_manager_t *player_manager)
{
	size_t i;
	for (i = 0; i < player_manager->size; ++i) {
		if (strcmp(player_manager->players[i].display_name, display_name) == 0) {
			return (&(player_manager->players[i]));
		}
	}
	return NULL;
}

uint8_t has_minecraft_player_entity_id(int64_t entity_id, minecraft_player_manager_t *player_manager)
{
	size_t i;
	for (i = 0; i < player_manager->size; ++i) {
		if (player_manager->players[i].entity_id == entity_id) {
			return 1;
		}
	}
	return 0;
}