cmake_minimum_required(VERSION 3.0)
project(PodrumR3)

set(CMAKE_C_STANDARD 11)

file(GLOB CBINARYSTREAM cbinarystream/*.c)
file(GLOB CNBT cnbt/*.c)
file(GLOB CZLIBHELPER czlibhelper/*.c)
file(GLOB MINIZ miniz/*.c)
file(GLOB PODRUM podrum/*.c podrum/misc/*.c podrum/world/*.c podrum/world/chunk/*.c podrum/command/*.c podrum/network/minecraft/*.c podrum/network/raknet/*.c)

include_directories(.)

add_executable(PodrumR3
        ${CBINARYSTREAM}
        ${CNBT}
        ${CZLIBHELPER}
        ${MINIZ}
        ${PODRUM})

IF(NOT ("${CMAKE_SYSTEM}" MATCHES "Windows"))
	TARGET_LINK_LIBRARIES(PodrumR3 pthread)
	TARGET_LINK_LIBRARIES(PodrumR3 m)
endif()
