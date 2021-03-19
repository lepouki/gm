// Copyright (c) Amaël Marquez.  Licensed under the MIT License.
// See the LICENSE file at the root of the repository for all the details.

#pragma once

#include <glad/glad.h>

#include "gm/error.h"
#include "gm/setup.h"

typedef GLuint gmId_;

typedef struct gmModel_ {
  gmId_ vertex_array;
  gmId_ buffers[2];  // Vertex and index buffers.
} gmModel_;

typedef struct gmFramebuffer_ {
  gmId_ id;
  gmId_ color_render_buffer;
} gmFramebuffer_;

typedef struct gmResources_ {
  gmId_ program;
  gmModel_ quad_model;
} gmResources_;

gmError gmCreateResources_(GM_OUT_PARAM gmResources_ *resources);
void gmDeleteResources_(const gmResources_ *resources);
