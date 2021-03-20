// Copyright (c) Amaël Marquez.  Licensed under the MIT License.
// See the LICENSE file at the root of the repository for all the details.

#pragma once

#include <glad/glad.h>

#include "frame-buffer/frame-buffer.h"
#include "gm/error.h"
#include "gm/gm.h"
#include "model/model.h"
#include "program/program.h"
#include "setup.h"

typedef struct gmRenderData_ {
  gmModel_ quad;
  gmProgram_ program;
} gmRenderData_;

typedef struct gmRenderFrameBuffers_ {
  gmFrameBuffer_ render;
  gmFrameBuffer_ final;
} gmRenderFrameBuffers_;

typedef struct gmResources_ {
  gmRenderData_ render_data;
  gmRenderFrameBuffers_ render_frame_buffers;
} gmResources_;

gmError gmCreateResources_(GM_OUT_PARAM gmResources_ *resources,
                           const gmImageConfig *image_config);

void gmDeleteResources_(const gmResources_ *resources);
