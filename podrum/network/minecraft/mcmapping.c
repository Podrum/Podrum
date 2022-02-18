/*
                   Podrum R3 Copyright MFDGaming & PodrumTeam
                 This file is licensed under the GPLv2 license.
              To use this file you must own a copy of the license.
                       If you do not you can get it from:
            http://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
 */
 
#include "./mcmapping.h"
#include <string.h>

mapping_block_state_t runtime_id_to_block_state(int32_t runtime_id, mapping_block_states_t states)
{
	return states.entries[runtime_id];
}

int32_t block_state_to_runtime_id(char *name, uint8_t metadata, mapping_block_states_t states)
{
	int64_t runtime_id;
	for (runtime_id = 0; runtime_id < states.size; ++runtime_id) {
		if ((strcmp(states.entries[runtime_id].name, name) == 0) && states.entries[runtime_id].metadata == metadata) {
			return runtime_id;
		}
	}
	return -1;
}

char *runtime_id_to_item_state(int16_t runtime_id, misc_item_states_t states)
{
	uint32_t i;
	for (i = 0; i < states.size; ++i) {
		if (states.entries[i].runtime_id == runtime_id) {
			return states.entries[i].name;
		}
	}
	return NULL;
}

int16_t item_state_to_runtime_id(char *name, misc_item_states_t states)
{
	uint32_t i;
	for (i = 0; i < states.size; ++i) {
		if (strcmp(states.entries[i].name, name) == 0) {
			return states.entries[i].runtime_id;
		}
	}
	return -1;
}
