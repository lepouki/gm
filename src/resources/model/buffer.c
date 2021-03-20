// Copyright (c) Amaël Marquez.  Licensed under the MIT License.
// See the LICENSE file at the root of the repository for all the details.

#include "buffer.h"

#include <glad/glad.h>

#include "gm/error.h"
#include "setup.h"

gmError gmCreateBuffers_(size_t count, GM_OUT_PARAM gmBuffer_ *buffers) {
  glGenBuffers(count, buffers);
  return gmError_Success;
}

void gmDeleteBuffers_(size_t count, const gmBuffer_ *buffers) {
  glDeleteBuffers(count, buffers);
}

void gmClearCurrentBuffer_(gmBufferType_ type) {
  glBindBuffer(type, 0);
}

void gmUseBufferAs_(const gmBuffer_ *buffer, gmBufferType_ type) {
  glBindBuffer(type, *buffer);
}

void gmLoadBufferDataAs_(gmBufferType_ type, size_t byte_count,
                         const void *data) {
  glBufferData(type, byte_count, data, GL_STATIC_DRAW);
}
