/*
                   Podrum R3 Copyright MFDGaming & PodrumTeam
                 This file is licensed under the GPLv2 license.
              To use this file you must own a copy of the license.
                       If you do not you can get it from:
            http://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
 */

#ifndef PODRUM_NETWORK_MINECRAFT_MCMISC_H
#define PODRUM_NETWORK_MINECRAFT_MCMISC_H

#include <stdint.h>
#include <cbinarystream/binary_stream.h>
#include <cnbt/nbt.h>

#define GAME_RULE_BOOLEAN 1
#define GAME_RULE_SIGNED_VAR_INT 2
#define GAME_RULE_FLOAT_LE 3

#define ITEM_EXTRA_DATA_WITH_NBT 0xffff
#define ITEM_EXTRA_DATA_WITHOUT_NBT 0x0000

#define WINDOW_ID_DROP_CONTENTS -100
#define WINDOW_ID_BEACON -24
#define WINDOW_ID_TRADING_OUTPUT -23
#define WINDOW_ID_TRADING_USE_INPUTS -22
#define WINDOW_ID_TRADING_INPUT_2 -21
#define WINDOW_ID_TRADING_INPUT_1 -20
#define WINDOW_ID_ENCHANT_OUTPUT -17
#define WINDOW_ID_ENCHANT_MATERIAL -16
#define WINDOW_ID_ENCHANT_INPUT -15
#define WINDOW_ID_ANVIL_OUTPUT -13
#define WINDOW_ID_ANVIL_RESULT -12
#define WINDOW_ID_ANVIL_MATERIAL -11
#define WINDOW_ID_CONTAINER_INPUT -10
#define WINDOW_ID_CRAFTING_USE_INGREDIENT -5
#define WINDOW_ID_CRAFTING_RESULT -4
#define WINDOW_ID_CRAFTING_REMOVE_INGREDIENT -3
#define WINDOW_ID_CRAFTING_ADD_INGREDIENT -2
#define WINDOW_ID_NONE -1
#define WINDOW_ID_INVENTORY 0
#define WINDOW_ID_FIRST 1
#define WINDOW_ID_LAST 100
#define WINDOW_ID_OFFHAND 119
#define WINDOW_ID_ARMOR 120
#define WINDOW_ID_CREATIVE 121
#define WINDOW_ID_HOTBAR 122
#define WINDOW_ID_FIXED_INVENTORY 123
#define WINDOW_ID_UI 124

#define WINDOW_TYPE_NONE -9
#define WINDOW_TYPE_INVENTORY -1
#define WINDOW_TYPE_CONTAINER 0
#define WINDOW_TYPE_WORKBENCH 1
#define WINDOW_TYPE_FURNACE 2
#define WINDOW_TYPE_ENCHANTMENT 3
#define WINDOW_TYPE_BREWING_STAND 4
#define WINDOW_TYPE_ANVIL 5
#define WINDOW_TYPE_DISPENSER 6
#define WINDOW_TYPE_DROPPER 7
#define WINDOW_TYPE_HOPPER 8
#define WINDOW_TYPE_CAULDRON 9
#define WINDOW_TYPE_MINECART_CHEST 10
#define WINDOW_TYPE_MINECART_HOPPER 11
#define WINDOW_TYPE_HORSE 12
#define WINDOW_TYPE_BEACON 13
#define WINDOW_TYPE_STRUCTURE_EDITOR 14
#define WINDOW_TYPE_TRADING 15
#define WINDOW_TYPE_COMMAND_BLOCK 16
#define WINDOW_TYPE_JUKEBOX 17
#define WINDOW_TYPE_ARMOR 18
#define WINDOW_TYPE_HAND 19
#define WINDOW_TYPE_COMPOUND_CREATOR 20
#define WINDOW_TYPE_ELEMENT_CONSTRUCTOR 21
#define WINDOW_TYPE_MATERIAL_REDUCER 22
#define WINDOW_TYPE_LAB_TABLE 23
#define WINDOW_TYPE_LOOM 24
#define WINDOW_TYPE_LECTERN 25
#define WINDOW_TYPE_GRINDSTONE 26
#define WINDOW_TYPE_BLAST_FURNACE 27
#define WINDOW_TYPE_SMOKER 28
#define WINDOW_TYPE_SONECUTTER 29
#define WINDOW_TYPE_CARTOGRAPHY 30
#define WINDOW_TYPE_HUD 31
#define WINDOW_TYPE_JIGSAW_EDITOR 32
#define WINDOW_TYPE_SMITHING_TABLE 33

typedef struct {
	char *identity;
	char *client;
} misc_login_tokens_t;

typedef struct {
	char *uuid;
	char *version;
	uint64_t size;
	char *content_key;
	char *sub_pack_name;
	char *content_identity;
	uint8_t has_scripts;
} misc_behavior_pack_info_t;

typedef struct {
	misc_behavior_pack_info_t *infos;
	int16_t size;
} misc_behavior_pack_infos_t;

typedef struct {
	char *uuid;
	char *version;
	uint64_t size;
	char *content_key;
	char *sub_pack_name;
	char *content_identity;
	uint8_t has_scripts;
	uint8_t rtx_enabled;
} misc_texture_pack_info_t;

typedef struct {
	misc_texture_pack_info_t *infos;
	int16_t size;
} misc_texture_pack_infos_t;

typedef struct {
	char *uuid;
	char *version;
	char *name;
} misc_resource_pack_id_version_t;

typedef struct {
	misc_resource_pack_id_version_t *id_versions;
	uint32_t size;
} misc_resource_pack_id_versions_t;

