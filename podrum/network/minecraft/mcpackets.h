/*
                   Podrum R3 Copyright MFDGaming & PodrumTeam
                 This file is licensed under the GPLv2 license.
              To use this file you must own a copy of the license.
                       If you do not you can get it from:
            http://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
 */

#ifndef PODRUM_NETWORK_MINECRAFT_MCPACKETS_H
#define PODRUM_NETWORK_MINECRAFT_MCPACKETS_H

#include <stdbool.h>
#include <podrum/debug.h>
#include <cbinarystream/binary_stream.h>
#include <stdlib.h>
#include <podrum/network/minecraft/mcmisc.h>
#include <cnbt/nbt.h>

#define GAME_VERSION "1.18.30"
#define GAME_PROTOCOL_VERSION 503
#define GAME_ENGINE "Podrum"

#define ID_LOGIN 0x01
#define ID_PLAY_STATUS 0x02
#define ID_RESOURCE_PACKS_INFO 0x06
#define ID_RESOURCE_PACK_STACK 0x07
#define ID_RESOURCE_PACK_CLIENT_RESPONSE 0x08
#define ID_TEXT 0x09
#define ID_START_GAME 0x0B
#define ID_MOVE_PLAYER 0x13
#define ID_INTERACT 0x21
#define ID_CONTAINER_OPEN 0x2E
#define ID_CONTAINER_CLOSE 0x2F
#define ID_LEVEL_CHUNK 0x3A
#define ID_REQUEST_CHUNK_RADIUS 0x45
#define ID_CHUNK_RADIUS_UPDATED 0x46
#define ID_AVAILABLE_ENTITY_IDENTIFIERS 0x77
#define ID_NETWORK_CHUNK_PUBLISHER_UPDATE 0x79
#define ID_BIOME_DEFINITION_LIST 0x7A
#define ID_CREATIVE_CONTENT 0x91
#define ID_PACKET_VIOLATION_WARNING 0x9C
#define ID_GAME 0xFE

#define PLAY_STATUS_LOGIN_SUCCESS 0
#define PLAY_STATUS_FAILED_CLIENT 1
#define PLAY_STATUS_FAILED_SPAWN 2
#define PLAY_STATUS_PLAYER_SPAWN 3
#define PLAY_STATUS_FAILED_INVALID_TENANT 4
#define PLAY_STATUS_FAILED_VANILLA_EDU 5
#define PLAY_STATUS_FAILED_EDU_VANILLA 6
#define PLAY_STATUS_FAILED_SERVER_FULL 7

#define RESOURCE_PACK_CLIENT_RESPONSE_NONE 0
#define RESOURCE_PACK_CLIENT_RESPONSE_REFUSED 1
#define RESOURCE_PACK_CLIENT_RESPONSE_SEND_PACKS 2
#define RESOURCE_PACK_CLIENT_RESPONSE_HAVE_ALL_PACKS 3
#define RESOURCE_PACK_CLIENT_RESPONSE_COMPLETED 4

#define MOVE_PLAYER_MODE_NORMAL 0
#define MOVE_PLAYER_MODE_RESET 1
#define MOVE_PLAYER_MODE_TELEPORT 2
#define MOVE_PLAYER_MODE_ROTATION 3

#define MOVE_PLAYER_TELEPORT_CAUSE_UNKNOWN 0
#define MOVE_PLAYER_TELEPORT_CAUSE_PROJECTILE 1
#define MOVE_PLAYER_TELEPORT_CAUSE_CHORUS_FRUIT 2
#define MOVE_PLAYER_TELEPORT_CAUSE_COMMAND 3
#define MOVE_PLAYER_TELEPORT_CAUSE_BEHAVIOR 4

#define INTERACT_LEAVE_VEHICLE 3
#define INTERACT_MOUSE_OVER_ENTITY 4
#define INTERACT_OPEN_INVENTORY 6

