/*
                   Podrum R3 Copyright MFDGaming & PodrumTeam
                 This file is licensed under the GPLv2 license.
              To use this file you must own a copy of the license.
                       If you do not you can get it from:
            http://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
 */

#include <podrum/misc/resourcemanager.h>
#include <podrum/misc/json.h>
#include <podrum/misc/logger.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <podrum/misc/base64.h>

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
	binary_stream_t biome_definitions_stream = read_file("./resource/biome_definition_list.nbt");
	resources.biome_definitions = get_misc_nbt_tag(&biome_definitions_stream);
	free(biome_definitions_stream.buffer);
	log_success("Loaded Biome Definitions");
	binary_stream_t entity_identifiers_stream = read_file("./resource/available_entity_identifiers.nbt");
	resources.entity_identifiers = get_misc_nbt_tag(&entity_identifiers_stream);
	free(entity_identifiers_stream.buffer);
	log_success("Loaded Entity Identifiers");
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
		json_object_t json_object = json_root.entry.json_array.members[i].json_object;
		misc_item_state_t item_state;
		char *name = get_json_object_value("name", json_object).entry.json_string;
		size_t size = strlen(name) + 1;
		item_state.name = (char *) malloc(size);
		memcpy(item_state.name, name, size);
		item_state.runtime_id = get_json_object_value("runtime_id", json_object).entry.json_number.number.int_number;
		item_state.component_based = get_json_object_value("component_based", json_object).entry.json_bool;
		resources.item_states.entries[i] = item_state;
	}
	destroy_json_root(json_root);
	log_success("Loaded Item States");
	binary_stream_t block_states_stream = read_file("./resource/block_states.nbt");
	nbt_compound_t block_states_compound = get_nbt_compound_tag(E_NETWORK_ENDIAN, &block_states_stream);
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
				name = nbt_compound.data[ii].string_tag;
				break;
			}
		}
		if (name == NULL) {
			log_error("Does not contain name key");
			exit(0);
		}
		size_t size = strlen(name) + 1;
		block_state.name = (char *) malloc(size);
		memcpy(block_state.name, name, size);
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
	binary_stream_t creative_items_stream = read_file("./resource/creative_items.json");
	put_unsigned_byte(0, &creative_items_stream);
	json_input.json = (char *) creative_items_stream.buffer;
	json_input.offset = 0;
	json_root = parse_json_root(&json_input);
	free(creative_items_stream.buffer);
	resources.creative_items.size = json_root.entry.json_array.size;
	resources.creative_items.entries = (misc_item_t *) malloc(resources.creative_items.size * sizeof(misc_item_t));
	for (i = 0; i < resources.creative_items.size; ++i) {
		json_object_t json_object = get_json_array_value(i, json_root.entry.json_array).entry.json_object;
		misc_item_t item;
		char *name = get_json_object_value("name", json_object).entry.json_string;
		item.network_id = item_state_to_runtime_id(name, resources.item_states);
		item.count = 1;
		json_root_t json_metadata = get_json_object_value("metadata", json_object);
		if (json_metadata.type != JSON_EMPTY) {
			item.metadata = json_metadata.entry.json_number.number.int_number;
		} else {
			item.metadata = 0;
		}
		json_root_t json_block_state_name = get_json_object_value("block_state_name", json_object);
		json_root_t json_block_state_metadata = get_json_object_value("block_state_metadata", json_object);
		if (json_block_state_name.type != JSON_EMPTY && json_block_state_metadata.type != JSON_EMPTY) {
			item.block_runtime_id = block_state_to_runtime_id(json_block_state_name.entry.json_string, json_block_state_metadata.entry.json_number.number.int_number, resources.block_states);
		} else {
			item.block_runtime_id = 0;
		}
		json_root_t json_nbt = get_json_object_value("nbt_b64", json_object);
		if (json_nbt.type != JSON_EMPTY) {
			item.extra.with_nbt = ITEM_EXTRA_DATA_WITH_NBT;
			item.extra.nbt_version = 1;
			binary_stream_t nbt_stream = base64_decode(json_nbt.entry.json_string);
			item.extra.nbt = get_misc_lnbt_tag(&nbt_stream);
			free(nbt_stream.buffer);
		} else {
			item.extra.with_nbt = ITEM_EXTRA_DATA_WITHOUT_NBT;
		}
		item.extra.can_place_on_size = 0;
		item.extra.can_destroy_size = 0;
		resources.creative_items.entries[i] = item;
	}
	destroy_json_root(json_root);
	log_success("Creative Items loaded.");
	log_success("Resources loaded.");
	return resources;
}

void destroy_resources(resources_t *resources)
{
	destroy_nbt_named(resources->biome_definitions);
	destroy_nbt_named(resources->entity_identifiers);
	size_t i;
	for (i = 0; i < resources->item_states.size; ++i) {
		free(resources->item_states.entries[i].name);
	}
	free(resources->item_states.entries);
	for (i = 0; i < resources->block_states.size; ++i) {
		free(resources->block_states.entries[i].name);
	}
	free(resources->block_states.entries);
	for (i = 0; i < resources->creative_items.size; ++i) {
		if (resources->creative_items.entries[i].extra.with_nbt == ITEM_EXTRA_DATA_WITH_NBT) {
			destroy_nbt_named(resources->creative_items.entries[i].extra.nbt);
		}
	}
	free(resources->creative_items.entries);
}
