/*
                   Podrum R3 Copyright MFDGaming & PodrumTeam
                 This file is licensed under the GPLv2 license.
              To use this file you must own a copy of the license.
                       If you do not you can get it from:
            http://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
 */

#ifndef PODRUM_WORLD_CHUNK_BLOCKSTORAGE_H
#define PODRUM_WORLD_CHUNK_BLOCKSTORAGE_H

#include <stdint.h>

#define BLOCK_STORAGE_EMPTY -1

typedef struct {
	uint32_t blocks[4096];
	int32_t palette_size;
	int32_t *palette;
} block_storage_t;

block_storage_t new_block_storage(int32_t runtime_id);

int32_t get_block_storage_block_runtime_id(uint8_t x, uint8_t y, uint8_t z, block_storage_t *block_storage);

void set_block_storage_block_runtime_id(uint8_t x, uint8_t y, uint8_t z, int32_t runtime_id, block_storage_t *block_storage);

int8_t get_block_storage_highest_block_at(uint8_t x, uint8_t z, block_storage_t *block_storage);

uint8_t is_empty_block_storage(block_storage_t *block_storage);

#endif