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

packet_login_t get_packet_login(binary_stream_t *stream)
{
	++stream->offset;
	packet_login_t login;
	login.protocol_version = get_int_be(stream);
	binary_stream_t temp_stream = get_misc_byte_array_var_int(stream);
	login.tokens = get_misc_login_tokens(&temp_stream);
	free(temp_stream.buffer);
	return login;
}

packet_play_status_t get_packet_play_status(binary_stream_t *stream)
{
	++stream->offset;
	packet_play_status_t play_status;
	play_status.status = get_int_be(stream);
	return play_status;
}

void put_packet_game(packet_game_t packet, binary_stream_t *stream)
{
	binary_stream_t temp_stream;
	temp_stream.buffer = (char *) malloc(0);
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
	put_bytes((char *) out.data, (size_t) out.size, stream);
	free(out.data);
}

void put_packet_login(packet_login_t packet, binary_stream_t *stream)
{
	put_unsigned_byte(ID_LOGIN, stream);
	put_int_be(packet.protocol_version, stream);
	binary_stream_t temp_stream;
	temp_stream.buffer = (char *) malloc(0);
	temp_stream.offset = 0;
	temp_stream.size = 0;
	put_misc_login_tokens(packet.tokens, &temp_stream);
	put_misc_byte_array_var_int(temp_stream, stream);
	free(temp_stream.buffer);
}

void put_packet_play_status(packet_play_status_t packet, binary_stream_t *stream)
{
	put_unsigned_byte(ID_PLAY_STATUS, stream);
	put_int_be(packet.status, stream);
}
