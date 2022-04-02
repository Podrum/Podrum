/*
                   Podrum R3 Copyright MFDGaming & PodrumTeam
                 This file is licensed under the GPLv2 license.
              To use this file you must own a copy of the license.
                       If you do not you can get it from:
            http://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
 */

#ifndef PODRUM_WORLD_FORMAT_ANVIL_REGION_H
#define PODRUM_WORLD_FORMAT_ANVIL_REGION_H

#include <stdlib.h>
#include <czlibhelper/zlib_helper.h>

#define VERSION_GZIP 1
#define VERSION_DEFLATE 2

#define SECTOR_BYTES 4096
#define SECTOR_INTS 1024

#define CHUNK_HEADER_SIZE 5

typedef struct {
	char empty_sector[4096];
	char *file_path;
	char *file_stream;
	size_t file_stream_size;
	long offsets[1024];
	long chunk_timestamps[1024];
	char *sector_free;
	size_t sector_free_size;
	int status;
	char is_open;
} region_t;

typedef struct {
        char *data;
        size_t size;
} chunk_buf_t;

region_t open_region(char *path);

long get_region_offset(int x, int z, region_t *region);

void set_region_offset(int x, int z, long offset, region_t *region);

long get_region_timestamp(int x, int z, region_t *region);

void set_region_timestamp(int x, int z, long timestamp, region_t *region);

char out_of_bounds(int x, int z);

void set_region_sector(long sector_index, zlib_buf_t buffer, region_t *region);

chunk_buf_t get_region_chunk(int x, int z, region_t *region);

void set_region_chunk(int x, int z, chunk_buf_t buffer, region_t *region);

void save_region(region_t *region);

void close_region(region_t *region);

#endif