typedef struct {
	char **ids;
	int16_t size;
} misc_resource_pack_ids_t;

typedef struct {
	char *name;
	uint8_t enabled;
} misc_experiment_t;

typedef struct {
	misc_experiment_t *entries;
	int32_t size;
} misc_experiments_t;

typedef union {
	uint8_t boolean;
	int32_t signed_var_int;
	float float_le;
} misc_multi_game_rule_t;

typedef struct {
	char *name;
	uint8_t editable;
	uint32_t type;
	misc_multi_game_rule_t value;
} misc_game_rule_t;

typedef struct {
	misc_game_rule_t *entries;
	uint32_t size;
} misc_game_rules_t;

typedef struct {
	char *button_name;
	char *link_uri;
} misc_education_shared_resource_uri_t;

typedef struct {
	char *name;
	int16_t runtime_id;
	uint8_t component_based;
} misc_item_state_t;

typedef struct {
	misc_item_state_t *entries;
	uint32_t size;
} misc_item_states_t;

typedef struct {
	char *name;
	nbt_compound_t nbt;
} misc_block_property_t;

typedef struct {
	misc_block_property_t *entries;
	uint32_t size;
} misc_block_properties_t;

typedef struct {
	uint16_t with_nbt;
	uint8_t nbt_version;
	nbt_compound_t nbt;
	int32_t can_place_on_size;
	binary_stream_t *can_place_on;
	int32_t can_destroy_size;
	binary_stream_t *can_destroy;
	int64_t blocking_tick;
} misc_item_extra_data_t;

typedef struct {
	int32_t network_id;
	uint16_t count;
	uint32_t metadata;
	uint8_t has_stack_id;
	int32_t stack_id;
	int32_t block_runtime_id;
	misc_item_extra_data_t extra;
} misc_item_t;

char *get_misc_string_var_int(binary_stream_t *stream);

char *get_misc_string_int_le(binary_stream_t *stream);

binary_stream_t get_misc_byte_array_var_int(binary_stream_t *stream);

binary_stream_t get_misc_byte_array_signed_var_int(binary_stream_t *stream);

binary_stream_t get_misc_byte_array_short_le(binary_stream_t *stream);

misc_login_tokens_t get_misc_login_tokens(binary_stream_t *stream);

misc_behavior_pack_infos_t get_misc_behavior_pack_infos(binary_stream_t *stream);

misc_texture_pack_infos_t get_misc_texture_pack_infos(binary_stream_t *stream);

misc_resource_pack_id_versions_t get_misc_resource_pack_id_versions(binary_stream_t *stream);

misc_resource_pack_ids_t get_misc_resource_pack_ids(binary_stream_t *stream);

misc_experiment_t get_misc_experiment(binary_stream_t *stream);

misc_experiments_t get_misc_experiments(binary_stream_t *stream);

misc_game_rule_t get_misc_game_rule(binary_stream_t *stream);

misc_game_rules_t get_misc_game_rules(binary_stream_t *stream);

misc_education_shared_resource_uri_t get_misc_education_shared_resource_uri(binary_stream_t *stream);

misc_item_states_t get_misc_item_states(binary_stream_t *stream);

misc_block_properties_t get_misc_block_properties(binary_stream_t *stream);

nbt_compound_t get_misc_nbt_tag(binary_stream_t *stream);

nbt_compound_t get_misc_lnbt_tag(binary_stream_t *stream);

misc_item_extra_data_t get_misc_item_extra_data(uint8_t has_blocking_tick, binary_stream_t *stream);

misc_item_t get_misc_item(uint8_t with_stack_id, binary_stream_t *stream);

void put_misc_string_var_int(char *value, binary_stream_t *stream);

void put_misc_string_int_le(char *value, binary_stream_t *stream);

void put_misc_byte_array_var_int(binary_stream_t value, binary_stream_t *stream);

void put_misc_byte_array_signed_var_int(binary_stream_t value, binary_stream_t *stream);

void put_misc_byte_array_short_le(binary_stream_t value, binary_stream_t *stream);

void put_misc_login_tokens(misc_login_tokens_t value, binary_stream_t *stream);

void put_misc_behavior_pack_infos(misc_behavior_pack_infos_t value, binary_stream_t *stream);

void put_misc_texture_pack_infos(misc_texture_pack_infos_t value, binary_stream_t *stream);

void put_misc_resource_pack_id_versions(misc_resource_pack_id_versions_t value, binary_stream_t *stream);

void put_misc_resource_pack_ids(misc_resource_pack_ids_t value, binary_stream_t *stream);

void put_misc_experiment(misc_experiment_t value, binary_stream_t *stream);

void put_misc_experiments(misc_experiments_t value, binary_stream_t *stream);

void put_misc_game_rule(misc_game_rule_t value, binary_stream_t *stream);

void put_misc_game_rules(misc_game_rules_t value, binary_stream_t *stream);

void put_misc_education_shared_resource_uri(misc_education_shared_resource_uri_t value, binary_stream_t *stream);

void put_misc_item_states(misc_item_states_t value, binary_stream_t *stream);

void put_misc_block_properties(misc_block_properties_t value, binary_stream_t *stream);

void put_misc_nbt_tag(nbt_compound_t value, binary_stream_t *stream);

void put_misc_lnbt_tag(nbt_compound_t value, binary_stream_t *stream);

void put_misc_item_extra_data(misc_item_extra_data_t value, uint8_t has_blocking_tick, binary_stream_t *stream);

void put_misc_item(misc_item_t value, uint8_t with_stack_id, binary_stream_t *stream);

#endif