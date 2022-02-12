cmake_minimum_required(VERSION 3.0)
project(PodrumR3)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(PodrumR3
        cbinarystream/binary_stream.c
		cnbt/nbt.c
        czlibhelper/zlib_helper.c
        miniz/miniz.c
        podrum/command/commandmanager.c
        podrum/misc/base64.c
        podrum/misc/json.c
        podrum/misc/logger.c
        podrum/network/minecraft/mcmisc.c
        podrum/network/minecraft/mcpackets.c
		podrum/network/minecraft/mcplayermanager.c
        podrum/network/raknet/rakhandler.c
        podrum/network/raknet/rakmisc.c
        podrum/network/raknet/rakpacket.c
        podrum/network/raknet/rakserver.c
        podrum/network/raknet/socket.c
        podrum/server.c
        podrum/worker.c)

IF(NOT ("${CMAKE_SYSTEM}" MATCHES "Windows"))
	TARGET_LINK_LIBRARIES(PodrumR3 pthread)
endif()