#define TEXT_TYPE_RAW 0
#define TEXT_TYPE_CHAT 1
#define TEXT_TYPE_TRANSLATION 2
#define TEXT_TYPE_POPUP 3
#define TEXT_TYPE_JUKEBOX_POPUP 4
#define TEXT_TYPE_TIP 5
#define TEXT_TYPE_SYSTEM 6
#define TEXT_TYPE_WHISPER 7
#define TEXT_TYPE_ANNOUNCEMENT 8
#define TEXT_TYPE_JSON_WHISPER 9
#define TEXT_TYPE_JSON 10

typedef struct {
	binary_stream_t *streams;
	size_t streams_count;
} packet_game_t;

typedef struct {
	int32_t protocol_version;
	misc_login_tokens_t tokens;
} packet_login_t;

typedef struct {
	int32_t status;
} packet_play_status_t;

typedef struct {
	bool hide_disconnect_reason;
	char *message;
} packet_disconnect_t;

typedef struct {
	bool must_accept;
	bool has_scripts;
	bool force_server_packs;
	misc_behavior_pack_infos_t behavior_packs;
	misc_texture_pack_infos_t texture_packs;
} packet_resource_packs_info_t;

typedef struct {
	bool must_accept;
	misc_resource_pack_id_versions_t behavior_packs;
	misc_resource_pack_id_versions_t resource_packs;
	char *game_version;
	misc_experiments_t experiments;
	bool experiments_previously_used;
} packet_resource_pack_stack_t;

typedef struct {
	uint8_t response_status;
	misc_resource_pack_ids_t resource_pack_ids;
} packet_resource_pack_client_response_t;

typedef struct {
	uint8_t type;
	char *source_name;
	char *message;
	uint32_t parameters_size;
	char **parameters;
	bool needs_translation;
	char* xuid;
	char *platform_chat_id;
} packet_text_t;

typedef struct {
	uint32_t time;
} packet_set_time_t;

typedef struct {
	int64_t entity_id;
	uint64_t runtime_entity_id;
	int32_t player_gamemode;
	vec3f_t player_position;
	float pitch;
	float yaw;
	int32_t seed;
	int16_t biome_type;
	char *biome_name;
	int32_t dimension;
	int32_t generator;
	int32_t world_gamemode;
	int32_t difficulty;
	misc_block_coordinates_t spawn_position;
	bool achievements_disabled;
	int32_t day_cycle_stop_time;
	int32_t edu_offer;
	bool edu_features_enabled;
	char *edu_product_uuid;
	float rain_level;
	float lightning_level;
	bool has_confirmed_platform_locked_content;
	bool is_multiplayer;
	bool broadcast_to_lan;
	uint32_t xbox_live_broadcast_mode;
	uint32_t platform_broadcast_mode;
	bool enable_commands;
	bool are_texture_packs_required;
	misc_game_rules_t gamerules;
	misc_experiments_t experiments;
	bool experiments_previously_used;
	bool bonus_chest;
	bool map_enabled;
	int32_t permission_level;
	int32_t server_chunk_tick_range;
	bool has_locked_behavior_pack;
	bool has_locked_texture_pack;
	bool is_from_locked_world_template;
	bool msa_gamertags_only;
	bool is_from_world_template;
	bool is_world_template_option_locked;
	bool only_spawn_v1_villagers;
	char *game_version;
	int32_t limited_world_width;
	int32_t limited_world_length;
	bool is_new_nether;
	misc_education_shared_resource_uri_t edu_resource_uri;
	bool experimental_gameplay_override;
	char *level_id;
	char *world_name;
	char *premium_world_template_id;
	bool is_trial;
	uint32_t movement_authority;
	int32_t rewind_history_size;
	bool server_authoritative_block_breaking;
	int64_t current_tick;
	int32_t enchantment_seed;
	misc_block_properties_t block_properties;
	misc_item_states_t item_states;
	char *multiplayer_correlation_id;
	bool server_authoritative_inventory;
	char *engine;
	uint64_t block_pallete_checksum;
} packet_start_game_t;

