// Copyright (c) Amaël Marquez.  Licensed under the MIT License.
// See the LICENSE file at the root of the repository for all the details.

#pragma once

#include <glad/glad.h>
#include <stdlib.h>  // For size_t.

#include "gm/error.h"
#include "resources/id.h"
#include "setup.h"

typedef gmId_ gmBuffer_;

typedef enum gmBufferType_ {
  gmBufferType_Vertex_ = GL_ARRAY_BUFFER,
  gmBufferType_Index_ = GL_ELEMENT_ARRAY_BUFFER
} gmBufferType_;

gmError gmCreateBuffers_(size_t count, GM_OUT_PARAM gmBuffer_ *buffers);
void gmDeleteBuffers_(size_t count, const gmBuffer_ *buffers);

void gmClearCurrentBuffer_(gmBufferType_ type);
void gmUseBufferAs_(const gmBuffer_ *buffer, gmBufferType_ type);

void gmLoadBufferDataAs_(gmBufferType_ type, size_t byte_count,
                         const void *data);
