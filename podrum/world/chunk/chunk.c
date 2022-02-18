/*
                   Podrum R3 Copyright MFDGaming & PodrumTeam
                 This file is licensed under the GPLv2 license.
              To use this file you must own a copy of the license.
                       If you do not you can get it from:
            http://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
 */

#include "./chunk.h"

chunk_t new_chunk(int32_t x, int32_t z, mapping_block_states_t block_states)
{
	chunk_t chunk;
	chunk.x = x;
	chunk.z = z;
	uint8_t i;
	for (i = 0; i < 24; ++i) {
		chunk.sub_chunks[i] = new_sub_chunk(block_states);
		chunk.biomes[i] = new_biome_storage();
	}
	return chunk;
}

int32_t get_chunk_block_runtime_id(uint8_t x, uint8_t y, uint8_t z, uint8_t layer, chunk_t *chunk)
{
	return get_sub_chunk_block_runtime_id(x & 0x0f, y & 0x0f, z & 0x0f, layer, (&(chunk->sub_chunks[y >> 4])));
}

void set_chunk_block_runtime_id(uint8_t x, uint8_t y, uint8_t z, int32_t runtime_id, uint8_t layer, chunk_t *chunk)
{
	set_sub_chunk_block_runtime_id(x & 0x0f, y & 0x0f, z & 0x0f, runtime_id, layer, (&(chunk->sub_chunks[y >> 4])));
}

int32_t get_chunk_biome_runtime_id(uint8_t x, uint8_t y, uint8_t z, uint8_t layer, chunk_t *chunk)
{
	return get_block_storage_block_runtime_id(x & 0x0f, y & 0x0f, z & 0x0f, (&(chunk->biomes[y >> 4])));
}

void set_chunk_biome_runtime_id(uint8_t x, uint8_t y, uint8_t z, int32_t runtime_id, uint8_t layer, chunk_t *chunk)
{
	set_block_storage_block_runtime_id(x & 0x0f, y & 0x0f, z & 0x0f, runtime_id, (&(chunk->biomes[y >> 4])));
}

int8_t get_chunk_highest_block_at(uint8_t x, uint8_t z, uint8_t layer, chunk_t *chunk)
{
	uint8_t i;
	for (i = 23; i >= 0; --i) {
		int8_t y = get_sub_chunk_highest_block_at(x & 0x0f, z & 0x0f, layer, (&(chunk->sub_chunks[i])));
		if (y != SUB_CHUNK_LAYER_NOT_PRESENT && y != BLOCK_STORAGE_EMPTY) {
			return y + (i << 4);
		}
	}
	return CHUNK_EMPTY;
}

uint8_t is_empty_chunk(chunk_t *chunk)
{
	uint8_t i;
	for (i = 0; i < 24; ++i) {
		if (is_empty_sub_chunk((&(chunk->sub_chunks[i]))) == 0) {
			return 0;
		}
	}
	return 1;
}