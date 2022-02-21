/*
                   Podrum R3 Copyright MFDGaming & PodrumTeam
                 This file is licensed under the GPLv2 license.
              To use this file you must own a copy of the license.
                       If you do not you can get it from:
            http://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
 */

#include <podrum/world/chunk/subchunk.h>
#include <stdlib.h>

sub_chunk_t new_sub_chunk(int32_t air_runtime_id)
{
	sub_chunk_t sub_chunk;
	sub_chunk.block_storages_count = 2;
	sub_chunk.block_storages = (block_storage_t *) malloc(sub_chunk.block_storages_count * sizeof(block_storage_t));
	uint8_t i;
	for (i = 0; i < sub_chunk.block_storages_count; ++i) {
		sub_chunk.block_storages[i] = new_block_storage(air_runtime_id);
	}
	return sub_chunk;
}

int32_t get_sub_chunk_block_runtime_id(uint8_t x, uint8_t y, uint8_t z, uint8_t layer, sub_chunk_t *sub_chunk)
{
	if (layer < sub_chunk->block_storages_count) {
		return get_block_storage_block_runtime_id(x, y, z, (&(sub_chunk->block_storages[layer])));
	}
	return SUB_CHUNK_LAYER_NOT_PRESENT;
}

void set_sub_chunk_block_runtime_id(uint8_t x, uint8_t y, uint8_t z, int32_t runtime_id, uint8_t layer, sub_chunk_t *sub_chunk)
{
	if (layer < sub_chunk->block_storages_count) {
		set_block_storage_block_runtime_id(x, y, z, runtime_id, (&(sub_chunk->block_storages[layer])));
	}
}

int8_t get_sub_chunk_highest_block_at(uint8_t x, uint8_t z, uint8_t layer, sub_chunk_t *sub_chunk)
{
	if (layer < sub_chunk->block_storages_count) {
		return get_block_storage_highest_block_at(x, z, (&(sub_chunk->block_storages[layer])));
	}
	return SUB_CHUNK_LAYER_NOT_PRESENT;
}

uint8_t is_empty_sub_chunk(sub_chunk_t *sub_chunk)
{
	uint8_t i;
	for (i = 0; i < sub_chunk->block_storages_count; ++i) {
		if (is_empty_block_storage((&(sub_chunk->block_storages[i]))) == 0) {
			return 0;
		}
	}
	return 1;
}

void destroy_sub_chunk(sub_chunk_t *sub_chunk)
{
	uint8_t i;
	for (i = 0; i < sub_chunk->block_storages_count; ++i) {
		free(sub_chunk->block_storages[i].palette);
		free(sub_chunk->block_storages[i].blocks);
	}
	free(sub_chunk->block_storages);
}