typedef struct {
	char *uuid;
	char *username;
	int64_t entity_id;
	uint64_t runtime_entity_id;
	char *platform_chat_id;
	vec3f_t position;
	vec3f_t velocity;
	float pitch;
	float yaw;
	float head_yaw;
	misc_item_t held_item;
	misc_metadata_dictionary_t metadata;
	uint32_t flags;
	uint32_t command_permission;
	uint32_t action_permissions;
	uint32_t permission_level;
	uint32_t custom_stored_permissions;
	int64_t user_id;
	misc_link_t link;
	char *device_id;
	misc_device_os_t device_os;
} packet_add_player_t;

typedef struct {
	int64_t entity_id;
	uint64_t runtime_entity_id;
	char *entity_type;
	vec3f_t position;
	vec3f_t velocity;
	float pitch;
	float yaw;
	float head_yaw;
	misc_entity_attributes_t attributes;
	misc_metadata_dictionary_t metadata;
	misc_link_t link;
} packet_add_entity_t;

typedef struct {
	int64_t entity_id;
} packet_remove_entity_t;

typedef struct {
	int64_t entity_id;
	uint64_t runtime_entity_id;
	misc_item_t item;
	vec3f_t position;
	vec3f_t velocity;
	misc_metadata_dictionary_t metadata;
	bool is_from_fishing;
} packet_add_item_entity_t;

typedef struct {
	uint64_t runtime_entity_id;
	uint32_t target;
} packet_take_item_entity_t;

typedef struct {
	uint64_t runtime_entity_id;
	uint8_t flags;
	vec3f_t position;
	misc_rotation_t rotation;
} packet_move_entity_t;

typedef struct {
	uint32_t runtime_id;
	vec3f_t position;
	float pitch;
	float yaw;
	float head_yaw;
	uint8_t mode;
	bool on_ground;
	uint32_t ridden_runtime_id;
	misc_teleport_t teleport;
	uint64_t tick;
} packet_move_player_t;

typedef struct {
	int32_t jump_strength;
} packet_rider_jump_t;

typedef struct {
	misc_block_coordinates_t position;
	uint32_t block_runtime_id;
	int32_t flags;
	uint32_t layer;
} packet_update_block_t;

typedef struct {
	int64_t entity_id;
	uint64_t entity_runtime_id;
	vec3f_t coordinates;
	int32_t direction;
	char *title;
} packet_add_painting_t;

typedef struct {
	int64_t request_time;
	int64_t response_time;
} packet_tick_sync_t;

typedef struct {
	uint8_t sound_id;
	vec3f_t position;
	int32_t block_id;
	int32_t entity_type;
	bool is_baby_mob;
	bool is_global;
} packet_level_sound_event_old_t;

typedef struct {
	int32_t event;
	vec3f_t position;
	int32_t data;
} packet_level_event_t;

typedef struct {
	misc_block_coordinates_t position;
	int32_t type;
	int32_t data;
} packet_block_event_t;

typedef struct {
	uint64_t runtime_entity_id;
	uint8_t event_id;
	int32_t effect_id;
	int32_t data;
} packet_entity_event_t;

typedef struct {
	uint64_t runtime_entity_id;
	uint8_t event_id;
	int32_t amplifier;
	bool show_particles;
	int32_t duration;
} packet_mob_effect_t;

typedef struct {
	uint64_t runtime_entity_id;
	misc_player_attributes_t attributes;
	uint64_t tick;
} packet_update_attributes_t;

typedef struct {
	misc_transaction_t transaction;
} packet_inventory_transaction_t;

typedef struct {
	uint64_t runtime_entity_id;
	misc_item_t item;
	uint8_t slot;
	uint8_t selected_slot;
	int8_t window_id;
} packet_mob_equipment_t;

typedef struct {
	uint64_t runtime_entity_id;
	misc_item_t item;
	misc_item_t helmet;
	misc_item_t chestplate;
	misc_item_t leggings;
	misc_item_t boots;
} packet_mob_armor_equipment_t;

