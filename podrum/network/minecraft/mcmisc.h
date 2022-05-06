/*
                   Podrum R3 Copyright MFDGaming & PodrumTeam
                 This file is licensed under the GPLv2 license.
              To use this file you must own a copy of the license.
                       If you do not you can get it from:
            http://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
 */

#ifndef PODRUM_NETWORK_MINECRAFT_MCMISC_H
#define PODRUM_NETWORK_MINECRAFT_MCMISC_H

#include <podrum/debug.h>
#include <stdbool.h>
#include <stdint.h>
#include <cbinarystream/binary_stream.h>
#include <cnbt/nbt.h>
#include <podrum/world/chunk/chunk.h>

#define GAMEMODE_SURVIVAL 0
#define GAMEMODE_CREATIVE 1
#define GAMEMODE_ADVENTURE 2
#define GAMEMODE_SURVIVAL_SPECTATOR 3
#define GAMEMODE_CREATIVE_SPECTATOR 4
#define GAMEMODE_FALLBACK 5

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

static int32_t BITARRAY_V1[4] = {1, 32, 128, 0};
static int32_t BITARRAY_V2[4] = {2, 16, 256, 0};
static int32_t BITARRAY_V3[4] = {3, 10, 410, 2};
static int32_t BITARRAY_V4[4] = {4, 8, 512, 0};
static int32_t BITARRAY_V5[4] = {5, 6, 683, 2};
static int32_t BITARRAY_V6[4] = {6, 5, 820, 2};
static int32_t BITARRAY_V8[4] = {8, 4, 1024, 0};
static int32_t BITARRAY_V16[4] = {16, 2, 2048, 0};

typedef struct {
	float x;
	float y;
} vec2f_t;

typedef struct {
	uint32_t x;
	uint32_t y;
	uint32_t z;
} vec3u_t;

typedef struct {
	float x;
	float y;
	float z;
} vec3f_t;

typedef struct {
	int32_t x;
	int32_t y;
	int32_t z;
} vec3i_t;

typedef struct {
	int32_t x;
	uint32_t y;
	int32_t z;
} misc_block_coordinates_t;

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
	bool has_scripts;
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
	bool has_scripts;
	bool rtx_enabled;
} misc_texture_pack_info_t;

typedef struct {
	char *palette_name;
	bool ignore_entities;
	bool ignore_blocks;
	misc_block_coordinates_t size;
	misc_block_coordinates_t offset;
	int64_t last_editing_player_unique_id ;
	uint8_t rotation;
	uint8_t mirror;
	uint8_t animation_mode;
	float animation_duration;
	float integrity;
	uint32_t seed;
	vec3f_t pivot;
} misc_structure_block_settings_t;

typedef struct {
	int64_t scoreboard_id;
	char *objective_name;
	int32_t score;
	int8_t entry_type;
	int64_t entity_unique_id;
	char *custom_name;
} misc_scoreboard_entry_t;

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
	bool enabled;
} misc_experiment_t;

typedef struct {
	misc_experiment_t *entries;
	int32_t size;
} misc_experiments_t;

typedef union {
	bool boolean;
	int32_t signed_var_int;
	float float_le;
} misc_multi_game_rule_t;

typedef struct {
	char *name;
	bool editable;
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
	bool component_based;
} misc_item_state_t;

typedef struct {
	misc_item_state_t *entries;
	uint32_t size;
} misc_item_states_t;

typedef struct {
	char *name;
	nbt_named_t nbt;
} misc_block_property_t;

typedef struct {
	misc_block_property_t *entries;
	uint32_t size;
} misc_block_properties_t;

typedef struct {
	uint16_t with_nbt;
	uint8_t nbt_version;
	nbt_named_t nbt;
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
	bool has_stack_id;
	int32_t stack_id;
	int32_t block_runtime_id;
	misc_item_extra_data_t extra;
} misc_item_t;

typedef struct {
	uint32_t size;
	//TODO: implementation all enum
} misc_metadata_dictionary_t;

typedef struct {
	int64_t ridden_entity_id;
	int64_t rider_entity_id;
	uint8_t type;
	bool immediate;
	bool rider_initiated;
} misc_link_t;

