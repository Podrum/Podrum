/*
                   Podrum R3 Copyright MFDGaming & PodrumTeam
                 This file is licensed under the GPLv2 license.
              To use this file you must own a copy of the license.
                       If you do not you can get it from:
            http://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
 */

#include <podrum/world/chunk/blockstorage.h>
#include <stdlib.h>
#include <string.h>

block_storage_t new_block_storage(int32_t runtime_id)
{
	block_storage_t block_storage;
	int i;
	block_storage.blocks = (uint32_t *) malloc(BLOCK_STORAGE_BLOCKS_SIZE * sizeof(uint32_t));
	for (i = 0; i < 4096; ++i) {
		block_storage.blocks[i] = 0;
	}
	block_storage.palette_size = 1;
	block_storage.palette = (int32_t *) malloc(block_storage.palette_size * sizeof(int32_t));
	block_storage.palette[0] = runtime_id;
	return block_storage;
}

int32_t get_block_storage_block_runtime_id(uint8_t x, uint8_t y, uint8_t z, block_storage_t *block_storage)
{
	int32_t index = block_storage->blocks[(x << 8) | (z << 4) | y];
	return block_storage->palette[index];
}

void set_block_storage_block_runtime_id(uint8_t x, uint8_t y, uint8_t z, int32_t runtime_id, block_storage_t *block_storage)
{
	int32_t i;
	for (i = 0; i < block_storage->palette_size; ++i) {
		if (block_storage->palette[i] == runtime_id) {
			break;
		}
	}
	if (i == block_storage->palette_size) {
		++block_storage->palette_size;
		block_storage->palette = (int32_t *) realloc(block_storage->palette, block_storage->palette_size * sizeof(int32_t));
		block_storage->palette[block_storage->palette_size - 1] = runtime_id;
	}
	block_storage->blocks[(x << 8) | (z << 4) | y] = i;
}

int8_t get_block_storage_highest_block_at(uint8_t x, uint8_t z, block_storage_t *block_storage)
{
	int8_t y;
	for (y = 15; y >= 0; --y) {
		if (block_storage->blocks[(x << 8) | (z << 4) | y] != 0) {
			return y;
		}
	}
	return BLOCK_STORAGE_EMPTY;
}

uint8_t is_empty_block_storage(block_storage_t *block_storage)
{
	int16_t i;
	for (i = 0; i < 4096; ++i) {
		if (block_storage->blocks[i] != 0) {
			return 0;
		}
	}
	return 1;
}
