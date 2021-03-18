add_library(stb INTERFACE)
target_include_directories(stb INTERFACE inc)

target_sources(stb
  INTERFACE
  inc/stb/stb_image_write.h)