typedef struct {
	//TODO: needs more infomation
	char *os_name;
} misc_device_os_t;

typedef struct {
	char *name;
	float min;
	float value;
	float max;
} misc_entity_attribute_t;

typedef struct {
	uint32_t size;
	misc_entity_attribute_t *entity_attributes;
} misc_entity_attributes_t;

typedef struct {
	float min;
	float max;
	float value;
	float default_value;
	char *name;
} misc_player_attribute_t;

typedef struct {
	uint32_t size;
	misc_player_attribute_t *player_attributes;
} misc_player_attributes_t;

typedef struct {
	float yaw;
	float pitch;
	float head_yaw;
} misc_rotation_t;

typedef struct {
	int32_t cause;
	int32_t source_entity_type;
} misc_teleport_t;

typedef struct {
	uint8_t container_id;
} misc_changed_container_info_t;

typedef struct {
	uint8_t container_id;
	uint32_t size;
	misc_changed_container_info_t *changed_slots;
} misc_legacy_transaction_t;

typedef struct {
	int32_t legacy_request_id;
	uint32_t size;
	misc_legacy_transaction_t *legacy_transaction;
} misc_transaction_legacy_t;

typedef struct {
	uint32_t source_type;
	int32_t inventory_id;
	int32_t flags;
	int32_t action;
	uint32_t slot_id;
	misc_item_t old_item;
	misc_item_t new_item;
} misc_transaction_action_t;

typedef struct {
	uint32_t size;
	misc_transaction_action_t *transaction_actions;
} misc_transaction_actions_t;

typedef struct {
	uint32_t action_type;
	vec3i_t block_position;
	uint32_t face;
	uint32_t hotbar_slot;
	misc_item_t item;
	vec3f_t player_position;
	vec3f_t click_position;
	uint32_t block_runtime_id;
} misc_transaction_use_item_t;

typedef struct {
	uint64_t runtime_entity_id;
	uint32_t action_type;
	int32_t hotbar_slot;
	misc_item_t item;
	vec3f_t player_position;
	vec3f_t click_position;
	uint32_t block_runtime_id;
} misc_transaction_use_item_on_entity_t;

typedef struct {
	uint32_t action_type;
	int32_t hotbar_slot;
	misc_item_t item;
	vec3f_t player_head_position;
} misc_transaction_item_release_t;

typedef struct {
	misc_transaction_use_item_t *item_used;
	misc_transaction_use_item_on_entity_t *item_used_on_entity;
	misc_transaction_item_release_t *item_release;
} misc_transaction_data_t;

typedef struct {
	misc_transaction_legacy_t *legacy;
	uint32_t transaction_type;
	misc_transaction_actions_t *actions;
	misc_transaction_data_t *transaction_data;
} misc_transaction_t;

typedef struct {
	int32_t type;
	//TODO: needs more infomation
} misc_recipe_ingredient_t;

typedef struct {
	uint32_t size;
	misc_recipe_ingredient_t *recipe_ingredient_row;
} misc_recipe_ingredient_row_t;

typedef struct {
	char *id;
	uint32_t input_size;
	misc_recipe_ingredient_t *input;
	uint32_t output_size;
	misc_item_t *output;
	char *uuid;
	char *block;
	int32_t priority;
	uint32_t network_id;
} misc_recipe_shapeless_t;

typedef struct {
	char *id;
	int32_t width;
	int32_t height;
	uint32_t input_width_size;
	misc_recipe_ingredient_row_t *input;
	uint32_t output_size;
	misc_item_t *output;
	char *uuid;
	char *block;
	int32_t priority;
	uint32_t network_id;
} misc_recipe_shaped_t;

typedef struct {
	int32_t input_id;
	int32_t input_metadata;
	misc_item_t *output;
	char *block;
} misc_recipe_furnace_t;

typedef struct {
	char *uuid;
	uint32_t network_id;
} misc_recipe_multi_t;

typedef struct {
	int32_t type;
	misc_recipe_shapeless_t *recipe_shapeless;
	misc_recipe_shaped_t *recipe_shaped;
	misc_recipe_furnace_t *recipe_furnace;
	misc_recipe_multi_t *recipe_multi;
} misc_recipe_t;

