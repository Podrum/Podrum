/*
                   Podrum R3 Copyright MFDGaming & PodrumTeam
                 This file is licensed under the GPLv2 license.
              To use this file you must own a copy of the license.
                       If you do not you can get it from:
            http://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
 */

#ifndef PODRUM_NETWORK_MINECRAFT_MCPACKETS_H
#define PODRUM_NETWORK_MINECRAFT_MCPACKETS_H

#include <cbinarystream/binary_stream.h>
#include <stdlib.h>
#include "./mcmisc.h"
#include <cnbt/nbt.h>

#define GAME_VERSION "1.18.10"
#define GAME_PROTOCOL_VERSION 486
#define GAME_ENGINE "Podrum"

#define ID_LOGIN 0x01
#define ID_PLAY_STATUS 0x02
#define ID_RESOURCE_PACKS_INFO 0x06
#define ID_RESOURCE_PACK_STACK 0x07
#define ID_RESOURCE_PACK_CLIENT_RESPONSE 0x08
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
	uint8_t must_accept;
	uint8_t has_scripts;
	uint8_t force_server_packs;
	misc_behavior_pack_infos_t behavior_packs;
	misc_texture_pack_infos_t texture_packs;
} packet_resource_packs_info_t;

typedef struct {
	uint8_t must_accept;
	misc_resource_pack_id_versions_t behavior_packs;
	misc_resource_pack_id_versions_t resource_packs;
	char *game_version;
	misc_experiments_t experiments;
	uint8_t experiments_previously_used;
} packet_resource_pack_stack_t;

typedef struct {
	uint8_t response_status;
	misc_resource_pack_ids_t resource_pack_ids;
} packet_resource_pack_client_response_t;

typedef struct {
	int64_t entity_id;
	uint64_t runtime_entity_id;
	int32_t player_gamemode;
	float player_x;
	float player_y;
	float player_z;
	float pitch;
	float yaw;
	int32_t seed;
	int16_t biome_type;
	char *biome_name;
	int32_t dimension;
	int32_t generator;
	int32_t world_gamemode;
	int32_t difficulty;
	int32_t spawn_x;
	uint32_t spawn_y;
	int32_t spawn_z;
	uint8_t achievements_disabled;
	int32_t day_cycle_stop_time;
	int32_t edu_offer;
	uint8_t edu_features_enabled;
	char *edu_product_uuid;
	float rain_level;
	float lightning_level;
	uint8_t has_confirmed_platform_locked_content;
	uint8_t is_multiplayer;
	uint8_t broadcast_to_lan;
	uint32_t xbox_live_broadcast_mode;
	uint32_t platform_broadcast_mode;
	uint8_t enable_commands;
	uint8_t are_texture_packs_required;
	misc_game_rules_t gamerules;
	misc_experiments_t experiments;
	uint8_t experiments_previously_used;
	uint8_t bonus_chest;
	uint8_t map_enabled;
	int32_t permission_level;
	int32_t server_chunk_tick_range;
	uint8_t has_locked_behavior_pack;
	uint8_t has_locked_texture_pack;
	uint8_t is_from_locked_world_template;
	uint8_t msa_gamertags_only;
	uint8_t is_from_world_template;
	uint8_t is_world_template_option_locked;
	uint8_t only_spawn_v1_villagers;
	char *game_version;
	int32_t limited_world_width;
	int32_t limited_world_length;
	uint8_t is_new_nether;
	misc_education_shared_resource_uri_t edu_resource_uri;
	uint8_t experimental_gameplay_override;
	char *level_id;
	char *world_name;
	char *premium_world_template_id;
	uint8_t is_trial;
	int32_t movement_authority;
	int32_t rewind_history_size;
	uint8_t server_authoritative_block_breaking;
	int64_t current_tick;
	int32_t enchantment_seed;
	misc_block_properties_t block_properties;
	misc_item_states_t item_states;
	char *multiplayer_correlation_id;
	uint8_t server_authoritative_inventory;
	char *engine;
	uint64_t block_pallete_checksum;
} packet_start_game_t;

typedef struct {
	nbt_compound_t nbt;
} packet_biome_definition_list_t;

typedef struct {
	nbt_compound_t nbt;
} packet_available_entity_identifiers_t;

typedef struct {
	uint32_t size;
	uint32_t *entry_ids;
	misc_item_t *items;
} packet_creative_content_t;

typedef struct {
	uint8_t action_id;
	uint64_t target_entity_id;
	float position_x;
	float position_y;
	float position_z;
} packet_interact_t;

typedef struct {
	int8_t window_id;
	int8_t window_type;
	int32_t coordinates_x;
	uint32_t coordinates_y;
	int32_t coordinates_z;
	int64_t runtime_entity_id;
} packet_container_open_t;

typedef struct {
	int8_t window_id;
	uint8_t server;
} packet_container_close_t;

typedef struct {
	uint32_t runtime_id;
	float position_x;
	float position_y;
	float position_z;
	float pitch;
	float yaw;
	float head_yaw;
	uint8_t mode;
	uint8_t on_ground;
	uint32_t ridden_runtime_id;
	int32_t teleport_cause;
	int32_t teleport_source_entity_type;
	uint64_t tick;
} packet_move_player_t;

typedef struct {
	int32_t x;
	uint32_t y;
	int32_t z;
	uint32_t radius;
} packet_network_chunk_publisher_update_t;

typedef struct {
	int32_t chunk_radius;
} packet_request_chunk_radius_t;

typedef struct {
	int32_t chunk_radius;
} packet_chunk_radius_updated_t;

typedef struct {
	int32_t x;
	int32_t z;
	uint32_t sub_chunk_count;
	uint16_t highest_subchunk_count;
	uint8_t cache_enabled;
	uint32_t hashes_count;
	uint64_t *hashes;
	binary_stream_t payload;
} packet_level_chunk_t;

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
