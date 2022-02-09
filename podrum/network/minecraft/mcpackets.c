/*
                   Podrum R3 Copyright MFDGaming & PodrumTeam
                 This file is licensed under the GPLv2 license.
              To use this file you must own a copy of the license.
                       If you do not you can get it from:
            http://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
 */

#include "./mcpackets.h"
#include <czlibhelper/zlib_helper.h>
#include <stdlib.h>

packet_game_t get_packet_game(binary_stream_t *stream)
{
	packet_game_t game;
	game.streams = (binary_stream_t *) malloc(0);
	game.streams_count = 0;
	stream->offset += stream->size;
	zlib_buf_t in;
	in.data = (Bytef *) (stream->buffer + 1);
	in.size = (uLong) (stream->size - 1);
	zlib_buf_t out;
	zlib_decode(in, &out, ZLIB_DEFLATE_MODE);
	binary_stream_t data_stream;
	data_stream.buffer = (int8_t *) out.data;
	data_stream.size = out.size;
	data_stream.offset = 0;
	while (data_stream.offset < data_stream.size) {
		binary_stream_t entry_stream;
		entry_stream.size = get_var_int(&data_stream);
		entry_stream.buffer = get_bytes(entry_stream.size, &data_stream);
		entry_stream.offset = 0;
		++game.streams_count;
		game.streams = (binary_stream_t *) realloc(game.streams, game.streams_count * sizeof(binary_stream_t));
		game.streams[game.streams_count - 1] = entry_stream;
	}
	free(data_stream.buffer);
	return game;
}

packet_login_t get_packet_login(binary_stream_t *stream)
{
	get_var_int(stream); /* Packet ID */
	packet_login_t login;
	login.protocol_version = get_int_be(stream);
	binary_stream_t temp_stream = get_misc_byte_array_var_int(stream);
	login.tokens = get_misc_login_tokens(&temp_stream);
	free(temp_stream.buffer);
	return login;
}

packet_play_status_t get_packet_play_status(binary_stream_t *stream)
{
	get_var_int(stream); /* Packet ID */
	packet_play_status_t play_status;
	play_status.status = get_int_be(stream);
	return play_status;
}

packet_resource_packs_info_t get_packet_resource_packs_info(binary_stream_t *stream)
{
	get_var_int(stream); /* Packet ID */
	packet_resource_packs_info_t resource_packs_info;
	resource_packs_info.must_accept = get_unsigned_byte(stream);
	resource_packs_info.has_scripts = get_unsigned_byte(stream);
	resource_packs_info.force_server_packs = get_unsigned_byte(stream);
	resource_packs_info.behavior_packs = get_misc_behavior_pack_infos(stream);
	resource_packs_info.texture_packs = get_misc_texture_pack_infos(stream);
	return resource_packs_info;
}

packet_resource_pack_stack_t get_packet_resource_pack_stack(binary_stream_t *stream)
{
	get_var_int(stream); /* Packet ID */
	packet_resource_pack_stack_t resource_pack_stack;
	resource_pack_stack.must_accept = get_unsigned_byte(stream);
	resource_pack_stack.behavior_packs = get_misc_resource_pack_id_versions(stream);
	resource_pack_stack.resource_packs = get_misc_resource_pack_id_versions(stream);
	resource_pack_stack.game_version = get_misc_string_var_int(stream);
	resource_pack_stack.experiments = get_misc_experiments(stream);
	resource_pack_stack.experiments_previously_used = get_unsigned_byte(stream);
	return resource_pack_stack;
}

packet_resource_pack_client_response_t get_packet_resource_pack_client_response(binary_stream_t *stream)
{
	get_var_int(stream); /* Packet ID */
	packet_resource_pack_client_response_t resource_pack_client_response;
	resource_pack_client_response.response_status = get_unsigned_byte(stream);
	resource_pack_client_response.resource_pack_ids = get_misc_resource_pack_ids(stream);
	return resource_pack_client_response;
}


packet_start_game_t get_packet_start_game(binary_stream_t *stream)
{
	get_var_int(stream); /* Packet ID */
	packet_start_game_t start_game;
	return start_game;
}

packet_biome_definition_list_t get_packet_biome_definition_list(binary_stream_t *stream)
{
	get_var_int(stream); /* Packet ID */
	packet_biome_definition_list_t biome_definition_list;
	biome_definition_list.stream.size = stream->size - stream->offset;
	biome_definition_list.stream.buffer = get_bytes(biome_definition_list.stream.size, stream);
	return biome_definition_list;
}

