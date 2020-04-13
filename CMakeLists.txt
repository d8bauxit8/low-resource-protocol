cmake_minimum_required(VERSION 3.15)
project(LowResourceProtocolLibrary C)

set(CMAKE_C_STANDARD 90)

file(GLOB SRC_SOURCES
        src/*.h
        src/*.c
        )
file(GLOB SOURCES
        *.h
        *.c
        )
add_executable(LowResourceProtocolLibrary ${SRC_SOURCES} ${SOURCES})