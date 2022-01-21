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

#define GAME_VERSION (char *) "1.18.2"
#define GAME_PROTOCOL_VERSION 475
#define GAME_ENGINE (char *) "Podrum"

#define ID_GAME 0xFE
#define ID_LOGIN 0x01
#define ID_PLAY_STATUS 0x02
#define ID_RESOURCE_PACKS_INFO 0x06
#define ID_RESOURCE_PACK_STACK 0x07
#define ID_RESOURCE_PACK_CLIENT_RESPONSE 0x08
#define ID_START_GAME 0x0b

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
	char must_accept;
	char has_scripts;
	char force_server_packs;
	misc_behavior_pack_infos_t behavior_packs;
	misc_texture_pack_infos_t texture_packs;
} packet_resource_packs_info_t;

typedef struct {
	char must_accept;
	misc_resource_pack_id_versions_t behavior_packs;
	misc_resource_pack_id_versions_t resource_packs;
	char *game_version;
	misc_experiments_t experiments;
	char experiments_previously_used;
} packet_resource_pack_stack_t;

typedef struct {
	unsigned char response_status;
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
	char biome_name;
	int32_t dimension;
	int32_t generator;
	int32_t world_gamemode;
	int32_t difficulty;
	int32_t spawn_x;
	uint32_t spawn_y;
	int32_t spawn_z;
	char achievements_disabled;
	int32_t day_cycle_stop_time;
	int32_t edu_offer;
	char edu_features_enabled;
	char *edu_product_uuid;
	float rain_level;
	float lightning_level;
	char has_confirmed_platform_locked_content;
	char is_multiplayer;
	char broadcast_to_lan;
	uint32_t xbox_live_broadcast_mode;
	uint32_t platform_broadcast_mode;
	char enable_commands;
	char is_texturepacks_required;
	misc_game_rules_t gamerules;
	misc_experiments_t experiments;
	char experiments_previously_used;
	char bonus_chest;
	char map_enabled;
	int32_t permission_level;
	int32_t server_chunk_tick_range;
	char has_locked_behavior_pack;
	char has_locked_resource_pack;
	char is_from_locked_world_template;
	char msa_gamertags_only;
	char is_from_world_template;
	char is_world_template_option_locked;
	char only_spawn_v1_villagers;
	char *game_version;
	int32_t limited_world_width;
	int32_t limited_world_length;
	char is_new_nether;
	misc_education_shared_resource_uri_t edu_resource_uri;
	char experimental_gameplay_override;
	char *level_id;
	char *world_name;
	char *premium_world_template_id;
	char is_trial;
	int32_t movement_authority;
	int32_t rewind_history_size;
	char server_authoritative_block_breaking;
	int64_t current_tick;
	int32_t enchantment_seed;
	misc_block_properties_t block_properties;
	misc_item_states_t itemstates;
	char *multiplayer_correlation_id;
	char server_authoritative_inventory;
	char *engine;
	uint64_t block_pallete_checksum;
} packet_start_game_t;

packet_game_t get_packet_game(binary_stream_t *stream);

packet_login_t get_packet_login(binary_stream_t *stream);

packet_play_status_t get_packet_play_status(binary_stream_t *stream);

packet_resource_packs_info_t get_packet_resource_packs_info(binary_stream_t *stream);

packet_resource_pack_stack_t get_packet_resource_pack_stack(binary_stream_t *stream);

packet_resource_pack_client_response_t get_packet_resource_pack_client_response(binary_stream_t *stream);

void put_packet_game(packet_game_t packet, binary_stream_t *stream);

void put_packet_login(packet_login_t packet, binary_stream_t *stream);

void put_packet_play_status(packet_play_status_t packet, binary_stream_t *stream);

void put_packet_resource_packs_info(packet_resource_packs_info_t packet, binary_stream_t *stream);

void put_packet_resource_pack_stack(packet_resource_pack_stack_t packet, binary_stream_t *stream);

void put_packet_resource_pack_client_response(packet_resource_pack_client_response_t packet, binary_stream_t *stream);

#endif
