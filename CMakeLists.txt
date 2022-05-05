cmake_minimum_required(VERSION 3.0)
project(PodrumR3)

set(CMAKE_C_STANDARD 11)
set(USE_LEAK_DETECTOR 1)

set(_CRT_SECURE_NO_WARNINGS)

file(GLOB CBINARYSTREAM cbinarystream/*.c)
file(GLOB CNBT cnbt/*.c)
file(GLOB CZLIBHELPER czlibhelper/*.c)
file(GLOB MINIZ miniz/*.c)
file(GLOB PODRUM podrum/*.c podrum/misc/*.c podrum/world/*.c podrum/world/format/anvil/*.c podrum/world/chunk/*.c podrum/command/*.c podrum/network/minecraft/*.c podrum/network/raknet/*.c)

include_directories(.)

IF("${CMAKE_SYSTEM}" MATCHES "Windows")
		if("${USE_LEAK_DETECTOR}" MATCHES "1")
				include_directories("c:\\Program Files (x86)\\Visual Leak Detector\\include")
				link_directories("c:\\Program Files (x86)\\Visual Leak Detector\\lib\\Win64")
		endif()
endif()

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
