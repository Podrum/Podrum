/*
                   Podrum R3 Copyright MFDGaming & PodrumTeam
                 This file is licensed under the GPLv2 license.
              To use this file you must own a copy of the license.
                       If you do not you can get it from:
            http://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
 */

#include "./resourcemanager.h"
#include "./json.h"
#include <stdio.h>
#include <string.h>

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
	resources_t resources;
	binary_stream_t biome_definitions_stream = read_file("./resource/biome_definitions.nbt");
	resources.biome_definitions = get_misc_nbt_tag(&biome_definitions_stream);
	free(biome_definitions_stream.buffer);
	binary_stream_t entity_identifiers_stream = read_file("./resource/entity_identifiers.nbt");
	resources.entity_identifiers = get_misc_nbt_tag(&entity_identifiers_stream);
	free(entity_identifiers_stream.buffer);
	binary_stream_t item_states_stream = read_file("./resource/item_states.json");
	put_unsigned_byte(0, &item_states_stream);
	json_input_t json_input;
	json_input.json = (char *) item_states_stream.buffer;
	json_input.offset = 0;
	json_root_t json_root = parse_json_root(&json_input);
	free(item_states_stream.buffer);
	resources.item_states.size = json_root.entry.json_array.size;
	resources.item_states.entries = (misc_item_state_t *) malloc(resources.item_states.size * sizeof(misc_item_state_t));
	size_t i;
	for (i = 0; i < resources.item_states.size; ++i) {
		json_object_t json_object = get_json_array_value(i, json_root.entry.json_array).entry.json_object;
		misc_item_state_t item_state;
		char *name = get_json_object_value("name", json_object).entry.json_string;
		size_t size = strlen(name) + 1;
		item_state.name = (char *) malloc(size);
		memcpy(item_state.name, name, size);
		item_state.name = get_json_object_value("name", json_object).entry.json_string;
		item_state.runtime_id = get_json_object_value("runtime_id", json_object).entry.json_number.number.int_number;
		item_state.component_based = get_json_object_value("component_based", json_object).entry.json_bool;
		resources.item_states.entries[i] = item_state;
	}
	destroy_json_root(json_root);
	return resources;
}