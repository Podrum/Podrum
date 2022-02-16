/*
                   Podrum R3 Copyright MFDGaming & PodrumTeam
                 This file is licensed under the GPLv2 license.
              To use this file you must own a copy of the license.
                       If you do not you can get it from:
            http://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
 */

#ifndef PODRUM_NETWORK_MINECRAFT_MCMAPPING_H
#define PODRUM_NETWORK_MINECRAFT_MCMAPPING_H

#include <stdint.h>
#include <stdlib.h>
#include "./mcmisc.h"

typedef struct {
	char *name;
	uint8_t metadata;
} mapping_block_state_t;

typedef struct {
	mapping_block_state_t *entries;
	size_t size;
} mapping_block_states_t;

mapping_block_state_t runtime_id_to_block_state(int64_t runtime_id, mapping_block_states_t states);

int64_t block_state_to_runtime_id(char *name, uint8_t metadata, mapping_block_states_t states);

char *runtime_id_to_item_state(int16_t runtime_id, misc_item_states_t states);

int16_t item_state_to_runtime_id(char *name, misc_item_states_t states);

#endif