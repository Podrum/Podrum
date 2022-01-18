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
	in.size = stream->size - 1;
	zlib_buf_t out;
	zlib_decode(in, &out, ZLIB_DEFLATE_MODE);
	binary_stream_t data_stream;
	data_stream.buffer = (char *) out.data;
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

void put_packet_game(packet_game_t packet, binary_stream_t *stream)
{
	binary_stream_t temp_stream;
	temp_stream.buffer = (char *) malloc(0);
	temp_stream.offset = 0;
	temp_stream.size = 0;
	int i;
	for (i = 0; i < packet.streams_count; ++i)
	{
		put_var_int(packet.streams[i].size, stream);
		put_bytes(packet.streams[i].buffer, packet.streams[i].size, &temp_stream);
	}
	zlib_buf_t in;
	in.data = (Bytef *) temp_stream.buffer;
	in.size = temp_stream.size;
	zlib_buf_t out;
	zlib_encode(in, &out, 7, ZLIB_DEFLATE_MODE);
	free(temp_stream.buffer);
	put_bytes((char *) out.data, (int) out.size, stream);
	free(out.data);
}
