/*
                   Podrum R3 Copyright MFDGaming & PodrumTeam
                 This file is licensed under the GPLv2 license.
              To use this file you must own a copy of the license.
                       If you do not you can get it from:
            http://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
 */

#include <stdio.h>
#include <string.h>
#include <time.h>

#include <podrum/world/format/anvil/region.h>

region_t open_region(char *path)
{
	region_t region;
	region.file_path = path;
	region.status = 0;
	region.is_open = 1;
	FILE *file = fopen(path, "r+");
	fseek(file, 0, SEEK_END);
	region.file_stream_size = ftell(file);
	region.file_stream = malloc(region.file_stream_size);
	fseek(file, 0, SEEK_SET);
	fread(region.file_stream, 1, region.file_stream_size, file);
	fclose(file);
	if (region.file_stream_size < SECTOR_BYTES) {
		region.file_stream_size = 8192;
		region.file_stream = realloc(region.file_stream, region.file_stream_size);
		memset(region.file_stream, 0, region.file_stream_size);
	}
	int bytes_needed = region.file_stream_size & 0xfff;
	if (bytes_needed != 0) {
		region.file_stream_size += bytes_needed;
		region.file_stream = realloc(region.file_stream, region.file_stream_size);
		int i;
		for (i = region.file_stream_size - bytes_needed; i < region.file_stream_size; ++i) {
			region.file_stream[i] = 0;
		}
	}
	region.sector_free_size = region.file_stream_size / SECTOR_BYTES;
	region.sector_free = malloc(region.sector_free_size);
	memset(region.sector_free, 1, region.sector_free_size);
	region.sector_free[0] = 0;
	region.sector_free[1] = 0;
	int i;
	int offset = 0;
	for (i = 0; i < SECTOR_INTS; ++i) {
		region.offsets[i] = (region.file_stream[offset] & 0xff) << 24;
		++offset;
		region.offsets[i] |= (region.file_stream[offset] & 0xff) << 16;
		++offset;
		region.offsets[i] |= (region.file_stream[offset] & 0xff) << 8;
		++offset;
		region.offsets[i] |= region.file_stream[offset] & 0xff;
		++offset;
		if (region.offsets[i] != 0 && (region.offsets[i] >> 8) + (region.offsets[i] & 0xff) <= region.sector_free_size) {
			int sector_index;
			for (sector_index = 0; sector_index < (region.offsets[i] & 0xff); ++sector_index) {
				region.sector_free[(region.offsets[i] >> 8) + sector_index] = 0;
			}
		}
	}
	for (i = 0; i < SECTOR_INTS; ++i) {
		region.chunk_timestamps[i] = (region.file_stream[offset] & 0xff) << 24;
		++offset;
		region.chunk_timestamps[i] |= (region.file_stream[offset] & 0xff) << 16;
		++offset;
		region.chunk_timestamps[i] |= (region.file_stream[offset] & 0xff) << 8;
		++offset;
		region.chunk_timestamps[i] |= region.file_stream[offset] & 0xff;
		++offset;
	}

	return region;
}

long get_region_offset(int x, int z, region_t *region)
{
	return region->offsets[x + z * 32];
}

void set_region_offset(int x, int z, long offset, region_t *region)
{
	region->offsets[x + z * 32] = offset;
	int i = (x + z * 32) * 4;
	region->file_stream[i] = (offset >> 24) & 0xff;
	region->file_stream[i + 1] = (offset >> 16) & 0xff;
	region->file_stream[i + 2] = (offset >> 8) & 0xff;
	region->file_stream[i + 3] = offset & 0xff;
}

long get_region_timestamp(int x, int z, region_t *region)
{
	return region->chunk_timestamps[x + z * 32];
}

void set_region_timestamp(int x, int z, long timestamp, region_t *region)
{
	region->chunk_timestamps[x + z * 32] = timestamp;
	int i = SECTOR_BYTES + (x + z * 32) * 4;
	region->file_stream[i] = (timestamp >> 24) & 0xff;
	region->file_stream[i + 1] = (timestamp >> 16) & 0xff;
	region->file_stream[i + 2] = (timestamp >> 8) & 0xff;
	region->file_stream[i + 3] = timestamp & 0xff;
}

char out_of_bounds(int x, int z)
{
	return x < 0 || x >= 32 || z < 0 || z >= 32;
}

void set_region_sector(long sector_index, zlib_buf_t buffer, region_t *region)
{
	if (region->is_open == 0) {
		return;
	}
	int offset = sector_index * SECTOR_BYTES;
	long length = buffer.size + 1;
	region->file_stream[offset] = (length >> 24) & 0xff;
	++offset;
	region->file_stream[offset] = (length >> 16) & 0xff;
	++offset;
	region->file_stream[offset] = (length >> 8) & 0xff;
	++offset;
	region->file_stream[offset] = length & 0xff;
	++offset;
	region->file_stream[offset] = VERSION_DEFLATE;
	++offset;
	int i;
	for (i = 0; i < buffer.size; ++i) {
		region->file_stream[offset] = buffer.data[i];
		++offset;
	}
}

