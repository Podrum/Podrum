/*
                   Podrum R3 Copyright MFDGaming & PodrumTeam
                 This file is licensed under the GPLv2 license.
              To use this file you must own a copy of the license.
                       If you do not you can get it from:
            http://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
 */

#ifndef PODRUM_MISC_RESOURCEMANAGER_H
#define PODRUM_MISC_RESOURCEMANAGER_H

#include <cbinarystream/binary_stream.h>
#include <cnbt/nbt.h>
#include "../network/minecraft/mcmisc.h"
#include "../network/minecraft/mcmapping.h"

typedef struct {
	nbt_compound_t biome_definitions;
	nbt_compound_t entity_identifiers;
	misc_item_states_t item_states;
	mapping_block_states_t block_states;
	mapping_creative_items_t creative_items;
} resources_t;

binary_stream_t read_file(char *path);

resources_t get_resources();

#endif
