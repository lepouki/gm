cmake_minimum_required(VERSION 3.17)
project(gm C)

add_executable(gm)

target_compile_features(gm PUBLIC c_std_99)
target_include_directories(gm PUBLIC inc PRIVATE src)

target_sources(gm
  PUBLIC
  inc/gm/error.h
  inc/gm/gm.h

  PRIVATE
  src/context/context.c
  src/context/context.h
  src/resources/frame-buffer/frame-buffer.c
  src/resources/frame-buffer/frame-buffer.h
  src/resources/model/quad/vertices.h
  src/resources/model/quad/quad.h
  src/resources/model/quad/indices.h
  src/resources/model/buffer.c
  src/resources/model/buffer.h
  src/resources/model/model.c
  src/resources/model/model.h
  src/resources/program/shaders/fragment-shader.h
  src/resources/program/shaders/shaders.h
  src/resources/program/shaders/vertex-shader.h
  src/resources/program/check-status.c
  src/resources/program/check-status.h
  src/resources/program/program.c
  src/resources/program/program.h
  src/resources/program/shader.c
  src/resources/program/shader.h
  src/resources/gl-error.c
  src/resources/gl-error.h
  src/resources/id.h
  src/resources/resources.c
  src/resources/resources.h
  src/error.c
  src/gm.c
  src/main.c
  src/setup.h)

add_subdirectory(vendor)
target_link_libraries(gm PUBLIC glfw glad stb)