packet_available_entity_identifiers_t get_packet_available_entity_identifiers(binary_stream_t *stream)
{
	get_var_int(stream); /* Packet ID */
	packet_available_entity_identifiers_t available_entity_identifiers;
	available_entity_identifiers.stream.size = stream->size - stream->offset;
	available_entity_identifiers.stream.buffer = get_bytes(available_entity_identifiers.stream.size, stream);
	return available_entity_identifiers;
}

packet_creative_content_t get_packet_creative_content(binary_stream_t *stream)
{
	get_var_int(stream); /* Packet ID */
	packet_creative_content_t creative_content;
	creative_content.size = get_var_int(stream);
	creative_content.entry_ids = (uint32_t *) malloc(creative_content.size * sizeof(uint32_t));
	creative_content.items = (misc_item_t *) malloc(creative_content.size * sizeof(misc_item_t));
	uint32_t i;
	for (i = 0; i < creative_content.size; ++i) {
		creative_content.entry_ids[i] = get_var_int(stream);
		/* TODO creative_content.items[i] = get_item(stream); TODO*/
	}
	return creative_content;
}

void put_packet_game(packet_game_t packet, binary_stream_t *stream)
{
	binary_stream_t temp_stream;
	temp_stream.buffer = (int8_t *) malloc(0);
	temp_stream.offset = 0;
	temp_stream.size = 0;
	size_t i;
	for (i = 0; i < packet.streams_count; ++i)
	{
		put_var_int(packet.streams[i].size, &temp_stream);
		put_bytes(packet.streams[i].buffer, packet.streams[i].size, &temp_stream);
	}
	zlib_buf_t in;
	in.data = (Bytef *) temp_stream.buffer;
	in.size = (uLong) temp_stream.size;
	zlib_buf_t out;
	zlib_encode(in, &out, 7, ZLIB_DEFLATE_MODE);
	free(temp_stream.buffer);
	put_unsigned_byte(ID_GAME, stream);
	put_bytes((int8_t *) out.data, (size_t) out.size, stream);
	free(out.data);
}

void put_packet_login(packet_login_t packet, binary_stream_t *stream)
{
	put_var_int(ID_LOGIN, stream);
	put_int_be(packet.protocol_version, stream);
	binary_stream_t temp_stream;
	temp_stream.buffer = (int8_t *) malloc(0);
	temp_stream.offset = 0;
	temp_stream.size = 0;
	put_misc_login_tokens(packet.tokens, &temp_stream);
	put_misc_byte_array_var_int(temp_stream, stream);
	free(temp_stream.buffer);
}

void put_packet_play_status(packet_play_status_t packet, binary_stream_t *stream)
{
	put_var_int(ID_PLAY_STATUS, stream);
	put_int_be(packet.status, stream);
}

void put_packet_resource_packs_info(packet_resource_packs_info_t packet, binary_stream_t *stream)
{
	put_var_int(ID_RESOURCE_PACKS_INFO, stream);
	put_unsigned_byte(packet.must_accept, stream);
	put_unsigned_byte(packet.has_scripts, stream);
	put_unsigned_byte(packet.force_server_packs, stream);
	put_misc_behavior_pack_infos(packet.behavior_packs, stream);
	put_misc_texture_pack_infos(packet.texture_packs, stream);
}

void put_packet_resource_pack_stack(packet_resource_pack_stack_t packet, binary_stream_t *stream)
{
	put_var_int(ID_RESOURCE_PACK_STACK, stream);
	put_unsigned_byte(packet.must_accept, stream);
	put_misc_resource_pack_id_versions(packet.behavior_packs, stream);
	put_misc_resource_pack_id_versions(packet.resource_packs, stream);
	put_misc_string_var_int(packet.game_version, stream);
	put_misc_experiments(packet.experiments, stream);
	put_unsigned_byte(packet.experiments_previously_used, stream);
}

void put_packet_resource_pack_client_response(packet_resource_pack_client_response_t packet, binary_stream_t *stream)
{
	put_var_int(ID_RESOURCE_PACK_CLIENT_RESPONSE, stream);
	put_unsigned_byte(packet.response_status, stream);
	put_misc_resource_pack_ids(packet.resource_pack_ids, stream);
}