typedef struct {
	uint32_t size;
	misc_recipe_t *recipe;
} misc_recipes_t;

typedef struct {
	int32_t input_item_id;
	int32_t input_item_metadata;
	int32_t ingredient_id;
	int32_t ingredient_metadata;
	int32_t output_item_id;
	int32_t output_item_metadata;
} misc_potion_type_recipe_t;

typedef struct {
	uint32_t size;
	misc_potion_type_recipe_t *potion_type_recipes;
} misc_potion_type_recipes_t;

typedef struct {
	uint32_t network_id;
	uint32_t count;
} misc_material_reducer_recipe_output_item_t;

typedef struct {
	uint32_t input_item_network_id;
	misc_material_reducer_recipe_output_item_t *output_items;
} misc_material_reducer_recipe_t;

typedef struct {
	uint32_t size;
	misc_material_reducer_recipe_t *material_reducer_recipes;
} misc_material_reducers_recipes_t;

typedef struct {
	int32_t input_item_id;
	int32_t ingredient_id;
	int32_t output_item_id;
} misc_potion_container_change_recipe_t;

typedef struct {
	uint32_t size;
	misc_potion_container_change_recipe_t *potion_container_change_recipes;
} misc_potion_container_change_recipes_t;

typedef struct {
	bool todo;
} misc_skin_t;

typedef struct {
	char *uuid;
	int64_t entity_unique_id;
	char *username;
	char *xbox_user_id;
	char *platform_chat_id;
	int32_t build_platform;
	misc_skin_t skin_data;
	bool is_teacher;
	bool is_host;
} misc_player_record_t;

typedef struct {
	uint8_t type;
	uint32_t count;
	misc_player_record_t *records;
} misc_player_records_t;

typedef struct {
	uint8_t type;
	uint32_t value;
} misc_enum_value_t;

typedef struct {
	char *name;
	uint32_t values_count;
	misc_enum_value_t *values;
} misc_enum_t;

typedef struct {
	char *name;
	uint16_t value_type;
	uint16_t enum_type;
	bool optional;
	uint8_t options;
} misc_command_data_overload_parameter_t;

typedef struct {
	misc_command_data_overload_parameter_t *parameters;
} misc_command_data_overload_t;

typedef struct {
	char *name;
	uint32_t values_count;
	char **values;
} misc_dynamic_enum_t;

typedef struct {
	int32_t value_index;
	int32_t enum_index;
	uint32_t constrains_count;
	uint8_t *constrains;
	char **values;
} misc_enum_constraints_t;

typedef struct {
	char *name;
	char *description;
	uint16_t flags;
	uint8_t permission_level;
	int32_t alias;
	uint32_t overloads_count;
	misc_command_data_overload_t *overloads;
} misc_command_data_t;

typedef struct {
	uint32_t type;
	char *uuid;
	char *request_id;
	int64_t player_entity_id;
} misc_command_origin_t;

typedef struct {
	bool success;
	char *message_id;
	uint32_t parameters_count;
	char **parameters;
} misc_command_output_t;

typedef struct {
	int64_t scoreboard_id;
	int64_t entity_unique_id;
} misc_scoreboard_identity_entry_t;

typedef struct {
	binary_stream_t data;
} misc_nbt_loop_t;

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

nbt_named_t get_misc_nbt_tag(binary_stream_t *stream);

nbt_named_t get_misc_lnbt_tag(binary_stream_t *stream);

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

void put_misc_nbt_tag(nbt_named_t value, binary_stream_t *stream);

void put_misc_lnbt_tag(nbt_named_t value, binary_stream_t *stream);

void put_misc_item_extra_data(misc_item_extra_data_t value, uint8_t has_blocking_tick, binary_stream_t *stream);

void put_misc_item(misc_item_t value, uint8_t with_stack_id, binary_stream_t *stream);

void put_misc_block_storage(block_storage_t *value, binary_stream_t *stream);

void put_misc_sub_chunk(sub_chunk_t *value, binary_stream_t *stream);

void put_misc_chunk(chunk_t *value, uint32_t sub_chunk_count, binary_stream_t *stream);

#endif
