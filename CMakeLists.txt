cmake_minimum_required(VERSION 3.17)
project(gm C)

set(CMAKE_C_STANDARD 99)
add_executable(gm)

target_include_directories(gm PUBLIC inc)

target_sources(gm
    PUBLIC
    inc/gm/gm.h

    PRIVATE
    src/main.c)
