/*
                   Podrum R3 Copyright MFDGaming & PodrumTeam
                 This file is licensed under the GPLv2 license.
              To use this file you must own a copy of the license.
                       If you do not you can get it from:
            http://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
 */

#ifndef PODRUM_WORLD_CHUNK_CHUNK_H
#define PODRUM_WORLD_CHUNK_CHUNK_H

#include "./subchunk.h"
#include "./blockstorage.h"

#define CHUNK_EMPTY -3

typedef struct {
	int32_t x;
	int32_t z;
	sub_chunk_t sub_chunks[24];
	block_storage_t biomes[24];
} chunk_t;

chunk_t new_chunk(int32_t x, int32_t z, mapping_block_states_t block_states);

int32_t get_chunk_block_runtime_id(uint8_t x, uint8_t y, uint8_t z, uint8_t layer, chunk_t *chunk);

int32_t get_chunk_biome_runtime_id(uint8_t x, uint8_t y, uint8_t z, uint8_t layer, chunk_t *chunk);

void set_chunk_biome_runtime_id(uint8_t x, uint8_t y, uint8_t z, int32_t runtime_id, uint8_t layer, chunk_t *chunk);

void set_chunk_block_runtime_id(uint8_t x, uint8_t y, uint8_t z, int32_t runtime_id, uint8_t layer, chunk_t *chunk);

int8_t get_chunk_highest_block_at(uint8_t x, uint8_t z, uint8_t layer, chunk_t *chunk);

uint8_t is_empty_chunk(chunk_t *chunk);

#endif