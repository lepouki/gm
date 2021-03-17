add_library(glad)
target_include_directories(glad PUBLIC include)

target_sources(glad
  PUBLIC
  include/glad/glad.h
  include/KHR/khrplatform.h

  PRIVATE
  src/glad.c)