typedef struct {
	uint8_t action_id;
	uint64_t target_runtime_entity_id;
	vec3f_t position;
} packet_interact_t;

typedef struct {
	vec3i_t block_position;
	bool with_block_data;
	uint8_t selected_slot;
} packet_block_pick_request_t;

typedef struct {
	uint64_t runtime_entity_id;
	uint8_t selected_slot;
	bool with_entity_data;
} packet_entity_pick_request_t;

typedef struct {
	uint64_t runtime_entity_id;
	int32_t action;
	misc_block_coordinates_t block_position;
	int32_t block_face;
} packet_player_action_t;

typedef struct {
	int32_t caused;
	int32_t damage;
	int64_t armor_slots;
} packet_hurt_armor_t;

typedef struct {
	uint64_t runtime_entity_id;
	misc_metadata_dictionary_t metadata;
	uint32_t tick;
} packet_set_entity_data_t;

typedef struct {
	uint64_t runtime_entity_id;
	vec3f_t velocity;
} packet_set_entity_motion_t;

typedef struct {
	misc_link_t *link;
} packet_set_entity_link_t;

typedef struct {
	int32_t health;
} packet_set_health_t;

typedef struct {
	int32_t spawn_type;
	misc_block_coordinates_t player_position;
	int32_t dimension;
	misc_block_coordinates_t world_position;
} packet_set_spawn_position_t;

typedef struct {
	int32_t action_id;
	uint64_t runtime_entity_id;
	float boat_rowing_time;
} packet_animate_t;

typedef struct {
	vec3f_t position;
	uint8_t states;
	uint64_t runtime_entity_id;
} packet_respawn_t;

typedef struct {
	int8_t window_id;
	int8_t window_type;
	misc_block_coordinates_t block_position;
	int64_t runtime_entity_id;
} packet_container_open_t;

typedef struct {
	int8_t window_id;
	bool server;
} packet_container_close_t;

typedef struct {
	int32_t selected_slot;
	int8_t window_id;
	bool select_slot;
} packet_player_hotbar_t;

typedef struct {
	int32_t window_id;
	misc_item_t *items;
} packet_inventory_content_t;

typedef struct {
	int32_t window_id;
	int32_t property;
	int32_t value;
} packet_container_set_data_t;

typedef struct {
	misc_recipes_t recipes;
	misc_potion_type_recipes_t potion_type_recipes;
	misc_material_reducers_recipes_t material_reducers_recipes;
	misc_potion_container_change_recipes_t potion_container_recipes;
	bool clear_recipes;
} packet_crafting_data_t;

typedef struct {
	char *item_name;
	char *item_effects;
	int32_t hotbar_slot;
} packet_gui_data_pick_item_t; // non affected to client

typedef struct {
	int32_t flag;
	//TODO
} misc_adventure_flags_t;

typedef struct {
	int32_t flag;
	//TODO
} misc_action_permissions_t;

typedef struct {
	misc_adventure_flags_t flags;
	uint32_t command_permission;
	misc_action_permissions_t action_permissions;
	uint32_t permission_level;
	uint32_t custom_permission;
	int64_t user_id;
} packet_adventure_settings_t;

typedef struct {
	misc_block_coordinates_t position;
	nbt_named_t nbt;
} packet_block_entity_data_t;

typedef struct {
	float motion_x;
	float motion_z;
	bool is_jumping;
	bool is_sneaking;
} packet_player_input_t;

typedef struct {
	int32_t x;
	int32_t z;
	uint32_t sub_chunk_count;
	uint16_t highest_subchunk_count;
	bool cache_enabled;
	uint32_t hashes_count;
	uint64_t *hashes;
	binary_stream_t payload;
} packet_level_chunk_t;

typedef struct {
	bool is_enabled;
} packet_set_commands_enabled_t;

