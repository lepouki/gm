cmake_minimum_required(VERSION 3.17)
project(gm C)

add_executable(gm)

target_compile_features(gm PUBLIC c_std_99)
target_include_directories(gm PUBLIC inc)

target_sources(gm
    PUBLIC
    inc/gm/gm.h

    PRIVATE
    src/main.c)

add_subdirectory(vendor)
target_link_libraries(gm PUBLIC glfw glad stb)