chunk_buf_t get_region_chunk(int x, int z, region_t *region)
{
	chunk_buf_t buffer;
	buffer.data = malloc(0);
	buffer.size = 0;
	if (region->is_open == 0) {
		return buffer;
	}
	if (out_of_bounds(x, z) == 1) {
		return buffer;
	}

	long region_offset = get_region_offset(x, z, region);
	if (region_offset == 0) {
		return buffer;
	}
	long sector_index = region_offset >> 8;
	char sector_count = region_offset & 0xff;
	if (sector_index + sector_count > region->sector_free_size) {
		return buffer;
	}
	int offset = sector_index * SECTOR_BYTES;
	long length = (region->file_stream[offset] & 0xff) << 24;
	++offset;
	length |= (region->file_stream[offset] & 0xff) << 16;
	++offset;
	length |= (region->file_stream[offset] & 0xff) << 8;
	++offset;
	length |= region->file_stream[offset] & 0xff;
	++offset;
	if (length > SECTOR_BYTES * sector_count) {
		return buffer;
	}
	char version = region->file_stream[offset];
	++offset;
	char *data = malloc(length - 1);
	int i;
	for (i = 0; i < length - 1; ++i) {
		data[i] = region->file_stream[offset];
		++offset;
	}
	zlib_buf_t out_buffer;
	out_buffer.data = (Bytef *) malloc(0);
	out_buffer.size = (uLong) 0;
	zlib_buf_t in_buffer;
	in_buffer.data = (Bytef *) data;
	in_buffer.size = (uLong) length - 1;
	int mode;
	if (version == VERSION_GZIP) {
		mode = ZLIB_GZIP_MODE;
	} else if (version == VERSION_DEFLATE) {
		mode = ZLIB_DEFLATE_MODE;
	} else {
		free(data);
		return buffer;
	}
	if (zlib_decode(in_buffer, &out_buffer, mode) == Z_OK) {
		buffer.data = (char *) out_buffer.data;
		buffer.size = (size_t) out_buffer.size;
	}
	free(data);
	return buffer;
}

void set_region_chunk(int x, int z, chunk_buf_t buffer, region_t *region)
{
	zlib_buf_t out_buffer;
	out_buffer.data = (Bytef *) malloc(0);
	out_buffer.size = (uLong) 0;
	zlib_buf_t in_buffer;
	in_buffer.data = (Bytef *) buffer.data;
	in_buffer.size = (uLong) buffer.size;
	if (zlib_encode(in_buffer, &out_buffer, 7, ZLIB_DEFLATE_MODE) != Z_OK) {
		free(out_buffer.data);
		return;
	}
	long region_offset = get_region_offset(x, z, region);
	long sector_index = region_offset >> 8;
	char sector_count = region_offset & 0xff;
	char required_sector_count = (out_buffer.size + CHUNK_HEADER_SIZE) / SECTOR_BYTES + 1;
	if (sector_index != 0 && sector_count == required_sector_count) {
		set_region_sector(sector_index, out_buffer, region);
	} else {
		int i;
		for (i = 0; i < sector_count; ++i) {
			region->sector_free[sector_index + i] = 1;
		}
		int run_start = -1;
		for (i = 0; i < region->sector_free_size; ++i) {
			if (region->sector_free[i] == 1) {
				run_start = i;
				break;
			}
		}
		int run_length = 0;
		if (run_start != -1) {
			for (i = run_start; i < region->sector_free_size; ++i) {
				if (run_length != 0) {
					if (region->sector_free[i] == 1) {
						++run_length;
					} else {
						run_length = 0;
					}
				} else if (region->sector_free[i] == 1) {
					run_start = i;
					run_length = 1;
				}
				if (run_length >= required_sector_count) {
					break;
				}
			}
		}
		if (run_length >= required_sector_count) {
			sector_index = run_start;
			set_region_offset(x, z, (sector_index << 8) | required_sector_count, region);
			for (i = 0; i < required_sector_count; ++i) {
				region->sector_free[sector_index + i] = 0;
			}
			set_region_sector(sector_index, out_buffer, region);
		} else {
			int offset = region->file_stream_size;
			sector_index = region->sector_free_size;
			region->sector_free_size += required_sector_count;
			region->sector_free = realloc(region->sector_free, region->sector_free_size);
			region->file_stream_size += SECTOR_BYTES * required_sector_count;
			region->file_stream = realloc(region->file_stream, region->file_stream_size);
			for (i = sector_index; i < region->sector_free_size; ++i) {
				region->sector_free[i] = 0;
			}
			while (offset < region->file_stream_size) {
				region->file_stream[offset] = 0;
				++offset;
			}
			set_region_sector(sector_index, out_buffer, region);
			set_region_offset(x, z, (sector_index << 8) | required_sector_count, region);
		}
		set_region_timestamp(x, z, (long) time(NULL), region);
	}
}

void save_region(region_t *region)
{
	FILE *file = fopen(region->file_path, "w");
	fwrite(region->file_stream, 1, region->file_stream_size, file);
	fclose(file);
}

void close_region(region_t *region)
{
	save_region(region);
	free(region->file_stream);
	free(region->sector_free);
	region->is_open = 0;
}