typedef struct {
	uint32_t difficulty;
} packet_set_difficulty_t;

typedef struct {
	int32_t dimension;
	vec3f_t position;
	bool respawn;
} packet_change_dimension_t;

typedef struct {
	int32_t gamemode; 
} packet_set_player_game_type_t;

typedef struct {
	misc_player_records_t records; 
} packet_player_list_t;

typedef struct {
	uint16_t event_type; 
} packet_simple_event_t;

typedef struct {
	uint64_t runtime_id; 
	int32_t event_type;
	uint8_t use_player_id;
	binary_stream_t event_data;
} packet_event_t;

typedef struct {
	vec3f_t position;
	int32_t count;
} packet_spawn_experience_orb_t;

// typedef struct {
// 	int64_t map_id;
// 	uint32_t update_flags;
// 	uint8_t dimension;
// 	bool is_locked;
// 	misc_map_included_in_t included_in;
// 	uint8_t scale;
// 	misc_map_tracked_t tracked;
// 	misc_map_texture_t texture;
// } packet_clientbound_map_item_data_t; needs more infomation

typedef struct {
	int64_t map_id;
} packet_map_info_request_t;

typedef struct {
	int32_t chunk_radius;
} packet_request_chunk_radius_t;

typedef struct {
	int32_t chunk_radius;
} packet_chunk_radius_updated_t;

typedef struct {
	misc_block_coordinates_t position;
} packet_item_frame_drop_item_t;

typedef struct {
	misc_game_rules_t rules;
} packet_game_rules_changed_t;

typedef struct {
	int64_t camera_entity_unique_id;
	int64_t target_player_unique_id;
} packet_camera_t;

typedef struct {
	int64_t boss_entity_id;
	uint32_t type;
	char *title;
	float progress;
	int16_t screen_darkening;
	uint32_t color;
	uint32_t overlay;
	int64_t player_id;
} packet_boss_event_t;

typedef struct {
	uint64_t runtime_entity_id;
	int32_t status;
} packet_show_credits_t;

typedef struct {
	uint32_t enum_values_count;
	char **enum_values;
	uint32_t suffixes_count;
	char **suffixes;
	uint32_t enums_count;
	misc_enum_t *enums;
	uint32_t command_data_count;
	misc_command_data_t *command_data;
	uint32_t dynamic_enums_count;
	misc_dynamic_enum_t *dynamic_enums;
	uint32_t enum_constraints_count;
	misc_enum_constraints_t *enum_constraints;
} packet_available_commands_t;

typedef struct {
	char *command;
	misc_command_origin_t origin;
	bool interval;
} packet_command_request_t;

typedef struct {
	bool is_block;
	misc_block_coordinates_t position;
	uint32_t mode;
	bool needs_redstone;
	bool conditional;
	uint64_t minecart_entity_runtime_id;
	char *command;
	char *last_output;
	char *name;
	bool should_track_output;
	int32_t tick_delay;
	bool execute_on_first_tick;
} packet_command_block_update_t;

typedef struct {
	misc_command_origin_t origin;
	int8_t output_type;
	uint32_t success_count;
	uint32_t output_length;
	misc_command_output_t output;
	char *data_set;
} packet_command_output_t;

typedef struct {
	int32_t window_id;
	int32_t window_type;
	uint32_t trading_options;
	uint32_t output_length;
	uint32_t trade_tier;
	uint64_t villager_unique_id;
	uint64_t player_unique_id;
	char *display_name;
	bool new_trade_ui;
	bool is_economic_trade;
	nbt_named_t offers;
} packet_update_trade_t;

typedef struct {
	int32_t window_id;
	int32_t window_type;
	uint8_t size;
	int64_t entity_unique_id;
	nbt_named_t inventory;
} packet_update_equipment_t;

typedef struct {
	char *uuid;
	uint32_t max_chunk_size;
	uint32_t chunk_count;
	uint64_t size;
	binary_stream_t hash;
	bool is_premium;
	uint8_t pack_type;
} packet_resource_pack_data_info_t;

