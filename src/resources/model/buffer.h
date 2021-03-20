// Copyright (c) Amaël Marquez.  Licensed under the MIT License.
// See the LICENSE file at the root of the repository for all the details.

#pragma once

#include <glad/glad.h>
#include <stdlib.h>  // For size_t.

#include "gm/error.h"
#include "resources/id.h"
#include "setup.h"

typedef gmId_ gmBuffer_;

gmError gmCreateBuffers_(size_t count, GM_OUT_PARAM gmBuffer_ *buffers);
void gmDeleteBuffers_(size_t count, const gmBuffer_ *buffers);

typedef enum gmBufferTarget_ {
  gmBufferTarget_Vertex_ = GL_ARRAY_BUFFER,
  gmBufferTarget_Index_ = GL_ELEMENT_ARRAY_BUFFER
} gmBufferTarget_;

void gmClearCurrentBuffer_(gmBufferTarget_ type);
void gmUseBufferAs_(const gmBuffer_ *buffer, gmBufferTarget_ type);

/**
 * Loads the specified data to the buffer bound to the specified buffer target.
 */
void gmLoadBufferDataAs_(gmBufferTarget_ type, size_t byte_count,
                         const void *data);
