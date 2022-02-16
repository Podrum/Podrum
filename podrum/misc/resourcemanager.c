/*
                   Podrum R3 Copyright MFDGaming & PodrumTeam
                 This file is licensed under the GPLv2 license.
              To use this file you must own a copy of the license.
                       If you do not you can get it from:
            http://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
 */

#include "./resourcemanager.h"
#include "./json.h"
#include "./logger.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

binary_stream_t read_file(char *path)
{
	binary_stream_t stream;
	FILE *file = fopen(path, "rb");
	fseek(file, 0, SEEK_END);
	stream.offset = 0;
	stream.size = ftell(file);
	stream.buffer = (int8_t *) malloc(stream.size);
	fseek(file, 0, SEEK_SET);
	fread(stream.buffer, 1, stream.size, file);
	fclose(file);
	return stream;
}

resources_t get_resources()
{
	log_info("Loading resources...");
	resources_t resources;
	binary_stream_t biome_definitions_stream = read_file("./resource/biome_definitions.nbt");
	resources.biome_definitions = get_misc_nbt_tag(&biome_definitions_stream);
	free(biome_definitions_stream.buffer);
	log_success("Loaded Biome Definitions");
	binary_stream_t entity_identifiers_stream = read_file("./resource/entity_identifiers.nbt");
	resources.entity_identifiers = get_misc_nbt_tag(&entity_identifiers_stream);
	free(entity_identifiers_stream.buffer);
	log_success("Loaded Entity Identifiers");
	binary_stream_t item_states_stream = read_file("./resource/required_item_list.json");
	put_unsigned_byte(0, &item_states_stream);
	json_input_t json_input;
	json_input.json = (char *) item_states_stream.buffer;
	json_input.offset = 0;
	json_root_t json_root = parse_json_root(&json_input);
	free(item_states_stream.buffer);
	resources.item_states.size = json_root.entry.json_object.size;
	resources.item_states.entries = (misc_item_state_t *) malloc(resources.item_states.size * sizeof(misc_item_state_t));
	size_t i;
	for (i = 0; i < resources.item_states.size; ++i) {
		json_object_t json_object = json_root.entry.json_object.members[i].json_object;
		misc_item_state_t item_state;
		size_t size = strlen(json_root.entry.json_object.keys[i]) + 1;
		item_state.name = (char *) malloc(size);
		memcpy(item_state.name, json_root.entry.json_object.keys[i], size);
		item_state.runtime_id = get_json_object_value("runtime_id", json_object).entry.json_number.number.int_number;
		item_state.component_based = get_json_object_value("component_based", json_object).entry.json_bool;
		resources.item_states.entries[i] = item_state;
	}
	destroy_json_root(json_root);
	log_success("Loaded Item States");
	binary_stream_t block_states_stream = read_file("./resource/canonical_block_states.nbt");
	nbt_compound_t block_states_compound = get_misc_nbt_tag(&block_states_stream);
	free(block_states_stream.buffer);
	resources.block_states.size = block_states_compound.size;
	resources.block_states.entries = (mapping_block_state_t *) malloc(resources.block_states.size * sizeof(mapping_block_state_t));
	char *old_name = "";
	uint8_t metadata_counter = 0;
	for (i = 0; i < resources.block_states.size; ++i) {
		nbt_compound_t nbt_compound = block_states_compound.data[i].compound_tag;
		mapping_block_state_t block_state;
		char *name = NULL;
		size_t ii;
		for (ii = 0; ii < nbt_compound.size; ++ii) {
			if (strcmp(nbt_compound.names[ii], "name") == 0) {
				binary_stream_t stream;
				stream.buffer = (int8_t *) malloc(0);
				stream.size = 0;
				uint8_t is_concrete_powder = 0;
				if (strcmp(nbt_compound.data[ii].string_tag, "minecraft:concretePowder") == 0) is_concrete_powder = 1;
				size_t iii;
				for (iii = 0; iii < strlen(nbt_compound.data[ii].string_tag); ++iii) {
					if(isupper(nbt_compound.data[ii].string_tag[iii])) {
						if (i != 0 && is_concrete_powder) put_unsigned_byte('_', &stream);
						put_unsigned_byte(tolower(nbt_compound.data[ii].string_tag[iii]), &stream);
					} else {
						put_unsigned_byte(nbt_compound.data[ii].string_tag[iii], &stream);
					}
				}
				put_unsigned_byte(0, &stream);
				name = (char *) stream.buffer;
				break;
			}
		}
		if (name == NULL) {
			log_error("Does not contain name key");
			exit(0);
		}
		block_state.name = name;
		if (strcmp(old_name, block_state.name) != 0) {
			metadata_counter = 0;
		}
		block_state.metadata = metadata_counter;
		++metadata_counter;
		old_name = block_state.name;
		resources.block_states.entries[i] = block_state;
	}
	destroy_nbt_compound(block_states_compound);
	log_success("Loaded Block States");
	log_success("Resources loaded.");
	return resources;
}