typedef struct {
	char *uuid;
	uint32_t chunk_index;
	uint64_t progress;
	binary_stream_t payload;
} packet_resource_pack_chunk_data_t;

typedef struct {
	char *uuid;
	uint32_t chunk_index;
} packet_resource_pack_chunk_request_t;

typedef struct {
	char *server_address;
	uint16_t port;
} packet_transfer_t;

typedef struct {
	char *name;
	misc_block_coordinates_t position;
	float volume;
	float pitch;
} packet_play_sound_t;

typedef struct {
	char *name;
	bool stop_all;
} packet_stop_sound_t;

typedef struct {
	int32_t type;
	char *text;
	int32_t fade_in_time;
	int32_t stay_time;
	int32_t fade_out_time;
	char *xuid;
	uint64_t platform_online_id;
} packet_set_title_t;

typedef struct {
	char *uuid;
} packet_add_behavior_tree_t;

typedef struct {
	misc_block_coordinates_t position;
	char *structure_name;
	char *data_field;
	bool include_players;
	bool show_bounding_box;
	int32_t structure_block_type;
	misc_structure_block_settings_t settings;
	int32_t redstone_save_mode;
	bool should_trigger;
} packet_structure_block_update_t;

typedef struct {
	char *offer_id;
	bool show_all;
} packet_show_store_offer_t;

typedef struct {
	uint32_t receipts_count;
	char **receipts;
} packet_purchased_marketplace_item_t;

typedef struct {
	char *uuid;
	misc_skin_t skin;
	char *skin_name;
	char *old_skin_name;
	bool is_verified;
} packet_player_skin_t;

typedef struct {
	misc_login_tokens_t tokens;
} packet_sub_client_login_t;

typedef struct {
	char *server;
} packet_initiate_web_socket_connection_t;

typedef struct {
	uint32_t entity_type;
} packet_set_last_hurt_by_t;

typedef struct {
	uint8_t type;
	uint8_t slot;
	uint8_t page_number;
	char *text;
	char *photo_name;
	uint8_t page_1;
	uint8_t page_2;
	char *title;
	char *author;
	char *xuid;
} packet_book_edit_t;

typedef struct {
	uint64_t runtime_entity_id;
	uint8_t request_type;
	char *command;
	uint8_t action_type;
	char *scene_name;
} packet_npc_request_t;

typedef struct {
	char *image_name;
	char *image_data;
	char *book_id;
	uint8_t photo_type;
	uint8_t source_type;
	int64_t owner_entity_unique_id;
	char *new_photo_name;
} packet_photo_transfer_t;

typedef struct {
	uint32_t form_id;
	char *data;
} packet_modal_form_request_t;

// typedef struct {
// } packet_server_settings_request_t; this packet has no fields

typedef struct {
	uint32_t form_id;
	char *data;
} packet_server_settings_response_t;

typedef struct {
	char *xuid;
} packet_show_profile_t;

typedef struct {
	int32_t gamemode;
} packet_set_default_game_type_t;

typedef struct {
	char *objective_name;
} packet_remove_objective_t;

typedef struct {
	char *display_slot;
	char *objective_name;
	char *display_name;
	char *criteria_name;
	int32_t sort_order;
} packet_set_display_objective_t;

typedef struct {
	uint8_t action;
	uint32_t entries_count;
	misc_scoreboard_entry_t *entries;
} packet_set_score_t;

typedef struct {
	uint8_t action_type;
	vec3u_t position;
	uint8_t reaction_type;
} packet_lab_table_t;

typedef struct {
	misc_block_coordinates_t position;
	uint32_t block_runtime_id;
	uint8_t *flags;
	uint32_t layer;
	int64_t entity_unique_id;
	uint8_t transition_type;
} packet_update_block_synced_t;