void put_packet_start_game(packet_start_game_t packet, binary_stream_t *stream)
{
	put_var_int(ID_START_GAME, stream);
	put_signed_var_long(packet.entity_id, stream);
	put_var_long(packet.runtime_entity_id, stream);
	put_signed_var_int(packet.player_gamemode, stream);
	put_float_le(packet.player_x, stream);
	put_float_le(packet.player_y, stream);
	put_float_le(packet.player_z, stream);
	put_float_le(packet.pitch, stream);
	put_float_le(packet.yaw, stream);
	put_signed_var_int(packet.seed, stream);
	put_short_le(packet.biome_type, stream);
	put_misc_string_var_int(packet.biome_name, stream);
	put_signed_var_int(packet.dimension, stream);
	put_signed_var_int(packet.generator, stream);
	put_signed_var_int(packet.world_gamemode, stream);
	put_signed_var_int(packet.difficulty, stream);
	put_signed_var_int(packet.spawn_x, stream);
	put_var_int(packet.spawn_y & 0xffffffff, stream);
	put_signed_var_int(packet.spawn_z, stream);
	put_unsigned_byte(packet.achievements_disabled, stream);
	put_signed_var_int(packet.day_cycle_stop_time, stream);
	put_signed_var_int(packet.edu_offer, stream);
	put_unsigned_byte(packet.edu_features_enabled, stream);
	put_misc_string_var_int(packet.edu_product_uuid, stream);
	put_float_le(packet.rain_level, stream);
	put_float_le(packet.lightning_level, stream);
	put_unsigned_byte(packet.has_confirmed_platform_locked_content, stream);
	put_unsigned_byte(packet.is_multiplayer, stream);
	put_unsigned_byte(packet.broadcast_to_lan, stream);
	put_var_int(packet.xbox_live_broadcast_mode, stream);
	put_var_int(packet.platform_broadcast_mode, stream);
	put_unsigned_byte(packet.enable_commands, stream);
	put_unsigned_byte(packet.are_texture_packs_required, stream);
	put_misc_game_rules(packet.gamerules, stream);
	put_misc_experiments(packet.experiments, stream);
	put_unsigned_byte(packet.experiments_previously_used, stream);
	put_unsigned_byte(packet.bonus_chest, stream);
	put_unsigned_byte(packet.map_enabled, stream);
	put_signed_var_int(packet.permission_level, stream);
	put_int_le(packet.server_chunk_tick_range, stream);
	put_unsigned_byte(packet.has_locked_behavior_pack, stream);
	put_unsigned_byte(packet.has_locked_texture_pack, stream);
	put_unsigned_byte(packet.is_from_locked_world_template, stream);
	put_unsigned_byte(packet.msa_gamertags_only, stream);
	put_unsigned_byte(packet.is_from_world_template, stream);
	put_unsigned_byte(packet.is_world_template_option_locked, stream);
	put_unsigned_byte(packet.only_spawn_v1_villagers, stream);
	put_misc_string_var_int(packet.game_version, stream);
	put_int_le(packet.limited_world_width, stream);
	put_int_le(packet.limited_world_length, stream);
	put_unsigned_byte(packet.is_new_nether, stream);
	put_misc_education_shared_resource_uri(packet.edu_resource_uri, stream);
	put_unsigned_byte(packet.experimental_gameplay_override, stream);
	put_misc_string_var_int(packet.level_id, stream);
	put_misc_string_var_int(packet.world_name, stream);
	put_misc_string_var_int(packet.premium_world_template_id, stream);
	put_unsigned_byte(packet.is_trial, stream);
	put_signed_var_int(packet.movement_authority, stream);
	put_signed_var_int(packet.rewind_history_size, stream);
	put_unsigned_byte(packet.server_authoritative_block_breaking, stream);
	put_long_le(packet.current_tick, stream);
	put_signed_var_int(packet.enchantment_seed, stream);
	put_var_int(0, stream); /* Block Properties */
	put_var_int(0, stream); /* Item States */
	put_misc_string_var_int(packet.multiplayer_correlation_id, stream);
	put_unsigned_byte(packet.server_authoritative_inventory, stream);
	put_misc_string_var_int(packet.engine, stream);
	put_unsigned_long_le(packet.block_pallete_checksum, stream);
}

void put_packet_biome_definition_list(packet_biome_definition_list_t packet, binary_stream_t *stream)
{
	put_var_int(ID_BIOME_DEFINITION_LIST, stream);
	put_bytes(packet.stream.buffer, packet.stream.size, stream);
}

void put_packet_available_entity_identifiers(packet_available_entity_identifiers_t packet, binary_stream_t *stream)
{
	put_var_int(ID_AVAILABLE_ENTITY_IDENTIFIERS, stream);
	put_bytes(packet.stream.buffer, packet.stream.size, stream);
}

void put_packet_creative_content(packet_creative_content_t packet, binary_stream_t *stream)
{
	put_var_int(ID_CREATIVE_CONTENT, stream);
	put_var_int(0, stream); /* Creative Items */
}