typedef struct {
	uint64_t runtime_entity_id;
	uint8_t *flags;
	float x;
	float y;
	float z;
	float rot_x;
	float rot_y;
	float rot_z;
} packet_move_entity_delta_t;

typedef struct {
	int8_t action;
	uint32_t entries_count;
	misc_scoreboard_identity_entry_t *entries;
} packet_set_scoreboard_identity_t;

typedef struct {
	uint64_t runtime_entity_id;
} packet_set_local_player_as_initialized_t;

// typedef struct {
// } packet_update_soft_enum_t; needs more infomation

typedef struct {
	uint64_t timestamp;
	uint8_t needs_response;
} packet_network_stack_latency_t;

typedef struct {
	char *name;
	char *data;
} packet_script_custom_event_t;

typedef struct {
	uint8_t dimension;
	int64_t entity_id;
	vec3f_t position;
	char *particle_name;
} packet_spawn_particle_effect_t;

typedef struct {
	nbt_named_t nbt;
} packet_available_entity_identifiers_t;

// typedef struct {
// } packet_level_sound_event_v2_t; use LevelSoundEvent instead

typedef struct {
	misc_block_coordinates_t position;
	uint32_t radius;
} packet_network_chunk_publisher_update_t;

typedef struct {
	nbt_named_t nbt;
} packet_biome_definition_list_t;

typedef struct {
	uint32_t sound_id;
	vec3f_t position;
	int32_t extra_data;
	char *entity_type;
	bool is_baby_mob;
	bool is_global;
} packet_level_sound_event_t;

typedef struct {
	uint32_t event_id;
	misc_nbt_loop_t nbt;
} packet_level_event_generic_t;

typedef struct {
	uint8_t page_number;
	uint8_t page_count;
	vec3i_t position;
	bool drop_book;
} packet_lectern_update_t;

// typedef struct {
// } packet_video_stream_connect_t; unused packet

// typedef struct {
// } packet_add_ecs_entity_t; unknown used for

// typedef struct {
// } packet_remove_ecs_entity_t; unknown used for

typedef struct {
	bool enabled;
} packet_client_cache_status_t;

typedef struct {
	uint32_t animation_type;
} packet_onscreen_texture_animation_t;

typedef struct {
	int64_t origin_map_id;
	int64_t new_map_id;
} packet_map_create_locked_copy_t;

typedef struct {
	char *name;
	misc_block_coordinates_t position;
	misc_structure_block_settings_t settings;
	uint8_t request_type;
} packet_structure_template_data_export_request_t;

typedef struct {
	char *name;
	bool success;
	nbt_named_t nbt;
	uint8_t response_type;
} packet_structure_template_data_export_response_t;

// typedef struct {
// } packet_update_block_properties_t; unused

typedef struct {
	uint32_t misses_count;
	uint32_t haves_count;
	uint32_t missing_count;
	uint64_t *missing;
	uint32_t have_count;
	uint64_t *have;
} packet_client_cache_blob_status_t;

// typedef struct {
// 	uint32_t blobs_count;
// 	misc_blob_t *blobs;
// } packet_client_cache_miss_response_t; needs implement blob datatype

// typedef struct {
// } packet_education_settings_t; unused by the normal base game

// typedef struct {
// 	uint64_t entity_id;
// 	char *remote_id;
// 	 *flags;
// } packet_emote_t; needs implement flags

typedef struct {
	int32_t action_type;
} packet_multiplayer_settings_t;

typedef struct {
	char *command_line;
	bool suppress_output;
} packet_settings_command_t;

typedef struct {
	uint8_t damage;
	misc_block_coordinates_t position;
} packet_anvil_damage_t;

typedef struct {
	int16_t used_item_id;
	int32_t use_method;
} packet_completed_using_item_t;

// typedef struct {
// 	uint16_t compression_threshold;
// } packet_network_settings_t; unused

// typedef struct {
// 	float pitch;
// 	float yaw;
// 	vec3f_t position;
// 	vec2f_t move_vector;
// 	head_yaw;
// 	input_data;
// 	uint32_t input_mode;
// 	uint32_t play_mode;
// 	vec3f_t gaze_direction;
// 	uint64_t tick;
// 	vec3f_t delta;
// 	transaction;
// 	item_stack_request;
// 	block_action;
// } packet_player_auth_input_t; TODO: implementation

typedef struct {
	uint32_t size;
	uint32_t *entry_ids;
	misc_item_t *items;
} packet_creative_content_t;

// typedef struct {
// 	uint32_t options_length;
// 	options;
// } packet_player_enchant_options_t; TODO: implementation

// typedef struct {
// 	uint32_t requests_length;
// 	requests;
// } packet_item_stack_request_t; TODO: implementation

packet_game_t get_packet_game(binary_stream_t *stream);

packet_login_t get_packet_login(binary_stream_t *stream);

packet_play_status_t get_packet_play_status(binary_stream_t *stream);

packet_resource_packs_info_t get_packet_resource_packs_info(binary_stream_t *stream);

packet_resource_pack_stack_t get_packet_resource_pack_stack(binary_stream_t *stream);

packet_resource_pack_client_response_t get_packet_resource_pack_client_response(binary_stream_t *stream);

packet_start_game_t get_packet_start_game(binary_stream_t *stream);

packet_biome_definition_list_t get_packet_biome_definition_list(binary_stream_t *stream);

packet_available_entity_identifiers_t get_packet_available_entity_identifiers(binary_stream_t *stream);

packet_creative_content_t get_packet_creative_content(binary_stream_t *stream);

packet_interact_t get_packet_interact(binary_stream_t *stream);

packet_container_open_t get_packet_container_open(binary_stream_t *stream);

packet_container_close_t get_packet_container_close(binary_stream_t *stream);

packet_move_player_t get_packet_move_player(binary_stream_t *stream);

packet_network_chunk_publisher_update_t get_packet_network_chunk_publisher_update(binary_stream_t *stream);

packet_request_chunk_radius_t get_packet_request_chunk_radius(binary_stream_t *stream);

packet_chunk_radius_updated_t get_packet_chunk_radius_updated(binary_stream_t *stream);

packet_level_chunk_t get_packet_level_chunk(binary_stream_t *stream);

void put_packet_game(packet_game_t packet, binary_stream_t *stream);

void put_packet_login(packet_login_t packet, binary_stream_t *stream);

void put_packet_play_status(packet_play_status_t packet, binary_stream_t *stream);

void put_packet_resource_packs_info(packet_resource_packs_info_t packet, binary_stream_t *stream);

void put_packet_resource_pack_stack(packet_resource_pack_stack_t packet, binary_stream_t *stream);

void put_packet_resource_pack_client_response(packet_resource_pack_client_response_t packet, binary_stream_t *stream);

void put_packet_start_game(packet_start_game_t packet, binary_stream_t *stream);

void put_packet_biome_definition_list(packet_biome_definition_list_t packet, binary_stream_t *stream);

void put_packet_available_entity_identifiers(packet_available_entity_identifiers_t packet, binary_stream_t *stream);

void put_packet_creative_content(packet_creative_content_t packet, binary_stream_t *stream);

void put_packet_interact(packet_interact_t packet, binary_stream_t *stream);

void put_packet_container_open(packet_container_open_t packet, binary_stream_t *stream);

void put_packet_container_close(packet_container_close_t packet, binary_stream_t *stream);

void put_packet_move_player(packet_move_player_t packet, binary_stream_t *stream);

void put_packet_network_chunk_publisher_update(packet_network_chunk_publisher_update_t packet, binary_stream_t *stream);

void put_packet_request_chunk_radius(packet_request_chunk_radius_t packet, binary_stream_t *stream);

void put_packet_chunk_radius_updated(packet_chunk_radius_updated_t packet, binary_stream_t *stream);

void put_packet_level_chunk(packet_level_chunk_t packet, binary_stream_t *stream);

#endif
